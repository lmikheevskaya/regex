#pragma once
#include <Windows.h>
#include <string>
#include <thread>

class WindowsTest 
{		
private:
	WindowsTest();

public:
	~WindowsTest();	

	static void WriteMessage(const char* msg);
	static int CALLBACK WindowsTest::DlgFunc(HWND DlgWin, UINT msg, WPARAM wp, LPARAM lp);		  
	static int threadTestWindow(LPVOID);

private:
	void SearchTag(const char*); // обработка текста сообщения, поиск тегов
	void ProcessTag(std::string, std::string);// обработка тега
	int CreateWin();				
	int RegClassWindow(HINSTANCE hIn);
	int loop();
	void close();

private:
	HWND m_edit;	
	HRGN m_hRgn;  // хэндл региона определяющего форму окна
	HWND m_hWndWin;	    
	std::thread m_thread;
	HINSTANCE hRTFLib;
};

