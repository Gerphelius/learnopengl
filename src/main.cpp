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

float cube[]{
     // Back side
     // Vertex coords     // Normals           // Texture coors
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

     // Front side
     // Vertex coords     // Normals           // Texture coors
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

     // Left side
     // Vertex coords     // Normals           // Texture coors
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,

     // Right side
     // Vertex coords     // Normals           // Texture coors
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

     // Bottom side
     // Vertex coords     // Normals           // Texture coors
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,

     // Top side
     // Vertex coords     // Normals           // Texture coors
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f
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

    unsigned int textures[3];
    glGenTextures(3, textures);

    createTexture2D("./src/assets/images/container2.png", GL_TEXTURE0, GL_RGBA, textures[0], false);
    createTexture2D("./src/assets/images/container2_spec.png", GL_TEXTURE1, GL_RGBA, textures[1], false);
    createTexture2D("./src/assets/images/awesomeface.png", GL_TEXTURE2, GL_RGBA, textures[2], false);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Adds vertices normals data to VAO
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Adds texture coordinates data to VAO
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

    /*//////////////////////////////////////////////////*/

    glEnable(GL_DEPTH_TEST);

    double deltaTime{ 0.0f };
    double lastFrame{ 0.0f };

    glm::vec3 cameraPos{ 0.0f, 0.0f, 0.0f };
    glm::vec3 lightPos{ 0.0f, 2.0f, 0.0f };
    glm::vec3 lightDir{ 0.0f, 1.0f, 0.0f };

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

        /////////////////////////////////// LIGHT MOVEMENT ///////////////////////////////////

        if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS)
        {
            lightPos += glm::vec3(0.0f, 0.0f, -1.0f) * static_cast<float>(deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS)
        {
            lightPos += glm::vec3(0.0f, 0.0f, 1.0f) * static_cast<float>(deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS)
        {
            lightPos += glm::vec3(1.0f, 0.0f, 0.0f) * static_cast<float>(deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS)
        {
            lightPos += glm::vec3(-1.0f, 0.0f, 0.0f) * static_cast<float>(deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_KP_9) == GLFW_PRESS)
        {
            lightPos += glm::vec3(0.0f, 1.0f, 0.0f) * static_cast<float>(deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS)
        {
            lightPos += glm::vec3(0.0f, -1.0f, 0.0f) * static_cast<float>(deltaTime);
        }

        /*lightPos.x = sin(timeValue);
        lightPos.z = cos(timeValue);*/

        //std::cout << "FPS: " << 1 / deltaTime << '\n';

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::vec3 lightColor{ glm::vec3(1.0f) };

        /////////////////////////////////// MAIN CUBE ///////////////////////////////////
 
        glm::mat4 model{ glm::mat4(1.0f) };
        glm::mat4 view{ glm::lookAt(cameraPos, cameraPos + target, glm::vec3(0.0f, 1.0f, 0.0f)) };
        glm::mat4 projection{ glm::perspective(glm::radians(45.0f), SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f) };

        glm::mat4 cameraView{ glm::lookAt(lightPos, lightPos - lightDir, glm::vec3(0.0f, 0.0f, 1.0f)) };

        shaderProgramCube.use();
        shaderProgramCube.setUniform("lightPos", lightPos);
        shaderProgramCube.setUniform("viewPos", cameraPos);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textures[1]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, textures[2]);

        shaderProgramCube.setUniform("model", model);
        shaderProgramCube.setUniform("view", view);
        shaderProgramCube.setUniform("projection", projection);
        shaderProgramCube.setUniform("cameraView", cameraView);

        shaderProgramCube.setUniform("material.ambient", glm::vec3(0.2f));
        shaderProgramCube.setUniform("material.diffuse", 0);
        shaderProgramCube.setUniform("material.specular", 1);
        shaderProgramCube.setUniform("material.emission", 2);
        shaderProgramCube.setUniform("material.shininess", 64.0f);

        shaderProgramCube.setUniform("lightDir", lightDir);
        shaderProgramCube.setUniform("light.ambient", glm::vec3(0.1f));
        shaderProgramCube.setUniform("light.diffuse", glm::vec3(0.5f));
        shaderProgramCube.setUniform("light.specular", glm::vec3(1.0f));
        shaderProgramCube.setUniform("light.attenuation", glm::vec3(1.0f, 0.045f, 0.0075f));
        shaderProgramCube.setUniform("light.spotCutoffIn", glm::cos(glm::radians(15.0f)));
        shaderProgramCube.setUniform("light.spotCutoffOut", glm::cos(glm::radians(20.0f)));

        shaderProgramCube.setUniform("time", static_cast<float>(timeValue));

        glDrawArrays(GL_TRIANGLES, 0, 36);

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
