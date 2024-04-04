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
    glm::mat4 m_Model;
    Shader m_Shader;
    VertexBufferObject m_VBO;
    VertexArrayObject m_VAO;
    Texture m_Texture;
    float m_Color[3];
};

#endif // !PYRAMID_H
