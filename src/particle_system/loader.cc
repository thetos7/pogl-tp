#include "loader.hh"

namespace pogl {
    RawModel Loader::LoadVAO(std::shared_ptr<ShaderProgram> shader, std::vector<float> positions, size_t particle_num) {
        this->shader = shader;
        GLuint VAO = createVAO();

        std::vector<GLuint> VBO_ids;
        createVBO(VBO_ids);
        Attribute();
        storeData(VBO_ids[0], positions, VBO_ids);
        unbindVBO();
        unbindVAO();
        return RawModel(VAO, particle_num, VBO_ids);
    }

    GLuint Loader::createVAO() {
        GLuint VAO;
        glGenVertexArrays(1, &VAO);
        CHECK_GL_ERROR();
        glBindVertexArray(VAO);
        CHECK_GL_ERROR();
        return VAO;
    }

    void Loader::storeData(int VBO_id, std::vector<float> positions, std::vector<GLuint> &VBO_ids) {
        glBindBuffer(GL_ARRAY_BUFFER, VBO_id);
        CHECK_GL_ERROR();
        glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_DYNAMIC_DRAW);
        CHECK_GL_ERROR();
    }

    void Loader::createVBO(std::vector<GLuint> &VBO_ids) {
        GLuint VBO;
        glGenBuffers(1, &VBO);
        CHECK_GL_ERROR();
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        CHECK_GL_ERROR();
        VBO_ids.push_back(VBO);
    }

    void Loader::Attribute() {
        auto program_id = shader->get_program();        
        const auto location = glGetAttribLocation(program_id, "vPosition");
        CHECK_GL_ERROR();
        if (location == -1) {
            std::cerr << "ParticleSystem : Attribute not found" << std::endl;
        }
        glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, 0);
        CHECK_GL_ERROR();
        glEnableVertexAttribArray(location);
        CHECK_GL_ERROR();
    }

    void Loader::unbindVBO() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        CHECK_GL_ERROR();
    }

    void Loader::unbindVAO() {
        glBindVertexArray(0);
        CHECK_GL_ERROR();
    }
}