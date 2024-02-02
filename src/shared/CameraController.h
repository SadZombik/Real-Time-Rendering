#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include <memory>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class CameraController {
public:
    CameraController(int width, int height);
    ~CameraController();

    glm::mat4 GetPerspectiveMatrix() const;
    glm::mat4 GetViewMatrix() const;

    void Update();
    void KeyboardCallback(GLFWwindow* window);

private:
    struct Camera;
    std::shared_ptr<Camera> m_Camera;

    float m_DeltaTime;
	float m_LastFrame;
};

#endif // !CAMERA_CONTROLLER_H
