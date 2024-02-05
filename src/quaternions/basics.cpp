#define GLM_ENABLE_EXPERIMENTAL
#include <Framework.h>
#include <Utils.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#define S_WIDTH 1280
#define S_HEIGHT 720


int main() {
    auto* window = Framework::CreateWindow(S_WIDTH, S_HEIGHT, "Quaternoins Basics");

    const glm::vec3 axis_x(1.0f, 0.0f, 0.0f);
    const float angle = 45.0f;

    float angleRad = glm::radians(angle);
    auto& axisNorm = glm::normalize(axis_x);

    float w = glm::cos(angleRad / 2);
    float v = glm::sin(angleRad / 2);
    glm::vec3 qv = axisNorm * v;

    glm::quat quaternion(w, qv);
    glm::mat4 quatTransform = glm::mat4_cast(quaternion);
    
    Utils::Print::Vec(qv);
    Utils::Print::Vec(quaternion);
    Utils::Print::Matrix(quatTransform);

    // Print::Vec(rotationQuat);
    

    // while (!glfwWindowShouldClose(window)) {
    //     if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
	// 	    glfwSetWindowShouldClose(window, true);
    //     }
        
    //     glClear(GL_COLOR_BUFFER_BIT);
    //     glClearColor(0.0f, 0.0f, 0.0f, 1.0f);


    // }

    Framework::Shutdown();

    return 0;
}
