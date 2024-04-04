#ifndef CYLINDER_H
#define CYLINDER_H

#include "../OpenGlObjects.h"
#include "../Shader.h"
#include "../CameraController.h"

class Cylinder {
public:
    Cylinder();

    void Update(const CameraController& cam);

    void SetShaders(const std::string& vertexPath, const std::string& fragmentPath);
    void SetModelMatrix(const glm::mat4& m);
    void SetColor(float* newColor);
    void SetRenderMode(GLenum mode);
    void GenerateVertices(float radius = 1.0f, float height = 1.0f, int circleVertices = 32, int circleInstances = 1);

private:
    void BindBuffers();

    glm::mat4 m_Model;
    Shader m_Shader;
    VertexBufferObject m_VBO;
    VertexArrayObject m_VAO;
    ElementsBufferObject m_EBO;
    VertexBufferObject m_InstanceVBO;
    float m_LineWidth;
    float m_Color[3];
    GLenum m_RenderMode = GL_LINE_STRIP;
};

#endif // !CYLINDER_H
