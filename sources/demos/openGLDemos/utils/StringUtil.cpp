#include "StringUtil.h"

using namespace OpenGLDemos::Util;

std::string StringUtil::replace(const std::string &s, const std::string &before, const std::string &after)
{
	if (s.empty())
	{
		return s;
	}
	std::string result = s;
	if (after.empty())
	{
		size_t pos = std::string::npos;
		while ((pos = result.find(before)) != std::string::npos)
		{
			result.erase(pos, before.length());
		}
	}
	else
	{
		for (std::string::size_type pos(0); pos != std::string::npos; pos += after.length())
		{
			pos = result.find(before, pos);
			if (pos != std::string::npos)
			{
				result.replace(pos, before.length(), after);
			}
			else
			{
				break;
			}
		}
	}
	return result;
}
