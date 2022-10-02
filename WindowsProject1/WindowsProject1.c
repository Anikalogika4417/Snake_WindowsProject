#include "framework.h"
#include "WindowsProject1.h"
#include <Windows.h>
#include <stdio.h>
#include <wchar.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#define NUM    1000
#define TWOPI  (2 * 3.14159)
#define LENGHT_ARR  100
#define ID_TIMER 1
#define W_LENGHT 926
#define H_LENGHT 520


#define NUMLINES ((int) (sizeof devcaps / sizeof devcaps [0]))



LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void createFood();
void moveSnake(POINT[]);
void defineInitMass(POINT[]);
int cheekMeetingWithCordinate();
int randomValue(int Max);
int normalise(int a);
POINT selectDirection(int direction);


int direction = 2;
int step = 4;
int food = 0;//еда
int counter = 0;//счетчик цикла
int snakeLenght = 0;//счетчик длины змеи
int startSpeed = 500; //стартовое время обновления счетчика
int startPontX = 250;
int startPontY = 100;

POINT apt[LENGHT_ARR] = {0,0};
POINT masForDel[LENGHT_ARR] = { 0,0 };
POINT foodCord = {0,0};
POINT lastCordinate = {0,0};


// TODO Надо задать минимальные размеры окна
// TODO Ускорение таймера при достижении длинны

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
    wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
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
        (WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME) | WS_VISIBLE,        // window style
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
    TCHAR       szBuffer[40], sz1Buffer[40];
    HDC         hdc;
    int         i, ifMeet, iStartSpeed, iLengthSnake, iSpeed;
    PAINTSTRUCT ps;
    TEXTMETRIC  tm;
    static HPEN hPenBlack, hPenFrame, hPenWhite;
    static HBRUSH hBrushHead, hBrushBody, hBrushEnd, hBrushFood;
    POINT newCordinate;
    


    switch (message)
    {
    case WM_CREATE:
        hdc = GetDC(hwnd);

        SetTimer(hwnd, ID_TIMER, startSpeed, NULL);

        hPenBlack = CreatePen(PS_SOLID, 1, RGB(0, 0, 0)); //black pen
        hPenWhite = CreatePen(PS_SOLID, 1, RGB(255, 255, 255)); //white pen
        hPenFrame = CreatePen(PS_DASHDOT, step, RGB(255, 0, 0)); //frame pen

        ReleaseDC(hwnd, hdc);
        return 0;

    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        return 0;

    case WM_TIMER:
        switch (wParam){
        case -1:
            KillTimer(hwnd, ID_TIMER);
        default:
            InvalidateRect(hwnd, NULL, FALSE);
        }
        return 0;

    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_RIGHT:
            direction = 1;
            InvalidateRect(hwnd, NULL, FALSE);
            break;

        case VK_UP:
            direction = 2;
            InvalidateRect(hwnd, NULL, FALSE);
            break;

        case VK_LEFT:
            direction = 3;
            InvalidateRect(hwnd, NULL, FALSE);
            break;

        case VK_DOWN:
            direction = 4;
            InvalidateRect(hwnd, NULL, FALSE);
            break;
        }
        return 0;

    case WM_PAINT:        
        hdc = BeginPaint(hwnd, &ps);
        


        //hBrushHead = CreateHatchBrush(HS_DIAGCROSS, RGB(255, 0, 0)); //Фон для головы (сеточка)
        hBrushHead = CreateSolidBrush(RGB(255, 255, 0)); //Фон для головы (желтый)
        hBrushBody = CreateSolidBrush(RGB(0, 255, 0)); //Фон для тела (зеленый)
        hBrushEnd = CreateSolidBrush(RGB(255, 255, 255)); //Фон для удаления
        hBrushFood = CreateSolidBrush(RGB(255, 0, 0)); //Фон для еды (красный)        

        
        if (counter == 0) {
            defineInitMass(apt);
        }
       iStartSpeed = (int)(startSpeed * (1.0 + (snakeLenght / 10.0))); //TODO Подумать, как ускорять таймер. Пока что-то считает, но не передает в таймер
        
        

        SelectObject(hdc, hPenFrame);
        Rectangle(hdc, step + step / 2, step+ step / 2, W_LENGHT - (step/2), H_LENGHT - (step / 2)); // Рисуем область, в которой будет ползать змея

        SelectObject(hdc, hPenBlack);
        createFood();

        newCordinate = selectDirection(direction);
        
        moveSnake(apt, LENGHT_ARR, newCordinate, food);
        if (snakeLenght == LENGHT_ARR - 1) {
            SendMessage(hwnd, WM_TIMER, -1, 0);
        }

        ifMeet = cheekMeetingWithCordinate();
        if (ifMeet == 1) {
            SendMessage(hwnd, WM_DESTROY, 0, 0);
        }
        
        SelectObject(hdc, hPenWhite);
        if (counter != 0) {
            if ((lastCordinate.x != 0) && ((lastCordinate.x != apt[0].x) || (lastCordinate.y != apt[0].y))) {
                SelectObject(hdc, hBrushEnd);
                Rectangle(hdc, lastCordinate.x, lastCordinate.y, lastCordinate.x + step + 1, lastCordinate.y + step + 1);
            }
        }

        SelectObject(hdc, hPenBlack);
        for (int i = 0; i < LENGHT_ARR; ++i) {
            if ((apt[i].x == 0) || (apt[i].y == 0)) {
                break;
            }
            switch (i) {
            case 0:
                SelectObject(hdc, hBrushHead);
                Ellipse (hdc, apt[i].x, apt[i].y, apt[i].x + step + 1, apt[i].y + step + 1);
                break;
            default:
                SelectObject(hdc, hBrushBody);
                Ellipse (hdc, apt[i].x, apt[i].y, apt[i].x + step + 1, apt[i].y + step + 1);
                break;
            }
        }
        
        if ((foodCord.x == 0) || (foodCord.y == 0)) {
            break;
        }
        else {
            SelectObject(hdc, hBrushFood);
            Ellipse(hdc, foodCord.x, foodCord.y, foodCord.x + step + 1, foodCord.y + step + 1);
        }

        iLengthSnake = wsprintf(szBuffer, TEXT("The snake leght is %i"),
            snakeLenght);
        TextOut(hdc, W_LENGHT - 200, 20, szBuffer, iLengthSnake);
        iSpeed = wsprintf(sz1Buffer, TEXT("The speed is %i"),
            startSpeed);
        TextOut(hdc, W_LENGHT - 200, 40, sz1Buffer, iSpeed);



        // TODO Окно победы!
        // TODO Окно проигрыша!
        // TODO Окно вопроса: продолжаем или конец игры?
        // TODO Задать счетчик на обновление в периоде таймера

        counter++;


        DeleteObject(SelectObject(hdc, hBrushBody));
        DeleteObject(SelectObject(hdc, hBrushHead));
        DeleteObject(SelectObject(hdc, hBrushEnd));
        DeleteObject(SelectObject(hdc, hBrushFood));
        

        EndPaint(hwnd, &ps);
        return 0;

    case WM_DESTROY:
        DeleteObject(hPenBlack);
        DeleteObject(hPenFrame);
        DeleteObject(hPenWhite);


        KillTimer(hwnd, ID_TIMER);
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
};

void moveSnake (POINT* point, int iLenght, POINT iCordinates, int iFood) {
    POINT perem;
    int countLenght;
    int insLenght = iLenght - 2;
    int lastI = 0;

    lastCordinate.x = point[iLenght - 1].x;
    lastCordinate.y = point[iLenght - 1].y;

    switch (iFood)
    {
    case 0:

        for (int i = insLenght; i >= 0; i--) {
            if((point[i].x == 0) && (point[i-1].x != 0)) {
                lastCordinate.x = point[i - 1].x;
                lastCordinate.y = point[i - 1].y;
                lastI = i;
            }
            perem.x = point[i].x;
            perem.y = point[i].y;
            point[i + 1].x = perem.x;
            point[i + 1].y = perem.y;
        }


        point[0].x = iCordinates.x;
        point[0].y = iCordinates.y;
        
        point[lastI].x = 0;
        point[lastI].y = 0;
        snakeLenght = lastI;

        break;
    case 1:
        lastCordinate.x = point[iLenght-1].x;
        lastCordinate.y = point[iLenght - 1].y;

        for (int i = insLenght; i >= 0; i--) {
            if ((point[i].x == 0) && (point[i - 1].x != 0)) {
                lastI = i;
            }
            perem.x = point[i].x;
            perem.y = point[i].y;
            point[i + 1].x = perem.x;
            point[i + 1].y = perem.y;
        }
        point[0].x = iCordinates.x;
        point[0].y = iCordinates.y;
       
        snakeLenght = lastI+1;
        food = 0;

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
    snakeLenght = 5;
}

void createFood() {
    if (foodCord.x == 0) {
        foodCord.x = (randomValue(W_LENGHT) / step) * step;
        foodCord.y = (randomValue(H_LENGHT) / step) * step;
    }
}

// TODO Надо как-то попраавить на милисекунды
int randomValue(int Max) { 
    srand(time(NULL));   // Initialization, should only be called once.
    int r = rand() % (Max - ((step + (step / 2))*2)) + step + (step / 2);
    return r;
}

POINT selectDirection(int direction) {
    POINT iNewCordinate = {0,0};

    switch (direction) {
    case 0:
        break;
    case 1: //right
        iNewCordinate.x = apt[0].x + step;
        iNewCordinate.y = apt[0].y;
        break;
    case 2: //up
        iNewCordinate.x = apt[0].x;
        iNewCordinate.y = apt[0].y - step;
        break;
    case 3: //left
        iNewCordinate.x = apt[0].x - step;
        iNewCordinate.y = apt[0].y;
        break;
    case 4: //down
        iNewCordinate.x = apt[0].x;
        iNewCordinate.y = apt[0].y + step;
        break;
    default:
        iNewCordinate.x = apt[0].x;
        iNewCordinate.y = apt[0].y - step;
    }
    return(iNewCordinate);
}

int cheekMeetingWithCordinate() {
    int insLenghtArr = LENGHT_ARR-2;
    if ((apt[0].x == foodCord.x) && (apt[0].y == foodCord.y)) {
        foodCord.x = 0;
        foodCord.y = 0;
        food = 1;
        return 0;
    }   

    for (int i = 1; i < insLenghtArr; i++) {
        if ((apt[0].x == apt[i].x) && (apt[0].y == apt[i].y)) {
            return 1;
        }
    }

    if ((apt[0].x == step) || (apt[0].y == step) ||(apt[0].y == normalise(W_LENGHT) - step) || (apt[0].y == normalise(H_LENGHT) - step)) {
        return 1;
    }
}

int normalise(int a) {
    a = (a / step) * step;
    return a;
}