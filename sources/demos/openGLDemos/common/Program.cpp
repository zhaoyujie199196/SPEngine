#include "Program.h"
#include "Shader.h"
#include "utils/ScopeGuard.h"
#include "Defines.h"
#include "OpenGLInclude.h"

using namespace OpenGLDemos::Common;

Program *Program::create(const std::string &vs, std::string &gs, std::string &fs)
{
	OD_Assert(false);
	return nullptr;
}

Program *Program::create(const std::string &vs, const std::string &fs)
{
	auto vsShader = Shader::create(GL_VERTEX_SHADER, vs);
	if (vsShader == nullptr)
	{
		return nullptr;
	}
	OD_NAMED_SCOPEGUARD(vsGuard, [vsShader]() {delete vsShader;})
	auto fsShader = Shader::create(GL_FRAGMENT_SHADER, fs);
	if (fsShader == nullptr)
	{
		return nullptr;
	}
	OD_NAMED_SCOPEGUARD(fsGuard, [fsShader]() {delete fsShader; })
		auto program = new Program;
	glAttachShader(program->m_program, vsShader->getShaderId());
	glAttachShader(program->m_program, fsShader->getShaderId());
	glLinkProgram(program->m_program);
	int success;
	glGetProgramiv(program->m_program, GL_LINK_STATUS, &success);
	if (!success)
	{
		char logInfo[512];
		glGetProgramInfoLog(program->m_program, 512, NULL, logInfo);
		OD_PRINT_INFO("compile shader failed : %s\n", logInfo);
		OD_Assert(false);
		delete program;
		return nullptr;
	}
	return program;
}

Program::Program()
{
	m_program = glCreateProgram();
}

Program::~Program()
{
	glDeleteProgram(m_program);
}

void Program::apply()
{
	glUseProgram(m_program);
}

void Program::reset()
{
	glUseProgram(0);
}