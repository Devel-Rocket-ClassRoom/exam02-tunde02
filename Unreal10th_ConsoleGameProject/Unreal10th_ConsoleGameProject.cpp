#include <iostream>
#include <vector>
#include <string>
#include <Windows.h>

// 콘솔 크기 정의
const int WIDTH = 80;
const int HEIGHT = 25;

void Render(int ballX, int ballY) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    // 1. 화면 크기만큼의 메모리 버퍼 생성 (기본 공백으로 초기화)
    std::vector<CHAR_INFO> buffer(WIDTH * HEIGHT);
    for (int i = 0; i < WIDTH * HEIGHT; ++i) {
        buffer[i].Char.UnicodeChar = L' '; // 기본 바탕은 공백
        buffer[i].Attributes = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED; // 흰색 글씨 기본
    }

    // ==========================================================
    // 2. [★ 여기에 출력할 내용을 입력합니다 ★]
    // ==========================================================

    // 예시 A: 특정 좌표(인덱스)에 원하는 문자 넣기
    // 2차원 좌표 (X, Y)를 1차원 배열 인덱스로 변환 공식: (Y * WIDTH) + X
    int ballIndex = (ballY * WIDTH) + ballX;
    if (ballIndex >= 0 && ballIndex < WIDTH * HEIGHT) {
        buffer[ballIndex].Char.UnicodeChar = L'●'; // 공 그리기
        buffer[ballIndex].Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY; // 빨간색 지정
    }

    // 예시 B: 특정 위치에 문자열(텍스트) 출력하기
    std::wstring status = L"Ball Position X: " + std::to_wstring(ballX) + L", Y: " + std::to_wstring(ballY);
    int startX = 2;
    int startY = 2;
    for (size_t i = 0; i < status.length(); ++i) {
        int textIndex = (startY * WIDTH) + (startX + i);
        if (textIndex < WIDTH * HEIGHT) {
            buffer[textIndex].Char.UnicodeChar = status[i];
            buffer[textIndex].Attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY; // 초록색 지정
        }
    }
    // ==========================================================

    // 3. 메모리 버퍼를 화면에 한 번에 출력 (깜빡임 없음)
    COORD bufferSize = { WIDTH, HEIGHT };
    COORD bufferCoord = { 0, 0 };
    SMALL_RECT writeRegion = { 0, 0, WIDTH - 1, HEIGHT - 1 };

    WriteConsoleOutputW(hOut, buffer.data(), bufferSize, bufferCoord, &writeRegion);
}

//int main() {
//	// 콘솔 커서 숨기기 (화면을 깔끔하게 보기 위함)
//	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
//	CONSOLE_CURSOR_INFO cursorInfo;
//	GetConsoleCursorInfo(hOut, &cursorInfo);
//	cursorInfo.bVisible = FALSE;
//	SetConsoleCursorInfo(hOut, &cursorInfo);
//
//	int ballX = 10;
//	int ballY = 5;
//	int dirX = 1;
//	int dirY = 1;
//
//	// 실시간 루프 (약 30~60번 화면 갱신)
//	for (int frame = 0; frame < 200; ++frame) {
//		// 공 위치 업데이트 (벽에 부딪히면 튕김)
//		ballX += dirX;
//		ballY += dirY;
//
//		if (ballX <= 0 || ballX >= WIDTH - 1) dirX *= -1;
//		if (ballY <= 0 || ballY >= HEIGHT - 1) dirY *= -1;
//
//		// 화면 그리기 함수 호출
//		Render(ballX, ballY);
//
//		// 너무 빠르지 않게 33ms 대기 (약 30 FPS)
//		Sleep(33);
//	}
//
//	return 0;
//}
