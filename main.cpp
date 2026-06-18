#include <Windows.h>
#include "framework.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
int initializeWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    //初期化処理
    initializeWindow(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
    CoInitializeEx(NULL, COINIT_MULTITHREADED);

    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_DESTROY: {
        PostQuitMessage(0);
        break;
    }
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

int initializeWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    WNDCLASSEX wndClass = {};
    wndClass.cbSize = sizeof(WNDCLASSEX);
    wndClass.lpszClassName = GameWindow::WINDOW_CLASS;
    wndClass.lpfnWndProc = WndProc;
    wndClass.style = CS_VREDRAW | CS_HREDRAW;
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    RegisterClassEx(&wndClass);

    //ウインドウを作成する
    GameWindow::mainHWND = CreateWindow(
        GameWindow::WINDOW_CLASS, //クラス名
        GameWindow::WINDOW_TITLE, //ウインドウタイトル
        WS_BORDER | WS_OVERLAPPEDWINDOW, //ウインドウスタイル
        CW_USEDEFAULT, //表示位置X（デフォルト）
        CW_USEDEFAULT, //表示位置Y（デフォルト）
        GameWindow::WINDOW_WIDTH, //ウインドウ幅
        GameWindow::WINDOW_HEIGHT, //ウインドウ高さ
        NULL, //親ウインドウ
        NULL, //メニュー
        hInstance, //インスタンス
        NULL //パラメータ
    );

    ShowWindow(GameWindow::mainHWND, nShowCmd); //ウインドウを表示
    UpdateWindow(GameWindow::mainHWND);

    MSG msg = {};
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {

        }
    }

    return 0;
}