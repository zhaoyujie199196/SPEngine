#include <iostream>
#include "example0.h"
#include "example1.h"
#include "example2.h"
#include "core/Application.h"

int main(int argc, char *argv[])
{
//	OpenGLDemos::HelloTrigger::execute();
	OpenGLDemos::Core::Application a(argc, argv);
	std::cout << a.applicationDir();
	OpenGLDemos::Example2::execute();
	std::cout << "hello world" << std::endl;
	return 0;
}