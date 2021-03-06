#include <iostream>

#include "mesh_renderer.hh"
#include "utils/buffer_offset_macro.hh"
#include "utils/definitions.hh"
#include "utils/gl_check.hh"
#include "utils/log.hh"

namespace pogl
{
    using Self = MeshRenderer::Builder;

    Self::Builder()
        : _buffers()
        , _shader(std::nullopt)
        , _draw_mode(GL_TRIANGLES)
        , _attribute_config()
        // assume all objects coordinates are already in worldspace
        // or that the object's anchor is at 0,0,0
        , _transform(Matrix4::identity())
    {}

    Self &Self::add_buffer(const BufferType &buffer)
    {
        _buffers.push_back(buffer);
        return *this;
    }

    Self &Self::shader(const ShaderType &shader)
    {
        _shader = shader;
        return *this;
    }

    Self &Self::draw_mode(DrawModeType draw_mode)
    {
        _draw_mode = draw_mode;
        return *this;
    }

    Self &Self::add_attribute(std::string name, int size, size_t buffer_id)
    {
        if (!_attribute_config.contains(buffer_id))
        {
            _attribute_config.emplace(buffer_id,
                                      std::vector<AttributeConfigType>());
        }
        _attribute_config.at(buffer_id).emplace_back(name, size);
        return *this;
    }

    Self &Self::transform(Matrix4 transform)
    {
        _transform = transform;
        return *this;
    }

    void Self::assert_integrity()
    {
        bool error = false;
        if (_buffers.empty())
        {
            std::cerr << LOG_ERROR
                      << "mesh object renderer builder has no vertex "
                         "buffer, which is mandatory.\n";
            error = true;
        }
        if (!_shader)
        {
            std::cerr << LOG_ERROR
                      << "mesh object renderer builder has no "
                         "associated shader program, which is mandatory.\n";
            error = true;
        }
        else
        {
            auto transform_u =
                (*_shader)->uniform(definitions::MODEL_TRANSFORM_UNIFORM_NAME);
            if (!transform_u)
            {
                std::cerr << LOG_WARNING
                          << "Mesh renderer shader does not have a "
                             "valid active model transform uniform.\n";
            }
        }
        if (_attribute_config.empty())
        {
            std::cerr << LOG_ERROR
                      << "no attribute has been specified in mesh "
                         "renderer builder.\n";
            error = true;
        }
        bool missing_attribute_configs = false;
        for (size_t id = 0; id < _buffers.size(); ++id)
        {
            if (!_attribute_config.contains(id))
            {
                missing_attribute_configs = true;
            }
        }

        if (missing_attribute_configs)
        {
            std::cerr
                << LOG_ERROR
                << "Some buffers are declares in mesh object renderer "
                   "builder, but no attribute has been defined for them.\n";
            error = true;
        }

        if (error)
        {
            throw std::logic_error(
                "Error while building renderer, check stderr.");
        }
    }

    std::shared_ptr<MeshRenderer> Self::build()
    {
        assert_integrity();

        VaoType vao_id = 0;
        const auto prog = (*_shader)->get_program();

        glGenVertexArrays(1, &vao_id);
        CHECK_GL_ERROR();

        glBindVertexArray(vao_id);
        CHECK_GL_ERROR();

        auto buffer_ids = std::vector<GLuint>(_buffers.size());

        glGenBuffers(_buffers.size(), buffer_ids.data());
        CHECK_GL_ERROR();
        std::vector<size_t> strides(buffer_ids.size());
        for (size_t i = 0; i < buffer_ids.size(); ++i)
        {
            glBindBuffer(GL_ARRAY_BUFFER, buffer_ids[i]);
            CHECK_GL_ERROR();
            glBufferData(GL_ARRAY_BUFFER, _buffers[i].size() * sizeof(GLfloat),
                         _buffers[i].data(), GL_STATIC_DRAW);
            CHECK_GL_ERROR();
            size_t stride = 0;
            for (auto [_, size] : _attribute_config.at(i))
            {
                stride += size;
            }
            strides[i] = stride;

            size_t offset = 0;
            for (auto [name, size] : _attribute_config.at(i))
            {
                const auto location = glGetAttribLocation(prog, name.c_str());
                CHECK_GL_ERROR();
                if (location == -1)
                {
                    std::cerr << LOG_ERROR << "attribute `" << name
                              << "` could not be found in shader program.\n";
                }
                glVertexAttribPointer(location, size, GL_FLOAT, GL_FALSE,
                                      stride * sizeof(GLfloat),
                                      BUFFER_OFFSET(offset));
                CHECK_GL_ERROR();
                glEnableVertexAttribArray(location);
                CHECK_GL_ERROR();
            }
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        CHECK_GL_ERROR();

        glBindVertexArray(0);
        CHECK_GL_ERROR();

        return std::make_shared<MeshRenderer>(
            vao_id, _draw_mode, *_shader, _buffers[0].size() / strides[0],
            buffer_ids, _transform,
            (*_shader)->uniform(definitions::MODEL_TRANSFORM_UNIFORM_NAME));
    }

} // namespace pogl
