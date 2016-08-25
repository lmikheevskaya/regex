#include "stdafx.h"
#include "test_windows.h"
#include <stdlib.h>
#include <Richedit.h>
#include <hash_map>
#include <regex>
#include <Shellapi.h>



std::hash_map <std::string, std::string> deq_link_mes;
const POINT position = {100, 100};
const SIZE sz_win = {400, 100};
const SIZE sz_edit = {300, 40};

WindowsTest::WindowsTest() : m_thread(threadTestWindow, this)
{
	// ��������� ���������� RICHED32.DLL
	hRTFLib = LoadLibraryA("RICHED32.DLL");
}

void WindowsTest::WriteMessage(const char* msg)
{
		WindowsTest window;
		::Sleep(100);
		//int err = window.CreateWin();
		while (true)
		{
			if(window.m_thread.joinable()){
		//		//����� � ��������� �����, ������ ���������
				window.SearchTag(msg);
				break;
			}
		}


		//ShowWindow(window.m_hWndWin, SW_SHOW);
	UpdateWindow(window.m_hWndWin);

}

void WindowsTest::close()
{
	m_thread.join();
}

int WindowsTest::loop()
{
	BOOL bRet(FALSE);
	MSG  msg;

	while( (bRet = GetMessage( &msg, NULL, 0, 0 )) != 0)
	{  
		if (bRet == -1)
		{
			int err = GetLastError();
			//ERROR_LOG_STR("ParentWindow(popup)", "Error in GetMessage! ", std::to_string((long long)err));
		}
		else
		{
			TranslateMessage(&msg); 
			DispatchMessage(&msg); 
		}
	}

	return (int)msg.wParam;
}

int WindowsTest::threadTestWindow(LPVOID pData)
{
	WindowsTest *ppwin = static_cast<WindowsTest *>(pData);
	
	int err = ppwin->CreateWin();
	if(err) {
		//ERROR_LOG_STR("ParentWindow(popup)", "Error in CreateWin(thread)! ", std::to_string((long long)err));
		return 0;
	}
	ppwin->loop(); 

	if(ppwin->m_hWndWin) {
		if(ppwin->m_edit)
			DestroyWindow(ppwin->m_edit);
		DestroyWindow(ppwin->m_hWndWin);

		deq_link_mes.clear();
	}

	if(ppwin->hRTFLib)
		FreeLibrary(ppwin->hRTFLib);
	return 0;
}

int CALLBACK WindowsTest::DlgFunc(HWND DlgWin, UINT msg, WPARAM wp, LPARAM lp)
{
	ENLINK     *enLinkInfo;
	REQRESIZE  *pReqResize;
	WindowsTest*	pWindow;
	pWindow = reinterpret_cast<WindowsTest*>(GetWindowLongPtr(DlgWin, GWL_USERDATA));
	switch (msg)
	{
		case WM_NOTIFY:
			switch (((LPNMHDR)lp)->code)                             
			{
				//��������� ������� �� ������
				case EN_LINK:
					enLinkInfo = (ENLINK *)lp;

					if (enLinkInfo->msg == WM_LBUTTONUP)
					{
						char temp2[100] = {0};
						HWND edit = enLinkInfo->nmhdr.hwndFrom;
						SendMessage(edit, EM_EXSETSEL, 0, (LPARAM)(&(enLinkInfo->chrg))); 
						SendMessage(edit, EM_GETSELTEXT, 0, (LPARAM)temp2); 
						
						auto it = deq_link_mes.find(temp2);
						if (it != deq_link_mes.end())
							strcpy_s(temp2, it->second.c_str());
						
						::ShellExecuteA(NULL, "open", temp2, NULL, NULL, SW_SHOWDEFAULT);
					}
					break;
				// ������ ������ ��������� ������ edit
				case EN_REQUESTRESIZE:
					pReqResize = (REQRESIZE *) lp;
					//�������� ������� ����, � ������������ � ����������� ��������� edit
					break;
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:  
			// ��� ��������� �� ������������ ���� ���������� ���� Windows
			return DefWindowProc(DlgWin, msg, wp, lp);
	}

	return 0;
}

int WindowsTest::CreateWin()
{
	//BITMAP  bitmap;

	HINSTANCE m_hInst(nullptr);
	RegClassWindow(m_hInst);

	m_hWndWin = ::CreateWindowA("Test", NULL,  WS_CAPTION | WS_VISIBLE | WS_POPUPWINDOW ,
								position.x, position.y, sz_win.cx, sz_win.cy, 
								NULL, NULL, m_hInst, NULL);

	//��������� � ���������� �� �����
	::SetWindowLongPtr(m_hWndWin, GWL_USERDATA,(LONG_PTR)this);

	//������� ����� ����������
	m_edit = CreateWindowA("RICHEDIT", "", WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY , 10, 10, 
								sz_edit.cx , sz_edit.cy, 
								m_hWndWin, NULL , m_hInst, NULL);	

	 if(!m_edit)
		return -1;


	//������ ���� ����������
	SetWindowLongPtr(m_hWndWin, GWL_EXSTYLE, GetWindowLongPtr(m_hWndWin, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes (m_hWndWin, 0, (255*90)/100, LWA_ALPHA);

	ShowWindow(m_hWndWin, SW_SHOW);
	UpdateWindow(m_hWndWin);
	return 0;
}

int WindowsTest::RegClassWindow(HINSTANCE hIn)
{                                        
	//* ���������� ������ ���� /
	WNDCLASS wc = {0};   // �������� ������ ����

	// ���������� ��������� WNDCLASSEX
	wc.style			 = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		 = (WNDPROC)DlgFunc;
	wc.cbClsExtra		 = 0;
	wc.cbWndExtra	     = 0;
	wc.hInstance	     = hIn;
	wc.hIcon             = LoadIcon(hIn, IDI_APPLICATION);
	// ������������ �������
	wc.hCursor			 = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	 = (HBRUSH) GetStockObject(WHITE_BRUSH);
	wc.lpszClassName	 = TEXT("Test");

	RegisterClass(&wc);
	return 0;
}

void WindowsTest::ProcessTag(std::string text, std::string tag) 
{
	CHARFORMAT cf = {0};
	ZeroMemory(&cf, sizeof(cf));
	cf.cbSize  = sizeof(CHARFORMAT);

	if (tag == "u")// ������������ �����
	{
		cf.dwMask       |= CFE_UNDERLINE;
		cf.dwEffects     = CFE_UNDERLINE;
		
		SendMessageA(m_edit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) & cf);
		SearchTag(text.c_str());

		cf.dwEffects     = ~DWORD(CFE_UNDERLINE);
		SendMessageA(m_edit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) & cf);
	}
	else if(tag == "strong") //��������� �����
	{
		cf.dwMask       |= CFE_ITALIC;
		cf.dwEffects     = CFE_ITALIC;
		
		DWORD lresult = SendMessageA(m_edit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &cf);
		SearchTag(text.c_str());

		cf.dwEffects     = ~DWORD(CFE_ITALIC);
		SendMessageA(m_edit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) & cf);
	}
	else if(tag == "a") //������
	{
		cf.dwMask        = CFE_LINK;
		cf.dwEffects     = CFE_LINK;

		LRESULT mask = SendMessageA(m_edit, EM_GETEVENTMASK, 0, 0); //hWndText is rich edit control
		SendMessageA(m_edit, EM_SETEVENTMASK, 0, mask | ENM_LINK);

		SendMessageA(m_edit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) & cf);
		SearchTag(text.c_str());

		cf.dwEffects     = ~DWORD(CFE_LINK);
		SendMessageA(m_edit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) & cf);
	}
	else if(tag == "h1") //������ ������
	{
		cf.dwMask        = CFM_SIZE;
		cf.yHeight		 = 738;

		SendMessageA(m_edit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) & cf);
		SearchTag(text.c_str());

		cf.yHeight       = 100;
		SendMessageA(m_edit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) & cf);
	}
	else if(tag == "h2") 
	{
		cf.dwMask          = CFM_SIZE;
		cf.yHeight		   = 558;
		
		SendMessageA(m_edit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) & cf);
		SearchTag(text.c_str());

		cf.yHeight       = 100;
		SendMessageA(m_edit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) & cf);
	}
	else if(tag == "h3") 
	{
		cf.dwMask        = CFM_SIZE;
		cf.yHeight		 = 338;
		SendMessageA(m_edit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) & cf);
		SearchTag(text.c_str());

		cf.yHeight       = 100;
		SendMessageA(m_edit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) & cf);
	}
	else if(tag == "h4") //����������� �����
	{
		cf.dwMask        = CFM_SIZE;
		cf.yHeight		 = 100;
		
		SendMessageA(m_edit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) & cf);
		SearchTag(text.c_str());
	}
	else if(tag == "br")	//������� ������
	{
		SendMessageA(m_edit, EM_REPLACESEL, FALSE, (LPARAM)"\n"); 
		SearchTag(text.c_str());
	}
	/*else if(tag == "color") // ������� ����
	{
		cf.dwMask        = CFM_COLOR;
		cf.crTextColor   = RGB(0xFF, 0x00, 0x00);
		SendMessageA(m_edit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) & cf);
		SearchTag(text.c_str());

		cf.crTextColor   = RGB(0, 0, 0) ;
		SendMessageA(m_edit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) & cf);
	}*/
	else 
		SendMessageA(m_edit, EM_REPLACESEL, FALSE, (LPARAM)text.c_str()); 
}

WindowsTest::~WindowsTest()
{
	close();
}

/*void WindowsTest::WriteMessage(const char* p_message)
{
	WindowsTest window;
	int err = window.CreateWin();

	//����� � ��������� �����, ������ ���������
	window.SearchTag(p_message);

	ShowWindow(window.m_hWndWin, SW_SHOW);
	UpdateWindow(window.m_hWndWin);
}*/

//��������� ���������. ����� �����
/*
����� ����� � ������� ���������� ���������. ������� ��� ���� ����� : ������� ������ ���,
�������� ���, ��������� ���
1) ������������ ����� �� ����.
2) ��� ������ ��������� '<', �������� �������� ���
	���� ����� '<' �������� ������, ��� '>', ������ ��� ��� �� ���, ���������� ���� ���� � ���� 
	� ������������ ����� ���
	���� '>' �� ��������� �� ��������� ������, �� ���������� ����� �����
	���� ����� ����������� '>', �� ����� ����� '<' � '>', ��������� �����
3) ���� ��� �� ������, �� ���������� ��� ������ �����, ��, ������� ��������� � ����������� �����
4) ���������� ��� ���� 
5) ������� �����, ������� ���� ����������
6) ������������ ���
7) ������� �� ��������� ������ edit
8) 1. �����, ���� �� �������� ������
*/
void WindowsTest::SearchTag(const char *p_message)
{
	std::string text;
	std::string text_link;
	std::string tag;               // ������� ���
	std::string tag_cl;            // ������ ����� ��������� ����
	std::string page = p_message;  // ������

	std::string closing;
	bool boolalpha = false;
	
	for(size_t i = 0; i < page.size(); ++i)
	{
		if(page[i] == '<')
		{
			++i;
			//���������� �����, ������� ����
			SendMessageA(m_edit, EM_REPLACESEL, FALSE, (LPARAM)text.c_str()); 
			//������� ������� �����
			text.clear();

			//���������� ���
			while(page[i] != '>')
			{
				if (page[i] == '<')
				{
					tag = '<' + tag;
					//���������� �����, ������� ����
					SendMessageA(m_edit, EM_REPLACESEL, FALSE, (LPARAM)tag.c_str()); 
					tag.clear();
					--i;
					break;
				}
				tag += page[i];
				if ((i + 1) < page.size())					
					++i;
				else
				{
					tag = '<' + tag;
					//���������� �����, ������� ����
					SendMessageA(m_edit, EM_REPLACESEL, FALSE, (LPARAM)tag.c_str()); 
					tag.clear();
					break;
				}
			}
			// ������� ������ ����� ��������� ����
			for (auto ii = tag.begin(); ii != tag.end(); ++ii)
			{
				if (*ii == ' ')
					break;
				tag_cl += *ii;
			}

			//�������� ��������� ����� �����
			std::regex regex1("<" + tag + ">(.*?)</" + tag + ">");              // ������� ������ �����
			std::regex regex2("<" + tag_cl + "(.*?)>(.*?)</" + tag_cl + ">");   // ��������  ���
//			std::regex regex3("<" + tag + ">(.*?)");						    // ��������� ���

			if(std::regex_search(page, regex1))
				closing = "</" + tag + ">";
			else if(std::regex_search(page, regex2))
			{
				std::regex link("a href=(.*?)");								  // ���������� ��������� ���� ������
				std::smatch xResults;
				boolalpha = std::regex_match(tag, xResults, link); 
				text_link = xResults[1];								          // ������� �����
				closing = "</" + tag_cl + ">";
			}
			else
				closing = "";

			tag_cl.clear();

			std::regex regex("<" + tag + ">(.*?)" + closing);   

			for (std::sregex_iterator it(page.cbegin(), page.cend(), regex); it != std::sregex_iterator(); ++it) 
			{
				int position = i - tag.length() - 1;
				if (it->position() == position) // ���������, ����� ������������� ������ ������� ���
					text += (*it)[1];
			}
			// ������� ���������� ������� ������������ ����
			i = i + text.length() + closing.length();			

			if (boolalpha) // ���� �������� ��� - ������, �� tag = "a", � ��������� ������� ��������� ��� ����
			{
				tag       = "a";
				deq_link_mes[text] = text_link;
				boolalpha = false;
			}
			ProcessTag(text, tag);

			text.clear();

			tag.clear();
		}
		else
			text += page[i];
	
		//������ ����������� ������ ����
		//�����������, ���� ������ edit ���������
		LRESULT mask = SendMessageA(m_edit, EM_GETEVENTMASK, 0, 0); //hWndText is rich edit control
		SendMessageA(m_edit, EM_SETEVENTMASK, 0, mask | ENM_REQUESTRESIZE);
	}
	
	SendMessageA(m_edit, EM_REPLACESEL, FALSE, (LPARAM)text.c_str()); 
}