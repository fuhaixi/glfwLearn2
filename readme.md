# 编译

## gcc 编译过程
预处理->转换成汇编语言->转换成机器语言->连接目标代码

## g++ 常用参数
-Idir:指定头文件目录
-Ldir:指定库文件目录
-llibrary:指定需要的库


## makefile基本规则
```
target ... : prerequisites ...
    command
    ...
    ...
```

## makefile常用函数
$(wildcard *.c) 获取当前目录所有.c结尾的文件
$(patsubst %.c,%.o,$(wildcard *.c)) 获取当前目录所有.o结尾的文件

# opengl
pipline:input vertex data -> vertex shader -> shape assembly -> geometry shader -> rasterization -> fragment shader -> test and blending.

c++ 多行字符串：R"(这里书写多行字符)"

## opengl函数解析
glVertexAttribPointer(loaction, size, type, ifNormalize, stride, offset);

