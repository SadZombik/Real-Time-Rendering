#include <Framework.h>
#include <shapes/Cylinder.h>

int main() {
    auto* window = Framework::CreateWindow("Arm Animation");
    Framework::EnableDepthTest();

    CameraController cam(DEFAULT_WIDTH, DEFAULT_HEIGHT);

    Cylinder cylinder;


    while (!glfwWindowShouldClose(window)) {
        cam.Update();
        cam.KeyboardCallback(window);

        Framework::ClearBuffer();

        // const auto model = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3());
        cylinder.SetModelMatrix(glm::mat4(1.0));
        cylinder.Update(cam);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}
