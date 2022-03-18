#pragma once

#include <string>
#include <list>
namespace OpenGLDemos
{
	namespace Util {
		class FileUtil
		{
		public:
			//application路径
			static const std::string &applicationDir();
			//builtiRes路径
			static const std::string &builtinResDir();
			//拼接路径
			static std::string combinePath(const std::list<std::string> &pathes, bool addSeperate = false);
			//规范文件分割符号
			static std::string standardPathSeperate(const std::string &path);
			//获取文件的dir路径
			static std::string dirPath(const std::string &path);
		};
    }
}