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
	// запись сообщени€
	//void SmoothlyShowWindow();		// функци€ обеспечивающа€ плавное по€вление окна
	//void CreateRegion();			// создание региона дл€ окна 
	//void CulculHeightWin(int);		// расчет высоты окна
	//void EventOnPaint(HWND);		// отрисовка окна
	//BOOL IncludedInRegion(POINT);	// отпредел€ет попадает ли точка в регион

public:
		
	//int m_height;	// высота окна
	//SIZE m_sz_edit;
	//SIZE m_sz_win;
	//POINT m_position;
	//int	m_pos_y_finish;	// конечна€ позици€ по у
	//HWND  m_ParWin;		// дeскриптор родительского окна
	//POINT m_pos_image_but_close;// координата кнопки "закрыть" относительно bitmap
	//POINT m_pos_but_close;		// координата кнопки "закрыть" относительно экранных координат
	//HINSTANCE m_hInst;			// дескриптор приложени€   

private:
	void					SearchTag(const char*);				 // обработка текста сообщени€, поиск тегов
	void					ProcessTag(std::string, std::string);// обработка тега
	int					    CreateWin();						 // создание окна
	int						RegClassWindow(HINSTANCE hIn);
	int		loop();
	void	close();

private:
	HWND					m_edit;				// дискриптор edit	
	HRGN					m_hRgn;  			// хэндл региона определ€ющего форму окна
	HWND					m_hWndWin;	        // дискриптор окна
	std::thread				m_thread;
	HINSTANCE hRTFLib;
};

