// CoinCollector.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "CoinCollector.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Images
HBITMAP hBitmapBeach;
HBITMAP hBitmapOcean;
HBITMAP hBitmapSand;
HBITMAP hBitmapPersonLeft;
HBITMAP hBitmapPersonRight;
HBITMAP hBitmapPersonUp;
HBITMAP hBitmapPersonDown;

const int mapWidth = 800;
const int mapHeight = 800;


const int numRows = 16;
const int numCols = 16;


const int movementRows = 16;
const int movementCols = 16;

int characterRow = 1;
int characterColumn = 1;
BOOL isCharacterFacingRight = FALSE;
BOOL isCharacterFacingLeft = FALSE;
BOOL isCharacterFacingUp = FALSE;
BOOL isCharacterFacingDown = TRUE;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void PaintImage(HWND hwnd);
void UpdateCharacterPosition(int x, int y);
void UpdateCharacterApperance();

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
    LoadStringW(hInstance, IDC_COINCOLLECTOR, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_COINCOLLECTOR));

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

    return (int)msg.wParam;
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

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_COINCOLLECTOR));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_COINCOLLECTOR);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED,
        CW_USEDEFAULT, 0, 800, 800, nullptr, nullptr, hInstance, nullptr);

    hBitmapBeach = (HBITMAP)LoadImage(NULL, _T("beach.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBitmapOcean = (HBITMAP)LoadImage(NULL, _T("ocean.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBitmapSand = (HBITMAP)LoadImage(NULL, _T("sand.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBitmapPersonLeft = (HBITMAP)LoadImage(NULL, _T("PersonLeft.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBitmapPersonRight = (HBITMAP)LoadImage(NULL, _T("PersonRight.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBitmapPersonUp = (HBITMAP)LoadImage(NULL, _T("PersonUp.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBitmapPersonDown = (HBITMAP)LoadImage(NULL, _T("PersonDown.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

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
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
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
        PaintImage(hWnd);
    }
    break;
    case WM_DESTROY:
        DeleteObject(hBitmapSand);
        DeleteObject(hBitmapOcean);
        DeleteObject(hBitmapBeach);
        DeleteObject(hBitmapPersonDown);
        DeleteObject(hBitmapPersonUp);
        DeleteObject(hBitmapPersonRight);
        DeleteObject(hBitmapPersonLeft);

        PostQuitMessage(0);
        break;
    case WM_KEYDOWN:
    {
        int originalColumn = characterColumn;
        int originalRow = characterRow;

        switch (wParam)
        {
        case VK_LEFT:
            // Left key: Update character appearance to face left
            isCharacterFacingLeft = TRUE;
            isCharacterFacingRight = FALSE;
            isCharacterFacingUp = FALSE;
            isCharacterFacingDown = FALSE;

            characterColumn = max(0, characterColumn - 1);
            break;
        case VK_RIGHT:
            // Right key: Update character appearance to face right
            isCharacterFacingLeft = FALSE;
            isCharacterFacingRight = TRUE;
            isCharacterFacingUp = FALSE;
            isCharacterFacingDown = FALSE;

            characterColumn = min(mapWidth, characterColumn + 1);
            break;
        case VK_DOWN:
            // Down arrow: Update character appearance to face down
            isCharacterFacingLeft = FALSE;
            isCharacterFacingRight = FALSE;
            isCharacterFacingUp = FALSE;
            isCharacterFacingDown = TRUE;

            characterRow = min(mapHeight, characterRow + 1);
            break;
        case VK_UP:
            // Up arrow: Update character appearance to face up
            isCharacterFacingLeft = FALSE;
            isCharacterFacingRight = FALSE;
            isCharacterFacingUp = TRUE;
            isCharacterFacingDown = FALSE;

            characterRow = max(0, characterRow - 1);
            break;

            // Add more cases for other keys as needed
        }

        if (originalColumn != characterColumn || originalRow != characterRow)
        {
            UpdateCharacterPosition(characterColumn, characterRow);
            InvalidateRect(hWnd, NULL, TRUE);
        }
    }
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

// Function to paint the image on the window
void PaintImage(HWND hwnd) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    if (hdc) {
        // Calculate the width and height of each cell in the matrix
        int cellWidth = mapWidth / numCols;
        int cellHeight = mapHeight / numRows;

        // Loop through the matrix and paint each cell with a different bitmap
        for (int row = 0; row < numRows; ++row) {
            for (int col = 0; col < numCols; ++col) {
                HBITMAP hBitmap = hBitmapBeach;

                // Determine which bitmap to use based on the row and column
                if (row < 12) {
                    hBitmap = hBitmapSand;
                }
                else if (row == 12) {
                    hBitmap = hBitmapBeach;
                }
                else {
                    hBitmap = hBitmapOcean;
                }

                // Create a memory device context for the bitmap
                HDC hdcMem = CreateCompatibleDC(hdc);

                // Select the bitmap into the memory device context
                SelectObject(hdcMem, hBitmap);

                // Draw the bitmap onto the window at the current cell position
                BitBlt(hdc, col * cellWidth, row * cellHeight, cellWidth, cellHeight, hdcMem, 0, 0, SRCCOPY);

                // Clean up
                DeleteDC(hdcMem);
            }
        }

        // Draw the character at its current position
        // Calculate the width and height of each cell in the matrix
        int characterCellWidth = 800 / movementCols;
        int characterCellHeight = 600 / movementRows;

        int characterX = characterColumn * characterCellWidth;
        int characterY = characterRow * characterCellHeight;


        HBITMAP hCharacterBitmap = hBitmapPersonDown;

        if (isCharacterFacingDown)
            hCharacterBitmap = hBitmapPersonDown;
        else if (isCharacterFacingUp)
            hCharacterBitmap = hBitmapPersonUp;
        else if (isCharacterFacingLeft)
            hCharacterBitmap = hBitmapPersonLeft;
        else
            hCharacterBitmap = hBitmapPersonRight;

        HDC hdcMemSprite = CreateCompatibleDC(hdc);
        SelectObject(hdcMemSprite, hCharacterBitmap);
        BitBlt(hdc, characterX, characterY, 50, 50, hdcMemSprite, 0, 0, SRCCOPY);

        DeleteDC(hdcMemSprite);
    }

    EndPaint(hwnd, &ps);
}

void UpdateCharacterPosition(int x, int y) {
    //int cellWidth = 800 / numCols;
    //int cellHeight = 600 / numRows;

    //characterColumn = x * cellWidth;
    //characterRow = y * cellHeight;

    // Ensure within valid range
    characterColumn = max(0, min(characterColumn, movementCols - 1));
    characterRow = max(0, min(characterRow, movementRows - 1));
}

