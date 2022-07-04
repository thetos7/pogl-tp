#include "mesh_renderer.hh"

#include "utils/definitions.hh"
#include "utils/gl_check.hh"

#define BUFFER_OFFSET(i) ((void *)(i))

namespace pogl
{
    using Self = MeshRenderer;
    MeshRenderer::MeshRenderer(VaoType vao_id, DrawModeType draw_mode,
                               const ShaderType &shader,
                               size_t vertex_array_index_count,
                               std::vector<GLuint> buffer_ids,
                               const Matrix4 &transform, UniformType transform_uniform)
        : _shader(shader)
        , _vao_id(vao_id)
        , _draw_mode(draw_mode)
        , _vertex_array_index_count(vertex_array_index_count)
        , _buffer_ids(buffer_ids)
        , _transform(transform)
        , _transform_uniform(transform_uniform)
    {}

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
        if (_transform_uniform)
        {
            _transform_uniform->set_mat4(_transform);
        }
        glBindVertexArray(_vao_id);
        CHECK_GL_ERROR();
        glDrawArrays(_draw_mode, 0, _vertex_array_index_count);
        CHECK_GL_ERROR();
        glBindVertexArray(0);
        CHECK_GL_ERROR();
    }

    void MeshRenderer::update_camera(const Matrix4 &camera_transform)
    {
        _shader->uniform(definitions::VIEW_TRANSFORM_UNIFORM_NAME)
            ->set_mat4(camera_transform);
    }

    void MeshRenderer::set_transform(const Matrix4 &transform)
    {
        _transform = transform;
    }

    Self::ShaderType MeshRenderer::shader()
    {
        return _shader;
    }
} // namespace pogl
