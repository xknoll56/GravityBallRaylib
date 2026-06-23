/*******************************************************************************************
*
*   raylib [core] example - 3d camera free
*
*   Example complexity rating: [★☆☆☆] 1/4
*
*   Example originally created with raylib 1.3, last time updated with raylib 1.3
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2015-2025 Ramon Santamaria (@raysan5)
*
********************************************************************************************/
#include "GBSimulation.h"
#include "raylib.h"
#include "raymath.h"

Vector3 toRayVec(GBVector3 v)
{
    return { v.y, v.z, -v.x };
}

Quaternion toRayQuat(GBQuaternion q)
{
    return { q.y, q.z, q.x, q.w };
}

Matrix makeTransform(
    GBVector3 position,
    GBQuaternion rotation,
    GBVector3 scale)
{
    Vector3 rayPos = toRayVec(position);
    Vector3 rayScale = toRayVec(scale);
    Matrix S = MatrixScale(rayScale.x, rayScale.y, rayScale.z);
    Matrix R = QuaternionToMatrix(toRayQuat(rotation));
    Matrix T = MatrixTranslate(rayPos.x, rayPos.y, rayPos.z);

    return MatrixMultiply(
        MatrixMultiply(S, R),
        T
    );
}

GBSimulation simulation;

void initSimulation()
{
    GBBody* pBody = simulation.createBody();
    GBBoxCollider* pBox = simulation.attachBoxCollider(pBody, { 0.5f,0.5f,0.5f });
    pBody->angularVelocity = { 2,2,2 };
    pBody->transform.position = { 0,0,10 };

    pBody = simulation.createBody(1.0f, true);
    pBox = simulation.attachBoxCollider(pBody, { 20,20, 0.05f });
    pBody->transform.position = { 0,0,-0.025 };
    simulation.init();
}

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1024;
    const int screenHeight = 768;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera free");

    // Define the camera to look into our 3d world
    Camera3D camera = { 0 };
    camera.position = { 10.0f, 10.0f, 10.0f }; // Camera position
    camera.target = { 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    Mesh cubeMesh = GenMeshCube(1.0f, 1.0f, 1.0f);
    Model cubeModel = LoadModelFromMesh(cubeMesh);


    DisableCursor();                    // Limit cursor to relative movement inside the window

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    initSimulation();

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        float dt = GetFrameTime();

        simulation.step(dt);
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera, CAMERA_FREE);

        if (IsKeyPressed(KEY_Z)) camera.target = { 0.0f, 0.0f, 0.0f };
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        GBBody* pBoxBody = simulation.getBody(0);

        cubeModel.transform = makeTransform(pBoxBody->transform.position, pBoxBody->transform.rotation, { 1,1,1 });
        DrawModel(cubeModel,{0,0,0}, 1.0f, pBoxBody->isSleeping?BLUE:GREEN);

        pBoxBody = simulation.getBody(1);

        cubeModel.transform = makeTransform(pBoxBody->transform.position, pBoxBody->transform.rotation, { 20,20,0.05 });
        DrawModel(cubeModel, { 0,0,0 }, 1.0f, RED);

        DrawGrid(10, 1.0f);

        EndMode3D();

        DrawText("GRAVITY BALL!", 20, 20, 10, BLACK);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}