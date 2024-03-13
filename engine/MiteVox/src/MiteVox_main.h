
#include "MiteVox.h"

#include <exception>

int main(int argc, char* argv[])
{
	mitevox::Engine* MiteVox = new mitevox::Engine(argc, argv);

	try
	{
		MiteVox->run();
		delete MiteVox;
	}
	catch (std::exception exc) 
	{
		std::cerr << exc.what();
		getchar();
	}

	return 0;
}