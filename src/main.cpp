#include"glstart.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "util.h"
#include "stb_image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
// settings

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float triangle[]={
    0.5, -0.5, 0,
    0, 0.5, 0,
    -0.5, -0.5, 0
};

using namespace std;
using namespace glm;
int main()
{
    

    
    GLFWwindow* window = 0;
    init(window, SCR_WIDTH, SCR_HEIGHT, "hello opengl");
    
    const char* v_shader= R"(#version 330 core
        layout (location = 0) in vec3 aPos;
        uniform float vv;

        out vec2 ndc_pos;
        void main(){
            gl_Position = aPos;
            ndc_pos = gl_Position.xy;
            
            
        }

    )";

    const char* v_shader2= R"(#version 330 core
        layout (location = 0) in vec3 aPos;

        void main(){
            gl_Position = vec4(aPos+0.2f, 1.0f);
        }

    )";

    const char* f_shader=R"(#version 330 core
        in vec2 ndc_pos;
        out vec4 gl_Color;
        uniform sampler2D tex_color;
        void main(){
            gl_Color = texture(tex_color, ndc_pos*0.5f + 0.5f);
        }
    )";

    

    unsigned int vshader;
    unsigned int fshader;
    vshader = glCreateShader(GL_VERTEX_SHADER);
    fshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vshader, 1, &v_shader, 0);
    glShaderSource(fshader, 1, &f_shader, 0);
    glCompileShader(vshader);
    glCompileShader(fshader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vshader);
    glAttachShader(shaderProgram, fshader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vshader);

    int uniform_vv = glGetUniformLocation(shaderProgram, "vv");
    glUniform1f(uniform_vv, -0.2f);
    // glDeleteShader(fshader);

    unsigned int shaderProgram2 = glCreateProgram();
    glShaderSource(vshader, 1, &v_shader2, 0);
    glCompileShader(vshader);
    
    glAttachShader(shaderProgram2, vshader);
    glAttachShader(shaderProgram2, fshader);
    glLinkProgram(shaderProgram2);

    char infolog[512];
    glGetShaderInfoLog(vshader, 512, 0, infolog);
    cout<<infolog;
    glGetShaderInfoLog(fshader, 512, 0, infolog);
    cout<<infolog;
    glGetShaderInfoLog(shaderProgram, 512, 0, infolog);
    cout<<infolog;


    unsigned int vbo, vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
    glEnableVertexAttribArray(0);

    int tt;

    unsigned char *data = stbi_load("src/tex/test.png", &tt, &tt, &tt, 0);
    cout<<data;
    unsigned int tex;
    glGenTextures(1, &tex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    
    glUseProgram(shaderProgram);
    int loc = glGetUniformLocation(shaderProgram, "tex_color");
    glUniform1i(loc, 0);
    
    // glBindBuffer()
 
    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.f,0.f,0.f,0.f);
        
        glBindVertexArray(vao);
        glUseProgram(shaderProgram);
        glUniform1f(uniform_vv, sin(time())*0.5);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

