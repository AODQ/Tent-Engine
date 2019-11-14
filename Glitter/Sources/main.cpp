// Local Headers
#include "glitter.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <random>
#include <string>

#include "Shader.h"
#include "Camera.h"
#include "Light.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = mWidth;
const unsigned int SCR_HEIGHT = mHeight;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
float texMix = 0.2f;

int main(int argc, char * argv[]) 
{
    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    auto mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

    // Check for Valid Context
    if (mWindow == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        return EXIT_FAILURE;
    }

    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);
    glfwSetFramebufferSizeCallback(mWindow, framebuffer_size_callback);
    glfwSetCursorPosCallback(mWindow, mouse_callback);
    glfwSetScrollCallback(mWindow, scroll_callback);

    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

    float vertices[] = {
        // Position           // UV         // Normals
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,   0, 0, -1,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,   0, 0, -1,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   0, 0, -1,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   0, 0, -1,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   0, 0, -1,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,   0, 0, -1,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   0, 0, 1,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,   0, 0, 1,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,   0, 0, 1,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,   0, 0, 1,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,   0, 0, 1,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   0, 0, 1,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1, 0, 0,   
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  -1, 0, 0,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1, 0, 0,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1, 0, 0,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  -1, 0, 0,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1, 0, 0,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   1, 0, 0,   
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   1, 0, 0,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   1, 0, 0,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   1, 0, 0,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   1, 0, 0,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   1, 0, 0,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   0, -1, 0,   
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,   0, -1, 0,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,   0, -1, 0,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,   0, -1, 0,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   0, -1, 0,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   0, -1, 0,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   0, 1, 0,   
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   0, 1, 0,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   0, 1, 0,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   0, 1, 0,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,   0, 1, 0,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   0, 1, 0
    };

    // texture 1
    unsigned int tex1, tex2;
    glGenTextures(1, &tex1);    
    glBindTexture(GL_TEXTURE_2D, tex1);    
    // set the texture wrapping/filtering options (on the currenty bound texture object) 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture 

    // Texture 1 
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    int width, height, nrChannels;
    unsigned char *data = stbi_load("Textures/revue.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load the texture" << std::endl;
    }
    stbi_image_free(data);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex1);

    // Texture 2
    glGenTextures(1, &tex2);    
    glBindTexture(GL_TEXTURE_2D, tex2);
    // set the texture wrapping/filtering options (on the currenty bound texture object) 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
 
    data = stbi_load("Textures/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load the texture" << std::endl;
    }
    stbi_image_free(data);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex2);

    // generating a VAO 
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // Initialization code, which is typically done once 
    // unless your object will freq change
    glBindVertexArray(VAO);

    // buffer objects allow us to send large batches of data at once to the GPU
    // so that we don't have to send data vertex by vertex
    unsigned int VBO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // copies vertex data in buffer's memory
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // parameter descriptions: 
    // 1. Which vertex attrib we want to configure. Relates to the layout location 
    // 2. Size of the vertex attribute so vec3 is 3 values. 
    // 3. The type of data
    // 4. Do we want data to be normalized? 
    // 5. Stride of data: the space between consecutive vertex attribs
    // 6. Offset of the attrib data. Needs to be casted to void*    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);   // position
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // texture
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float))); // normals

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);    
    glEnableVertexAttribArray(2);

    // unbinding VBO
    // this is allowed, since the call to glVertexAttribPointer registered VBO
    // as the vertex attribute's bound vertex buffer object so afterwards we 
    // can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // unbinding VAO for later use
    glBindVertexArray(0);

    // use our shader program when we want to render an object
    Shader lightShader("../Glitter/Shaders/light.vert", "../Glitter/Shaders/light.frag");
    Shader boxShader("../Glitter/Shaders/box.vert", "../Glitter/Shaders/box.frag");

    glUniform1i(glGetUniformLocation(boxShader.ID, "tex"), 0);

    std::vector<glm::vec3> lightPositions = {
        glm::vec3( 1.0f,  0.0f, -2.0f),
        //glm::vec3( 1.0f,  2.0f, -3.0f),
        //glm::vec3(-3.5f,  2.5f, -4.0f),
        //glm::vec3(-2.4f, -0.5f, -5.0f),
        //glm::vec3( 1.5f, -1.0f, -7.0f),
    };

    std::vector<glm::vec3> lightColors = {
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 1.0f),
    };

    std::vector<Light> lights;
    for (int i = 0; i < lightPositions.size(); ++i)
    {
        lights.push_back(Light(lightPositions[i], lightColors[i]));   
    }

    std::vector<glm::vec3> boxPositions = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        //     glm::vec3( 1.0f,  2.0f, -3.0f),
        //     glm::vec3(-3.5f,  2.5f, -4.0f),
        //     glm::vec3(-2.4f, -0.5f, -5.0f),
        //     glm::vec3( 1.5f, -1.0f, -7.0f),
    };

   
    // Rendering Loop
    while (glfwWindowShouldClose(mWindow) == false) 
    {        
        // per-frame time logic
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        // TODO add an FPS counter
        // have it render to the screen
        // look at text rendering tutorials
        //printf("%f ms/frame\n", 1000.0/double
        lastFrame = currentFrame;

        processInput(mWindow); 

        glEnable(GL_DEPTH_TEST);

        // Background Fill Color
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Add texture to our box
        boxShader.use();            
        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex1);
        //glActiveTexture(GL_TEXTURE1);
        //glBindTexture(GL_TEXTURE_2D, tex2);
      
        // transformation matrix 
        // DH note: make sure to initialize the matrices first
        // Translate the scene in the reverse direction of where we want to
        // move
        // Rotate the camera around the scene
        glm::mat4 model = glm::mat4(1.0f);

        //glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "proj"), 1, GL_FALSE, glm::value_ptr(proj));
        glm::mat4 view  = glm::mat4(1.0f);
        view = camera.GetViewMatrix();

        glm::mat4 proj  = glm::mat4(1.0f);
        proj = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f);

         // Draw our box
        boxShader.use();
        for (int i = 0; i < boxPositions.size(); ++i)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, boxPositions[i]);

            glm::mat4 mvp = proj * view * model;

            glUniformMatrix4fv(glGetUniformLocation(boxShader.ID, "mvp"),
                    1, GL_FALSE, glm::value_ptr(mvp));

            // Draw the box
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }

        // Draw our lights
        lightShader.use();
        for (int i = 0; i < lights.size(); ++i)
        {
            glm::vec3 newPos;
            // first light is moving light
            if (i == 0)
            {
                float radius = 5.0f;
                float omega = 1.0f;

                newPos = lights[i].pos + 
                    radius * glm::vec3(cos(omega * glfwGetTime()), 
                            0.0f,
                            sin(omega * glfwGetTime())
                            );
                model = glm::mat4(1.0f); 
                model = glm::translate(model, newPos);
                model = glm::scale(model, glm::vec3(0.2f));
            }
            else
            {
                model = glm::mat4(1.0f); 
                model = glm::translate(model, lights[i].pos);
                model = glm::scale(model, glm::vec3(0.2f));
            }

            // Also add our light info to each box
            boxShader.use();
            GLuint lightPosLoc = glGetUniformLocation(boxShader.ID, "lights[0].pos");
            for (int j = 0; j < lights.size(); ++j)
            {
                std::string lightPos = "lights[" + std::to_string(j) + "].pos";
                std::string lightColor = "lights[" + std::to_string(j) + "].color";

                if (i == 0)
                {
                    glUniform3fv(glGetUniformLocation(boxShader.ID, lightPos.c_str()), 1, glm::value_ptr(newPos));
                }
                else
                {
                    glUniform3fv(glGetUniformLocation(boxShader.ID, lightPos.c_str()), 1, glm::value_ptr(lights[i].pos));
                }
                glUniform3fv(glGetUniformLocation(boxShader.ID, lightColor.c_str()), 1, glm::value_ptr(lights[i].color));
            }


            lightShader.use();
            glm::mat4 mvp = proj * view * model;
            // Send in MVP
            glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "mvp"), 1, GL_FALSE, glm::value_ptr(mvp));

            // Pass in light color to shader
            glUniform3fv(glGetUniformLocation(lightShader.ID, "lightColor"), 1, glm::value_ptr(lights[i].color));

             // Draw the object
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
       }

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }   

    glfwTerminate();
    return EXIT_SUCCESS;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        camera.MovementSpeed = maxSpeed;
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
    {
        camera.MovementSpeed = normalSpeed;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
