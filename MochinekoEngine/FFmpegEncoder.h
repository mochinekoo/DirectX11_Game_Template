#pragma once
#include <Windows.h>
#include <string>
#include <vector>

class FFmpegEncoder  {
private:
	bool isEncode_;

	std::string fileName_;
	int width_, height_;
	int fps_;

	HANDLE writePipe_;
	PROCESS_INFORMATION processInfo_;
public:

	FFmpegEncoder() {
		isEncode_ = false;
		fileName_ = "";
		width_ = 0, height_ = 0;
		fps_ = 0;
		writePipe_ = nullptr;
		processInfo_ = {};
	}

	~FFmpegEncoder();

	static FFmpegEncoder& GetInstance() {
		static FFmpegEncoder instance = FFmpegEncoder();
		return instance;
	}

	void StartEncode(const std::string& fileName, int width, int height, int fps = 60);
	void StopEncode();

	void WriteData(const std::vector<unsigned char> encodeData);

	bool IsEncode() const { return isEncode_; };
};