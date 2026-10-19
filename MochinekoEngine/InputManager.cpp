#include "InputManager.h"
#include "Framework.h"
#include <array>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

namespace InputManager {
    inline LPDIRECTINPUT8 input_ = {};
    inline LPDIRECTINPUTDEVICE8 inputDevice_ = {};

    inline std::array<BYTE, 256> beforeKeys_ = std::array<BYTE, 256>();
    inline std::array<BYTE, 256> afterKeys_ = std::array<BYTE, 256>();

    inline LPDIRECTINPUTDEVICE8 mouseInputDevice_ = {};
    inline DIMOUSESTATE beforeMouseState_ = {};
    inline DIMOUSESTATE afterMouseState_ = {};
    inline POINT mousePostion_ = {};

    inline XINPUT_STATE controllerState_ = {};

    inline std::string keyBuffer_ = "";
}

int InputManager::Init(const HINSTANCE hInstance, const HWND hwnd) {
    DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&input_, nullptr);

    input_->CreateDevice(GUID_SysKeyboard, &inputDevice_, nullptr);
    inputDevice_->SetDataFormat(&c_dfDIKeyboard);
    inputDevice_->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

    input_->CreateDevice(GUID_SysMouse, &mouseInputDevice_, nullptr);
    mouseInputDevice_->SetDataFormat(&c_dfDIMouse); 
    mouseInputDevice_->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

    keyBuffer_ = "";

    return 0;
}

void InputManager::Update() {
    ///
    /// キーボードの状態を更新
    /// 
    beforeKeys_ = afterKeys_;

    inputDevice_->Acquire();
    inputDevice_->GetDeviceState(sizeof(afterKeys_), &afterKeys_);

    ///
    /// デフォルトのキーバッファを更新
    /// 
    for (int i = 0; i < 256; i++) {
        if (CheckDownKey(i)) {
            if (i == DIK_BACKSPACE) {
                if (keyBuffer_.size() > 0) continue;
                keyBuffer_.pop_back();
            }
            else {
                if (ConvertKeyName(i) == nullptr) continue;
                keyBuffer_ += ConvertKeyName(i);
            }
        }
    }

    ///
    /// マウスの状態を更新
    /// 
    beforeMouseState_ = afterMouseState_;

    mouseInputDevice_->Acquire();
    mouseInputDevice_->GetDeviceState(
        sizeof(afterMouseState_),
        &afterMouseState_
    );

    ///
    /// マウスの位置を更新する
    /// 
    GetCursorPos(&mousePostion_);
    ScreenToClient(MochinekoEngine::GetGameWindowHandle(), &mousePostion_);

    /// 
    /// コントローラの状態を更新
    /// 
    XInputGetState(0, &controllerState_);
}

bool InputManager::CheckPushKey(const int key) {
    return (afterKeys_[key] & 0x80);
}

bool InputManager::CheckUpKey(const int key) {
    return (beforeKeys_[key] & 0x80) && !(afterKeys_[key] & 0x80);
}

const char* InputManager::ConvertKeyName(const int key) {
    switch (key) {
    case DIK_A: return "A";
    case DIK_B: return "B";
    case DIK_C: return "C";
    case DIK_D: return "D";
    case DIK_E: return "E";
    case DIK_F: return "F";
    case DIK_G: return "G";
    case DIK_H: return "H";
    case DIK_I: return "I";
    case DIK_J: return "J";
    case DIK_K: return "K";
    case DIK_L: return "L";
    case DIK_N: return "N";
    case DIK_M: return "M";
    case DIK_O: return "O";
    case DIK_P: return "P";
    case DIK_Q: return "Q";
    case DIK_R: return "R";
    case DIK_S: return "S";
    case DIK_T: return "T";
    case DIK_U: return "U";
    case DIK_V: return "V";
    case DIK_W: return "W";
    case DIK_X: return "X";
    case DIK_Y: return "Y";
    case DIK_Z: return "Z";
    default: return nullptr;
    }
}

std::string InputManager::GetUpKeyBuffer() {
    return keyBuffer_;
}

std::string InputManager::GetLowKeyBuffer() {
    std::string keyBuffer = GetUpKeyBuffer();
    std::string lowKeyBuffer = "";
    for (int i = 0; i < keyBuffer.size(); i++) {
        char key = keyBuffer[i];
        lowKeyBuffer += std::tolower(key);
    }

    return lowKeyBuffer;
}

void InputManager::DestoryKeyBuffer() {
    keyBuffer_ = "";
}

bool InputManager::CheckDownKey(const int key) {
    return !(beforeKeys_[key] & 0x80) && (afterKeys_[key] & 0x80);
}

bool InputManager::CheckPushMouse(const int button) {
    if (afterMouseState_.rgbButtons[button] & 0x80) {
        return true;
    }
    return false;
}

bool InputManager::CheckUpMouse(const int button) {
    return (beforeMouseState_.rgbButtons[button] & 0x80) && !(afterMouseState_.rgbButtons[button] & 0x80);
}

bool InputManager::CheckDownMouse(const int button) {
    return !(beforeMouseState_.rgbButtons[button] & 0x80) && (afterMouseState_.rgbButtons[button] & 0x80);
}

POINT InputManager::GetMousePostion() {
    return mousePostion_;
}

bool InputManager::IsControllerButtonPush(const int button) {
    return (controllerState_.Gamepad.wButtons & button);
}

DirectX::XMFLOAT2 InputManager::GetControllerTiltLeft() {
    return DirectX::XMFLOAT2(controllerState_.Gamepad.sThumbLX, controllerState_.Gamepad.sThumbLY);
}

DirectX::XMFLOAT2 InputManager::GetControllerTiltRight() {
    return DirectX::XMFLOAT2(controllerState_.Gamepad.sThumbRX, controllerState_.Gamepad.sThumbRY);
}

float InputManager::GetControllerLeftTrigger() {
    return controllerState_.Gamepad.bLeftTrigger;
}

float InputManager::GetControllerRightTrigger() {
    return controllerState_.Gamepad.bRightTrigger;
}

void InputManager::StartControllerVibration(const int leftSpeed, const int rightSpeed) {
    XINPUT_VIBRATION vibration = {};
    vibration.wLeftMotorSpeed = leftSpeed;
    vibration.wRightMotorSpeed = rightSpeed;
    DWORD result = XInputSetState(0, &vibration);
}

void InputManager::StopControllerVibration() {
    XINPUT_VIBRATION vibration = {};
    vibration.wLeftMotorSpeed = 0;
    vibration.wRightMotorSpeed = 0;
    XInputSetState(0, &vibration);
}
