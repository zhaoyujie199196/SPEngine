#include "Application.h"
#include "Defines.h"
#include "utils/FileUtil.h"

namespace OpenGLDemos
{
	namespace Core {
		struct ApplicationPrivate
		{
			ApplicationPrivate(OpenGLDemos::Core::Application *d, int argc, char **argv)
				: m_d(d)
				, m_argc(argc)
				, m_argv(argv)
			{

			}

			Application *m_d = nullptr;
			int m_argc = 0;
			char **m_argv = nullptr;
			std::string m_cachedAppliationDirPath;
			std::string m_cachedApplicaitonFilePath;
		};
	}
}

using namespace OpenGLDemos::Core;

Application *Application::m_self = nullptr;

Application::Application(int argc, char **argv)
	: m_p(new ApplicationPrivate(this, argc, argv))
{
	init();
}

Application::~Application()
{
	delete m_p;
}

void Application::init()
{
	OD_Assert(m_self == nullptr);
	m_self = this;
}

Application *Application::instance()
{
	return m_self;
}

ApplicationPrivate *Application::p_ptr() const
{
	return m_p;
}

const std::string &Application::applicationDir()
{
	auto p = p_ptr();
	if (p->m_cachedAppliationDirPath.empty())
	{
		p->m_cachedAppliationDirPath = OpenGLDemos::Util::FileUtil::dirPath(applicationPath());
	}
	return p->m_cachedAppliationDirPath;
}

const std::string &Application::applicationPath()
{
	auto p = p_ptr();
	if (p->m_cachedApplicaitonFilePath.empty())
	{
		p->m_cachedApplicaitonFilePath = Util::FileUtil::standardPathSeperate(p->m_argv[0]);
	}
	return p->m_cachedApplicaitonFilePath;
}






