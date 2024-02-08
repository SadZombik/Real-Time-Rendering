#ifndef PYRAMID_H
#define PYRAMID_H

#include "../OpenGlObjects.h"
#include "../Shader.h"
#include "../CameraController.h"

class Pyramid {
public:
    Pyramid();

    
    void Update(const CameraController& cam);

    void SetShaders(const std::string& vertexPath, const std::string& fragmentPath);
    void SetModelMatrix(const glm::mat4& m);
    void SetColor(float* newColor);

private:
    glm::mat4 model;
    Shader shader;
    VertexBufferObject VBO;
    VertexArrayObject VAO;
    Texture texture;
    float color[3];
};

#endif // !PYRAMID_H
