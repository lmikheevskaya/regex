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
	// ������ ���������
	//void SmoothlyShowWindow();		// ������� �������������� ������� ��������� ����
	//void CreateRegion();			// �������� ������� ��� ���� 
	//void CulculHeightWin(int);		// ������ ������ ����
	//void EventOnPaint(HWND);		// ��������� ����
	//BOOL IncludedInRegion(POINT);	// ����������� �������� �� ����� � ������

public:
		
	//int m_height;	// ������ ����
	//SIZE m_sz_edit;
	//SIZE m_sz_win;
	//POINT m_position;
	//int	m_pos_y_finish;	// �������� ������� �� �
	//HWND  m_ParWin;		// �e�������� ������������� ����
	//POINT m_pos_image_but_close;// ���������� ������ "�������" ������������ bitmap
	//POINT m_pos_but_close;		// ���������� ������ "�������" ������������ �������� ���������
	//HINSTANCE m_hInst;			// ���������� ����������   

private:
	void					SearchTag(const char*);				 // ��������� ������ ���������, ����� �����
	void					ProcessTag(std::string, std::string);// ��������� ����
	int					    CreateWin();						 // �������� ����
	int						RegClassWindow(HINSTANCE hIn);
	int		loop();
	void	close();

private:
	HWND					m_edit;				// ���������� edit	
	HRGN					m_hRgn;  			// ����� ������� ������������� ����� ����
	HWND					m_hWndWin;	        // ���������� ����
	std::thread				m_thread;
	HINSTANCE hRTFLib;
};

