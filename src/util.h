#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;
float _cube_vertex [] = {
    0.5f, 0.5f, 0.5f, 
    -0.5f, 0.5f, 0.5f,  
    -0.5f,  -0.5f, 0.5f,  
    0.5f,  -0.5f, 0.5f,  

};



// unsigned int * getCubeIndices(float * aa, int size){
//     unsigned int * re = new unsigned int(12);
    

// }

unsigned int _cube_indices []={
    1,2,3,

};

float _plane_vertices[] = {
    //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
    0.5f,  0.5f, 0.0f,   // 右上
    -0.5f, 0.5f, 0.0f,   // 右下
    -0.5f, -0.5f, 0.0f,   // 左下
    0.5f,  -0.5f, 0.0f,   // 左上
};
unsigned int _plane_indices[]={
    0, 1, 2,
    2, 3, 0
};

struct simpleMesh
{
    float * vertices;
    unsigned int * indices;
    
};

struct mesh_array{
    
}

simpleMesh plane_mesh = {_plane_vertices, _plane_indices};



