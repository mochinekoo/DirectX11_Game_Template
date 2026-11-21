#include "FFmpegEncoder.h"

FFmpegEncoder::~FFmpegEncoder()
{
}

void FFmpegEncoder::StartEncode(const std::string& fileName, int width, int height, int fps) {
	if (isEncode_) {
		return;
	}
	fileName_ = fileName;
	width_ = width;
	height_ = height;
	fps_ = fps;
    isEncode_ = true;

    std::string command =
        "ffmpeg -y -f rawvideo "
        "-pixel_format bgra "
        "-video_size " +
        std::to_string(width_) + "x" +
        std::to_string(height_) + " "
        "-framerate " + std::to_string(fps_) + " "
        "-i pipe:0 "
        "-c:v libx264 "
        "-pix_fmt yuv420p "
        "-crf 18 "
        "\"" + fileName_ + "\"";

    SECURITY_ATTRIBUTES sa = {};
    sa.nLength = sizeof(sa);
    sa.bInheritHandle = TRUE;

    HANDLE readPipe = nullptr;

    if (!CreatePipe(&readPipe, &writePipe_, &sa, NULL)) {
        isEncode_ = false;
        OutputDebugString(L"パイプの作成に失敗したよ\n");
        return;
    }
    SetHandleInformation(writePipe_, HANDLE_FLAG_INHERIT, NULL);

    STARTUPINFOA startupInfo = {};
    startupInfo.cb = sizeof(startupInfo);
    startupInfo.dwFlags = STARTF_USESTDHANDLES;
    startupInfo.hStdInput = readPipe;
    startupInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    startupInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);

    BOOL resultFunc = FALSE;

    resultFunc = CreateProcessA(
        nullptr,
        command.data(),
        nullptr,
        nullptr,
        TRUE,
        CREATE_NO_WINDOW,
        nullptr,
        nullptr,
        &startupInfo,
        &processInfo_
    );

    if (!resultFunc) {
        isEncode_ = false;
        CloseHandle(writePipe_);
        OutputDebugString(L"プロセスの作成に失敗したよ");
        return;
    }
    isEncode_ = true;
    CloseHandle(readPipe);
}

void FFmpegEncoder::StopEncode() {
    CloseHandle(writePipe_);
    WaitForSingleObject(processInfo_.hProcess, INFINITE);

    CloseHandle(processInfo_.hThread);
    CloseHandle(processInfo_.hProcess);
    isEncode_ = false;
    processInfo_ = {};
}

void FFmpegEncoder::WriteData(const std::vector<unsigned char> encodeData) {
    auto rawData = encodeData.data();

    size_t offset = 0; // 位置
    while (offset < encodeData.size()) {    // 読み取り位置が、ピクセルサイズを超えたら終了
        DWORD chunk = (DWORD)min(encodeData.size() - offset, 1024 * 1024);
        DWORD writeen = 0; // 書き込み済み
        if (!WriteFile(writePipe_, rawData + offset, chunk, &writeen, nullptr) || writeen == 0) {
            isEncode_ = false;
            break;
        }
        offset += writeen;
    }
}
