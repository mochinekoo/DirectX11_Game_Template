#pragma once
#include "BaseObject.h"
#include "Image.h"
#include <string>
#include <functional>

class ImageButton : public BaseObject {
private:
	Image* image_;
	bool isPush_;
	bool isHover_;
	std::function<void()> hoverFunc;
	std::function<void()> pushFunc;
public:

	ImageButton(const std::string& fileName)
		: BaseObject("ImageButton") {
		image_ = new Image(fileName);
		isPush_ = false;
		isHover_ = false;
	}

	~ImageButton() {}

	void Init() override;
	void Update() override;
	void Draw() override;
	void DrawImGUI() override;
	void Release() override;

	bool IsPush() const { return isPush_; }
	bool IsHover() const { return isHover_; }

	Image* GetImage() const { return image_; }

	void SetHoverFunc(std::function<void()> func) { hoverFunc = func; }
	void SetPushFunc(std::function<void()> func) { pushFunc = func; }
};