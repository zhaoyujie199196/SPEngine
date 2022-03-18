#include "FileUtil.h"
#include "core/Application.h"
#include "StringUtil.h"
#include <mutex>

using namespace OpenGLDemos::Util;

const std::string &FileUtil::applicationDir()
{
	static std::string s_applicationDir;
	static std::once_flag s_onceFlag;
	std::call_once(s_onceFlag, []() {
		s_applicationDir = OD_APP->applicationDir();
	});
	return s_applicationDir;
}

const std::string &FileUtil::builtinResDir()
{
	static std::string s_builtinResDir;
	static std::once_flag s_onceFlag;
	std::call_once(s_onceFlag, []() {
		s_builtinResDir = applicationDir() + "/builtinRes";
	});
	return s_builtinResDir;
}

std::string FileUtil::combinePath(const std::list<std::string> &pathes, bool addSeperate /* = false */)
{
	std::string result;
	for (auto &path : pathes)
	{
		standardPathSeperate(path);
		result += path;
		if (result[result.length() - 1] != '/')
		{
			result += '/';
		}
	}
	if (!addSeperate)
	{
		result = result.substr(0, result.length() - 1);
	}
	return result;
}

std::string FileUtil::standardPathSeperate(const std::string &path)
{
	return StringUtil::replace(path, "\\", "/");
}

std::string FileUtil::dirPath(const std::string &path)
{
	auto pos = path.find_last_of('/');
	if (pos == std::string::npos)
	{
		return "/";
	}
	return path.substr(0, pos);
}