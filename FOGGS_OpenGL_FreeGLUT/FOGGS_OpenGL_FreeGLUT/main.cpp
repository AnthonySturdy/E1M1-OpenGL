#include "HelloGL.h"
#include<ctime>

int main(int argc, char* argv[]) {
	srand(time(NULL));

	HelloGL* game = new HelloGL(argc, argv);
	return 0;
}