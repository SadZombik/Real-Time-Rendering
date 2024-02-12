#include "Cylinder.h"
#include "../Framework.h"
#include "../Utils.h"

#include <array>

static auto GenerateVertices(float radius, int vCount)
{
    float angle = 360.0f / vCount;
    int triangleCount = vCount - 2;

    std::vector<glm::vec3> vertices;

    // positions
    for (int i = 0; i < vCount; i++)
    {
        float currentAngle = angle * i;
        float x = radius * cos(glm::radians(currentAngle));
        float y = radius * sin(glm::radians(currentAngle));
        float z = 0.0f;

        vertices.push_back(glm::vec3(x, y, z));
    }

    vertices.push_back(vertices[0]);
    return vertices;
}

static constexpr int NUM_VERTICES = 32;

static auto vertices = GenerateVertices(1, NUM_VERTICES);

Cylinder::Cylinder() {
    // texture = Texture(res_dir + "/textures/3d/wood.png");
    // texture.Bind(0);

    shader = Shader(
        res_dir + "/shaders/3d/vertex_no_texture.glsl", 
        res_dir + "/shaders/3d/fragment_no_texture.glsl"
    );
    shader.Use();

    const auto pVertices = &vertices[0];

    VAO.Bind();
    VBO.Bind();
    VBO.SetData(sizeof(glm::vec3) * vertices.size(), pVertices);

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
    glDrawArrays(GL_LINE_STRIP, 0, NUM_VERTICES);
    VAO.Unbind();
}

void Cylinder::SetShaders(const std::string& vertexPath, const std::string& fragmentPath) {
    shader = Shader(vertexPath, fragmentPath);
    shader.Use();
    shader.SetInt("texture", 0);
}

void Cylinder::SetModelMatrix(const glm::mat4& m) {
    model = m;
}

void Cylinder::SetColor(float* newColor) {
    memcpy(color, newColor, sizeof(color));
}
