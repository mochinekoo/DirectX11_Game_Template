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
	bool sepia_;			// セピア
	float sepiaStrength_;	// セピアの強さ
public:

	Image(const std::string& fileName)
		: BaseObject("Image") {
		texture_ = new Texture(fileName);
		opacity_ = 1.0f;

		enableGray_ = false;
		colorInversion_ = false;
		sepia_ = false;
		sepiaStrength_ = 1.0f;
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

