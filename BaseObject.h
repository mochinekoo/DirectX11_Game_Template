#pragma once
#include <string>
#include <DirectXMath.h>

/// <summary>
/// オブジェクトの基底クラス
/// </summary>
class BaseObject {
protected:
	DirectX::XMFLOAT3 location_ = {};
private:
	std::string name_;
	bool isDead_;
public:

	BaseObject(std::string name) {
		isDead_ = false;
		name_ = name;
	};
	virtual ~BaseObject() {}

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Release() = 0;

	void Dead() { isDead_ = true;  };
	bool IsDead() const { return isDead_; }

};