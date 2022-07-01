#include "mesh_object_renderer.hh"

#include <iostream>

#include "utils/gl_check.hh"

#define BUFFER_OFFSET(i) ((void *)(i))

namespace pogl
{
    using Self = MeshObjectRenderer;
    MeshObjectRenderer::MeshObjectRenderer(VaoType vao_id,
                                           DrawModeType draw_mode,
                                           const ShaderType &shader,
                                           size_t vertex_array_index_count)
        : shader_(shader)
        , vao_id_(vao_id)
        , draw_mode_(draw_mode)
        , vertex_array_index_count_(vertex_array_index_count)
    {
        // assume all objects coordinates are already in worldspace
        // or that the object's anchor is at 0,0,0
        auto transform = shader_->uniform("model_transform_matrix");
        if (transform)
        {
            transform->set_mat4(Matrix4::identity());
        }
    }

    Self::Builder MeshObjectRenderer::builder()
    {
        return Builder();
    }

    void MeshObjectRenderer::draw()
    {
        shader_->use();
        glBindVertexArray(vao_id_);
        CHECK_GL_ERROR();
        glDrawArrays(draw_mode_, 0, vertex_array_index_count_);
        CHECK_GL_ERROR();
        glBindVertexArray(0);
        CHECK_GL_ERROR();
    }

    void MeshObjectRenderer::update_camera(const Matrix4 &camera_transform)
    {
        shader_->uniform("view_transform_matrix")->set_mat4(camera_transform);
    }

    Self::Builder::Builder()
        : buffers_()
        , shader_(std::nullopt)
        , draw_mode_(GL_TRIANGLES)
        , attribute_config_()
    {}

    Self::Builder &Self::Builder::add_buffer(const BufferType &buffer)
    {
        buffers_.push_back(buffer);
        return *this;
    }

    Self::Builder &Self::Builder::shader(const ShaderType &shader)
    {
        shader_ = shader;
        return *this;
    }

    Self::Builder &Self::Builder::draw_mode(DrawModeType draw_mode)
    {
        draw_mode_ = draw_mode;
        return *this;
    }

    Self::Builder &Self::Builder::add_attribute(std::string name, int size,
                                                size_t buffer_id)
    {
        if (!attribute_config_.contains(buffer_id))
        {
            attribute_config_.emplace(buffer_id,
                                      std::vector<AttributeConfigType>());
        }
        attribute_config_.at(buffer_id).emplace_back(name, size);
        return *this;
    }

    void Self::Builder::assert_integrity()
    {
        bool error = false;
        if (buffers_.empty())
        {
            std::cerr << "Error: mesh object renderer builder has no vertex "
                         "buffer, which is mandatory.\n";
            error = true;
        }
        if (!shader_)
        {
            std::cerr << "Error: mesh object renderer builder has no "
                         "associated shader program, which is mandatory.\n";
            error = true;
        }
        if (attribute_config_.empty())
        {
            std::cerr << "Error: no attribute has been specified in mesh "
                         "renderer builder.\n";
            error = true;
        }
        bool missing_attribute_configs = false;
        for (size_t id = 0; id < buffers_.size(); ++id)
        {
            if (!attribute_config_.contains(id))
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

    std::shared_ptr<MeshObjectRenderer> Self::Builder::build()
    {
        assert_integrity();

        VaoType vao_id = 0;
        const auto prog = (*shader_)->get_program();

        glGenVertexArrays(1, &vao_id);
        CHECK_GL_ERROR();

        glBindVertexArray(vao_id);
        CHECK_GL_ERROR();

        auto buffer_ids = std::vector<GLuint>(buffers_.size());

        glGenBuffers(buffers_.size(), buffer_ids.data());
        CHECK_GL_ERROR();

        for (size_t i = 0; i < buffer_ids.size(); ++i)
        {
            glBindBuffer(GL_ARRAY_BUFFER, buffer_ids[i]);
            CHECK_GL_ERROR();
            glBufferData(GL_ARRAY_BUFFER, buffers_[i].size() * sizeof(GLfloat),
                         buffers_[i].data(), GL_STATIC_DRAW);
            CHECK_GL_ERROR();
            size_t stride = 0;
            for (auto [_, size] : attribute_config_.at(i))
            {
                stride += size;
            }

            size_t offset = 0;
            for (auto [name, size] : attribute_config_.at(i))
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

        return std::make_shared<MeshObjectRenderer>(
            vao_id, draw_mode_, *shader_, buffers_[0].size());
    }

} // namespace pogl
