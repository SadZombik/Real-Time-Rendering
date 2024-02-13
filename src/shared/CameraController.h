#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include <memory>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

enum class ArrowKeysFunction {
    NONE,
    MOVEMENT,
    VIEW
};

class CameraController {
public:
    CameraController(int width, int height);
    ~CameraController();

    glm::mat4 GetPerspectiveMatrix() const;
    glm::mat4 GetViewMatrix() const;

    void Update();
    void KeyboardCallback(GLFWwindow* window);
    void ArrowKeysCallback(GLFWwindow* window, ArrowKeysFunction func = ArrowKeysFunction::MOVEMENT);
    
    void SetRatio(float newRatio);
    void SetYaw(float yaw);
    void SetPitch(float pitch);

private:
    void CalculateFronVector();

    struct Camera;
    std::shared_ptr<Camera> m_Camera;
    
    float m_CameraSpeed = 2.5f;
    float m_CurrentCameraSpeed = 0.0f;
    float m_Sensitivity = 0.7f;

    float m_DeltaTime;
	float m_LastFrame;
};

#endif // !CAMERA_CONTROLLER_H
