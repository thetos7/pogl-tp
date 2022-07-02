#include "mesh_renderer.hh"

#include "utils/gl_check.hh"
#include "utils/definitions.hh"

#define BUFFER_OFFSET(i) ((void *)(i))

namespace pogl
{
    using Self = MeshRenderer;
    MeshRenderer::MeshRenderer(VaoType vao_id, DrawModeType draw_mode,
                               const ShaderType &shader,
                               size_t vertex_array_index_count,
                               std::vector<GLuint> buffer_ids)
        : _shader(shader)
        , _vao_id(vao_id)
        , _draw_mode(draw_mode)
        , _vertex_array_index_count(vertex_array_index_count)
        , _buffer_ids(buffer_ids)
    {
        // assume all objects coordinates are already in worldspace
        // or that the object's anchor is at 0,0,0
        auto transform = _shader->uniform(definitions::MODEL_TRANSFORM_UNIFORM_NAME);
        if (transform)
        {
            transform->set_mat4(Matrix4::identity());
        }
    }
    MeshRenderer::~MeshRenderer()
    {
        glDeleteBuffers(_buffer_ids.size(), _buffer_ids.data());
        glDeleteVertexArrays(1, &_vao_id);
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
        _shader->uniform(definitions::VIEW_TRANSFORM_UNIFORM_NAME)->set_mat4(camera_transform);
    }
} // namespace pogl
