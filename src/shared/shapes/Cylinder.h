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

private:
    glm::mat4 model;
    Shader shader;
    VertexBufferObject VBO;
    VertexArrayObject VAO;
    float color[3];
};

#endif // !CYLINDER_H
