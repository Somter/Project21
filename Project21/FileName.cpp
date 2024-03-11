#include <iostream>
#include <string>
#include "resource.h"	
#include "WinSock2.h" //здесь наход€тс€ объ€влени€, необходимые
//дл€ Winsock 2 API.
#include <ws2tcpip.h> //содержит функции дл€ работы с адресами
 //напр. inet_pton
#pragma comment(lib, "Ws2_32.lib") //линкуем библиотеку Windows Sockets
using namespace std;	

HWND hedit;
HWND hButton1;
HWND hButton2;
HWND hList1;	
const int MAXSTRLEN = 255;
WSADATA wsaData;
SOCKET _socket; 
sockaddr_in addr;	


int CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp)
{
    switch (mes)
    {
    case WM_INITDIALOG:
    {
        hedit = GetDlgItem(hWnd, IDC_EDIT1);
        hButton1 = GetDlgItem(hWnd, IDC_BUTTON1);
        hButton2 = GetDlgItem(hWnd, IDC_BUTTON2);
        hList1 = GetDlgItem(hWnd, IDC_LIST1);

        WSAStartup(MAKEWORD(2, 2), &wsaData);
        _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        addr.sin_family = AF_INET;
        inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
        addr.sin_port = htons(20000);

        break;
    }
    case WM_COMMAND:
    {
        connect(_socket, (SOCKADDR*)&addr, sizeof(addr));   
        switch (LOWORD(wp))
        {
        case IDC_BUTTON1:
        {
            char buf[MAXSTRLEN];
            int i = recv(_socket, buf, MAXSTRLEN, 0);
            buf[i] = '\0';

            SendMessage(hList1, LB_ADDSTRING, 0, (LPARAM)buf);
            break;
        }
        case IDC_BUTTON2:   
        {
            char str[MAXSTRLEN];
            GetWindowText(hedit, (LPWSTR)str, MAXSTRLEN);
            send(_socket, str, strlen(str), 0);
            break;
        }
        }
        break;
    }
    case WM_CLOSE:
        closesocket(_socket);
        WSACleanup();
        EndDialog(hWnd, 0);
        return TRUE;
    }
    return FALSE;
}
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpszCmdLine, int nCmdShow) { 
    return DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);  
}

