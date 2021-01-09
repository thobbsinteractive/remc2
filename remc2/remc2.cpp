//#include "stdafx.h"

#include "engine/engine_support.h"
#include "sub_main.h"

using namespace std;

int main(int argc, char** argv)
{
	
	//char *argv[] = { "netherw.exe","-level","2", NULL };
	//int argc = (sizeof(argv) / sizeof(argv[0])) - 1;
	char *envp[] = { NULL };
	//char *envp[] = { "env=xx", NULL };
	//ds:esi - cesta k nazvu
	//mine_texts((char*)"0160-0022A288", 0x2a1000, 1000, (char*)"mined-texts.txt");

	support_begin();
	int retval = sub_main(argc, argv, envp);
	support_end();
	//saveactstate();
    return 0;
}

