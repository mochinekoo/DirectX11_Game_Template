#pragma once
#include <string>
#include <DirectXMath.h>

class BaseObject {
private:
	std::string name_;
protected:
	std::string tag_;
	bool isDead_;

	DirectX::XMFLOAT3 location_;
	DirectX::XMFLOAT3 velocity_;
	DirectX::XMFLOAT3 rotation_;
	DirectX::XMFLOAT3 scale_;
public:

	BaseObject(const std::string& name) {
		name_ = name;
		tag_ = "";
		isDead_ = false;
		location_ = { 0.0f, 0.0f, 0.0f };
		velocity_ = { 0.0f, 0.0f, 0.0f };
		rotation_ = { 0.0f, 0.0f, 0.0f };
		scale_ = { 1.0f, 1.0f, 1.0f };
	};
	virtual ~BaseObject() {};

	virtual void Init() {};
	virtual void Update() {};
	virtual void Draw() {};
	virtual void Release() {};

	std::string GetName() const { return name_; }
	std::string GetTag() const { return tag_; }
	void SetTag(const std::string& tag) { tag_ = tag; }
	void KillMe() { isDead_ = true; }
	bool IsDead() const { return isDead_; }

};