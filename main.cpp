#include <Windows.h>
#include "framework.h"
#include "ShaderManager.h"
#include "DX3D.h"
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_win32.h"
#include "ImGUI/imgui_impl_dx11.h"
#include "Box.h"
#include "CameraManager.h"
#include <DirectXMath.h>
#include "FBXModel.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
int initializeWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);
int initializeImGUI();

using namespace DirectX;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    //初期化処理
    initializeWindow(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
    CoInitializeEx(NULL, COINIT_MULTITHREADED);
    DX3D::initializeDX3D();
    ShaderManager::initialize();
    initializeImGUI();
	Box* box = new Box({ 100.0f, 100.0f, 0.0f });
    box->Initialize();
    FBXModel* fbx = new FBXModel("Assets/TestModel.fbx", {100.0f, 100.0f, 0.0f});
    fbx->Initialize();

	Camera* camera = CameraManager::AddCamera("RootCamera");
	camera->name_ = "RootCamera";
	camera->location_ = { 0.0f, 0.0f, -5.0f };
	camera->target_ = { 0.0f, 0.0f, 0.0f };
	camera->upDirection_ = { 0.0f, 1.0f, 0.0f };
    camera->projection_ = XMMatrixOrthographicLH(1280, 720, 0, 100);
	CameraManager::SetCurrentCamera("RootCamera");

    MSG msg = {};
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            auto deviceContext = DX3D::GetDeviceContext();
            auto renderTargetView = DX3D::GetRenderTargetView();        

            deviceContext->OMSetRenderTargets(1, &renderTargetView, nullptr);
            deviceContext->ClearRenderTargetView(renderTargetView, GameWindow::BACKGROUND_COLOR);

            auto& imguiIO = ImGui::GetIO();
            ImGui_ImplDX11_NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();

            box->Update();
            box->Draw();
            fbx->Update();
            fbx->Draw();

            ImGui::Begin("Game");
            ImGui::Text("Camera: %s", camera == nullptr ? "(null)" : camera->name_.c_str());
            if (camera != nullptr) {
                ImGui::SliderFloat("CameraLocX: ", &camera->location_.x, -1280, 1280);
                ImGui::SliderFloat("CameraLocY: ", &camera->location_.y, -1280, 1280);
                ImGui::SliderFloat("CameraLocZ: ", &camera->location_.z, -1280, 1280);
                ImGui::SliderFloat("CameraTargetX: ", &camera->target_.x, -1280, 1280);
                ImGui::SliderFloat("CameraTargetY: ", &camera->target_.y, -1280, 1280);
                ImGui::SliderFloat("CameraTatgetZ: ", &camera->target_.z, -1280, 1280);
            }
            ImGui::End();


            ImGui::EndFrame();
            ImGui::Render();
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
            if (imguiIO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
            }

            

            DX3D::GetSwapChain()->Present(1, 0);
        }
    }

    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
        return true;

    switch (msg) {
    case WM_DESTROY: {
        PostQuitMessage(0);
        break;
    }
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
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
    return 0;
}

int initializeImGUI() {
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsLight();
    ImGui_ImplWin32_Init(GameWindow::mainHWND);
    ID3D11Device* device = (ID3D11Device*)DX3D::GetDevice();
    ID3D11DeviceContext* deviceContext = (ID3D11DeviceContext*)DX3D::GetDeviceContext();
    ImGui_ImplDX11_Init(device, deviceContext);

    return 0;
}
