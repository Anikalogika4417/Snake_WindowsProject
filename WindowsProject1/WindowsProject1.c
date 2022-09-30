
//
#include "framework.h"
#include "WindowsProject1.h"
#include <Windows.h>
#include <stdio.h>
#include <wchar.h>
#include <math.h>

#define NUM    1000
#define TWOPI  (2 * 3.14159)
#define LENGHT_ARR  6


#define NUMLINES ((int) (sizeof devcaps / sizeof devcaps [0]))



LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void moveSnake(POINT[]);
void defineInitMass(POINT[]);
int direction = 2;
//#define xstartPontCord  250;
//#define ystartPontCord  100;
//Не понимаю, как использовать. Не хотят использоваться с делением

int step = 20; //надо понять, как убрать и заменить на STEP. 
int food = 0;//еда
int counter = 0;

POINT apt[LENGHT_ARR] = {0,0};


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR szCmdLine, int iCmdShow) {
    //wchar_t szBuffer[100];
    //wsprintfW(szBuffer, "The sum of %i and %i is %i", 5, 3, 5 + 3);
    ////puts(szBuffer);
    ///*PWSTR pString = L"Text русский";
    //int ILength = lstrlen(pString);*/
    //MessageBoxW(NULL, TEXT(szBuffer), L"Title", MB_OKCANCEL);

    static TCHAR szAppName[] = TEXT("HelloWin");
    HWND         hwnd;
    MSG          msg;
    WNDCLASS     wndclass;

    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;

    if (!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("This program requires Windows NT!"),
            szAppName, MB_ICONERROR);
        return 0;
    }
    hwnd = CreateWindow(szAppName,                  // window class name
        TEXT("The Snake Program"), // window caption
        WS_OVERLAPPEDWINDOW,        // window style
        CW_USEDEFAULT,              // initial x position
        CW_USEDEFAULT,              // initial y position
        CW_USEDEFAULT,              // initial x size
        CW_USEDEFAULT,              // initial y size
        NULL,                       // parent window handle
        NULL,                       // window menu handle
        hInstance,                  // program instance handle
        NULL);                     // creation parameters

    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;


    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int  cxChar, cxCaps, cyChar, cxClient, cyClient;
    TCHAR       szBuffer[10], sz1Buffer[10];
    HDC         hdc;
    int         i;
    PAINTSTRUCT ps;
    TEXTMETRIC  tm;
    int maxWinLen;
    HPEN hPen;
    static HPEN hPenRed;
    HBRUSH hBrushHead, hBrushBody;
    //int xstartPont = 250, ystartPont = 100;
    //int direction;
    POINT newCordinate;
    


    switch (message)
    {
    case WM_CREATE:
        hdc = GetDC(hwnd);

        GetTextMetrics(hdc, &tm);
        cxChar = tm.tmAveCharWidth;
        cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
        cyChar = tm.tmHeight + tm.tmExternalLeading;

        hPenRed = CreatePen(PS_SOLID, 1, RGB(255, 0, 0)); //red pen
        

        ReleaseDC(hwnd, hdc);
        return 0;

    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        return 0;

    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_RIGHT:
            direction = 1;
            SendMessage(hwnd, WM_PAINT, 0, 0);
            break;

        case VK_UP:
            direction = 2;
            SendMessage(hwnd, WM_PAINT, 0, 0);
            break;

        case VK_LEFT:
            direction = 3;
            SendMessage(hwnd, WM_PAINT, 0, 0);
            break;

        case VK_DOWN:
            direction = 4;
            SendMessage(hwnd, WM_PAINT, 0, 0);
            break;
        }
        return 0;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);

        hBrushHead = CreateHatchBrush(HS_DIAGCROSS, RGB(255, 0, 0)); //Фон для головы
        hBrushBody = CreateSolidBrush(RGB(0, 255, 0)); //Фон для тела
        
        //SelectObject(hdc, hPenRed);

        maxWinLen = cxClient;
        if (cxClient >= cyClient) {
            maxWinLen = cyClient;
        }
        maxWinLen = (maxWinLen / step)* step +1;

        for (int x = step; x < maxWinLen; x += step)
        {
            MoveToEx(hdc, x, step, NULL);
            LineTo(hdc, x, maxWinLen);
        }
        for (int y = step; y < maxWinLen; y += step)
        {
            MoveToEx(hdc, step, y, NULL);
            LineTo(hdc, maxWinLen, y);
        }

        // printing with rectangls
        /*for (int col = 0; col < maxWinLen; col += step) {
            for (int str = 0; str < maxWinLen; str += step) {
                Rectangle(hdc, str, col, str+step+1, col+step+1);
            }
        }*/

        if (counter == 0) {
            defineInitMass(apt);
        }
        counter++;

        switch (direction) {
        case 0:
            break;
        case 1: //right
            newCordinate.x = apt[0].x + step;
            newCordinate.y = apt[0].y;
            break;
        case 2: //up
            newCordinate.x = apt[0].x;
            newCordinate.y = apt[0].y - step;
            break;
        case 3: //left
            newCordinate.x = apt[0].x - step;
            newCordinate.y = apt[0].y;
            break;
        case 4: //down
            newCordinate.x = apt[0].x;
            newCordinate.y = apt[0].y + step;
            break;
        default:
            newCordinate.x = apt[0].x;
            newCordinate.y = apt[0].y - step;
        }

        moveSnake(apt, LENGHT_ARR, newCordinate, food);

        

        for (int i = 0; i < LENGHT_ARR; ++i) {
            if ((apt[i].x == 0) & (apt[i].y == 0)) {
                break;
            }
            switch (i) {
            case 0:
                SelectObject(hdc, hBrushHead);
                Rectangle(hdc, apt[i].x, apt[i].y, apt[i].x + step + 1, apt[i].y + step + 1);
                break;
            default:
                SelectObject(hdc, hBrushBody);
                Rectangle(hdc, apt[i].x, apt[i].y, apt[i].x + step + 1, apt[i].y + step + 1);
                break;
            }
            
        }

        // TODO Если массив кординат заполнен до конца - ура, победа! Конец игры
        // TODO Змея развернется в совю же сторону - проигрыш!
        
        maxWinLen = (maxWinLen / step) * step + 1;

        SelectObject(hdc, GetStockObject(BLACK_PEN));
        Rectangle(hdc, maxWinLen + 40, 5, maxWinLen + 120, 55);


        DeleteObject(SelectObject(hdc, hBrushBody));
        DeleteObject(SelectObject(hdc, hBrushHead));

        EndPaint(hwnd, &ps);
        return 0;

    case WM_DESTROY:
        DeleteObject(hPenRed);
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
};

void moveSnake (POINT* point, int iLenght, POINT iCordinates, int iFood) {
    POINT perem;
    int insLenght = iLenght - 2;
    int xLastElem = point[iLenght - 1].x;

    switch (iFood)
    {
    case 0:
        for (int i = insLenght; i >= 0; i--) {
            perem.x = point[i].x;
            perem.y = point[i].y;
            point[i + 1].x = perem.x;
            point[i + 1].y = perem.y;
        }
        point[0].x = iCordinates.x;
        point[0].y = iCordinates.y;

        if (xLastElem == 0) {
            point[iLenght - 1].x = 0;
            point[iLenght - 1].y = 0;
        }
        break;
    case 1:
        for (int i = insLenght; i >= 0; i--) {
            perem.x = point[i].x;
            perem.y = point[i].y;
            point[i + 1].x = perem.x;
            point[i + 1].y = perem.y;
        }
        point[0].x = iCordinates.x;
        point[0].y = iCordinates.y;
        break;
    default:
        break;
    }
}

void defineInitMass(POINT* point) {
    int xstartPont = (250 / step) * step;
    int ystartPont = (100 / step) * step;

    apt[0].x = xstartPont;
    apt[0].y = ystartPont;
    apt[1].x = xstartPont;
    apt[1].y = ystartPont + step;
    apt[2].x = xstartPont + step;
    apt[2].y = ystartPont + step;
    apt[3].x = xstartPont + step;
    apt[3].y = ystartPont + step * 2;
    apt[4].x = xstartPont + step;
    apt[4].y = ystartPont + step * 3;
}