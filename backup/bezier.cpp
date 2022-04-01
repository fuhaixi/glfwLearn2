#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include <iostream>
using namespace std;
// GLFWwindow *window; 

struct Point {
	double x, y;
	Point() { }
	Point(double a, double b)
		:x(a), y(b) { }
	Point(const Point &p) {
		this->x = p.x;
		this->y = p.y;
	}

	// scaler * Point
	Point operator* (const double &t) {
		return Point(t*this->x, t*this->y);
	}
	// Point + Point
	Point operator+ (const Point &p) {
		return Point(this->x + p.x, this->y + p.y);
	}
};

vector<Point> canvas;

vector<Point> bezier_points;//控制点

vector<Point> points;//构成bezier曲线的点
//draw line
void drawLine(int x0, int y0, int x1, int y1);

void bezier3(double t){

    for(int i=1; i < bezier_points.size(); i++)
        for(int k = 0; k < bezier_points.size()-i; k++){
            bezier_points[k] = bezier_points[k]*(1-t) + bezier_points[k+1]*t;
        }
    
    points.push_back(bezier_points[0]);
    
}
void drawPixel(int x, int y);

void drawBezier(){

    for (double t = 0; t <= 1; t+=0.00001)
    {
        bezier3(t);
    }
    for (auto p : points)
    {
        drawPixel(p.x, p.y);
    }
     
    
}




void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

double x,y;
void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos){
    x = xpos;
    y = ypos;
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);

void renderCanvas(){
    glPointSize(4.0f);
    glBegin(GL_POINTS);
    
    for (auto p : canvas)
    {
        glVertex2f(p.x,p.y);
    }
    glEnd();
    
}
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

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
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

    //start

    // render loop  
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        processInput(window);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if(mouse_state == 1){
            bezier_points.push_back({x,y});
            drawPixel(x,y);
            if(bezier_points.size() == 4){
                drawBezier();
            }
        }

        //UI
        ImGui::Begin("Hello, world!");
        ImGui::Text("mousePos: %d,%d", (int)x, (int)y);
        if(ImGui::Button("clear")){
            bezier_points.clear();
            points.clear();
            canvas.clear();
        }
        ImGui::End();

        mouse_state = 0;
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        renderCanvas();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}


void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{

    glViewport(0, 0, width, height);
}


void drawPixel(int x, int y){
    
    float winx = -1.0f + 2*x/(float)SCR_WIDTH;
    float winy = 1.0f + -2*y/(float)SCR_HEIGHT;

    canvas.push_back({winx,winy});
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
//鼠标左键生成控制点，右键绘制曲线
Point start_point;
bool start_line =false;
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods){
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        if(bezier_points.size()<10)
            bezier_points.push_back({x,y});
        if(!start_line){
            start_point = {x , y};
            start_line = true;

        }
        else{
            drawLine(x, y, start_point.x, start_point.y);
            start_point = {x, y};
            // start_line = false;

        }
        drawPixel(x,y);
    }
    if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS){
        drawBezier();
        bool start_line =false;

    }



}