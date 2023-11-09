#include <iostream>
#include <Windows.h>

const int screenWidth = 80;
const int screenHeight = 24;

CHAR_INFO consoleBuffer[screenWidth * screenHeight];
HANDLE hConsole;

void ClearScreen() {
    for (int i = 0; i < screenWidth * screenHeight; i++) {
        consoleBuffer[i].Char.AsciiChar = ' ';
        consoleBuffer[i].Attributes = FOREGROUND_GREEN;
    }
}

void SetChar(int x, int y, char c) {
    if (x >= 0 && x < screenWidth && y >= 0 && y < screenHeight) {
        consoleBuffer[y * screenWidth + x].Char.AsciiChar = c;
    }
}

void Render() {
    COORD bufferSize = { (short)screenWidth, (short)screenHeight };
    COORD bufferCoord = { 0, 0 };
    SMALL_RECT writeRegion = { 0, 0, (short)(screenWidth - 1), (short)(screenHeight - 1) };
    
    WriteConsoleOutputA(hConsole, consoleBuffer, bufferSize, bufferCoord, &writeRegion);
}

int main() {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Enable double buffering for the console
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    SetConsoleActiveScreenBuffer(hConsole);

    ClearScreen();

    bool isRunning = true;
    while (isRunning) {
        // Handle user input or update game state

        // Clear the console buffer
        ClearScreen();

        // Draw your game or application here
        SetChar(10, 10, 'X');

        // Render the frame
        Render();

        // You can use Sleep() or other methods to control the frame rate
        Sleep(16);  // Sleep for approximately 60 FPS
    }

    return 0;
}
