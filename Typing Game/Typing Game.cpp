#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "framework.h"
#include "Typing Game.h"
#include "Windows.h"



#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

int left = 100, top = 20, right = left + 305, bottom = top + 400;
char c1, c2;
int x = -1, y = -1;
int iScoring = 0, iFail = 0;
int gameover = 0;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void DrawBk(HDC hdc, int x1, int y1, int x2, int y2)
{
    Rectangle(hdc, x1, y1, x2, y2);
    char s[100] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    TextOutA(hdc, x1 + 5, y2 - 25, s, strlen(s));
}

void ShowScoring(HDC hdc, int x, int y, int iScoring, int iFail)
{
    char szTemp[32];
    TextOutA(hdc, x, y, "Score:", strlen("Score:"));
    y = y + 20;
    sprintf(szTemp, "%d", iScoring);
    TextOutA(hdc, x, y,szTemp, strlen(szTemp));
    y = y + 20;
    TextOutA(hdc, x, y, "Miss:", strlen("Miss:"));
    y = y + 20;
    sprintf(szTemp, "%d", iFail);
    TextOutA(hdc, x, y, szTemp, strlen(szTemp));
}

void GameOver(HDC hdc, int x, int y)
{
    COLORREF OldColor, NewColor = RGB(rand() % 225, rand() % 225, rand() % 225);
    OldColor = SetTextColor(hdc, NewColor);
    TextOutA(hdc, x, y, "GAME OVER !", strlen("GAME OVER !"));
    SetTextColor(hdc, OldColor);
}

void Fire(HDC hdc, int x, int y1, int y2)
{
    HPEN hOldPen, hNewPen = CreatePen(PS_DASHDOTDOT, 1, RGB(255, 0, 0));
    hOldPen = (HPEN)SelectObject(hdc, hNewPen);
    MoveToEx(hdc, x, y1, NULL);
    LineTo(hdc, x, y2);
    Sleep(100);
    HPEN hNewPen2 = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
    SelectObject(hdc, hNewPen2);
    MoveToEx(hdc, x, y2,NULL);
    LineTo(hdc, x, y2);
    SelectObject(hdc, hOldPen);
    DeleteObject(hNewPen);
    DeleteObject(hNewPen2);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TYPINGGAME, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TYPINGGAME));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TYPINGGAME));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TYPINGGAME);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_TIMER:
        switch (wParam)
        {
        case 1:
            y = y + iScoring / 10 + 1;
            if (y > bottom - 40)
            {
                gameover = 1;
                KillTimer(hWnd, 1);
                SetTimer(hWnd, 2, 300, NULL);
            }
            InvalidateRect(hWnd, 0, 0);
            break;
        case 2:
            InvalidateRect(hWnd, 0, 0);
            break;
        }
        break;
    case WM_CHAR:
    {
        c2 = (wParam >= 'a' && wParam <= 'z') ? wParam + 'A' - 'a' : wParam;
        HDC hdc = GetDC(hWnd);
        Fire(hdc, left + 5 + (c2 - 'A') * 9 + 4, top, bottom);
        ReleaseDC(hWnd, hdc);
        if (c2 == c1)
        {
            c1 = rand() % 26 + 'A';
            x = left + 5 + (c1 - 'A') * 9;
            y = top;
            iScoring = iScoring + 1;
        }
        else
            iFail = iFail + 1;
    }
    break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case ID_START:
                if (gameover == 1)
                    KillTimer(hWnd, 2);
                gameover = 0;
                iScoring = 0;
                iFail = 0;
                c1 = rand() % 26 + 'A';
                x = left + 5 + (c1 - 'A') * 9;
                y = top;
                SetTimer(hWnd, 1, 10, NULL);
                break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            DrawBk(hdc, left, top, right, bottom);
            ShowScoring(hdc, right + 20, top + 50, iScoring, iFail);
            if (gameover)
                GameOver(hdc, left + 95, top + 130);
            else
            {
                char szTemp[8];
                sprintf(szTemp, "%c", c1);
                TextOutA(hdc, x, y, szTemp, strlen(szTemp));
            }
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    
    
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hWnd, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
