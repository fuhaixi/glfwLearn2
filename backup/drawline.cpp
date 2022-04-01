#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

// GLFWwindow *window; 


//draw line
void drawLine(int x0, int y0, int x1, int y1);


void drawPixel(int x, int y);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

double x,y;
void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos){
    x = xpos;
    y = ypos;
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
//intput values
int mouse_state = 0;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);//core profile removed old api


#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    unsigned char *data = new unsigned char[100*100*3];

    for(int y =0; y<100; y++)
        for(int x =0; x<100; x++){
            data[y * 100 * 3 + x * 3    ] = 0xff;
            data[y * 100 * 3 + x * 3 + 1] = 0x00;
            data[y * 100 * 3 + x * 3 + 2] = 0x00;
            
        }

    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    //Setup Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();(void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui::Text("Hello, world %d", 123);
    //start
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);

    // drawLine(400,300,600,200);
    // drawLine(400,300,200,200);
    // drawLine(400,300,450,200);
    // drawLine(400,300,350,200);
    // drawLine(400,300,450,390);
    // drawLine(400,300,350,390);
    // drawLine(400,300,200,380);
    // drawLine(400,300,600,380);
    glfwSwapBuffers(window);
    int state = 0;
    int startx, starty = 0;
    // render loop  
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // glDrawPixels(100, 100, GL_RGB, GL_UNSIGNED_BYTE, data);

        switch (state)
        {
        case 0:
            if(mouse_state == 1)
            {
                startx = x;
                starty = y;
                state = 1;
            }
            break;
        case 1:
            if(mouse_state == 1)
            {
                drawLine(startx, starty, x, y);
                glfwSwapBuffers(window);
                drawLine(startx, starty, x, y);

                state = 0;
            }
            break;
        case 2:
            if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE){
                state = 0;
                
            }
            break;
        default:
            break;
        }
        
        // drawPixel(400,450);
        mouse_state = 0;
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


void drawPixel(int x, int y){
    
    float winx = -1.0f + 2*x/(float)SCR_WIDTH;
    float winy = 1.0f + -2*y/(float)SCR_HEIGHT;
    glPointSize(4.0f);
    glBegin(GL_POINTS);
    glVertex2f(winx,winy);
    glEnd();
}

void drawLine(int x0, int y0, int x1, int y1){
    if(x0 == x1&& y0 == y1){
        std::cout<<"draw a pixel"<<x0<<","<<y0<<std::endl;
        drawPixel(x0, y0);
        return;
    }
    if(y1 < y0){
        int t =x0;
        x0 = x1;
        x1 = t;

        t = y0;
        y0 = y1;
        y1 = t;
    }
    int x = x0;
    int y = y0;

    float acc = 0.000001;
    float k = (y1 - y0)/(float)(x1 - x0);


    if(k>1.0f||k<-1.0f){          
        if(x1 > x0)
            for(;y <=y1; y++){
                if(acc > 1){
                    acc-=1;
                    x+=1;
                }
                drawPixel(x,y);
                std::cout<<"("<<x<<","<<y<<')'<<std::endl;
                acc+=1/k;
            }
        else
            for(;y <=y1; y++){
                if(acc > 1){
                    acc-=1;
                    x-=1;
                }
                drawPixel(x,y);
                std::cout<<"("<<x<<"-"<<y<<')'<<std::endl;
                acc-= 1/k;
            }
    }
    else{
        if(x1 > x0)
            for(;x <=x1; x++){
                if(acc > 1){
                    acc-=1;
                    y+=1;
                }
                drawPixel(x,y);
                std::cout<<"("<<x<<","<<y<<')'<<std::endl;
                acc+=k;
            }
        else{
            for(;x >=x1; x--){
                if(acc > 1){
                    acc-=1;
                    y+=1;
                }
                drawPixel(x,y);
                std::cout<<"("<<x<<"-"<<y<<')'<<std::endl;
                acc-=k;
            }
        }
    }
    std::cout<<k<<std::endl;

    

}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods){
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        std::cout<<"mouse pressed"<<std::endl;
        mouse_state = 1;
    }
    else if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
        std::cout<<"mouse released"<<std::endl;
        mouse_state = 2;
    }

}