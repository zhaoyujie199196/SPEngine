#include "Shader.h"
#include "Defines.h"
#include "OpenGLInclude.h"

using namespace OpenGLDemos::Common;

Shader *Shader::create(unsigned int type, const std::string &source)
{
	if (type != GL_VERTEX_SHADER && type != GL_FRAGMENT_SHADER && type != GL_GEOMETRY_SHADER)
	{
		OD_PRINT_INFO("create %d shader failed! Type!\n", type);
		OD_Assert(false);
		return nullptr;
	}
	auto shader = new Shader(type);
	if (!shader->setSource(source))
	{
		OD_Assert(false);
		delete shader;
		return nullptr;
	}
	return shader;
}

Shader::~Shader()
{
	glDeleteShader(m_shader);
}

Shader::Shader(int type)
{
	m_shader = glCreateShader(type);
}

bool Shader::setSource(const std::string &source)
{
	const char *sourceContent = source.c_str();
	glShaderSource(m_shader, 1, &sourceContent, NULL);
	glCompileShader(m_shader);
	int success;
	glGetShaderiv(m_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char logInfo[512] = {};
		glGetShaderInfoLog(m_shader, 512, NULL, logInfo);
		OD_PRINT_INFO("compile shader failed : %s", logInfo);
		return false;
	}
	return true;
}

