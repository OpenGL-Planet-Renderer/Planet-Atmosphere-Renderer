//
//  Created by Shabnam Sahay and Sambit Behera on 07/06/20.
//  Rendering of Earth with simple atmospheric scattering for SOC OpenGL.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "utility/stb_image.h"
#include "utility/shader.hpp"
#include "utility/texture2D.hpp"
#include "utility/camera.hpp"
#include "utility/sphere.hpp"
#include "utility/skybox.hpp"

#include "utility/glm/glm/glm.hpp"
#include "utility/glm/glm/gtc/matrix_transform.hpp"
#include "utility/glm/glm/gtc/type_ptr.hpp"

#include <iostream>
#include <cmath>
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
bool blinncarrier = false;
bool blinnKeyPressed = false;


// camera
Camera camera(glm::vec3(0.0f, 0.0f, 20.0f));
float lastX = SCR_WIDTH/2.0f;
float lastY = SCR_HEIGHT/2.0f;
bool firstMouse = true;


// timing
float deltaTime = 0.0f; //time between current frame and last frame
float lastFrame = 0.0f; //time of last frame


// constants for scattering use
glm::vec3 sunPosn = glm::vec3 (-100.0f, 0.0f, 0.0f);
glm::vec3 InvLambdas = glm::vec3 (1.0f / pow(0.650f, 4.0f), 1.0f / pow(0.570f, 4.0f), 1.0f / pow(0.475f, 4.0f));


int main () {
    
    //glfw: initialize and configure
    
    glfwInit();
    if (!glfwInit ()) { // glfwInit() attempts to initialize GLFW lib
        cerr << "ERROR: could not start GLFW3" << endl;
        return 1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "eArTh", NULL, NULL);
    
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    glViewport(0,0,SCR_WIDTH,SCR_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    //tell GLFW to hide the cursor and capture it
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    
    //start GLEW extension handler
    glewExperimental = GL_TRUE; //enables experimental extensions
    glewInit();
    
    //enable depth testing for z buffer
    glEnable(GL_DEPTH_TEST);
    
    
    //generating spheres
    Sphere earth(10.0f);
    Sphere atmosphere(10.4f);
    
    //declaring vector containing filepaths of skybox images
    std::string arr[] = {    
        "images/bluespace/right.png",
        "images/bluespace/left.png",
        "images/bluespace/top.png",
        "images/bluespace/bottom.png",
        "images/bluespace/front.png",
        "images/bluespace/back.png"
    };
    std::vector<std::string> faces(arr, arr + sizeof(arr)/ sizeof(std::string));
    
    //generate skybox
    Skybox blueSpace(faces);
    
    //create shader programs
    Shader containerShader("shaders/diffusemapinit.vs", "shaders/sunlight.fs");
    Shader skyboxShader("shaders/skyboxinit.vs", "shaders/skyboxinit.fs");
    Shader atmoShader("shaders/skyfromspace.vs", "shaders/skyfromspace.fs");
    
    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);
    
    
    containerShader.use();

    Texture diffmap("images/earth/earthdaymap.jpg");
    Texture specmap("images/earth/earthspec.jpg");
    Texture nightmap("images/earth/earthnightmap.jpg");

    containerShader.setInt("material.diffuse", 0);
    containerShader.setInt("material.specular", 1);
    containerShader.setInt("material.ambient",2);
    containerShader.setFloat("material.shininess", 32.0f);
    
    containerShader.setVec3("dirLight.ambient",  0.5f, 0.5f, 0.5f);
    containerShader.setVec3("dirLight.diffuse",  0.9f, 0.9f, 0.9f);
    containerShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
    containerShader.setVec3("dirLight.direction", -1.0*sunPosn.x, -1.0*sunPosn.y, -1.0*sunPosn.z);
    
    
    atmoShader.use();
    
    atmoShader.setFloat("fInnerRadius",earth.radius);
    atmoShader.setFloat("fInnerRadius2",earth.radius*earth.radius);
    atmoShader.setFloat("fOuterRadius",atmosphere.radius);
    atmoShader.setFloat("fOuterRadius2",atmosphere.radius*atmosphere.radius);
    
    atmoShader.setFloat("fKrESun",0.0025f * 20.0f);
    atmoShader.setFloat("fKmESun",0.0015f * 20.0f);
    atmoShader.setFloat("fKr4PI",0.0025f * 4.0f * 3.141592653f);
    atmoShader.setFloat("fKm4PI",0.0015 * 4.0f * 3.141592653f);
    
    atmoShader.setFloat("fScale",1.0f /(atmosphere.radius-earth.radius));
    atmoShader.setFloat("fScaleDepth",0.25f);
    atmoShader.setFloat("fScaleOverScaleDepth",4.0f /(atmosphere.radius-earth.radius));
    
    atmoShader.setFloat("g",-0.85f);
    atmoShader.setFloat("g2",-0.85f * -0.85f);
    atmoShader.setVec3("v3InvWavelength", InvLambdas);
    
    
    // render loop
    
    while(!glfwWindowShouldClose(window)) {
        
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        //input
        processInput(window);
        
        //render
        glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //also clear depth buffer
        
        
        //earth rendering
        
        containerShader.use();
        diffmap.use(0);
        specmap.use(1);
        nightmap.use(2);        
        containerShader.setVec3("viewPos", camera.Position);
        containerShader.setBool("blinn", blinncarrier);
        
        
        //pass projection matrix to shader
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH/SCR_HEIGHT, 0.1f, 100.0f);
        containerShader.setMat4("projection", projection);
        
        //camera transformation
        glm::mat4 view = camera.GetViewMatrix();
        containerShader.setMat4("view", view);
        
        glBindVertexArray(earth.sphereVAO);
        
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(120.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        containerShader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, earth.indSize, GL_UNSIGNED_INT,0);
        glBindVertexArray(0);
        
        
        //atmosphere rendering
        
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
        
        atmoShader.use();
        
        atmoShader.setVec3("v3CameraPos", camera.Position);
        atmoShader.setVec3("v3LightPos", normalize(sunPosn));
        
        atmoShader.setFloat("fCameraHeight",earth.radius + length(camera.Position));
        atmoShader.setFloat("fCameraHeight2",(earth.radius + length(camera.Position))*(earth.radius + length(camera.Position)));
        
        
        //pass projection matrix to shader
        glm::mat4 atmoProjection = glm::mat4(1.0f);
        atmoProjection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH/SCR_HEIGHT, 0.1f, 100.0f);
        
        //camera transformation
        glm::mat4 atmoView = camera.GetViewMatrix();
        
        glBindVertexArray(atmosphere.sphereVAO);
        
        glm::mat4 atmoModel = glm::mat4(1.0f);
        
        glm::mat4 atmoMVP = atmoProjection*atmoView*atmoModel;
        atmoShader.setMat4("MVPmatrix", atmoMVP);
        
        glDrawElements(GL_TRIANGLES, atmosphere.indSize, GL_UNSIGNED_INT,0);
        
        glDisable(GL_CULL_FACE);
        glDisable(GL_BLEND);
        glBindVertexArray(0);
        
        
        //skybox rendering
        
        glDepthFunc(GL_LEQUAL); // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.use();
        
        //pass projection matrix to shader
        glm::mat4 skyboxProjection = glm::mat4(1.0f);
        skyboxProjection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH/SCR_HEIGHT, 0.1f, 100.0f);
        skyboxShader.setMat4("projection", skyboxProjection);
        
        //camera transformation
        glm::mat4 skyboxView = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        skyboxShader.setMat4("view", skyboxView);
        
        glBindVertexArray(blueSpace.skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, blueSpace.cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default
        
        //std::cout << (blinncarrier ? "Blinn-Phong" : "Phong") << std::endl;
        
        // check and call events and swap the buffers
        glfwSwapBuffers(window); //swaps color buffer used to render
        glfwPollEvents(); //checks if events are triggered and calls corresp fns
        
    }
    
    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &blueSpace.skyboxVAO);
    glDeleteBuffers(1, &blueSpace.skyboxVBO);
    glDeleteVertexArrays(1, &earth.sphereVAO);
    glDeleteBuffers(1, &earth.sphereVBO);

    //clear all previously allocated GLFW resources
    glfwTerminate();
    return 0;
    
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if(glfwGetKey(window, GLFW_KEY_W)==GLFW_PRESS)
    {camera.ProcessKeyboard(FORWARD, deltaTime);}
    
    if(glfwGetKey(window, GLFW_KEY_S)==GLFW_PRESS)
    {camera.ProcessKeyboard(BACKWARD, deltaTime);}
    
    if(glfwGetKey(window, GLFW_KEY_A)==GLFW_PRESS)
    {camera.ProcessKeyboard(LEFT, deltaTime);}
    
    if(glfwGetKey(window,GLFW_KEY_D)==GLFW_PRESS)
    {camera.ProcessKeyboard(RIGHT, deltaTime);}
    
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !blinnKeyPressed)
    {blinncarrier = !blinncarrier; blinnKeyPressed = true;}
    
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE)
    {blinnKeyPressed = false;}
    
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    
    if(firstMouse) { //initially set to true
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; //reversed since y coords range from bottom to top
    lastX = xpos;
    lastY = ypos;
    
    camera.ProcessMouseMovement(xoffset, yoffset);
    
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{camera.ProcessMouseScroll(yoffset);}
