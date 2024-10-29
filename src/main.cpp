/* DeFacto engine. All rights belongs to me */

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utils/ShaderProgram.h"
#include "utils/fileReader.h"

static void createTexture2D(const char* path, GLuint unit, GLuint format, GLuint obj, bool flipped);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void onKeyboardInput(GLFWwindow* window, int key, [[maybe_unused]] int scancode, [[maybe_unused]] int action, [[maybe_unused]] int mods);
static void onCursorPosChange(GLFWwindow* window, double xpos, double ypos);
static void onMouseKeysInput(GLFWwindow* window, int button, int action, int mods);

float SCREEN_WIDTH{ 800.0f };
float SCREEN_HEIGHT{ 600.0f };

//float cube[] = {
//    // positions           // colors            // texture coords
//    0.5f,  0.5f,  0.5f,    1.0f, 0.0f, 0.0f,    1.0f, 1.0f,   // front top right
//    0.5f, -0.5f,  0.5f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f,   // front bottom right
//   -0.5f, -0.5f,  0.5f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f,   // front bottom left
//   -0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 0.0f,    0.0f, 1.0f,   // front top left
//
//    0.5f,  0.5f, -0.5f,    1.0f, 0.0f, 0.0f,    1.0f, 1.0f,   // back top right
//    0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f,   // back bottom right
//   -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f,   // back bottom left
//   -0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 0.0f,    0.0f, 1.0f,   // back top left
//};
//
//unsigned int cubeIndices[] = {
//    0, 1, 2,    // front first triangle
//    2, 3, 0,    // front second triangle
//
//    2, 3, 7,    // left first triangle
//    2, 6, 7,    // left second triangle
//
//    0, 4, 1,    // rigth first triangle
//    1, 4, 5,    // rigth second triangle
//
//    4, 5, 7,    // back first triangle
//    5, 6, 7,    // back second triangle
//
//    3, 7, 0,    // top first triangle
//    0, 7, 4,    // top second triangle
//
//    2, 6, 1,    // bottom first triangle
//    1, 5, 6     // bottom second triangle
//};

float cube[]{
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

struct Material
{
    glm::vec3 color{};
    glm::vec3 ambient{};
    glm::vec3 diffuse{};
    glm::vec3 specular{};
    float shininess{};
};

Material materials[]
{
    // Emerald
    {
        .color{ glm::vec3(0.31f, 0.78f, 0.47f) },
        .ambient{ glm::vec3(0.0215f, 0.1745f, 0.0215f) },
        .diffuse{ glm::vec3(0.07568f, 0.61424f, 0.07568f) },
        .specular{ glm::vec3(0.633f, 0.727811f, 0.633f) },
        .shininess{ 0.6f }
    },
    // Chrome
    {
        .color{ glm::vec3(0.75f, 0.75f, 0.75f) },
        .ambient{ glm::vec3(0.25f, 0.25f, 0.25f) },
        .diffuse{ glm::vec3(0.4f, 0.4f, 0.4f) },
        .specular{ glm::vec3(0.774597f, 0.774597f, 0.774597f) },
        .shininess{ 0.6f }
    },
    // Gold
    {
        .color{ glm::vec3(0.83f, 0.69f, 0.22f) },
        .ambient{ glm::vec3(0.24725f, 0.1995f, 0.0745f) },
        .diffuse{ glm::vec3(0.75164f, 0.60648f, 0.22648f) },
        .specular{ glm::vec3(0.628281f, 0.555802f, 0.366065f) },
        .shininess{ 0.4f }
    },
    // Silver
    {
        .color{ glm::vec3(0.75f, 0.75f, 0.75f) },
        .ambient{ glm::vec3(0.19225f, 0.19225f, 0.19225f) },
        .diffuse{ glm::vec3(0.50754f, 0.50754f, 0.50754f) },
        .specular{ glm::vec3(0.508273f, 0.508273f, 0.508273f) },
        .shininess{ 0.4f }
    },
    // Ruby
    {
        .color{ glm::vec3(0.88f, 0.07f, 0.37f) },
        .ambient{ glm::vec3(0.1745f, 0.01175f, 0.01175f) },
        .diffuse{ glm::vec3(0.61424f, 0.04136f, 0.04136f) },
        .specular{ glm::vec3(0.727811f, 0.626959f, 0.626959f) },
        .shininess{ 0.6f }
    },
    // Yellow plastic
    {
        .color{ glm::vec3(1.0f, 0.88f, 0.21f) },
        .ambient{ glm::vec3(0.0f, 0.0f, 0.0f) },
        .diffuse{ glm::vec3(0.5f, 0.5f, 0.0f) },
        .specular{ glm::vec3(0.6f, 0.6f, 0.5f) },
        .shininess{ 0.25f }
    },
    // Pearl
    {
        .color{ glm::vec3(0.94f, 0.92f, 0.84f) },
        .ambient{ glm::vec3(0.25f, 0.20725f, 0.20725f) },
        .diffuse{ glm::vec3(1.0f, 0.829f, 0.829f) },
        .specular{ glm::vec3(0.296648f, 0.296648f, 0.296648f) },
        .shininess{ 0.088f }
    },
    // Turquoise
    {
        .color{ glm::vec3(0.25f, 0.88f, 0.82f) },
        .ambient{ glm::vec3(0.1f, 0.18725f, 0.1745f) },
        .diffuse{ glm::vec3(0.396f, 0.74151f, 0.69102f) },
        .specular{ glm::vec3(0.297254f, 0.30829f, 0.306678f) },
        .shininess{ 0.1f }
    },
    // Red rubber
    {
        .color{ glm::vec3(0.77f, 0.13f, 0.13f) },
        .ambient{ glm::vec3(0.05f, 0.0f, 0.0f) },
        .diffuse{ glm::vec3(0.5, 0.4f, 0.4f) },
        .specular{ glm::vec3(0.7f, 0.04f, 0.04f) },
        .shininess{ 0.078125f }
    },
};

/////////////////////////////////// CAMERA MOVEMENT ///////////////////////////////////

float cameraMovementSpeed{ 1.5f };

bool rmbPressed{ false };
glm::vec2 initialMousePos{ glm::vec2(-1.0f, -1.0f) };
glm::vec2 mousePosDelta{ glm::vec2(0.0f, 0.0f) };
glm::vec2 mousePosOffset{ glm::vec2(0.0f, 0.0f) };
glm::vec3 target{ glm::vec3(0.0f, 0.0f, 0.0f) };

///////////////////////////////////////////////////////////////////////////////////////

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // GLFW window object creation
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "THE WINDOW!", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // End of window creation

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Falied to initialize GLAD" << std::endl;

        return -1;
    }

    ShaderProgram shaderProgramCube{ readFile("./src/shaders/cubeVertex.glsl").c_str(), readFile("./src/shaders/cubeFrag.glsl").c_str() };
    ShaderProgram shaderProgramLight{ readFile("./src/shaders/lightVertex.glsl").c_str(), readFile("./src/shaders/lightFrag.glsl").c_str() };

    /////////////////////////////  TEXTURES  /////////////////////////////

    unsigned int textures[2];
    glGenTextures(2, textures);

    createTexture2D("./src/assets/images/container.jpg", GL_TEXTURE0, GL_RGB, textures[0], false);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    createTexture2D("./src/assets/images/awesomeface.png", GL_TEXTURE1, GL_RGBA, textures[1], true);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    //////////////////////////////////////////////////////////////////////

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetKeyCallback(window, onKeyboardInput);
    glfwSetCursorPosCallback(window, onCursorPosChange);
    glfwSetMouseButtonCallback(window, onMouseKeysInput);

    unsigned int vaos[2]{};
    glGenVertexArrays(2, vaos);

    unsigned int vbos[2]{};
    glGenBuffers(2, vbos);

    unsigned int ebos[2]{};
    glGenBuffers(2, ebos);

    glBindVertexArray(vaos[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebos[0]);

    /*////////////////////// CUBE //////////////////////*/

    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

    // Adds vertices position data to VAO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Adds vertices normals data to VAO
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

    /*//////////////////////////////////////////////////*/

    glEnable(GL_DEPTH_TEST);

    double deltaTime{ 0.0f };
    double lastFrame{ 0.0f };

    glm::vec3 cameraPos{ glm::vec3(0.0f, 0.0f, 0.0f) };
    glm::vec3 lightPos(3.0f, 1.0f, 3.0f);

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        double timeValue{ glfwGetTime() };
        double blueValue{ 0.5f * sin(timeValue / 1.0f) + 1.0f };

        double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        /////////////////////////////////// CAMERA MOVEMENT ///////////////////////////////////

        glm::vec3 rightNorm{ glm::normalize(glm::cross(target, glm::vec3(0.0f, 1.0f, 0.0f))) };
        glm::vec3 forwardNorm{ glm::cross(-rightNorm, glm::vec3(0.0f, 1.0f, 0.0f)) };
        glm::vec3 upNorm{ glm::vec3(0.0f, 1.0f, 0.0f) };

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            cameraPos += target * cameraMovementSpeed * static_cast<float>(deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            cameraPos -= target * cameraMovementSpeed * static_cast<float>(deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            cameraPos += rightNorm * cameraMovementSpeed * static_cast<float>(deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            cameraPos -= rightNorm * cameraMovementSpeed * static_cast<float>(deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            cameraPos += upNorm * cameraMovementSpeed * static_cast<float>(deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        {
            cameraPos -= upNorm * cameraMovementSpeed * static_cast<float>(deltaTime);
        }

        /*lightPos.x = sin(timeValue);
        lightPos.z = cos(timeValue);*/

        //std::cout << "FPS: " << 1 / deltaTime << '\n';

        //glClearColor(0.2f, 0.0f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::vec3 lightColor{ glm::vec3(1.0f) };

        /////////////////////////////////// MAIN CUBE ///////////////////////////////////
 
        glm::mat4 model{ glm::mat4(1.0f) };
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + target, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 projection{ glm::perspective(glm::radians(45.0f), SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f) };

        shaderProgramCube.use();
        shaderProgramCube.setUniform("lightPos", lightPos);
        shaderProgramCube.setUniform("viewPos", cameraPos);

        //glm::vec3 diffuse = lightColor * glm::vec3(0.5f);
        //glm::vec3 ambient = diffuse * glm::vec3(0.2f);

        shaderProgramCube.setUniform("light.ambient", glm::vec3(1.0f));
        shaderProgramCube.setUniform("light.diffuse", glm::vec3(1.0f));
        shaderProgramCube.setUniform("light.specular", glm::vec3(1.0f));

        shaderProgramCube.setUniform("view", view);
        shaderProgramCube.setUniform("projection", projection);

        int matOffset{ 0 };

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                float gap{ 0.5f };
                float topOffset = static_cast<float>(i) * 1.0f + static_cast<float>(i) * gap;
                float rightOffset = static_cast<float>(j) * 1.0f + static_cast<float>(j) * gap;

                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(rightOffset, topOffset, 0.0f));
                //model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

                shaderProgramCube.setUniform("objectColor", materials[matOffset].color);

                shaderProgramCube.setUniform("material.ambient", materials[matOffset].ambient);
                shaderProgramCube.setUniform("material.diffuse", materials[matOffset].diffuse);
                shaderProgramCube.setUniform("material.specular", materials[matOffset].specular);
                shaderProgramCube.setUniform("material.shininess", 32.0f);

                shaderProgramCube.setUniform("model", model);

                matOffset++;

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }

        /////////////////////////////////// LIGHT CUBE ///////////////////////////////////
        shaderProgramLight.use();

        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));

        shaderProgramLight.setUniform("model", model);
        shaderProgramLight.setUniform("view", view);
        shaderProgramLight.setUniform("projection", projection);
        shaderProgramLight.setUniform("lightColor", lightColor);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        /////////////////////////////////////////////////////////////////////////////////

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clearing all previously allocated GLFW resources
    glfwTerminate();

    return 0;
}

static void createTexture2D(const char* path, GLuint unit, GLuint format, GLuint obj, bool flipped)
{
    stbi_set_flip_vertically_on_load(flipped);

    int width{};
    int height{};
    int channelsNum{};

    unsigned char* texData = stbi_load(path, &width, &height, &channelsNum, 0);

    glActiveTexture(unit);
    glBindTexture(GL_TEXTURE_2D, obj);

    if (texData)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, texData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "ERROR::IMAGE::FAILED_TO_LOAD::PATH:<" << path << ">\n";
    }

    stbi_image_free(texData);
}

/*
* Callback function that executes on every window resize (by OS or user)
*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

static void onKeyboardInput(GLFWwindow* window, int key, [[maybe_unused]] int scancode, [[maybe_unused]] int action, [[maybe_unused]] int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (key == GLFW_KEY_N && action == GLFW_PRESS)
    {
        GLint mode{};
        glGetIntegerv(GL_POLYGON_MODE, &mode);

        if (mode == GL_FILL) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }
}

static void onMouseKeysInput(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        rmbPressed = true;
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    {
        rmbPressed = false;
    }
}

static void onCursorPosChange(GLFWwindow* window, double xpos, double ypos)
{
    if (rmbPressed)
    {
        mousePosDelta = glm::vec2(xpos, ypos) - initialMousePos;

        mousePosOffset.x += mousePosDelta.x * 0.1f;
        mousePosOffset.y -= mousePosDelta.y * 0.1f;
        mousePosOffset.y = glm::clamp(mousePosOffset.y, -89.0f, 89.0f);

        target.x = cos(glm::radians(mousePosOffset.x)) * cos(glm::radians(mousePosOffset.y));
        target.y = sin(glm::radians(mousePosOffset.y));
        target.z = sin(glm::radians(mousePosOffset.x)) * cos(glm::radians(mousePosOffset.y));

        target = glm::normalize(target);
    }

    initialMousePos = glm::vec2(xpos, ypos);
}
