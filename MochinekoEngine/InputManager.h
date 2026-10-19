#pragma once

#include <Windows.h>
#include <dinput.h>
#include <Xinput.h>
#include <DirectXMath.h>
#include <string>

#pragma comment(lib, "XInput.lib")

namespace InputManager {

	/// <summary>
	/// 入出力マネージャーを初期化する関数（※自動なため、手動で実行する必要はありません）
	/// </summary>
	/// <param name="hInstance"></param>
	/// <param name="hwnd"></param>
	/// <returns></returns>
	int Init(const HINSTANCE hInstance, const HWND hwnd);

	/// <summary>
	/// 入出力マネージャーを更新する関数（※自動なため、手動で実行する必要はありません）
	/// </summary>
	void Update();


	///
	/// キーボードの入力
	/// 

	/// <summary>
	/// キーを押しているかをを返す関数（※押しっぱなしは関係ない）
	/// </summary>
	/// <param name="key">DirectInputのキー番号</param>
	/// <returns>引数のキーが押されている場合はtrueを返す。</returns>
	bool CheckPushKey(const int key);

	/// <summary>
	/// キーが押した瞬間かを返す関数（※押しっぱなしは無効）
	/// </summary>
	/// <param name="key">>DirectInputのキー番号</param>
	/// <returns>引数のキーが押されている場合はtrueを返す。</returns>
	bool CheckDownKey(const int key);

	/// <summary>
	/// キーが上げた瞬間かを返す関数（※押しっぱなしは無効）
	/// </summary>
	/// <param name="key"></param>
	/// <returns></returns>
	bool CheckUpKey(const int key);

	/// <summary>
	/// DirectInputのキー番号から、キー名に変換する関数
	/// </summary>
	/// <param name="key"></param>
	/// <returns></returns>
	const char* ConvertKeyName(const int key);

	/// <summary>
	/// デフォルトのキーバッファの、大文字版
	/// </summary>
	/// <returns></returns>
	std::string GetUpKeyBuffer();

	/// <summary>
	/// デフォルトのキーバッファの、小文字版
	/// </summary>
	/// <returns></returns>
	std::string GetLowKeyBuffer();

	/// <summary>
	/// デフォルトのキーバッファを破棄する関数
	/// </summary>
	void DestoryKeyBuffer();


	///
	/// マウスの入力
	/// 


	/// <summary>
	/// マウスが押されているかを返す関数（※押しっぱなしは関係ない）
	/// </summary>
	/// <param name="button"></param>
	/// <returns></returns>
	bool CheckPushMouse(const int button);

	/// <summary>
	/// マウスが押した瞬間かを返す関数（※押しっぱなしは無効）
	/// </summary>
	/// <param name="button"></param>
	/// <returns></returns>
	bool CheckDownMouse(const int button);

	/// <summary>
	/// マウスが上げた瞬間かを返す関数（※押しっぱなしは無効）
	/// </summary>
	/// <param name="button"></param>
	/// <returns></returns>
	bool CheckUpMouse(const int button);

	/// <summary>
	/// マウスの位置を返す関数
	/// </summary>
	/// <returns></returns>
	POINT GetMousePostion();



	///
	/// コントローラーの入力
	/// 


	/// <summary>
	/// 
	/// </summary>
	/// <param name="button"></param>
	/// <returns></returns>
	bool IsControllerButtonPush(const int button);

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	DirectX::XMFLOAT2 GetControllerTiltLeft();

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	DirectX::XMFLOAT2 GetControllerTiltRight();

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	float GetControllerLeftTrigger();

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	float GetControllerRightTrigger();

	/// <summary>
	/// コントローラーの振動させる関数
	/// </summary>
	/// <param name="leftSpeed">左側の振動の強さ</param>
	/// <param name="rightSpeed">右側の振動の強さ</param>
	void StartControllerVibration(const int leftSpeed, const int rightSpeed);

	/// <summary>
	/// コントローラーの振動を止める関数
	/// </summary>
	void StopControllerVibration();
}