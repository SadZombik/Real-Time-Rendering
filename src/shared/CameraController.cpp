#include "CameraController.h"

#include <glm/gtc/matrix_transform.hpp>

struct CameraController::Camera {
    glm::vec3 Position;
	glm::vec3 FrontVector;
	glm::vec3 UpVector;

	float yaw;
	float pitch;
	float lastX;
	float lastY;
	float fov;
	bool  firstMouse;
    float aspect;
};

CameraController::CameraController(int width, int height) {
	m_DeltaTime = 0.0f;
	m_LastFrame = 0.0f;

    m_Camera = std::make_shared<Camera>();

    m_Camera->Position    = glm::vec3(0.0f, 0.0f, 3.0f);
    m_Camera->FrontVector = glm::vec3(0.0f, 0.0f, -1.0f);
    m_Camera->UpVector    = glm::vec3(0.0f, 1.0f, 0.0f);
    m_Camera->yaw         = 0.0f;
    m_Camera->pitch       = -45.0f;
    m_Camera->lastX       = (float)width  / 2.0f;
    m_Camera->lastY       = (float)height / 2.0f;
    m_Camera->fov         = 45.0f;
    m_Camera->firstMouse  = true;
    m_Camera->aspect      = (float)width / (float)height;
}

CameraController::~CameraController() {

}

glm::mat4 CameraController::GetPerspectiveMatrix() const
{
	return glm::perspective(
		glm::radians(m_Camera->fov),
		m_Camera->aspect,
		0.1f, 
		100.0f
	);
}

glm::mat4 CameraController::GetViewMatrix() const
{
	return glm::lookAt(
		m_Camera->Position,
		m_Camera->Position + m_Camera->FrontVector,
		m_Camera->UpVector
	);
}

void CameraController::Update() {
	float currentFrame = (float)glfwGetTime();
	m_DeltaTime = currentFrame - m_LastFrame;
	m_LastFrame = currentFrame;

	m_CameraSpeed *= m_DeltaTime;
}

void CameraController::SetRatio(float newRatio) {
	m_Camera->aspect = newRatio;
}

void CameraController::ArrowKeysCallback(GLFWwindow* window, ArrowKeysFunction func = ArrowKeysFunction::MOVEMENT) {
	switch (func) {
	case ArrowKeysFunction::NONE:
		break;
	case ArrowKeysFunction::MOVEMENT:
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			m_Camera->Position += m_CameraSpeed * m_Camera->FrontVector;
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			m_Camera->Position -= m_CameraSpeed * m_Camera->FrontVector;
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			m_Camera->Position -= glm::normalize(glm::cross(m_Camera->FrontVector, m_Camera->UpVector)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			m_Camera->Position += glm::normalize(glm::cross(m_Camera->FrontVector, m_Camera->UpVector)) * cameraSpeed;	
		break;
	case ArrowKeysFunction::VIEW:

			glm::vec3 front = glm::vec3(
				cos(glm::radians(m_camera->yaw)) * cos(glm::radians(m_camera->pitch)),
				sin(glm::radians(m_camera->pitch)),
				sin(glm::radians(m_camera->yaw)) * cos(glm::radians(m_camera->pitch))
			);
			m_camera->FrontVector = glm::normalize(front);
		break;
	default:
		break;
	}
}

void CameraController::KeyboardCallback(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		m_Camera->Position += m_CameraSpeed * m_Camera->FrontVector;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		m_Camera->Position -= m_CameraSpeed * m_Camera->FrontVector;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		m_Camera->Position -= glm::normalize(glm::cross(m_Camera->FrontVector, m_Camera->UpVector)) * m_CameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		m_Camera->Position += glm::normalize(glm::cross(m_Camera->FrontVector, m_Camera->UpVector)) * m_CameraSpeed;
	
	// if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	// {
	// 	m_MouseMode = false;
	// 	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// 	m_Camera->firstMouse = true;
	// }
	// if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	// {
	// 	m_MouseMode = true;
	// 	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	// }
}
