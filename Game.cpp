// labWinAPI.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Game.h"
#include "time.h"
#include <string>

#pragma comment(lib, "Msimg32.lib")

#define MAX_LOADSTRING 100
#define SIZE_CHILD_WINDOW_X 60
#define SIZE_CHILD_WINDOW_Y 60
#define NUMBER_OF_CHILDS 16
#define NUMBER_OF_CHILDS_IN_ROW 4
#define NUMBER_OF_CHILDS_IN_COLUMN 5
#define SPACE 10
#define MAIN_WINDOW_START_POSITION 350
// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
int xSizeMainWindow = 0;                        // x parameter of size main window
int ySizeMainWindow = 0;                        // y parameter of size main window
HWND hWndParentWindow;                          // handle to main window
HWND hWndChildWindow;                           // handle to second main window
HWND hWndChildMain[4][4];                       // handle to child main window
HWND hWndChildSecond[4][4];                     // handle to child second window
HWND hWndScore1;                                // handle to score window in main
HWND hWndScore2;                                // handle to score window in second
int scoreTable[4][4];                           // score table
int goal = 8;                                   // the goal of the game (default 8)
bool gameOver = false;                          // points if the game is over
int points = 0;                                 // store points

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
ATOM                MyRegisterClassChild(HINSTANCE hInstance);
BOOL                InitInstanceChild(HINSTANCE, int, HWND, int, int, int, int);
LRESULT CALLBACK    WndProcChild(HWND, UINT, WPARAM, LPARAM);
void                CreateFields(HWND hWnd);
ATOM                MyRegisterClassSecondWindow(HINSTANCE hInstance);
BOOL                InitInstanceSecondWindow(HINSTANCE, int, HWND, int, int);
LRESULT CALLBACK    WndProcSecondWindow(HWND, UINT, WPARAM, LPARAM);
void                MovingWindow(HWND, LPARAM);
void                ChangeLayer(HWND , HWND);
bool                Overlap(RECT, RECT);
ATOM                MyRegisterClassScore(HINSTANCE hInstance);
BOOL                InitInstanceScore(HINSTANCE, int, HWND, int, int);
LRESULT CALLBACK    WndProcScore(HWND, UINT, WPARAM, LPARAM);
void                GameDown();
void                GameLeft();
void                GameRight();
void                GameUp();
void                Draw(HWND, TCHAR[]);
void                Paint(HWND hWnd, COLORREF color);
void                UpdateFields();
bool                IsItEnd();
void                End(bool win);
bool                isPlace();
void                Unchecked();
void                EndGameCommunicate(HWND, HBITMAP, TCHAR[], int);
void                SavingToFile();
void                ReadingFromFile();

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
    LoadStringW(hInstance, IDC_LABWINAPI, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    MyRegisterClassChild(hInstance);
    MyRegisterClassSecondWindow(hInstance);
    MyRegisterClassScore(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LABWINAPI));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(132));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = CreateSolidBrush(RGB(250, 247, 238));
    wcex.lpszMenuName   = MAKEINTRESOURCE(IDC_LABWINAPI);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(132));

    return RegisterClassExW(&wcex);
}

ATOM MyRegisterClassChild(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProcChild;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = NULL;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = CreateSolidBrush(RGB(250, 247, 238));
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"Child";
    wcex.hIconSm = NULL;

    return RegisterClassExW(&wcex);
}

ATOM MyRegisterClassSecondWindow(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProcSecondWindow;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = NULL;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = CreateSolidBrush(RGB(250, 247, 238));
    wcex.lpszMenuName = MAKEINTRESOURCE(IDC_LABWINAPI);
    wcex.lpszClassName = L"Second Window";
    wcex.hIconSm = NULL;

    return RegisterClassExW(&wcex);
}

ATOM MyRegisterClassScore(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProcScore;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = NULL;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = CreateSolidBrush(RGB(250, 247, 238));
    wcex.lpszMenuName = MAKEINTRESOURCE(IDC_LABWINAPI);
    wcex.lpszClassName = L"Score";;
    wcex.hIconSm = NULL;

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

   // Calculation of the desired size
   RECT rcSize;
   rcSize.left = 0;
   rcSize.right = (NUMBER_OF_CHILDS_IN_ROW + 1) * SPACE + NUMBER_OF_CHILDS_IN_ROW * SIZE_CHILD_WINDOW_X;
   rcSize.top = 0;
   rcSize.bottom = (NUMBER_OF_CHILDS_IN_COLUMN + 1) * SPACE + NUMBER_OF_CHILDS_IN_COLUMN * SIZE_CHILD_WINDOW_Y;
   AdjustWindowRect(&rcSize, WS_OVERLAPPEDWINDOW, TRUE);
   xSizeMainWindow = rcSize.right - rcSize.left;
   ySizeMainWindow = rcSize.bottom - rcSize.top;

   // Start position first window
   int xPos1 = GetSystemMetrics(SM_CXSCREEN) / 2 - MAIN_WINDOW_START_POSITION;
   int yPos1 = GetSystemMetrics(SM_CYSCREEN) / 2 - MAIN_WINDOW_START_POSITION;

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
       xPos1, yPos1, xSizeMainWindow, ySizeMainWindow, nullptr, nullptr, hInstance, nullptr);
   hWndParentWindow = hWnd;

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   CreateFields(hWnd);

   if (!(InitInstanceSecondWindow(hInstance, nCmdShow, hWnd, xSizeMainWindow, ySizeMainWindow)
       && InitInstanceScore(hInstance, nCmdShow, hWnd, xSizeMainWindow, ySizeMainWindow)))
   {
       return false;
   }

   srand((unsigned int)time(NULL));
   int x = rand() % NUMBER_OF_CHILDS_IN_ROW;
   int y = rand() % (NUMBER_OF_CHILDS_IN_COLUMN - 1);
   TCHAR s[] = _T("2");
   scoreTable[x][y] = 2;
   Draw(hWndChildMain[x][y], s);
   Draw(hWndChildSecond[x][y], s);

   goal = 8;
   Unchecked();
   CheckMenuItem(GetMenu(hWndParentWindow), ID_GOAL_8, MF_CHECKED);
   CheckMenuItem(GetMenu(hWndChildWindow), ID_GOAL_8, MF_CHECKED);

   FILE* stream = NULL;
   // make sure the file does not already exist
   if (_wfopen_s(&stream, L"2048.ini", L"r") == ENOENT)
   {
       _wfopen_s(&stream, L"2048.ini", L"w, ccs=UTF-8");
   }
   if (stream != NULL) fclose(stream);
   ReadingFromFile();

   return true;
}

BOOL InitInstanceChild(HINSTANCE hInstance, int nCmdShow, HWND hWndParent, int x, int y, int i, int j)
{
    hInst = hInstance; // Store instance handle in our global variable

    if (hWndParent == hWndParentWindow)
    {
        hWndChildMain[i][j] = CreateWindowW(L"Child", szTitle, WS_CHILD | WS_VISIBLE,
            x, y, SIZE_CHILD_WINDOW_X, SIZE_CHILD_WINDOW_Y, hWndParent, nullptr, hInstance, nullptr);

        if (!hWndChildMain[i][j])
        {
            return FALSE;
        }

        ShowWindow(hWndChildMain[i][j], nCmdShow);
        UpdateWindow(hWndChildMain[i][j]);
    }
    else
    {
        hWndChildSecond[i][j] = CreateWindowW(L"Child", szTitle, WS_CHILD | WS_VISIBLE,
            x, y, SIZE_CHILD_WINDOW_X, SIZE_CHILD_WINDOW_Y, hWndParent, nullptr, hInstance, nullptr);

        if (!hWndChildMain[i][j])
        {
            return FALSE;
        }

        ShowWindow(hWndChildSecond[i][j], nCmdShow);
        UpdateWindow(hWndChildSecond[i][j]);
    }

    return TRUE;
}

BOOL InitInstanceSecondWindow(HINSTANCE hInstance, int nCmdShow, HWND hWnd, int xSizeMainWindow, int ySizeMainWindow)
{
    hInst = hInstance; // Store instance handle in our global variable

    // Start position second window
    int xPos2 = GetSystemMetrics(SM_CXSCREEN) / 2 + MAIN_WINDOW_START_POSITION - xSizeMainWindow;
    int yPos2 = GetSystemMetrics(SM_CYSCREEN) / 2 + MAIN_WINDOW_START_POSITION - ySizeMainWindow;

    HWND hWnd2 = CreateWindowW(L"Second Window", szTitle, WS_CAPTION,
        xPos2, yPos2, xSizeMainWindow, ySizeMainWindow, hWnd, nullptr, hInstance, nullptr);
    hWndChildWindow = hWnd2;

    if (!hWnd2)
    {
        return FALSE;
    }

    ShowWindow(hWnd2, nCmdShow);
    UpdateWindow(hWnd2);
    CreateFields(hWnd2);

    return InitInstanceScore(hInstance, nCmdShow, hWnd2, xSizeMainWindow, ySizeMainWindow);;
}

BOOL InitInstanceScore(HINSTANCE hInstance, int nCmdShow, HWND hWndParent, int x, int y)
{
    hInst = hInstance; // Store instance handle in our global variable

    if (hWndParent == hWndParentWindow)
    {
        hWndScore1 = CreateWindowW(L"Score", szTitle, WS_CHILD | WS_VISIBLE,
            SPACE, SPACE, SIZE_CHILD_WINDOW_X * 4 + 3 * SPACE, SIZE_CHILD_WINDOW_Y, hWndParent, nullptr, hInstance, nullptr);
        ShowWindow(hWndScore1, nCmdShow);
        UpdateWindow(hWndScore1);
    }
    else
    {
        hWndScore2 = CreateWindowW(L"Score", szTitle, WS_CHILD | WS_VISIBLE,
            SPACE, SPACE, SIZE_CHILD_WINDOW_X * 4 + 3 * SPACE, SIZE_CHILD_WINDOW_Y, hWndChildWindow, nullptr, hInstance, nullptr);
        ShowWindow(hWndScore2, nCmdShow);
        UpdateWindow(hWndScore2);
    }

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
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case ID_GOAL_8:
        {
            goal = 8;
            Unchecked();
            CheckMenuItem(GetMenu(hWndParentWindow), ID_GOAL_8, MF_CHECKED);
            CheckMenuItem(GetMenu(hWndChildWindow), ID_GOAL_8, MF_CHECKED);
        }
        break;
        case ID_GOAL_16:
        {
            goal = 16;
            Unchecked();
            CheckMenuItem(GetMenu(hWndParentWindow), ID_GOAL_16, MF_CHECKED);
            CheckMenuItem(GetMenu(hWndChildWindow), ID_GOAL_16, MF_CHECKED);
        }
        break;
        case ID_GOAL_64:
        {
            goal = 64;
            Unchecked();
            CheckMenuItem(GetMenu(hWndParentWindow), ID_GOAL_64, MF_CHECKED);
            CheckMenuItem(GetMenu(hWndChildWindow), ID_GOAL_64, MF_CHECKED);
        }
        break;
        case ID_GOAL_2048:
        {
            goal = 2048;
            Unchecked();
            CheckMenuItem(GetMenu(hWndParentWindow), ID_GOAL_2048, MF_CHECKED);
            CheckMenuItem(GetMenu(hWndChildWindow), ID_GOAL_2048, MF_CHECKED);
        }
        break;
        case IDM_EXIT:
        {
            gameOver = false;
            points = 0;
            for (int i = 0; i < NUMBER_OF_CHILDS_IN_ROW; i++)
            {
                for (int j = 0; j < NUMBER_OF_CHILDS_IN_COLUMN - 1; j++) scoreTable[i][j] = 0;
            }

            InvalidateRect(hWndParentWindow, NULL, TRUE);
            InvalidateRect(hWndChildWindow, NULL, TRUE);
            /*InvalidateRect(hWndScore1, NULL, TRUE);
            InvalidateRect(hWndScore2, NULL, TRUE);*/
            UpdateWindow(hWndParentWindow);
            UpdateWindow(hWndChildWindow);
            srand((unsigned int)time(NULL));
            int x = rand() % NUMBER_OF_CHILDS_IN_ROW;
            int y = rand() % (NUMBER_OF_CHILDS_IN_COLUMN - 1);
            scoreTable[x][y] = 2;
            UpdateFields();
            SavingToFile();
        }
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
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
    {  
        SavingToFile();
        PostQuitMessage(0);
    }
        break;
    case WM_GETMINMAXINFO:
         {
            MINMAXINFO * minMaxInfo = (MINMAXINFO*)lParam;
            minMaxInfo->ptMaxSize.x = minMaxInfo->ptMaxTrackSize.x = xSizeMainWindow;
            minMaxInfo->ptMaxSize.y = minMaxInfo->ptMaxTrackSize.y = ySizeMainWindow;
            minMaxInfo->ptMinTrackSize.x = xSizeMainWindow;
            minMaxInfo->ptMinTrackSize.y = ySizeMainWindow;
         }
        break;
    case WM_CHAR:
    {
        if (gameOver == false)
        {
            switch (wParam)
            {
            case 0x77:
                GameUp();
                break;
            case 0x73:
                GameDown();
                break;
            case 0x61:
                GameLeft();
                break;
            case 0x64:
                GameRight();
                break;
            default:
                break;
            }
        }
    }
    break;
    case WM_MOVE:
        {
           MovingWindow(hWndChildWindow, lParam);
        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK WndProcChild(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {
        RECT rc;
        GetWindowRect(hWnd, &rc);
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...
        HBRUSH brush = CreateSolidBrush(RGB(204, 192, 174));
        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);
        HPEN pen = CreatePen(PS_SOLID, 1, RGB(204, 192, 174));
        HPEN oldPen = (HPEN)SelectObject(hdc, pen);
        RoundRect(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, 20, 20);
        SelectObject(hdc, oldBrush);
        DeleteObject(brush);
        SelectObject(hdc, oldPen);
        DeleteObject(pen);
        EndPaint(hWnd, &ps);
    }
    break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK WndProcSecondWindow(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case ID_GOAL_8:
        {
            goal = 8;
            Unchecked();
            CheckMenuItem(GetMenu(hWndParentWindow), ID_GOAL_8, MF_CHECKED);
            CheckMenuItem(GetMenu(hWndChildWindow), ID_GOAL_8, MF_CHECKED);
        }
        break;
        case ID_GOAL_16:
        {
            goal = 16;
            Unchecked();
            CheckMenuItem(GetMenu(hWndParentWindow), ID_GOAL_16, MF_CHECKED);
            CheckMenuItem(GetMenu(hWndChildWindow), ID_GOAL_16, MF_CHECKED);
        }
        break;
        case ID_GOAL_64:
        {
            goal = 64;
            Unchecked();
            CheckMenuItem(GetMenu(hWndParentWindow), ID_GOAL_64, MF_CHECKED);
            CheckMenuItem(GetMenu(hWndChildWindow), ID_GOAL_64, MF_CHECKED);
        }
        break;
        case ID_GOAL_2048:
        {
            goal = 2048;
            Unchecked();
            CheckMenuItem(GetMenu(hWndParentWindow), ID_GOAL_2048, MF_CHECKED);
            CheckMenuItem(GetMenu(hWndChildWindow), ID_GOAL_2048, MF_CHECKED);
        }
        break;
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
        {
            points = 0;
            for (int i = 0; i < NUMBER_OF_CHILDS_IN_ROW; i++)
            {
                for (int j = 0; j < NUMBER_OF_CHILDS_IN_COLUMN - 1; j++) scoreTable[i][j] = 0;
            }

            srand((unsigned int)time(NULL));
            int x = rand() % NUMBER_OF_CHILDS_IN_ROW;
            int y = rand() % (NUMBER_OF_CHILDS_IN_COLUMN - 1);
            TCHAR s[] = _T("2");
            scoreTable[x][y] = 2;
            UpdateFields();
        }
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
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        SavingToFile();
        PostQuitMessage(0);
        break;
    case WM_GETMINMAXINFO:
    {
        MINMAXINFO* minMaxInfo = (MINMAXINFO*)lParam;
        minMaxInfo->ptMaxSize.x = minMaxInfo->ptMaxTrackSize.x = xSizeMainWindow;
        minMaxInfo->ptMaxSize.y = minMaxInfo->ptMaxTrackSize.y = ySizeMainWindow;
        minMaxInfo->ptMinTrackSize.x = xSizeMainWindow;
        minMaxInfo->ptMinTrackSize.y = ySizeMainWindow;
    }
    break;
    case WM_CHAR:
    {
        if (gameOver == false)
        {
            switch (wParam)
            {
            case 0x77:
                GameUp();
                break;
            case 0x73:
                GameDown();
                break;
            case 0x61:
                GameLeft();
                break;
            case 0x64:
                GameRight();
                break;
            default:
                break;
            }
        }
    }
    break;
    case WM_MOVE:
    {
        MovingWindow(hWndParentWindow, lParam);
        ChangeLayer(hWnd, hWndParentWindow);
    }
    break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK WndProcScore(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...
        HBRUSH brush = CreateSolidBrush(RGB(204, 192, 174));
        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);
        HPEN pen = CreatePen(PS_SOLID, 1, RGB(204, 192, 174));
        HPEN oldPen = (HPEN)SelectObject(hdc, pen);
        RoundRect(hdc, 0, 0, SIZE_CHILD_WINDOW_X * 4 + 3 * SPACE, SIZE_CHILD_WINDOW_Y, 20, 20);
        SelectObject(hdc, oldBrush);
        DeleteObject(brush);
        SelectObject(hdc, oldPen);
        DeleteObject(pen);
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
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void CreateFields(HWND hWnd)
{
    RECT rc;
    GetClientRect(hWnd, &rc);

    int x = 0;
    int y = 0;

    for (int i = 0; i < NUMBER_OF_CHILDS_IN_ROW; i++)
    {
        x = rc.left + SPACE + i * SIZE_CHILD_WINDOW_X + i * SPACE;
        for (int j = 0; j < NUMBER_OF_CHILDS_IN_COLUMN - 1; j++)
        {
            y = rc.top + SIZE_CHILD_WINDOW_Y + 2 * SPACE + j * SIZE_CHILD_WINDOW_Y + j * SPACE;
            InitInstanceChild(hInst, SW_NORMAL, hWnd, x, y, i, j);
        }

    }
}

void MovingWindow(HWND hWndToMove, LPARAM lParam)
{
    int xMoving = (int)(short)LOWORD(lParam) + xSizeMainWindow / 2;   // horizontal position 
    int yMoving = (int)(short)HIWORD(lParam) + ySizeMainWindow / 2;   // vertical position 

    // Center positions
    int xCenter = GetSystemMetrics(SM_CXSCREEN) / 2;
    int yCenter = GetSystemMetrics(SM_CYSCREEN) / 2;

    // New positions another window
    int xToMove = (xCenter - xMoving) + xCenter - xSizeMainWindow / 2;
    int yToMove = (yCenter - yMoving) + yCenter - ySizeMainWindow / 2;

    MoveWindow(hWndToMove, xToMove, yToMove, xSizeMainWindow, ySizeMainWindow, TRUE);
}

void ChangeLayer(HWND hWnd1, HWND hWnd2)
{
    RECT rc1, rc2;
    GetWindowRect(hWnd1, &rc1);
    GetWindowRect(hWnd2, &rc2);
    if (Overlap(rc1, rc2))
    {
        SetWindowLong(hWnd1, GWL_EXSTYLE, GetWindowLong(hWnd2, GWL_EXSTYLE) | WS_EX_LAYERED);
        SetLayeredWindowAttributes(hWnd1, 0, (255 * 50) / 100, LWA_ALPHA);
    }
    else
    {
        SetWindowLong(hWnd1, GWL_EXSTYLE, GetWindowLong(hWnd2, GWL_EXSTYLE));
    }
}

bool Overlap(RECT rc1, RECT rc2)
{
    if (rc1.left < rc2.left && rc2.left < rc1.right && rc1.top < rc2.top && rc2.top < rc1.bottom) return true;
    if (rc1.left < rc2.left && rc2.left < rc1.right && rc1.top < rc2.bottom && rc2.bottom < rc1.bottom) return true;
    if (rc1.left < rc2.right && rc2.right < rc1.right && rc1.top < rc2.top && rc2.top < rc1.bottom) return true;
    if (rc1.left < rc2.right && rc2.right < rc1.right && rc1.top < rc2.bottom && rc2.bottom < rc1.bottom) return true;
    return false;
}

void GameDown()
{
    for (int i = 0; i < NUMBER_OF_CHILDS_IN_ROW; i++)
    {
        for (int j = NUMBER_OF_CHILDS_IN_ROW - 1; j > 0; --j)
        {
                int k = j;
                while (k >= 0 && scoreTable[i][j] == 0)
                {
                    if (scoreTable[i][k] != 0)
                    {
                        scoreTable[i][j] = scoreTable[i][k];
                        scoreTable[i][k] = 0;
                    }
                    --k;
                }
        }
    }

    for (int i = 0; i < NUMBER_OF_CHILDS_IN_ROW; i++)
    {
        int m = NUMBER_OF_CHILDS_IN_ROW - 1;
        for (int j = NUMBER_OF_CHILDS_IN_ROW - 2; j >= 0; --j)
        {
            if (scoreTable[i][j] == scoreTable[i][j + 1])
            {
                scoreTable[i][m] = scoreTable[i][j] + scoreTable[i][j + 1];
                points += scoreTable[i][m];
                scoreTable[i][j] = 0;
                m = j;
                --j;
            }
            else --m;
        }
    }

    for (int i = 0; i < NUMBER_OF_CHILDS_IN_ROW; i++)
    {
        for (int j = NUMBER_OF_CHILDS_IN_ROW - 1; j > 0; --j)
        {
            int k = j;
            while (k >= 0 && scoreTable[i][j] == 0)
            {
                if (scoreTable[i][k] != 0)
                {
                    scoreTable[i][j] = scoreTable[i][k];
                    scoreTable[i][k] = 0;
                }
                --k;
            }
        }
    }

    if (isPlace())
    {
        int x = rand() % NUMBER_OF_CHILDS_IN_ROW;
        int y = rand() % NUMBER_OF_CHILDS_IN_ROW;
        while (scoreTable[x][y] != 0)
        {
            x = rand() % NUMBER_OF_CHILDS_IN_ROW;
            y = rand() % NUMBER_OF_CHILDS_IN_ROW;
        }
        scoreTable[x][y] = 2;
    }
    UpdateFields();
    bool win = IsItEnd();
    if (gameOver) End(win);
}

void GameLeft()
{
    for (int i = 0; i < NUMBER_OF_CHILDS_IN_ROW; ++i)
    {
        for (int j = 0; j < NUMBER_OF_CHILDS_IN_ROW - 1; ++j)
        {
            int k = j;
            while (k <= NUMBER_OF_CHILDS_IN_ROW - 1 && scoreTable[j][i] == 0)
            {
                if (scoreTable[k][i] != 0)
                {
                    scoreTable[j][i] = scoreTable[k][i];
                    scoreTable[k][i] = 0;
                }
                ++k;
            }
        }
    }

    for (int i = 0; i < NUMBER_OF_CHILDS_IN_ROW; i++)
    {
        int m = 0;
        for (int j = 1; j <= NUMBER_OF_CHILDS_IN_ROW - 1; ++j)
        {
            if (scoreTable[j][i] == scoreTable[j - 1][i])
            {
                scoreTable[m][i] = scoreTable[j][i] + scoreTable[j - 1][i];
                points += scoreTable[m][i];
                scoreTable[j][i] = 0;
                m = j;
                ++j;
            }
            else ++m;
        }
    }

    for (int i = 0; i < NUMBER_OF_CHILDS_IN_ROW; ++i)
    {
        for (int j = 0; j < NUMBER_OF_CHILDS_IN_ROW - 1; ++j)
        {
            int k = j;
            while (k <= NUMBER_OF_CHILDS_IN_ROW - 1 && scoreTable[j][i] == 0)
            {
                if (scoreTable[k][i] != 0)
                {
                    scoreTable[j][i] = scoreTable[k][i];
                    scoreTable[k][i] = 0;
                }
                ++k;
            }
        }
    }

    if (isPlace())
    {
        int x = rand() % NUMBER_OF_CHILDS_IN_ROW;
        int y = rand() % NUMBER_OF_CHILDS_IN_ROW;
        while (scoreTable[x][y] != 0)
        {
            x = rand() % NUMBER_OF_CHILDS_IN_ROW;
            y = rand() % NUMBER_OF_CHILDS_IN_ROW;
        }
        scoreTable[x][y] = 2;
    }
    UpdateFields();
    bool win = IsItEnd();
    if (gameOver) End(win);
}

void GameRight()
{
    for (int i = 0; i < NUMBER_OF_CHILDS_IN_ROW; i++)
    {
        for (int j = NUMBER_OF_CHILDS_IN_ROW - 1; j > 0; --j)
        {
            int k = j;
            while (k >= 0 && scoreTable[j][i] == 0)
            {
                if (scoreTable[k][i] != 0)
                {
                    scoreTable[j][i] = scoreTable[k][i];
                    scoreTable[k][i] = 0;
                }
                --k;
            }
        }
    }

    for (int i = 0; i < NUMBER_OF_CHILDS_IN_ROW; i++)
    {
        int m = NUMBER_OF_CHILDS_IN_ROW - 1;
        for (int j = NUMBER_OF_CHILDS_IN_ROW - 2; j >= 0; --j)
        {
            if (scoreTable[j][i] == scoreTable[j + 1][i])
            {
                scoreTable[m][i] = scoreTable[j][i] + scoreTable[j + 1][i];
                points += scoreTable[m][i];
                scoreTable[j][i] = 0;
                m = j;
                --j;
            }
            else --m;
        }
    }

    for (int i = 0; i < NUMBER_OF_CHILDS_IN_ROW; i++)
    {
        for (int j = NUMBER_OF_CHILDS_IN_ROW - 1; j > 0; --j)
        {
            int k = j;
            while (k >= 0 && scoreTable[j][i] == 0)
            {
                if (scoreTable[k][i] != 0)
                {
                    scoreTable[j][i] = scoreTable[k][i];
                    scoreTable[k][i] = 0;
                }
                --k;
            }
        }
    }

    if (isPlace())
    {
        int x = rand() % NUMBER_OF_CHILDS_IN_ROW;
        int y = rand() % NUMBER_OF_CHILDS_IN_ROW;
        while (scoreTable[x][y] != 0)
        {
            x = rand() % NUMBER_OF_CHILDS_IN_ROW;
            y = rand() % NUMBER_OF_CHILDS_IN_ROW;
        }
        scoreTable[x][y] = 2;
    }
    UpdateFields();
    bool win = IsItEnd();
    if (gameOver) End(win);
}

void GameUp()
{
    for (int i = 0; i < NUMBER_OF_CHILDS_IN_ROW; ++i)
    {
        for (int j = 0; j < NUMBER_OF_CHILDS_IN_ROW - 1; ++j)
        {
            int k = j;
            while (k <= NUMBER_OF_CHILDS_IN_ROW - 1 && scoreTable[i][j] == 0)
            {
                if (scoreTable[i][k] != 0)
                {
                    scoreTable[i][j] = scoreTable[i][k];
                    scoreTable[i][k] = 0;
                }
                ++k;
            }
        }
    }

    for (int i = 0; i < NUMBER_OF_CHILDS_IN_ROW; i++)
    {
        int m = 0;
        for (int j = 1; j <= NUMBER_OF_CHILDS_IN_ROW - 1; ++j)
        {
            if (scoreTable[i][j] == scoreTable[i][j - 1])
            {
                scoreTable[i][m] = scoreTable[i][j] + scoreTable[i][j - 1];
                points += scoreTable[i][m];;
                scoreTable[i][j] = 0;
                m = j;
                ++j;
            }
            else ++m;
        }
    }

    for (int i = 0; i < NUMBER_OF_CHILDS_IN_ROW; ++i)
    {
        for (int j = 0; j < NUMBER_OF_CHILDS_IN_ROW - 1; ++j)
        {
            int k = j;
            while (k <= NUMBER_OF_CHILDS_IN_ROW - 1 && scoreTable[i][j] == 0)
            {
                if (scoreTable[i][k] != 0)
                {
                    scoreTable[i][j] = scoreTable[i][k];
                    scoreTable[i][k] = 0;
                }
                ++k;
            }
        }
    }

    bool win = IsItEnd();
    if (gameOver) End(win);

    if (isPlace())
    {
        int x = rand() % NUMBER_OF_CHILDS_IN_ROW;
        int y = rand() % NUMBER_OF_CHILDS_IN_ROW;
        while (scoreTable[x][y] != 0)
        {
            x = rand() % NUMBER_OF_CHILDS_IN_ROW;
            y = rand() % NUMBER_OF_CHILDS_IN_ROW;
        }
        scoreTable[x][y] = 2;
    }

    UpdateFields();
    win = IsItEnd();
    if (gameOver) End(win);
        
}

void Draw(HWND hWnd, TCHAR s[])
{
    HDC hdc = GetDC(hWnd);
    SetTextColor(hdc, RGB(255, 255, 255));
    if (strcmp((const char*)s, "2") == 0)
    {
        Paint(hWnd, RGB(238, 228, 198)); 
        SetBkColor(hdc, RGB(238, 228, 198));
    }
    else if (strcmp((const char*)s, "4") == 0)
    {
        Paint(hWnd, RGB(239, 225, 218)); 
        SetBkColor(hdc, RGB(239, 225, 218));
    }
    else if (strcmp((const char*)s, "8") == 0)
    {
        Paint(hWnd, RGB(243, 179, 124));
        SetBkColor(hdc, RGB(243, 179, 124));
    }
    else if (strcmp((const char*)s, "16") == 0)
    {
        Paint(hWnd, RGB(246, 153, 100));
        SetBkColor(hdc, RGB(246, 153, 100));
    }
    else if (strcmp((const char*)s, "32") == 0)
    {
        Paint(hWnd, RGB(246, 125, 98));
        SetBkColor(hdc, RGB(246, 125, 98));
    }
    else if (strcmp((const char*)s, "64") == 0)
    {
        Paint(hWnd, RGB(247, 93, 60)); 
        SetBkColor(hdc, RGB(247, 93, 60));
    }
    else if (strcmp((const char*)s, "128") == 0)
    {
        Paint(hWnd, RGB(237, 206, 116)); 
        SetBkColor(hdc, RGB(237, 206, 116));
    }
    else if (strcmp((const char*)s, "256") == 0)
    {
        Paint(hWnd, RGB(239, 204, 98));
        SetBkColor(hdc, RGB(239, 204, 98));
    }
    else if (strcmp((const char*)s, "512") == 0)
    {
        Paint(hWnd, RGB(243, 201, 85));
        SetBkColor(hdc, RGB(243, 201, 85));
    }
    else if (strcmp((const char*)s, "1024") == 0)
    {
        Paint(hWnd, RGB(238, 200, 72)); 
        SetBkColor(hdc, RGB(238, 200, 72));
    }
    else if (strcmp((const char*)s, "2048") == 0)
    {
        Paint(hWnd, RGB(239, 192, 47));
        SetBkColor(hdc, RGB(239, 192, 47));
    }

    HFONT font = CreateFont(
        MulDiv(24, GetDeviceCaps(hdc, LOGPIXELSY), 72), // Height
        0, // Width
        0, // Escapement
        0, // Orientation
        FW_BOLD, // Weight
        false, // Italic
        FALSE, // Underline
        0, // StrikeOut
        EASTEUROPE_CHARSET, // CharSet
        OUT_DEFAULT_PRECIS, // OutPrecision
        CLIP_DEFAULT_PRECIS, // ClipPrecision
        DEFAULT_QUALITY, // Quality
        DEFAULT_PITCH | FF_SWISS, // PitchAndFamily
        _T(" Verdana ")); // Facename
    HFONT oldFont = (HFONT)SelectObject(hdc, font);
    RECT rc;
    GetClientRect(hWnd, &rc);
    if (strcmp((const char*)s, "16") == 0) DrawText(hdc, _T("16"), 2, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    else if (strcmp((const char*)s, "32") == 0) DrawText(hdc, _T("32"), 2, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    else if (strcmp((const char*)s, "64") == 0) DrawText(hdc, _T("64"), 2, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    else if (strcmp((const char*)s, "128") == 0) DrawText(hdc, _T("128"), 3, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    else if (strcmp((const char*)s, "256") == 0) DrawText(hdc, _T("256"), 3, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    else if (strcmp((const char*)s, "512") == 0) DrawText(hdc, _T("512"), 3, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    else if (strcmp((const char*)s, "1024") == 0) DrawText(hdc, _T("1024"), 4, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    else if (strcmp((const char*)s, "2048") == 0) DrawText(hdc, _T("2048"), 4, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    else DrawText(hdc, s, (int)_tcslen(s), &rc,  DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    SelectObject(hdc, oldFont);
    DeleteObject(font);
    ReleaseDC(hWnd, hdc);
}

void Paint(HWND hWnd, COLORREF color)
{
    HDC hdc = GetDC(hWnd);
    HBRUSH brush = CreateSolidBrush(color);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);
    HPEN pen = CreatePen(PS_SOLID, 1, color);
    HPEN oldPen = (HPEN)SelectObject(hdc, pen);
    RoundRect(hdc, 0, 0, SIZE_CHILD_WINDOW_X, SIZE_CHILD_WINDOW_Y, 20, 20);
    SelectObject(hdc, oldBrush);
    DeleteObject(brush);
    SelectObject(hdc, oldPen);
    DeleteObject(pen);
    ReleaseDC(hWnd, hdc);
}

void UpdateFields()
{
    HDC hdc = GetDC(hWndScore1);
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkColor(hdc, RGB(204, 192, 174));
    HFONT font = CreateFont( MulDiv(24, GetDeviceCaps(hdc, LOGPIXELSY), 72), 0, 0, 0, FW_BOLD, false, FALSE, 0, EASTEUROPE_CHARSET, 
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T(" Verdana "));
    HFONT oldFont = (HFONT)SelectObject(hdc, font);
    RECT rc;
    GetClientRect(hWndScore1, &rc);
    TCHAR buf[300];
    _stprintf_s(buf, TEXT("%d"), points);
    DrawText(hdc, buf, (int)_tcslen(buf), &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    SelectObject(hdc, oldFont);
    DeleteObject(font);
    ReleaseDC(hWndScore1, hdc);

    HDC hdc2 = GetDC(hWndScore2);
    SetTextColor(hdc2, RGB(255, 255, 255));
    SetBkColor(hdc2, RGB(204, 192, 174));
    HFONT font2 = CreateFont(MulDiv(24, GetDeviceCaps(hdc2, LOGPIXELSY), 72), 0, 0, 0, FW_BOLD, false, FALSE, 0, EASTEUROPE_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T(" Verdana "));
    HFONT oldFont2 = (HFONT)SelectObject(hdc2, font2);
    RECT rc2;
    GetClientRect(hWndScore2, &rc2);
    _stprintf_s(buf, TEXT("%d"), points);
    DrawText(hdc2, buf, (int)_tcslen(buf), &rc2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    SelectObject(hdc2, oldFont2);
    DeleteObject(font2);
    ReleaseDC(hWndScore2, hdc2);

    for (int i = 0; i < NUMBER_OF_CHILDS_IN_ROW; i++)
    {
        for (int j = 0; j < NUMBER_OF_CHILDS_IN_ROW; j++)
        {
            if (scoreTable[i][j] != 0)
            {
                std::string s = std::to_string(scoreTable[i][j]);
                const char* pchar = s.c_str();
                Draw(hWndChildMain[i][j], (TCHAR*)pchar);
                Draw(hWndChildSecond[i][j], (TCHAR*)pchar);
            }
            else
            {
                Paint(hWndChildMain[i][j], RGB(204, 192, 174));
                Paint(hWndChildSecond[i][j], RGB(204, 192, 174));
            }
        }
    }
}

bool IsItEnd()
{
    /// <summary>
    /// Function check if the game is over or the user won. Function modify the global variable gameOver (true - end of game, 
    /// false - game continues). Function returns true if user won, otherwise returns false.
    /// </summary>
    /// <returns></returns>
    bool isZero = false;
    for (int i = 0; i < NUMBER_OF_CHILDS_IN_ROW; i++)
    {
        for (int j = 0; j < NUMBER_OF_CHILDS_IN_ROW; j++)
        {
            if (scoreTable[i][j] == goal)
            {
                gameOver = true;
                return true;
            }
            if (scoreTable[i][j] == 0) isZero = true;
        }
    }
    
    if (isZero) return false;
    
    for (int i = 0; i < NUMBER_OF_CHILDS_IN_ROW; i++)
    {
        for (int j = 0; j < NUMBER_OF_CHILDS_IN_ROW; j++)
        {
            if (i == 0 && j == 0)
                if (scoreTable[i][j] == scoreTable[i + 1][j] || scoreTable[i][j] == scoreTable[i][j + 1]) return false;
                else continue;

            if (i == NUMBER_OF_CHILDS_IN_ROW - 1 && j == 0)
                if (scoreTable[i][j] == scoreTable[i - 1][j] || scoreTable[i][j] == scoreTable[i][j + 1]) return false;
                else continue;

            if (i == NUMBER_OF_CHILDS_IN_ROW - 1 && j == NUMBER_OF_CHILDS_IN_ROW - 1)
                if (scoreTable[i][j] == scoreTable[i - 1][j] || scoreTable[i][j] == scoreTable[i][j - 1]) return false;
                else continue;

            if (i == 0 && j == NUMBER_OF_CHILDS_IN_ROW - 1)
                if (scoreTable[i][j] == scoreTable[i][j - 1] || scoreTable[i][j] == scoreTable[i + 1][j]) return false;
                else continue;

            if (i == 0)
                if (scoreTable[i][j] == scoreTable[i][j - 1] || scoreTable[i][j] == scoreTable[i][j + 1] || scoreTable[i][j] == scoreTable[i + 1][j])
                    return false;
                else continue;

            if (j == 0)
                if (scoreTable[i][j] == scoreTable[i - 1][j] || scoreTable[i][j] == scoreTable[i + 1][j] || scoreTable[i][j] == scoreTable[i][j + 1])
                    return false;
                else continue;

            if (i == NUMBER_OF_CHILDS_IN_ROW - 1)
                if (scoreTable[i][j] == scoreTable[i][j - 1] || scoreTable[i][j] == scoreTable[i][j + 1] || scoreTable[i][j] == scoreTable[i - 1][j])
                    return false;
                else continue;

            if (j == NUMBER_OF_CHILDS_IN_ROW - 1)
                if (scoreTable[i][j] == scoreTable[i - 1][j] || scoreTable[i][j] == scoreTable[i + 1][j] || scoreTable[i][j] == scoreTable[i][j - 1])
                    return false;
                else continue;

            if (i > 0 && j > 0 && i < NUMBER_OF_CHILDS_IN_ROW - 1 && j < NUMBER_OF_CHILDS_IN_ROW - 1)
                if (scoreTable[i][j] == scoreTable[i + 1][j] || scoreTable[i][j] == scoreTable[i - 1][j] ||
                    scoreTable[i][j] == scoreTable[i][j + 1] || scoreTable[i][j] == scoreTable[i][j - 1])
                    return false;
                else continue;
        }
    }
    gameOver = true;
    return false;
}

void End(bool win)
{
    if (win)
    {
        TCHAR s[] = _T("WIN!");
        EndGameCommunicate(hWndParentWindow, LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1)), s, 4);
        EndGameCommunicate(hWndChildWindow, LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1)), s, 4);
    }
    else
    {
        TCHAR s[] = _T("GAME OVER");
        EndGameCommunicate(hWndParentWindow, LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2)), s, 9);
        EndGameCommunicate(hWndChildWindow, LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2)), s, 9);
        
    }
}

bool isPlace()
{
    for (int i = 0; i < NUMBER_OF_CHILDS_IN_ROW; i++)
    {
        for (int j = 0; j < NUMBER_OF_CHILDS_IN_ROW; j++)
        {
            if (scoreTable[i][j] == 0) return true;
        }
    }
    return false;
}

void Unchecked()
{
    CheckMenuItem(GetMenu(hWndParentWindow), ID_GOAL_8, MF_UNCHECKED);
    CheckMenuItem(GetMenu(hWndChildWindow), ID_GOAL_8, MF_UNCHECKED);
    CheckMenuItem(GetMenu(hWndParentWindow), ID_GOAL_16, MF_UNCHECKED);
    CheckMenuItem(GetMenu(hWndChildWindow), ID_GOAL_16, MF_UNCHECKED);
    CheckMenuItem(GetMenu(hWndParentWindow), ID_GOAL_64, MF_UNCHECKED);
    CheckMenuItem(GetMenu(hWndChildWindow), ID_GOAL_64, MF_UNCHECKED);
    CheckMenuItem(GetMenu(hWndParentWindow), ID_GOAL_2048, MF_UNCHECKED);
    CheckMenuItem(GetMenu(hWndChildWindow), ID_GOAL_2048, MF_UNCHECKED);
}

void EndGameCommunicate(HWND hWnd, HBITMAP bitmap, TCHAR s[], int sLength)
{
    HDC hdc = GetDC(hWnd);
    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, bitmap);
    BLENDFUNCTION bf;
    bf.BlendOp = AC_SRC_OVER;
    bf.BlendFlags = 0;
    bf.AlphaFormat = 0;
    bf.SourceConstantAlpha = 0xbf;
    AlphaBlend(hdc, 0, 0, xSizeMainWindow, ySizeMainWindow, memDC, 0, 0, 48, 48, bf);
    HFONT font = CreateFont( MulDiv(40 /*24*/, GetDeviceCaps(hdc, LOGPIXELSY), 72), 0, 0, 0, FW_BOLD, false, FALSE, 0, EASTEUROPE_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T(" Verdana "));
    HFONT oldFont = (HFONT)SelectObject(hdc, font);
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);
    RECT rc;
    GetClientRect(hWnd, &rc);
    DrawText(hdc, s, sLength, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    SelectObject(hdc, oldFont);
    DeleteObject(font);
    SelectObject(memDC, oldBitmap);
    DeleteObject(bitmap);
    DeleteDC(memDC);
    ReleaseDC(hWnd, hdc);
}

void SavingToFile()
{
    /// <summary>
    /// Function saves state of the game to 2048.ini file.
    /// </summary>
    TCHAR buf[100];
    _stprintf_s(buf, L"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\0",
        scoreTable[0][0], scoreTable[0][1], scoreTable[0][2], scoreTable[0][3], scoreTable[1][0], scoreTable[1][1], scoreTable[1][2], scoreTable[1][3],
        scoreTable[2][0], scoreTable[2][1], scoreTable[2][2], scoreTable[2][3], scoreTable[3][0], scoreTable[3][1], scoreTable[3][2], scoreTable[3][3]);
    WritePrivateProfileStringW(L"2048", L"ScoreTable", buf, L"2048.ini");
    // 0 - game is still pending
    // 1 - win
    // 2 - loss
    TCHAR WinOrLoss[] = L"0";
    if (gameOver == true)
    {
        bool win = IsItEnd();
        if (win) WinOrLoss[0] = L'1';
        else WinOrLoss[0] = L'2';
    }
    WritePrivateProfileStringW(L"2048", L"WinOrLoss", WinOrLoss, L"2048.ini");
    TCHAR buf1[10];
    _stprintf_s(buf1, TEXT("%d"), points);
    WritePrivateProfileStringW(L"2048", L"Score", buf1, L"2048.ini");
    TCHAR buf2[10];
    _stprintf_s(buf2, TEXT("%d"), goal);
    WritePrivateProfileStringW(L"2048", L"Goal",buf2, L"2048.ini");
}

void ReadingFromFile()
{
    /// <summary>
    /// Function reads state of the game from 2048.ini file and modifies windows;
    /// </summary>
    
    // Updating table scorePoints
    TCHAR buf[512];
    GetPrivateProfileStringW(L"2048", L"ScoreTable", NULL, buf, 512,
        L"2048.ini");

    int i, j, n;
    i = j = n = 0;
    while (i < NUMBER_OF_CHILDS_IN_ROW)
    {
        scoreTable[i][j] = _tstoi(&buf[n]);
        if (scoreTable[i][j] / 1000 > 1) n += 5;
        else if (scoreTable[i][j] / 100 > 1) n += 4;
        else if (scoreTable[i][j] / 10 > 1) n += 3;
        else n += 2;
        ++j;
        if (j >= NUMBER_OF_CHILDS_IN_ROW)
        {
            j = 0;
            ++i;
        }
    }

    // Updating points value
    TCHAR buf3[10];
    GetPrivateProfileStringW(L"2048", L"Score", NULL, buf3, 10,
        L"2048.ini");
    points = _tstoi(buf3);

    UpdateFields();

    // Updating gameOver value
    TCHAR buf2[10];
    GetPrivateProfileStringW(L"2048", L"WinOrLoss", NULL, buf2, 10,
        L"2048.ini");
    int temp = _tstoi(buf2);
    if (temp == 0)
    {
        gameOver = false;
    }
    else if(temp == 1) // win
    {
        gameOver = true;
        End(true);

    }
    else // loss
    {
        gameOver = true;
        End(false);
    }

    // Updating goal value
    TCHAR buf4[10];
    GetPrivateProfileStringW(L"2048", L"Goal", NULL, buf4, 10,
        L"2048.ini");
    goal = _tstoi(buf4);
    Unchecked();
    if (goal == 8)
    {
        CheckMenuItem(GetMenu(hWndParentWindow), ID_GOAL_8, MF_CHECKED);
        CheckMenuItem(GetMenu(hWndChildWindow), ID_GOAL_8, MF_CHECKED);
    }
    else if (goal == 16)
    {
        CheckMenuItem(GetMenu(hWndParentWindow), ID_GOAL_16, MF_CHECKED);
        CheckMenuItem(GetMenu(hWndChildWindow), ID_GOAL_16, MF_CHECKED);
    }
    else if (goal == 64)
    {
        CheckMenuItem(GetMenu(hWndParentWindow), ID_GOAL_64, MF_CHECKED);
        CheckMenuItem(GetMenu(hWndChildWindow), ID_GOAL_64, MF_CHECKED);
    }
    else if (goal == 2048)
    {
        CheckMenuItem(GetMenu(hWndParentWindow), ID_GOAL_2048, MF_CHECKED);
        CheckMenuItem(GetMenu(hWndChildWindow), ID_GOAL_2048, MF_CHECKED);
    }
}

