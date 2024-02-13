#include "Cylinder.h"
#include "../Framework.h"
#include "../Utils.h"

#include <vector>
#include <array>

static int NUM_VERTICES;
static int NUM_INDICES; 
static int NUM_INSTANCES;

static std::vector<float> vertices;
static std::vector<int> indices;
static std::vector<float> positions;

static bool isGenerated = false;

void Cylinder::GenerateVertices(float radius, int circle_vertices, int circle_instances) {
    NUM_VERTICES = (circle_vertices + 1) * 3;
    NUM_INDICES = circle_vertices * 3;
    NUM_INSTANCES = circle_instances * 3;

    const float angle = 360.0f / circle_vertices;

    vertices.resize(NUM_VERTICES);
    indices.resize(NUM_INDICES);
    positions.resize(NUM_INSTANCES);

    // origin
    vertices[0] = 0.0f; // x
    vertices[1] = 0.0f; // y
    vertices[2] = 0.0f; // z

    indices[0] = 0;
    indices[1] = 1;
    indices[2] = circle_vertices;

    positions[0] = 0.0f;
    positions[1] = 0.0f;
    positions[2] = 0.0f;

    for (auto i = 1; i < circle_vertices + 1; ++i) {
        const float currentAngle = angle * (i - 1);
        const float x = radius * cos(glm::radians(currentAngle));
        const float y = radius * sin(glm::radians(currentAngle));
        const float z = 0.0f;

        vertices[(i * 3)]     = x;
        vertices[(i * 3) + 1] = y;
        vertices[(i * 3) + 2] = z;

        if (i < circle_vertices) {
            indices[(i * 3)]     = 0;
            indices[(i * 3) + 1] = i + 1;
            indices[(i * 3) + 2] = i;
        }
    }

    for (auto i = 1; i < circle_instances; ++i) {
        positions[(i * 3)]     = 0.0f;
        positions[(i * 3) + 1] = 0.0f;
        positions[(i * 3) + 2] = (float)i;
    }

    // std::cout << "positions.size() = " << positions.size();
    // for (auto i = 0; i < positions.size(); ++i) {
    //     if (i % 3 == 0) std::cout << '\n';
    //     std::cout << positions[i] << " ";
    // }
    BindBuffers();
    isGenerated = true;
}

Cylinder::Cylinder() {
    shader = Shader(
        res_dir + "/shaders/cylinder/vertex.glsl", 
        res_dir + "/shaders/3d/fragment_no_texture.glsl"
    );
    shader.Use();

    GenerateVertices(1.0f, 32, 3);

    color[0] = 128;
    color[1] = 128;
    color[2] = 128;
}

void Cylinder::BindBuffers() {
    instanceVBO.Bind();
    instanceVBO.SetData(NUM_INSTANCES * sizeof(float), positions.data());
    instanceVBO.Unbind();

    VAO.Bind();
    VBO.Bind();
    VBO.SetData(NUM_VERTICES * sizeof(float), vertices.data());

    EBO.Bind();
    EBO.SetData(NUM_INDICES * sizeof(int), indices.data());

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    instanceVBO.Bind();
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    instanceVBO.Unbind();
    glVertexAttribDivisor(1, 1);
}

void Cylinder::Update(const CameraController& cam) {
    shader.Use();
    shader.SetMat4("projection", cam.GetPerspectiveMatrix());
    shader.SetMat4("view", cam.GetViewMatrix());
    shader.SetMat4("model", model);
    shader.SetVec3("in_color", color[0], color[1], color[2]);

    VAO.Bind();
    glDrawElementsInstanced(renderMode, NUM_INDICES, GL_UNSIGNED_INT, 0, NUM_INSTANCES / 3);
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
