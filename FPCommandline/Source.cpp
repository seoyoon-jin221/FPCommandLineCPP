#include<iostream>
#include<chrono>

using namespace std;

#include <Windows.h>

int nScreenWidth = 120;
int nScreenHeight = 40;

float fPlayerX = 8.0f;
float fPlayerY = 8.0f;
float fPlayerA = 0.0f; 

int nMapHeight = 16;
int nMapWidth = 16;

float fFov = 3.14159 / 4.0; //field of view 45 degrees 
float fDepth = 16.0f;

int main()
{
    wchar_t *screen = new wchar_t[nScreenWidth*nScreenHeight];
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER,NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;
    

    wstring map;
    map += L"################";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"################";

    auto tp1 = chrono::system_clock::now();
    auto tp2 = chrono::system_clock::now();

    while (1)
    {
        tp2 = chrono::system_clock::now();
        chrono::duration<float> elapsedTime = tp2 - tp1; 
        tp1 - tp2;
        float fElapsedTime = elapsedTime.count();


        if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
            fPlayerA -= (0.1f) * fElapsedTime;

        if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
            fPlayerA -= (0.1f) * fElapsedTime;

        for (int x = 0; x < nScreenWidth; x++)
        {
            float fRayAngle = (fPlayerA - fFov / 2.0f) + ((float)x / (float)nScreenWidth) * fFov;

            float fDistanceToWall = 0;
            bool bHitWall = false;

            float fEyeX = sinf(fRayAngle); // Unit Vector for ray in player space
            float fEyeY = cosf(fRayAngle);

            while (!bHitWall && fDistanceToWall < fDepth)
            {
                fDistanceToWall += 0.1f;
                int nTestX = (int)(fPlayerX + fEyeX * fDistanceToWall);
                int nTestY = (int)(fPlayerY + fEyeY * fDistanceToWall);

                // Test if ray is out of bounds
                if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight)
                {
                    //Ray does not hit the wall, just set distance to maximum depth
                    bHitWall = true;
                    fDistanceToWall = fDepth;
                }
                else
                {
                    //Ray is inbounds so test to see if the ray cell is a wall block 
                    if (map[nTestY*nMapWidth + nTestX] == '#')
                    {
                        bHitWall = true;
                    }
                }
            }
        
            int nCeiling = (float)(nScreenHeight / 2.0) - nScreenHeight / ((float)fDistanceToWall);
            int nFloor = nScreenHeight = nCeiling; // mirror of the ceiling 

            short nShade = ' ';

            if (fDistanceToWall <= fDepth / 4.0f)       nShade = 0x2588; //different unicode characters for shading 
            else if (fDistanceToWall < fDepth / 3.0f)   nShade = 0x2593;
            else if (fDistanceToWall < fDepth / 2.0f)   nShade = 0x2592;
            else if (fDistanceToWall < fDepth)          nShade = 0x2591;
            else                                        nShade = ' ';
            
                   
       

            for (int y = 0; y < nScreenHeight; y++)
            {
                if (y < nCeiling)
                    screen[y*nScreenWidth + x] = ' '; //The ceiling is blank space
                else if (y > nCeiling && y <= nFloor)
                    screen[y*nScreenWidth + x] = '#'; //The wall is represented by #
                else
                    screen[y*nScreenWidth + x] = ' '; //The floor is blank space

            }
        }

        screen[nScreenWidth * nScreenHeight - 1] = '\0';
        WriteConsoleOutputCharacterW(hConsole, screen, nScreenWidth*nScreenHeight, { 0,0 }, &dwBytesWritten);
    }
   
    return 0;
}