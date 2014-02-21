#include "console.h"
#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#include <fstream>

int DebugConsole::open()
{
	// Create a console for this application
	BOOL ret = AllocConsole();

	// Redirect unbuffered STDOUT to the console 
	HANDLE console_output = GetStdHandle(STD_OUTPUT_HANDLE);
	int sys_output = _open_osfhandle(intptr_t(console_output), _O_TEXT); 
	FILE *output_handle = _fdopen(sys_output, "w"); 
	*stdout = *output_handle; 
	setvbuf(stdout, NULL, _IONBF, 0);
	
	//make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog point to console as well
	std::ios::sync_with_stdio(true); 

	return ret; 
}

void DebugConsole::shutdown()
{
	// Write "Press any key to exit"
	HANDLE console_output = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD chars_written; 
	WriteConsole(console_output, "\nPress any key to exit", 22, &chars_written, 0); 
	
	//Disable line-based input mode so we can get a single character
	HANDLE ConsoleInput = GetStdHandle(STD_INPUT_HANDLE);
	SetConsoleMode(ConsoleInput, 0);
	//Read a single character
	TCHAR InputBuffer;
	DWORD CharsRead;
	ReadConsole(ConsoleInput, &InputBuffer, 1, &CharsRead, 0); 	
}