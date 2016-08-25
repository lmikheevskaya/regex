// regex.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "test_windows.h"

int _tmain(int argc, _TCHAR* argv[])
{
	WindowsTest::WriteMessage("<strong> test message </strong>");

	WindowsTest::WriteMessage("<u> test message </u>");
	return 0;
}

