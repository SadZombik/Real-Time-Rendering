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
    m_Camera->yaw         = -90.0f;
    m_Camera->pitch       = 0.0f;
    m_Camera->lastX       = (float)width  / 2.0f;
    m_Camera->lastY       = (float)height / 2.0f;
    m_Camera->fov         = 45.0f;
    m_Camera->firstMouse  = true;
    m_Camera->aspect      = (float)width / (float)height;

	glm::vec3 front = glm::vec3(
		glm::cos(glm::radians(m_Camera->yaw)) * glm::cos(glm::radians(m_Camera->pitch)),
		glm::sin(glm::radians(m_Camera->pitch)),
		glm::sin(glm::radians(m_Camera->yaw)) * glm::cos(glm::radians(m_Camera->pitch))
	);
	m_Camera->FrontVector = glm::normalize(front);
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

	m_CurrentCameraSpeed = m_CameraSpeed * m_DeltaTime;
}

void CameraController::SetRatio(float newRatio) {
	m_Camera->aspect = newRatio;
}

enum class Direction {
	LEFT = -1,
	NONE = 0,
	RIGHT = 1,
	UP = LEFT,
	DOWN = RIGHT
};

void CameraController::ArrowKeysCallback(GLFWwindow* window, ArrowKeysFunction func) {
	switch (func) {
	case ArrowKeysFunction::NONE:
		break;
	case ArrowKeysFunction::MOVEMENT:
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			m_Camera->Position += m_CurrentCameraSpeed * m_Camera->FrontVector;
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			m_Camera->Position -= m_CurrentCameraSpeed * m_Camera->FrontVector;
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			m_Camera->Position -= glm::normalize(glm::cross(m_Camera->FrontVector, m_Camera->UpVector)) * m_CurrentCameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			m_Camera->Position += glm::normalize(glm::cross(m_Camera->FrontVector, m_Camera->UpVector)) * m_CurrentCameraSpeed;
		break;
	case ArrowKeysFunction::VIEW:
		{
			Direction xDirection = Direction::NONE;
			Direction yDirection = Direction::NONE;

			if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
				yDirection = Direction::UP;
			if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
				yDirection = Direction::DOWN;
			if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
				xDirection = Direction::LEFT;
			if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
				xDirection = Direction::RIGHT;

			if (xDirection != Direction::NONE || yDirection != Direction::NONE) {
				m_Camera->yaw   += static_cast<float>(xDirection) * m_Sensitivity;
				m_Camera->pitch += static_cast<float>(yDirection) * m_Sensitivity;

				m_Camera->pitch = m_Camera->pitch > 89.0f ? 89.0f : m_Camera->pitch;
				m_Camera->pitch = m_Camera->pitch < -89.0f ? -89.0f : m_Camera->pitch;

				glm::vec3 front = glm::vec3(
					glm::cos(glm::radians(m_Camera->yaw)) * glm::cos(glm::radians(m_Camera->pitch)),
					glm::sin(glm::radians(m_Camera->pitch)),
					glm::sin(glm::radians(m_Camera->yaw)) * glm::cos(glm::radians(m_Camera->pitch))
				);
				m_Camera->FrontVector = glm::normalize(front);
			}
		}
		break;
	default:
		break;
	}
}

void CameraController::KeyboardCallback(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		m_Camera->Position += m_CurrentCameraSpeed * m_Camera->FrontVector;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		m_Camera->Position -= m_CurrentCameraSpeed * m_Camera->FrontVector;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		m_Camera->Position -= glm::normalize(glm::cross(m_Camera->FrontVector, m_Camera->UpVector)) * m_CurrentCameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		m_Camera->Position += glm::normalize(glm::cross(m_Camera->FrontVector, m_Camera->UpVector)) * m_CurrentCameraSpeed;
}
