#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <ctime>

using namespace std;

const int screenWidth = 720;
const int screenHeight = 720;
const float cell = 45.0f;
const float offset = 35.0f;
const float tokenSpeed = 3.0f;
const int totalMainSteps = 52;
const int homeSteps = 6;

struct Token
{
    int pathIndex = -1;
    bool active = false;
    bool isMoving = false;
    int stepsToMove = 0;
    Vector2 pos = {0, 0};
    Color color;
    bool reachedHome = false;
};

vector<Vector2> GenerateMainPath()
{
    int coords[52][2] = {
        {6, 0}, {6, 1}, {6, 2}, {6, 3}, {6, 4}, {6, 5}, // Red side →
        {5, 6},
        {4, 6},
        {3, 6},
        {2, 6},
        {1, 6},
        {0, 6}, // Green ↑
        {0, 7},
        {0, 8}, // Green entry →
        {1, 8},
        {2, 8},
        {3, 8},
        {4, 8},
        {5, 8}, // ↓
        {6, 9},
        {6, 10},
        {6, 11},
        {6, 12},
        {6, 13},
        {6, 14}, // Yellow ↓
        {7, 14},
        {8, 14}, // Yellow turn →
        {8, 13},
        {8, 12},
        {8, 11},
        {8, 10},
        {8, 9}, // ←
        {9, 8},
        {10, 8},
        {11, 8},
        {12, 8},
        {13, 8},
        {14, 8}, // Blue →
        {14, 7},
        {14, 6}, // ↑
        {13, 6},
        {12, 6},
        {11, 6},
        {10, 6},
        {9, 6}, // ↑
        {8, 5},
        {8, 4},
        {8, 3},
        {8, 2},
        {8, 1},
        {8, 0}, // ← back to Red
        {7, 0}};

    vector<Vector2> path;
    for (int i = 0; i < 52; i++)
    {
        float x = offset + coords[i][1] * cell;
        float y = offset + coords[i][0] * cell;
        path.push_back({x, y});
    }
    return path;
}

vector<Vector2> GenerateRedHomePath()
{
    vector<Vector2> path;
    for (int i = 1; i <= homeSteps; i++)
    {
        float x = offset + (i)*cell;
        float y = offset + 6 * cell;
        path.push_back({x, y});
    }
    return path;
}

vector<Vector2> GenerateGreenHomePath()
{
    vector<Vector2> path;
    for (int i = 1; i <= homeSteps; i++)
    {
        float x = offset + 8 * cell;
        float y = offset + i * cell;
        path.push_back({x, y});
    }
    return path;
}

int main()
{
    InitWindow(screenWidth, screenHeight, "Ludo - Red vs Green (4 Tokens)");
    SetTargetFPS(60);
    srand(time(0));

    Texture2D board = LoadTexture("resource/ludo.jpg");
    Texture2D diceSheet = LoadTexture("resource/dice.png");
    int diceSize = diceSheet.width / 6;

    Rectangle diceSrc = {0, 0, (float)diceSize, (float)diceSheet.height};
    Rectangle diceDest = {screenWidth - 80, 20, 64, 64};

    vector<Vector2> mainPath = GenerateMainPath();
    vector<Vector2> redHomePath = GenerateRedHomePath();
    vector<Vector2> greenHomePath = GenerateGreenHomePath();

    vector<Token> redTokens(4), greenTokens(4);

    for (int i = 0; i < 4; i++)
    {
        redTokens[i].color = RED;
        redTokens[i].pos = {offset + (i % 2) * cell + cell, offset + (13 + (i / 2)) * cell};
        greenTokens[i].color = GREEN;
        greenTokens[i].pos = {offset + (13 + (i % 2)) * cell, offset + (i / 2) * cell + cell};
    }

    bool isRedTurn = true, canRoll = true, selecting = false;
    int diceRolled = 1;

    while (!WindowShouldClose())
    {
        // Dice roll
        if (IsKeyPressed(KEY_SPACE) && canRoll && !selecting)
        {
            diceRolled = GetRandomValue(1, 6);
            diceSrc.x = (diceRolled - 1) * diceSize;
            selecting = true;
            canRoll = false;
        }

        // Movement for all tokens
        for (auto &token : redTokens)
        {
            if (token.isMoving)
            {
                Vector2 target = token.pathIndex < totalMainSteps
                                     ? mainPath[token.pathIndex]
                                     : redHomePath[token.pathIndex - totalMainSteps];

                Vector2 dir = Vector2Subtract(target, token.pos);
                float dist = Vector2Length(dir);
                if (dist < tokenSpeed)
                {
                    token.pos = target;
                    token.stepsToMove--;
                    if (token.stepsToMove > 0)
                        token.pathIndex++;
                    else
                    {
                        token.isMoving = false;
                        if (token.pathIndex == totalMainSteps + homeSteps - 1)
                            token.reachedHome = true;
                    }
                }
                else
                {
                    token.pos = Vector2Add(token.pos, Vector2Scale(Vector2Normalize(dir), tokenSpeed));
                }
            }
        }

        for (auto &token : greenTokens)
        {
            if (token.isMoving)
            {
                Vector2 target = token.pathIndex < totalMainSteps
                                     ? mainPath[token.pathIndex]
                                     : greenHomePath[token.pathIndex - totalMainSteps];

                Vector2 dir = Vector2Subtract(target, token.pos);
                float dist = Vector2Length(dir);
                if (dist < tokenSpeed)
                {
                    token.pos = target;
                    token.stepsToMove--;
                    if (token.stepsToMove > 0)
                        token.pathIndex++;
                    else
                    {
                        token.isMoving = false;
                        if (token.pathIndex == totalMainSteps + homeSteps - 1)
                            token.reachedHome = true;
                    }
                }
                else
                {
                    token.pos = Vector2Add(token.pos, Vector2Scale(Vector2Normalize(dir), tokenSpeed));
                }
            }
        }

        // Auto-select first movable token
        if (selecting)
        {
            vector<Token> &tokens = isRedTurn ? redTokens : greenTokens;
            int entryIndex = isRedTurn ? 0 : 13;
            bool moved = false;

            for (auto &token : tokens)
            {
                if (!token.active && diceRolled == 6)
                {
                    token.active = true;
                    token.pathIndex = entryIndex;
                    token.pos = mainPath[entryIndex];
                    moved = true;
                    break;
                }
                else if (token.active && !token.isMoving && !token.reachedHome &&
                         token.pathIndex + diceRolled < totalMainSteps + homeSteps)
                {
                    token.stepsToMove = diceRolled;
                    token.isMoving = true;
                    token.pathIndex++;
                    moved = true;
                    break;
                }
            }

            selecting = false;
            if (!moved && diceRolled != 6)
                isRedTurn = !isRedTurn;
        }

        // Check if all tokens are idle to allow dice roll
        bool allIdle = true;
        for (const auto &t : redTokens)
            if (t.isMoving)
                allIdle = false;
        for (const auto &t : greenTokens)
            if (t.isMoving)
                allIdle = false;

        if (allIdle && !selecting)
            canRoll = true;

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawTexturePro(board, {0, 0, (float)board.width, (float)board.height},
                       {0, 0, (float)screenWidth, (float)screenHeight}, {0, 0}, 0.0f, WHITE);
        DrawTexturePro(diceSheet, diceSrc, diceDest, {0, 0}, 0.0f, WHITE);

        for (auto &t : redTokens)
            DrawCircleV(t.pos, 14, t.color);
        for (auto &t : greenTokens)
            DrawCircleV(t.pos, 14, t.color);

        DrawRectangle(20, 100, 130, 30, isRedTurn ? RED : GREEN);
        DrawText(isRedTurn ? "Red's Turn" : "Green's Turn", 30, 108, 18, WHITE);
        DrawText("SPACE to roll", 20, 140, 18, DARKGRAY);

        EndDrawing();
    }

    UnloadTexture(board);
    UnloadTexture(diceSheet);
    CloseWindow();
    return 0;
}
