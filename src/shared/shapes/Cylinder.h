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

private:
    glm::mat4 model;
    Shader shader;
    VertexBufferObject VBO;
    VertexArrayObject VAO;
    ElementsBufferObject EBO;
    VertexBufferObject instanceVBO;
    float lineWidth;
    float color[3];
    GLenum renderMode = GL_LINE_STRIP;
};

#endif // !CYLINDER_H
