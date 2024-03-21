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

int main(void) {
    GLFWwindow* window = init();
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    
    Scene scene;
    Loader loader;
    Renderer renderer;
    
    GameObject* object = new GameObject();

    vector<Mesh> meshes = loader.loadModel("backpack/backpack.obj");

    Transform* objectTransform = new Transform(vec3(0, 0, -10), vec3(1, 1, 1), quat(vec3(0, 0, 0)));

    for(int i=0; i < meshes.size(); i++) {
        Hierarchy::addComponent(object, &meshes[i]);
    }
    
    Hierarchy::addGameObject(object);
    Hierarchy::addComponent(object, objectTransform);
    
    // = camera =
    GameObject* cameraObject = new GameObject();
    
    Camera* camera = new Camera(radians(45.0f), 0.1f, 100.0f);
    Transform* cameraTransform = new Transform(vec3(0, 0, 0), vec3(1), quat(vec3(0, 0, 0)));
    
    Hierarchy::addGameObject(cameraObject);
    Hierarchy::addComponent(cameraObject, camera);
    Hierarchy::addComponent(cameraObject, cameraTransform);
    
    camera->setScreenSizes((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);

    // = light source =
    GameObject* lightSource = new GameObject();
    
    Transform* lightTransform = new Transform(vec3(-2, 0, -3), vec3(0.2));
    DirectLight* directLight0 = new DirectLight();
    PointLight* pointLight0 = PointLight::D100();
    
    Hierarchy::addGameObject(lightSource);
    Hierarchy::addComponent(lightSource, lightTransform);
    Hierarchy::addComponent(lightSource, directLight0);
    Hierarchy::addComponent(lightSource, pointLight0);
    
    lightTransform->rotate(quat(vec3(0, radians(180.0), 0)));
    
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
            cameraTransform->rotate(verticalRotation, objectTransform->World);
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
