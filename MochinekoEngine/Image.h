#pragma once
#include "BaseObject.h"
#include <string>
#include "Texture.h"

class Image : public BaseObject {
private:
	Texture* texture_;
	float opacity_;

	//
	// エフェクト系
	//

	bool enableGray_;		// グレースケール
	bool colorInversion_;	// 反転
public:

	Image(const std::string& fileName)
		: BaseObject("Image") {
		texture_ = new Texture(fileName);
		enableGray_ = false;
		opacity_ = 1.0f;
		colorInversion_ = false;
	}

	~Image() {
		delete texture_;
	}

	void Init() override;
	void Update() override;
	void Draw() override;
	void DrawImGUI() override;
	void Release() override;

	std::string GetFileName() const { return texture_->GetFileName(); }
	int GetWidth() const { return texture_->GetWidth(); }
	int GetHeight() const { return texture_->GetHeight(); }

	bool IsGray() const { return enableGray_; }
	void SetGray(bool flag) { enableGray_ = flag; }

	float GetOpacity() const { return opacity_; }
	void SetOpacity(const float opacity) { opacity_ = opacity; }
};

