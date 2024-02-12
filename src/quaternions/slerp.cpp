#define GLM_ENABLE_EXPERIMENTAL
#include <Framework.h>
#include <Utils.h>
#include <shapes/Pyramid.h>
#include <CameraController.h>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

int main() {
    auto* window = Framework::CreateWindow("Spherical Linear Interpolation");
    Framework::EnableDepthTest();

    CameraController cam(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    
    const auto q1 = glm::angleAxis(glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    const auto q2 = glm::angleAxis(glm::radians(89.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    int i = 0;

    Pyramid pyramid;
    pyramid.SetShaders(
        res_dir + "/shaders/3d/vertex_no_color.glsl",
        res_dir + "/shaders/3d/fragment_no_color.glsl"
    );

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_RIGHT)) {

            cam.Update();
            cam.KeyboardCallback(window);

            Framework::ClearBuffer();

            const auto q3 = glm::slerp(q1, q2, ++i * 0.001f);

            pyramid.SetModelMatrix(glm::mat4_cast(q3));
            pyramid.Update(cam);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    Framework::Shutdown();

    return 0;
}
