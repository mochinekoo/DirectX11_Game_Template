#pragma once
#include <string>
#include <DirectXMath.h>

/// <summary>
/// オブジェクトの基底クラス
/// </summary>
class BaseObject {
protected:
	DirectX::XMFLOAT3 location_ = {};
	DirectX::XMFLOAT3 rotation_ = {};
	DirectX::XMFLOAT3 scale_ = {};
private:
	std::string name_;
	bool isDead_;
public:

	BaseObject(std::string name) {
		isDead_ = false;
		name_ = name;
		location_ = { 0.0f, 0.0f, 0.0f };
		rotation_ = { 0.0f, 0.0f, 0.0f };
		scale_ = { 1.0f, 1.0f, 1.0f };
	};
	virtual ~BaseObject() {}

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Release() = 0;

	void Dead() { isDead_ = true;  };
	bool IsDead() const { return isDead_; }

};