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
}

void FFmpegEncoder::StopEncode() {
    if (!isEncode_) return;
	
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
    HANDLE writePipe = nullptr;

    if (!CreatePipe(&readPipe, &writePipe, &sa, NULL)) {
        isEncode_ = false;
        OutputDebugString(L"パイプの作成に失敗したよ\n");
        return;
    }
    SetHandleInformation(writePipe, HANDLE_FLAG_INHERIT, NULL);

    STARTUPINFOA startupInfo = {};
    startupInfo.cb = sizeof(startupInfo);
    startupInfo.dwFlags = STARTF_USESTDHANDLES;
    startupInfo.hStdInput = readPipe;
    startupInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    startupInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);

    PROCESS_INFORMATION processInfo = {};
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
        &processInfo
    );

    if (!resultFunc) {
        isEncode_ = false;
        CloseHandle(writePipe);
        OutputDebugString(L"プロセスの作成に失敗したよ");
        return;
    }
    isEncode_ = true;
    CloseHandle(readPipe);

    for (auto& data : encodeData_) {
        auto rawData = data.data();

        size_t offset = 0; // 位置
        while (offset < data.size()) {    // 読み取り位置が、ピクセルサイズを超えたら終了
            DWORD chunk = (DWORD)min(data.size() - offset, 1024 * 1024);
            DWORD writeen = 0; // 書き込み済み
            if (!WriteFile(writePipe, rawData + offset, chunk, &writeen, nullptr) || writeen == 0) {
                isEncode_ = false;
                break;
            }
            offset += writeen;
        }
    }

    CloseHandle(writePipe);
    WaitForSingleObject(processInfo.hProcess, INFINITE);

    CloseHandle(processInfo.hThread);
    CloseHandle(processInfo.hProcess);
    isEncode_ = false;
}

void FFmpegEncoder::WriteData(const std::vector<unsigned char> encodeData) {
    encodeData_.push_back(encodeData);
}
