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
    texture = Texture(res_dir + "/textures/3d/wood.png");
    texture.Bind(0);

    shader = Shader(
        res_dir + "/shaders/3d/vertex.glsl", 
        res_dir + "/shaders/3d/fragment.glsl"
    );
    shader.Use();

    VAO.Bind();
    VBO.Bind();
    VBO.SetData(sizeof(vertices), vertices);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    color[0] = 128;
    color[1] = 128;
    color[2] = 128;
}

void Pyramid::SetModelMatrix(const glm::mat4& m) {
    model = m;
}

void Pyramid::Update(const CameraController& cam) {
    shader.Use();
    shader.SetMat4("projection", cam.GetPerspectiveMatrix());
    shader.SetMat4("view", cam.GetViewMatrix());
    shader.SetMat4("model", model);
    shader.SetVec3("in_color", color[0], color[1], color[2]);

    VAO.Bind();
    glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES);
    VAO.Unbind();
}

void Pyramid::SetColor(float* newColor) {
    memcpy(color, newColor, sizeof(color));
}

void Pyramid::SetShaders(const std::string& vertexPath, const std::string& fragmentPath) {
    shader = Shader(vertexPath, fragmentPath);
    shader.Use();
    shader.SetInt("texture", 0);
}
