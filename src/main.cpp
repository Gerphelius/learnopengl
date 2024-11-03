/* DeFacto engine. All rights belongs to me */

#include "main.h"

/////////////////////////////////// CAMERA MOVEMENT ///////////////////////////////////

// TODO: get rid of this global shit

float cameraMovementSpeed{ 1.5f };

bool rmbPressed{ false };
glm::vec2 initialMousePos{ glm::vec2(-1.0f, -1.0f) };
glm::vec2 mousePosDelta{ glm::vec2(0.0f, 0.0f) };
glm::vec2 mousePosOffset{ glm::vec2(0.0f, 0.0f) };
glm::vec3 target{ glm::vec3(0.0f, 0.0f, 0.0f) };

///////////////////////////////////////////////////////////////////////////////////////

int main()
{
    float SCREEN_WIDTH{ 800.0f };
    float SCREEN_HEIGHT{ 600.0f };

    GLFWwindow* window{ createWindow(
        static_cast<int>(SCREEN_WIDTH),
        static_cast<int>(SCREEN_HEIGHT),
        "DeFacto Engine",
        false
    ) };

    if (window == NULL) return -1;

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

    glBindVertexArray(vaos[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);

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

    glEnable(GL_DEPTH_TEST);

    double deltaTime{ 0.0f };
    double lastFrame{ 0.0f };

    glm::vec3 cameraPos{ 0.0f, 0.0f, 0.0f };
    glm::vec3 lightPos{ 0.0f, 2.0f, 0.0f };
    glm::vec3 lightDir{ 0.0f, 1.0f, 0.0f };
    glm::vec3 lightColor{ 1.0f, 1.0f, 1.0f };

    std::array<PointLight, 2> pointLights
    {{
        {
            { -1.0f, 2.0f, 0.0f },
            { 0.0f, 1.0f, 0.0f },
            { 1.0f, 0.0f, 0.0f }
        },
        {
            { 1.0f, 1.0f, 0.0f },
            { 0.0f, 1.0f, 0.0f },
            { 0.0f, 1.0f, 0.0f }
        }
    }};

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        double timeValue{ glfwGetTime() };
        double blueValue{ 0.5f * sin(timeValue / 1.0f) + 1.0f };

        double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        moveCamera(window, cameraPos, static_cast<float>(deltaTime));
        moveLight(window, pointLights[0].position, static_cast<float>(deltaTime));

        pointLights[1].position.z = sin(timeValue);

        /*lightPos.x = sin(timeValue);
        lightPos.z = cos(timeValue);*/

        //std::cout << "FPS: " << 1 / deltaTime << '\n';

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /////////////////////////////////// MAIN CUBE ///////////////////////////////////

        glm::mat4 model{ glm::mat4(1.0f) };
        glm::mat4 view{ glm::lookAt(cameraPos, cameraPos + target, glm::vec3(0.0f, 1.0f, 0.0f)) };
        glm::mat4 projection{ glm::perspective(glm::radians(45.0f), SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f) };

        shaderProgramCube.use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textures[1]);

        shaderProgramCube.setUniform("model", model);
        shaderProgramCube.setUniform("view", view);
        shaderProgramCube.setUniform("projection", projection);

        shaderProgramCube.setUniform("material.diffuse", 0);
        shaderProgramCube.setUniform("material.specular", 1);
        shaderProgramCube.setUniform("material.shininess", 64.0f);

        for (int i{ 0 }; i < pointLights.size(); i++)
        {
            std::string propertyPos{ "pointLights[" + std::to_string(i) + "]" };

            glm::vec3 posInView{ glm::vec3(view * glm::vec4(pointLights[i].position, 1.0f)) };

            shaderProgramCube.setUniform(propertyPos + ".color", pointLights[i].color);
            shaderProgramCube.setUniform(propertyPos + ".position", posInView);
            shaderProgramCube.setUniform(propertyPos + ".diffuse", glm::vec3(1.0f));
            shaderProgramCube.setUniform(propertyPos + ".specular", glm::vec3(1.0f));
            shaderProgramCube.setUniform(propertyPos + ".constant", 1.0f);
            shaderProgramCube.setUniform(propertyPos + ".linear", 0.35f);
            shaderProgramCube.setUniform(propertyPos + ".quadratic", 0.44f);
        }

        //shaderProgramCube.setUniform("light.spotCutoffIn", glm::cos(glm::radians(15.0f)));
        //shaderProgramCube.setUniform("light.spotCutoffOut", glm::cos(glm::radians(20.0f)));

        glDrawArrays(GL_TRIANGLES, 0, 36);

        /////////////////////////////////// LIGHT CUBE ///////////////////////////////////

        shaderProgramLight.use();

        shaderProgramLight.setUniform("view", view);
        shaderProgramLight.setUniform("projection", projection);

        for (const auto &light : pointLights)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, light.position);
            model = glm::scale(model, glm::vec3(0.2f));

            shaderProgramLight.setUniform("model", model);
            shaderProgramLight.setUniform("color", light.color);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

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

static GLFWwindow* createWindow(int width, int height, const char* name, bool fullscreen)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* monitor{ fullscreen ? glfwGetPrimaryMonitor() : NULL };

    GLFWwindow* window = glfwCreateWindow(width, height, name, monitor, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, onWindowResize);

    return window;
}


void onWindowResize(GLFWwindow* window, int width, int height)
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

static void moveCamera(GLFWwindow* window, glm::vec3& cameraPos, float deltaTime)
{
    glm::vec3 rightNorm{ glm::normalize(glm::cross(target, glm::vec3(0.0f, 1.0f, 0.0f))) };
    glm::vec3 forwardNorm{ glm::cross(-rightNorm, glm::vec3(0.0f, 1.0f, 0.0f)) };
    glm::vec3 upNorm{ glm::vec3(0.0f, 1.0f, 0.0f) };

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraPos += target * cameraMovementSpeed * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraPos -= target * cameraMovementSpeed * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraPos += rightNorm * cameraMovementSpeed * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cameraPos -= rightNorm * cameraMovementSpeed * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        cameraPos += upNorm * cameraMovementSpeed * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        cameraPos -= upNorm * cameraMovementSpeed * deltaTime;
    }
}

static void moveLight(GLFWwindow* window, glm::vec3& lightPos, float deltaTime)
{
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
}
