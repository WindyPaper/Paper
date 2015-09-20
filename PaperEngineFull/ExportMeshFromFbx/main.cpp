#include <iostream>
#include "ExportFromFbx.h"

int main(int argc, char **argv)
{
	ExportFromFbx fbx("test.FBX");
	fbx.saveData("test\\test");

	system("pause");
	return 0;
}