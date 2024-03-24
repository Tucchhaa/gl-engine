#define GL_SILENCE_DEPRECATION

#include <iostream>

#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "apis/opengl/include.hpp"
#include "core/include.hpp"

using namespace glm;
using namespace std;

GLFWwindow* init();
void configureGL();
void processInput(GLFWwindow* window, float deltaTime);
uint createTexture(const char* textureFileName);

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 800;

int main() {
    GLFWwindow* window = init();
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    ResourceManager* resourceManager = &ResourceManager::getInstance();
    Scene scene;
    CoreLoader loader(resourceManager);
    Renderer renderer;

    GameObject* object = loader.loadModel("models/backpack/backpack.obj");

    Hierarchy::addGameObject(object);
    Transform* objectTransform = Hierarchy::getTransform(object);

    // = camera =
    auto* cameraObject = new GameObject();
    Hierarchy::addGameObject(cameraObject);
    Transform* cameraTransform = Hierarchy::getTransform(cameraObject);

    loader.loadCubeMap("textures/skybox");
    auto* camera = new Camera(radians(45.0f), 0.1f, 100.0f);
    camera->cubeMap = "textures/skybox";
    Hierarchy::addComponent(cameraObject, camera);

    camera->setScreenSizes((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);

    // = light source =
    auto* lightSource = new GameObject();
    Hierarchy::addGameObject(lightSource);

    Hierarchy::getTransform(lightSource)->setValues(vec3(-10, 20, -50), quat(vec3(0, radians(180.0), 0)));

    auto* directLight0 = new DirectLight();
    PointLight* pointLight0 = PointLight::D3250();

    Hierarchy::addComponent(lightSource, directLight0);
    Hierarchy::addComponent(lightSource, pointLight0);

    Input input(window);
    
    // ===
    
    scene.setCamera(camera);
    scene.processHierarchy();
    renderer.setScene(&scene);
    
    float speed = 7.5f;
    float rotationSpeed = 2.0f;
    

    while (!glfwWindowShouldClose(window))
    {
        // = Input =
        input.process();
        
        if(input.isShiftPressed) {
            quat horizontalRotation = quat(vec3(0, input.axisHorizontal() * rotationSpeed * input.deltaTime, 0));
            quat verticalRotation   = quat(vec3(input.axisVertical() * rotationSpeed * input.deltaTime, 0, 0));
            
            cameraTransform->rotate(horizontalRotation);
            cameraTransform->rotate(verticalRotation, Transform::World);
        }
        else {
            cameraTransform->translate(input.axisVec3() * speed * input.deltaTime);
        }
        
        objectTransform->rotate(quat(vec3(0, radians(0.15f), 0)));

        renderer.render();

//        glfwSetWindowShouldClose(window, true);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    
    return 0;
}

GLFWwindow* init() {
    if (!glfwInit())
        exit(-1);
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World", NULL, NULL);
    
    if (!window)
    {
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window);
    
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    return window;
}
