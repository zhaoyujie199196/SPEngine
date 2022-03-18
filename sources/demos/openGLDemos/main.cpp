#include <iostream>
#include "helloTrigger.h"
#include "textureDemo.h"
#include "core/Application.h"

int main(int argc, char *argv[])
{
//	OpenGLDemos::HelloTrigger::execute();
	OpenGLDemos::Core::Application a(argc, argv);
	std::cout << a.applicationDir();
	OpenGLDemos::textureDemo::execute();
	std::cout << "hello world" << std::endl;
	return 0;
}