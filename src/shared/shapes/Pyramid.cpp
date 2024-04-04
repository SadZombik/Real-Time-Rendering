#include "Pyramid.h"
#include "../Framework.h"

constexpr static float vertices[] = {
    // base
        // first triangle
        // position           // tex coords
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f, // top right
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, // top left 
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom right

        // second triangle
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom right
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, // top left
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom left

    // walls
        // right
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
         0.0f,  0.5f,  0.0f,  0.5f, 0.5f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,

        // left
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
         0.0f,  0.5f,  0.0f,  0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        // near
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
         0.0f,  0.5f,  0.0f,  0.5f, 0.5f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,

        // far
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.0f,  0.5f,  0.0f,  0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f
};

constexpr static unsigned NUM_VERTICES = sizeof(vertices) / sizeof(float) / 5;

Pyramid::Pyramid() {
    m_Texture = Texture(res_dir + "/textures/3d/wood.png");
    m_Texture.Bind(0);

    m_Shader = Shader(
        res_dir + "/shaders/3d/vertex.glsl", 
        res_dir + "/shaders/3d/fragment.glsl"
    );
    m_Shader.Use();

    m_VAO.Bind();
    m_VBO.Bind();
    m_VBO.SetData(sizeof(vertices), vertices);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    m_Color[0] = 128;
    m_Color[1] = 128;
    m_Color[2] = 128;
}

void Pyramid::SetModelMatrix(const glm::mat4& m) {
    m_Model = m;
}

void Pyramid::Update(const CameraController& cam) {
    m_Shader.Use();
    m_Shader.SetMat4("projection", cam.GetPerspectiveMatrix());
    m_Shader.SetMat4("view", cam.GetViewMatrix());
    m_Shader.SetMat4("model", m_Model);
    m_Shader.SetVec3("in_color", m_Color[0], m_Color[1], m_Color[2]);

    m_VAO.Bind();
    glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES);
    m_VAO.Unbind();
}

void Pyramid::SetColor(float* newColor) {
    memcpy(m_Color, newColor, sizeof(m_Color));
}

void Pyramid::SetShaders(const std::string& vertexPath, const std::string& fragmentPath) {
    m_Shader = Shader(vertexPath, fragmentPath);
    m_Shader.Use();
    //shader.SetInt("texture_sampler", 0);
}
