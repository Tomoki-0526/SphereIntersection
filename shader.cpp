#include "shader.h"

#include <sstream>
#include <fstream>

#include <QDebug>

string read_shader_file(const char* shader_path)
{
    string shader_code;
    ifstream shader_file;
    shader_file.exceptions(ifstream::failbit | ifstream::badbit);
    try {
        shader_file.open(shader_path);
        stringstream shader_stream;
        shader_stream << shader_file.rdbuf();
        shader_file.close();
        shader_code = shader_stream.str();
    } catch (ifstream::failure& e) {
        qDebug() << "Error: Shader file not read successfully!\n" << e.what();
    }
    return shader_code;
}
