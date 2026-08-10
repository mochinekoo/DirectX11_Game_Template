#include "ImageManager.h"
#include <vector>
#include "Image.h"
#include <filesystem>

namespace ImageManager {
	std::vector<Image*> imageList_;
}

void ImageManager::Init() {
	imageList_.clear();
}

int ImageManager::Load(const std::string& fileName) {
	for (int i = 0; i < imageList_.size(); i++) {
		Image* image = imageList_[i];
		if (image->GetFileName() == fileName) {
			return i;
		}
	}

	if (!std::filesystem::exists(fileName)) {
		return -1;
	}

	Image* image = new Image(fileName);
	image->Init();
	imageList_.push_back(image);

	return imageList_.size() - 1;
}

void ImageManager::Draw(const int id) {
	imageList_[id]->Update();
	imageList_[id]->Draw();
}

void ImageManager::SetPostion(const int id, const DirectX::XMFLOAT3 postion) {

}

void ImageManager::SetVelocity(const int id, const DirectX::XMFLOAT3 velocity)
{
}

void ImageManager::SetRotation(const int id, const DirectX::XMFLOAT3 rote)
{
}

void ImageManager::SetScale(const int id, const DirectX::XMFLOAT3 scale)
{
}
