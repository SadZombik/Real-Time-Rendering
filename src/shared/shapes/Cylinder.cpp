#include "Cylinder.h"
#include "../Framework.h"
#include "../Utils.h"

#include <vector>
#include <array>

static constexpr int CIRCLE_VERTICES = 32;
static constexpr int NUM_ELEMENTS = (CIRCLE_VERTICES + 1) * 3;
static constexpr int NUM_INDICES = CIRCLE_VERTICES * 3;

static std::array<float, NUM_ELEMENTS> vertices;
static std::array<int, NUM_INDICES> indices;

static void GenerateVertices(float radius) {
    constexpr float angle = 360.0f / CIRCLE_VERTICES;

    // origin
    vertices[0] = 0.0f; // x
    vertices[1] = 0.0f; // y
    vertices[2] = 0.0f; // z

    indices[0] = 0;
    indices[1] = 1;
    indices[2] = CIRCLE_VERTICES;

    // positions
    for (auto i = 1; i < CIRCLE_VERTICES + 1; ++i) {
        const float currentAngle = angle * (i - 1);
        const float x = radius * cos(glm::radians(currentAngle));
        const float y = radius * sin(glm::radians(currentAngle));
        const float z = 0.0f;

        vertices[(i * 3)]     = x;
        vertices[(i * 3) + 1] = y;
        vertices[(i * 3) + 2] = z;

        if (i < CIRCLE_VERTICES) {
            indices[(i * 3)]     = 0;
            indices[(i * 3) + 1] = i + 1;
            indices[(i * 3) + 2] = i;
        }
    }
}

Cylinder::Cylinder() {
    GenerateVertices(1.0f);

    shader = Shader(
        res_dir + "/shaders/3d/vertex_no_texture.glsl", 
        res_dir + "/shaders/3d/fragment_no_texture.glsl"
    );
    shader.Use();

    VAO.Bind();
    VBO.Bind();

    VBO.SetData(sizeof(vertices), vertices.data());

    EBO.Bind();
    EBO.SetData(sizeof(indices), indices.data());

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    color[0] = 128;
    color[1] = 128;
    color[2] = 128;
}

void Cylinder::Update(const CameraController& cam) {
    shader.Use();
    shader.SetMat4("projection", cam.GetPerspectiveMatrix());
    shader.SetMat4("view", cam.GetViewMatrix());
    shader.SetMat4("model", model);
    shader.SetVec3("in_color", color[0], color[1], color[2]);

    VAO.Bind();
    glDrawElements(renderMode, NUM_INDICES * sizeof(int), GL_UNSIGNED_INT, 0);
    VAO.Unbind();
}

void Cylinder::SetShaders(const std::string& vertexPath, const std::string& fragmentPath) {
    shader = Shader(vertexPath, fragmentPath);
    shader.Use();
}

void Cylinder::SetModelMatrix(const glm::mat4& m) {
    model = m;
}

void Cylinder::SetColor(float* newColor) {
    memcpy(color, newColor, sizeof(color));
}

void Cylinder::SetRenderMode(GLenum mode) {
    renderMode = mode;
}
