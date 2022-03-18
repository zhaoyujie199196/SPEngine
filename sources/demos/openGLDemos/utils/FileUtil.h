#pragma once

#include <string>
#include <list>
namespace OpenGLDemos
{
	namespace Util {
		class FileUtil
		{
		public:
			//application·��
			static const std::string &applicationDir();
			//builtiRes·��
			static const std::string &builtinResDir();
			//ƴ��·��
			static std::string combinePath(const std::list<std::string> &pathes, bool addSeperate = false);
			//�淶�ļ��ָ����
			static std::string standardPathSeperate(const std::string &path);
			//��ȡ�ļ���dir·��
			static std::string dirPath(const std::string &path);
		};
    }
}