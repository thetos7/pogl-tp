#include "mesh_renderer.hh"

#include <iostream>

#include "utils/gl_check.hh"

#define BUFFER_OFFSET(i) ((void *)(i))

namespace pogl
{
    using Self = MeshRenderer;
    MeshRenderer::MeshRenderer(VaoType vao_id,
                                           DrawModeType draw_mode,
                                           const ShaderType &shader,
                                           size_t vertex_array_index_count)
        : _shader(shader)
        , _vao_id(vao_id)
        , _draw_mode(draw_mode)
        , _vertex_array_index_count(vertex_array_index_count)
    {
        // assume all objects coordinates are already in worldspace
        // or that the object's anchor is at 0,0,0
        auto transform = _shader->uniform("model_transform_matrix");
        if (transform)
        {
            transform->set_mat4(Matrix4::identity());
        }
    }

    Self::Builder MeshRenderer::builder()
    {
        return Builder();
    }

    void MeshRenderer::draw()
    {
        _shader->use();
        glBindVertexArray(_vao_id);
        CHECK_GL_ERROR();
        glDrawArrays(_draw_mode, 0, _vertex_array_index_count);
        CHECK_GL_ERROR();
        glBindVertexArray(0);
        CHECK_GL_ERROR();
    }

    void MeshRenderer::update_camera(const Matrix4 &camera_transform)
    {
        _shader->uniform("view_transform_matrix")->set_mat4(camera_transform);
    }

    Self::Builder::Builder()
        : _buffers()
        , _shader(std::nullopt)
        , _draw_mode(GL_TRIANGLES)
        , _attribute_config()
    {}

    Self::Builder &Self::Builder::add_buffer(const BufferType &buffer)
    {
        _buffers.push_back(buffer);
        return *this;
    }

    Self::Builder &Self::Builder::shader(const ShaderType &shader)
    {
        _shader = shader;
        return *this;
    }

    Self::Builder &Self::Builder::draw_mode(DrawModeType draw_mode)
    {
        _draw_mode = draw_mode;
        return *this;
    }

    Self::Builder &Self::Builder::add_attribute(std::string name, int size,
                                                size_t buffer_id)
    {
        if (!_attribute_config.contains(buffer_id))
        {
            _attribute_config.emplace(buffer_id,
                                      std::vector<AttributeConfigType>());
        }
        _attribute_config.at(buffer_id).emplace_back(name, size);
        return *this;
    }

    void Self::Builder::assert_integrity()
    {
        bool error = false;
        if (_buffers.empty())
        {
            std::cerr << "Error: mesh object renderer builder has no vertex "
                         "buffer, which is mandatory.\n";
            error = true;
        }
        if (!_shader)
        {
            std::cerr << "Error: mesh object renderer builder has no "
                         "associated shader program, which is mandatory.\n";
            error = true;
        }
        if (_attribute_config.empty())
        {
            std::cerr << "Error: no attribute has been specified in mesh "
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
                << "Error: Some buffers are declares in mesh object renderer "
                   "builder, but no attribute has been defined for them.\n";
            error = true;
        }

        if (error)
        {
            throw std::logic_error(
                "Error while building renderer, check stderr.");
        }
    }

    std::shared_ptr<MeshRenderer> Self::Builder::build()
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

            size_t offset = 0;
            for (auto [name, size] : _attribute_config.at(i))
            {
                const auto location = glGetAttribLocation(prog, name.c_str());
                CHECK_GL_ERROR();
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
            vao_id, _draw_mode, *_shader, _buffers[0].size());
    }

} // namespace pogl
