#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>
#include <glad/glad.h>   
#include <GLFW/glfw3.h>  

#include "utils.h"
#include <iostream>

#define PI 3.14159265

GLuint BuildZero(float offset); // Constrói o número zero
GLuint BuildOne(float offset); // Constrói o número um
GLuint LoadShader_Vertex(const char* filename);   // Carrega um vertex shader
GLuint LoadShader_Fragment(const char* filename); // Carrega um fragment shader
void LoadShader(const char* filename, GLuint shader_id); // Função utilizada pelas duas acima
GLuint CreateGpuProgram(GLuint vertex_shader_id, GLuint fragment_shader_id); // Cria um programa de GPU

// Funções callback para comunicação com o sistema operacional e interação do
// usuário. Veja mais comentários nas definições das mesmas, abaixo.
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void ErrorCallback(int error, const char* description);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main()
{
    int success = glfwInit();
    if (!success)
    {
        fprintf(stderr, "ERROR: glfwInit() failed.\n");
        std::exit(EXIT_FAILURE);
    }

    glfwSetErrorCallback(ErrorCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;
    window = glfwCreateWindow(1000, 1000, "INF01047 - 00303397 - PUTOJoão Pedro Silveira e Silva", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        fprintf(stderr, "ERROR: glfwCreateWindow() failed.\n");
        std::exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, KeyCallback);

    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

    glfwMakeContextCurrent(window);

    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    const GLubyte *vendor      = glGetString(GL_VENDOR);
    const GLubyte *renderer    = glGetString(GL_RENDERER);
    const GLubyte *glversion   = glGetString(GL_VERSION);
    const GLubyte *glslversion = glGetString(GL_SHADING_LANGUAGE_VERSION);

    printf("GPU: %s, %s, OpenGL %s, GLSL %s\n", vendor, renderer, glversion, glslversion);

    GLuint vertex_shader_id = LoadShader_Vertex("../../src/shader_vertex.glsl");
    GLuint fragment_shader_id = LoadShader_Fragment("../../src/shader_fragment.glsl");

    GLuint program_id = CreateGpuProgram(vertex_shader_id, fragment_shader_id);

    int digits = 4;

    GLuint one[] = { BuildOne(1.05), BuildOne(0.30), BuildOne(-0.45), BuildOne(-1.15)  };
    GLuint zero[] = { BuildZero(1.5), BuildZero(0.5), BuildZero(-0.5), BuildZero(-1.5)  };

    int counter[] = {0, 0, 0, 0};
    int limit[] = {1, 2, 4, 8};
    int valueIsOne[] = { false, false, false, false};
    
    int currentTimeMeasure, lastTimeMeasure = 0;

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program_id);

        currentTimeMeasure = (int)glfwGetTime();

        if (currentTimeMeasure > lastTimeMeasure) {
            lastTimeMeasure = currentTimeMeasure;
            for (int x = 0; x < digits; x++) {
                counter[x] = counter[x] + 1;
                if (counter[x] == limit[x]) {
                    counter[x] = 0;
                    valueIsOne[x] = !valueIsOne[x];
                }
            }
        }

        for (int x = 0; x < digits; x++) {
            if (valueIsOne[x]) {
                glBindVertexArray(one[x]);
            } else {
                glBindVertexArray(zero[x]);
            }
            glDrawElements(GL_TRIANGLE_STRIP, 40, GL_UNSIGNED_BYTE, 0);
            glBindVertexArray(0);
        }

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

float cos_degree(float value) {
    return (float) cos(value * PI / 180.0);
}

float sin_degree(float value) {
    return (float) sin(value * PI / 180.0);
}

GLuint BuildZero(float offset)
{
    float outer_hyp = 0.7f;
    float inner_hyp = 0.5f;
    float w = 2.0f;
    float z = 1.0f;
    float reducer[] = {0.12f,0.16f,0.2f,0.21f,0.2f,0.16,0.12f,0.0f,-0.12f,-0.16f,-0.2f,-0.21f,-0.2f,-0.16f,-0.12f};

    GLfloat NDC_coefficients[] = {
        #pragma region Outer Circle
        cos_degree(90.0) * outer_hyp + offset, sin_degree(90.0) * outer_hyp , z, w,
        cos_degree(67.5) * outer_hyp + offset - reducer[0], sin_degree(67.5) * outer_hyp, z, w, 
        cos_degree(45.0) * outer_hyp + offset - reducer[1], sin_degree(45.0) * outer_hyp, z, w,
        cos_degree(22.5) * outer_hyp + offset - reducer[2], sin_degree(22.5) * outer_hyp, z, w,
        cos_degree(0.0) * outer_hyp + offset - reducer[3], sin_degree(0.0) * outer_hyp, z, w, 
        cos_degree(337.5) * outer_hyp + offset - reducer[4], sin_degree(337.5) * outer_hyp, z, w, 
        cos_degree(315.0) * outer_hyp + offset - reducer[5], sin_degree(315.0) * outer_hyp, z, w,
        cos_degree(292.5) * outer_hyp + offset - reducer[6], sin_degree(292.5) * outer_hyp, z, w,
        cos_degree(270.0) * outer_hyp + offset - reducer[7], sin_degree(270.0) * outer_hyp, z, w,
        cos_degree(247.5) * outer_hyp + offset - reducer[8], sin_degree(247.5) * outer_hyp, z, w,
        cos_degree(225.0) * outer_hyp + offset - reducer[9], sin_degree(225.0) * outer_hyp, z, w,
        cos_degree(202.5) * outer_hyp + offset - reducer[10], sin_degree(202.5) * outer_hyp, z, w,
        cos_degree(180.0) * outer_hyp + offset - reducer[11], sin_degree(180.0) * outer_hyp, z, w, 
        cos_degree(157.5) * outer_hyp + offset - reducer[12], sin_degree(157.5) * outer_hyp, z, w,
        cos_degree(135.0) * outer_hyp + offset - reducer[13], sin_degree(135.0) * outer_hyp, z, w,
        cos_degree(112.5) * outer_hyp + offset - reducer[14], sin_degree(112.5) * outer_hyp, z, w,
        #pragma endregion

        #pragma region Inner Circle
        cos_degree(90.0) * inner_hyp + offset, sin_degree(90.0) * inner_hyp, z, w,
        cos_degree(67.5) * inner_hyp + offset - reducer[0], sin_degree(67.5) * inner_hyp, z, w, 
        cos_degree(45.0) * inner_hyp + offset - reducer[1], sin_degree(45.0) * inner_hyp, z, w,
        cos_degree(22.5) * inner_hyp + offset - reducer[2], sin_degree(22.5) * inner_hyp, z, w,
        cos_degree(0.0) * inner_hyp + offset - reducer[3], sin_degree(0.0) * inner_hyp, z, w, 
        cos_degree(337.5) * inner_hyp + offset - reducer[4], sin_degree(337.5) * inner_hyp, z, w, 
        cos_degree(315.0) * inner_hyp + offset - reducer[5], sin_degree(315.0) * inner_hyp, z, w,
        cos_degree(292.5) * inner_hyp + offset - reducer[6], sin_degree(292.5) * inner_hyp, z, w,
        cos_degree(270.0) * inner_hyp + offset - reducer[7], sin_degree(270.0) * inner_hyp, z, w,
        cos_degree(247.5) * inner_hyp + offset - reducer[8], sin_degree(247.5) * inner_hyp, z, w,
        cos_degree(225.0) * inner_hyp + offset - reducer[9], sin_degree(225.0) * inner_hyp, z, w,
        cos_degree(202.5) * inner_hyp + offset - reducer[10], sin_degree(202.5) * inner_hyp, z, w,
        cos_degree(180.0) * inner_hyp + offset - reducer[11], sin_degree(180.0) * inner_hyp, z, w, 
        cos_degree(157.5) * inner_hyp + offset - reducer[12], sin_degree(157.5) * inner_hyp, z, w,
        cos_degree(135.0) * inner_hyp + offset - reducer[13], sin_degree(135.0) * inner_hyp, z, w,
        cos_degree(112.5) * inner_hyp + offset - reducer[14], sin_degree(112.5) * inner_hyp, z, w,
        #pragma endregion
    };

    GLuint VBO_NDC_coefficients_id;
    glGenBuffers(1, &VBO_NDC_coefficients_id);

    GLuint vertex_array_object_id;
    glGenVertexArrays(1, &vertex_array_object_id);

    glBindVertexArray(vertex_array_object_id);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_NDC_coefficients_id);

    glBufferData(GL_ARRAY_BUFFER, sizeof(NDC_coefficients), NULL, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(NDC_coefficients), NDC_coefficients);

    GLuint location = 0; // "(location = 0)" em "shader_vertex.glsl"
    GLint  number_of_dimensions = 4; // vec4 em "shader_vertex.glsl"
    glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(location);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    float a = 1.0f;
    GLfloat color_coefficients[] = {
        // R,G,B,A
        // outer
        1.0f, 0.0f, 0.0f,  a,
        1.0f, 0.0f, 0.0f,  a,
        1.0f, 0.0f, 0.0f,  a,
        1.0f, 0.0f, 0.0f,  a,
        1.0f, 0.0f, 0.0f,  a,
        1.0f, 0.0f, 0.0f,  a,
        1.0f, 0.0f, 0.0f,  a,
        1.0f, 0.0f, 0.0f,  a,
        1.0f, 0.0f, 0.0f,  a,
        1.0f, 0.0f, 0.0f,  a,
        1.0f, 0.0f, 0.0f,  a,
        1.0f, 0.0f, 0.0f,  a,
        1.0f, 0.0f, 0.0f,  a,
        1.0f, 0.0f, 0.0f,  a,
        1.0f, 0.0f, 0.0f,  a,
        1.0f, 0.0f, 0.0f,  a,
        // inner
        1.0f, 0.0f, 0.0f,  a,
        1.0f, 0.0f, 0.0f,  a,
        1.0f, 0.0f, 0.0f,  a,
        1.0f, 0.0f, 0.0f,  a,
        1.0f, 0.0f, 0.0f,  a,
        1.0f, 0.0f, 0.0f,  a,
        1.0f, 0.0f, 0.0f,  a,
        1.0f, 0.0f, 0.0f,  a,
        1.0f, 0.0f, 0.0f,  a,
        1.0f, 0.0f, 0.0f,  a,
        1.0f, 0.0f, 0.0f,  a,
        1.0f, 0.0f, 0.0f,  a,
        1.0f, 0.0f, 0.0f,  a,
        1.0f, 0.0f, 0.0f,  a,
        1.0f, 0.0f, 0.0f,  a,
        1.0f, 0.0f, 0.0f,  a,
    };
    GLuint VBO_color_coefficients_id;
    glGenBuffers(1, &VBO_color_coefficients_id);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_color_coefficients_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color_coefficients), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(color_coefficients), color_coefficients);
    location = 1; // "(location = 1)" em "shader_vertex.glsl"
    number_of_dimensions = 4; // vec4 em "shader_vertex.glsl"
    glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(location);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLubyte indices[] = { 0,16,1,17,2,18,3,19,4,20,5,21,6,22,7,23,8,24,9,25,10,26,11,27,12,28,13,29,14,30,15,31,0,16 }; // GLubyte: valores entre 0 e 255 (8 bits sem sinal).

    GLuint indices_id;
    glGenBuffers(1, &indices_id);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), NULL, GL_STATIC_DRAW);

    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indices), indices);

    glBindVertexArray(0);

    return vertex_array_object_id;
}

GLuint BuildOne(float offset)
{
    float w = 1.5f;
    float z = 0.0f;
    float weight = 0.2f;
    float height = 1.0f;

    GLfloat NDC_coefficients[] = {
        0.0f + offset, height/2, z, w,
        -0.1f + offset, 0.25f, z, w,
        0.0f + offset, 0.35f, z, w,
        0.0f + offset, -height/2, z, w,
        weight + offset, height/2, z, w,
        weight + offset, -height/2, z, w,
    };

    GLuint VBO_NDC_coefficients_id;
    glGenBuffers(1, &VBO_NDC_coefficients_id);

    GLuint vertex_array_object_id;
    glGenVertexArrays(1, &vertex_array_object_id);

    glBindVertexArray(vertex_array_object_id);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_NDC_coefficients_id);

    glBufferData(GL_ARRAY_BUFFER, sizeof(NDC_coefficients), NULL, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(NDC_coefficients), NDC_coefficients);

    GLuint location = 0; // "(location = 0)" em "shader_vertex.glsl"
    GLint  number_of_dimensions = 4; // vec4 em "shader_vertex.glsl"
    glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(location);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    float a = 1.0f;
    GLfloat color_coefficients[] = {
        // R,G,B,A
        0.0f, 0.0f, 1.0f,  a,
        0.0f, 0.0f, 1.0f,  a,
        0.0f, 0.0f, 1.0f,  a,
        0.0f, 0.0f, 1.0f,  a,
        0.0f, 0.0f, 1.0f,  a,
        0.0f, 0.0f, 1.0f,  a,
    };
    GLuint VBO_color_coefficients_id;
    glGenBuffers(1, &VBO_color_coefficients_id);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_color_coefficients_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color_coefficients), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(color_coefficients), color_coefficients);
    location = 1; // "(location = 1)" em "shader_vertex.glsl"
    number_of_dimensions = 4; // vec4 em "shader_vertex.glsl"
    glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(location);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    GLubyte indices[] = { 0, 1, 2, 0, 3, 4, 5 }; // GLubyte: valores entre 0 e 255 (8 bits sem sinal).

    GLuint indices_id;
    glGenBuffers(1, &indices_id);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), NULL, GL_STATIC_DRAW);

    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indices), indices);

    glBindVertexArray(0);

    return vertex_array_object_id;
}

// Carrega um Vertex Shader de um arquivo. Veja definição de LoadShader() abaixo.
GLuint LoadShader_Vertex(const char* filename)
{
    // Criamos um identificador (ID) para este shader, informando que o mesmo
    // será aplicado nos vértices.
    GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);

    // Carregamos e compilamos o shader
    LoadShader(filename, vertex_shader_id);

    // Retorna o ID gerado acima
    return vertex_shader_id;
}

// Carrega um Fragment Shader de um arquivo. Veja definição de LoadShader() abaixo.
GLuint LoadShader_Fragment(const char* filename)
{
    // Criamos um identificador (ID) para este shader, informando que o mesmo
    // será aplicado nos fragmentos.
    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    // Carregamos e compilamos o shader
    LoadShader(filename, fragment_shader_id);

    // Retorna o ID gerado acima
    return fragment_shader_id;
}


void LoadShader(const char* filename, GLuint shader_id)
{
    std::ifstream file;
    try {
        file.exceptions(std::ifstream::failbit);
        file.open(filename);
    } catch ( std::exception& e ) {
        fprintf(stderr, "ERROR: Cannot open file \"%s\".\n", filename);
        std::exit(EXIT_FAILURE);
    }
    std::stringstream shader;
    shader << file.rdbuf();
    std::string str = shader.str();
    const GLchar* shader_string = str.c_str();
    const GLint   shader_string_length = static_cast<GLint>( str.length() );

    glShaderSource(shader_id, 1, &shader_string, &shader_string_length);

    glCompileShader(shader_id);

    GLint compiled_ok;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled_ok);

    GLint log_length = 0;
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);

    GLchar* log = new GLchar[log_length];
    glGetShaderInfoLog(shader_id, log_length, &log_length, log);

    if ( log_length != 0 )
    {
        std::string  output;

        if ( !compiled_ok )
        {
            output += "ERROR: OpenGL compilation of \"";
            output += filename;
            output += "\" failed.\n";
            output += "== Start of compilation log\n";
            output += log;
            output += "== End of compilation log\n";
        }
        else
        {
            output += "WARNING: OpenGL compilation of \"";
            output += filename;
            output += "\".\n";
            output += "== Start of compilation log\n";
            output += log;
            output += "== End of compilation log\n";
        }

        fprintf(stderr, "%s", output.c_str());
    }

    delete [] log;
}

GLuint CreateGpuProgram(GLuint vertex_shader_id, GLuint fragment_shader_id)
{
    GLuint program_id = glCreateProgram();

    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);

    glLinkProgram(program_id);

    GLint linked_ok = GL_FALSE;
    glGetProgramiv(program_id, GL_LINK_STATUS, &linked_ok);

    if ( linked_ok == GL_FALSE )
    {
        GLint log_length = 0;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);

        GLchar* log = new GLchar[log_length];

        glGetProgramInfoLog(program_id, log_length, &log_length, log);

        std::string output;

        output += "ERROR: OpenGL linking of program failed.\n";
        output += "== Start of link log\n";
        output += log;
        output += "== End of link log\n";

        delete [] log;

        fprintf(stderr, "%s", output.c_str());
    }

    return program_id;
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
   for (int i = 0; i < 10; ++i)
        if (key == GLFW_KEY_0 + i && action == GLFW_PRESS && mode == GLFW_MOD_SHIFT)
            std::exit(100 + i);

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void ErrorCallback(int error, const char* description)
{
    fprintf(stderr, "ERROR: GLFW: %s\n", description);
}