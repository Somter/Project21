#include <iostream>
#include <string>
#include "resource.h"
#include "WinSock2.h"
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

HWND hedit;
HWND hButton1;
HWND hButton2;
HWND hList1;
const int MAXSTRLEN = 255;
WSADATA wsaData;
SOCKET _socket;
SOCKET acceptSocket;
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
        inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr);
        addr.sin_port = htons(20000);
        bind(_socket, (SOCKADDR*)&addr, sizeof(addr));

        break;
    }
    case WM_COMMAND:
    {
        listen(_socket, 1);
        switch (LOWORD(wp))
        {
        case IDC_BUTTON1:   
        {
           
            acceptSocket = accept(_socket, NULL, NULL);
            char buf[MAXSTRLEN];
            int i = recv(acceptSocket, buf, MAXSTRLEN, 0);
            buf[i] = '\0';

            SendMessage(hList1, LB_ADDSTRING, 0, (LPARAM)buf); 
            break;
        }
        case IDC_BUTTON2:
        {
            const char* text = "Hello from server!";
            send(acceptSocket, text, strlen(text), 0);
            break;
        }
        }
        break;
    }
    case WM_CLOSE:
        closesocket(acceptSocket);
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
