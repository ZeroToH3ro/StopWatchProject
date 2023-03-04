#include <windows.h>
#include <iostream>

/*
    Trịnh Ngọc Minh
    Phạm Thị Ly
    Hồ Thị Kiều Linh
    Nguyễn Thị Nhật Phương
    Phạm Cao Đại Ân
    Trần Thanh Dương
    Nguyễn Quốc Bảo
    Trần Trọng Nhân
    Lê Vũ Phước
    Điệp Hữu Nghĩa
*/
using namespace std;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Register the window class.
    const char CLASS_NAME[] = "StopWatchClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.
    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        "StopWatch",                    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,                           // Parent window    
        NULL,                           // Menu
        hInstance,                      // Instance handle
        NULL                            // Additional application data
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    // Show the window
    ShowWindow(hwnd, nCmdShow);

    // Run the message loop.
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

// Message handler for the window.
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static LARGE_INTEGER frequency;        // ticks per second
    static LARGE_INTEGER t1, t2;           // ticks
    static double elapsedTime;
    static bool isRunning = false;
    static HWND hwndLabel;
    static char szElapsedTime[128];

    switch (msg)
    {
    case WM_CREATE:
        // Create a label to display the elapsed time.
        hwndLabel = CreateWindow("STATIC", "Elapsed time: 0.0 sec", WS_CHILD | WS_VISIBLE, 10, 10, 200, 20, hwnd, NULL, NULL, NULL);
        QueryPerformanceFrequency(&frequency);
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case 1: // Start button
            if (!isRunning)
            {
                QueryPerformanceCounter(&t1);
                isRunning = true;
            }
            break;

        case 2: // Stop button
            if (isRunning)
            {
                QueryPerformanceCounter(&t2);
                elapsedTime = (t2.QuadPart - t1.QuadPart) * 1.0 / frequency.QuadPart;
                sprintf_s(szElapsedTime, "Elapsed time: %.2f sec", elapsedTime);
                SetWindowText(hwndLabel, szElapsedTime);
                isRunning = false;
            }
            break;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_PAINT:
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        EndPaint(hwnd, &ps);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}
