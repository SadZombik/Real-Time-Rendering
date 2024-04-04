#include "Cylinder.h"
#include "../Framework.h"
#include "../Utils.h"

#include <vector>

static int NUM_VERTICES;
static int NUM_INDICES; 
static int NUM_INSTANCES;

static std::vector<float> vertices;
static std::vector<int> indices;
static std::vector<float> positions;
static bool isGenerated = false;

void Cylinder::GenerateVertices(float radius, float height, int circle_vertices, int circle_instances) {
    // "* 2" was added for cylinder shape
    NUM_VERTICES = (circle_vertices + 1) * 3 * 2;
    NUM_INDICES = circle_vertices * 3 * 2;
    NUM_INSTANCES = circle_instances * 3;

    const float angle = 360.0f / circle_vertices;
    const int num_indices_2 = NUM_INDICES / 2;
    const int num_vertices_2 = NUM_VERTICES / 2;

    vertices.resize(NUM_VERTICES);
    indices.resize(NUM_INDICES);
    positions.resize(NUM_INSTANCES);

    // origin
    vertices[0] = 0.0f; // x
    vertices[1] = 0.0f; // y
    vertices[2] = 0.0f; // z
    vertices[num_vertices_2]     = 0.0f; // x
    vertices[num_vertices_2 + 1] = 0.0f; // y
    vertices[num_vertices_2 + 2] = height; // z

    indices[0] = 0;
    indices[1] = 1;
    indices[2] = circle_vertices;
    indices[num_indices_2]     = num_indices_2 / 3 + 1;
    indices[num_indices_2 + 1] = num_indices_2 / 3 + 1 + 1;
    indices[num_indices_2 + 2] = num_indices_2 / 3 + 1 + circle_vertices;

    positions[0] = 0.0f;
    positions[1] = 0.0f;
    positions[2] = 0.0f;

    for (auto i = 1; i < circle_vertices + 1; ++i) {
        const float currentAngle = angle * (i - 1);
        const float x = radius * cos(glm::radians(currentAngle));
        const float y = radius * sin(glm::radians(currentAngle));
        const float z = 0.0f;

        vertices[i * 3]     = x;
        vertices[i * 3 + 1] = y;
        vertices[i * 3 + 2] = z;

        vertices[i * 3 + num_vertices_2]     = x;
        vertices[i * 3 + num_vertices_2 + 1] = y;
        vertices[i * 3 + num_vertices_2 + 2] = height;

        if (i < circle_vertices) {
            indices[i * 3]     = 0;
            indices[i * 3 + 1] = i + 1;
            indices[i * 3 + 2] = i;

            indices[i * 3 + num_indices_2]     = i;
            indices[i * 3 + num_indices_2 + 1] = indices[i * 3 + 1] + circle_vertices + 1;
            indices[i * 3 + num_indices_2 + 2] = indices[i * 3 + 2] + circle_vertices + 1;
        }
    }

    for (auto i = 1; i < circle_instances; ++i) {
        positions[i * 3]     = 0.0f;
        positions[i * 3 + 1] = 0.0f;
        positions[i * 3 + 2] = (float)i;
    }
    BindBuffers();
    isGenerated = true;
}

Cylinder::Cylinder() {
    m_Shader = Shader(
        res_dir + "/shaders/cylinder/vertex.glsl", 
        res_dir + "/shaders/3d/fragment_no_texture.glsl"
    );
    m_Shader.Use();

    GenerateVertices(1.0f, 1.0f, 6, 1);

    m_Color[0] = 128;
    m_Color[1] = 128;
    m_Color[2] = 128;
}

void Cylinder::BindBuffers() {
    m_InstanceVBO.Bind();
    m_InstanceVBO.SetData(NUM_INSTANCES * sizeof(float), positions.data());
    m_InstanceVBO.Unbind();

    m_VAO.Bind();
    m_VBO.Bind();
    m_VBO.SetData(NUM_VERTICES * sizeof(float), vertices.data());

    m_EBO.Bind();
    m_EBO.SetData(NUM_INDICES * sizeof(int), indices.data());

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    m_InstanceVBO.Bind();
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    m_InstanceVBO.Unbind();
    glVertexAttribDivisor(1, 1);
}

void Cylinder::Update(const CameraController& cam) {
    m_Shader.Use();
    m_Shader.SetMat4("projection", cam.GetPerspectiveMatrix());
    m_Shader.SetMat4("view", cam.GetViewMatrix());
    m_Shader.SetMat4("model", m_Model);
    m_Shader.SetVec3("in_color", m_Color[0], m_Color[1], m_Color[2]);

    m_VAO.Bind();
    glDrawElementsInstanced(m_RenderMode, NUM_INDICES, GL_UNSIGNED_INT, 0, NUM_INSTANCES / 3);
    m_VAO.Unbind();
}

void Cylinder::SetShaders(const std::string& vertexPath, const std::string& fragmentPath) {
    m_Shader = Shader(vertexPath, fragmentPath);
    m_Shader.Use();
}

void Cylinder::SetModelMatrix(const glm::mat4& m) {
    m_Model = m;
}

void Cylinder::SetColor(float* newColor) {
    memcpy(m_Color, newColor, sizeof(m_Color));
}

void Cylinder::SetRenderMode(GLenum mode) {
    m_RenderMode = mode;
}
