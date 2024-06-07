#include <windows.h>
#include "resource.h"
#include "Shape.h"

#define N 101

Shape* pcshape[N] = { nullptr };
int shapeCount = 0;
POINT startPoint, endPoint;
bool isDrawing = false;
Shape* currentShape = nullptr;
int currentObject = 0;

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
    HWND hwnd;
    MSG messages;
    WNDCLASSEX wincl;

    wincl.hInstance = hInst;
    wincl.lpszClassName = TEXT("Lab5Class");
    wincl.lpfnWndProc = WindowProcedure;
    wincl.style = CS_DBLCLKS;
    wincl.cbSize = sizeof(WNDCLASSEX);

    wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
    wincl.lpszMenuName = MAKEINTRESOURCE(IDR_MAINMENU);
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = (HBRUSH)COLOR_BACKGROUND;

    if (!RegisterClassEx(&wincl))
        return 0;

    hwnd = CreateWindowEx(
        0,
        TEXT("Lab5Class"),
        TEXT("Lab5"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        544,
        375,
        HWND_DESKTOP,
        NULL,
        hInst,
        NULL
    );

    ShowWindow(hwnd, ncmdshow);

    while (GetMessage(&messages, NULL, 0, 0))
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }

    return messages.wParam;
}

void StartDrawing(HWND hwnd, POINT point) {
    isDrawing = true;
    startPoint = point;

    switch (currentObject) {
    case IDM_POINT:
        currentShape = new PointShape();
        break;
    case IDM_LINE:
        currentShape = new LineShape();
        break;
    case IDM_RECTANGLE:
        currentShape = new RectangleShape();
        break;
    case IDM_ELLIPSE:
        currentShape = new EllipseShape();
        break;
    }

    currentShape->SetData(startPoint, startPoint);
}

void UpdateDrawing(HWND hwnd, POINT point) {
    endPoint = point;
    currentShape->SetData(startPoint, endPoint);
    InvalidateRect(hwnd, NULL, TRUE);
}

void EndDrawing(HWND hwnd, POINT point) {
    isDrawing = false;
    endPoint = point;
    currentShape->SetData(startPoint, endPoint);

    if (shapeCount < N) {
        pcshape[shapeCount++] = currentShape;
    }
    else {
        delete currentShape;
    }

    InvalidateRect(hwnd, NULL, TRUE);
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
    case WM_CREATE:
    {
        HMENU hMenu = CreateMenu();
        HMENU hFileMenu = CreateMenu();
        HMENU hObjectMenu = CreateMenu();
        HMENU hHelpMenu = CreateMenu();

        AppendMenu(hFileMenu, MF_STRING, IDM_EXIT, TEXT("Вихід"));
        AppendMenu(hObjectMenu, MF_STRING, IDM_POINT, TEXT("Крапка"));
        AppendMenu(hObjectMenu, MF_STRING, IDM_LINE, TEXT("Лінія"));
        AppendMenu(hObjectMenu, MF_STRING, IDM_RECTANGLE, TEXT("Прямокутник"));
        AppendMenu(hObjectMenu, MF_STRING, IDM_ELLIPSE, TEXT("Еліпс"));
        AppendMenu(hHelpMenu, MF_STRING, IDM_ABOUT, TEXT("Про програму"));

        AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, TEXT("Файл"));
        AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hObjectMenu, TEXT("Об’єкти"));
        AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hHelpMenu, TEXT("Довідка"));

        SetMenu(hwnd, hMenu);
    }
    break;

    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case IDM_POINT:
            currentObject = IDM_POINT;
            SetWindowText(hwnd, TEXT("Lab5 - Крапка"));
            break;

        case IDM_LINE:
            currentObject = IDM_LINE;
            SetWindowText(hwnd, TEXT("Lab5 - Лінія"));
            break;

        case IDM_RECTANGLE:
            currentObject = IDM_RECTANGLE;
            SetWindowText(hwnd, TEXT("Lab5 - Прямокутник"));
            break;

        case IDM_ELLIPSE:
            currentObject = IDM_ELLIPSE;
            SetWindowText(hwnd, TEXT("Lab5 - Еліпс"));
            break;

        case IDM_EXIT:
            PostQuitMessage(0);
            break;

        case IDM_ABOUT:
            DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, [](HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) -> INT_PTR {
                if (message == WM_COMMAND)
                {
                    if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
                    {
                        EndDialog(hDlg, LOWORD(wParam));
                        return (INT_PTR)TRUE;
                    }
                }
                return (INT_PTR)FALSE;
                });
            break;
        }
    }
    break;

    case WM_LBUTTONDOWN:
        StartDrawing(hwnd, { LOWORD(lParam), HIWORD(lParam) });
        break;

    case WM_MOUSEMOVE:
        if (isDrawing) {
            UpdateDrawing(hwnd, { LOWORD(lParam), HIWORD(lParam) });
        }
        break;

    case WM_LBUTTONUP:
        EndDrawing(hwnd, { LOWORD(lParam), HIWORD(lParam) });
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        for (int i = 0; i < shapeCount; ++i) {
            pcshape[i]->Draw(hdc);
        }

        if (isDrawing && currentShape) {
            currentShape->Draw(hdc);
        }

        EndPaint(hwnd, &ps);
    }
    break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}
