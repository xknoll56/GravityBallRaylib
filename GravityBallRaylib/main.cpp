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
    return { v.y, v.z, v.x };
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

struct RenderingMaterial
{
    GBVector3 color = { 1.0f,1.0f,1.0f };
    bool drawWireFrame = false;
    bool useTexture = false;
    RenderingMaterial(GBVector3 col, bool drawWireFrame = false, bool useTexture = false) :
        color(col) , drawWireFrame(drawWireFrame), useTexture(useTexture)
    {
    }

    Color getColor()
    {
        Color c;
        c.r = color.x * 255;
        c.g = color.y * 255;
        c.b = color.z * 255;
        c.a = 255;
        return c;
    }
};

void initSimulation()
{
    GBBody* pBody = simulation.createBody();
    GBBoxCollider* pBox = simulation.attachBoxCollider(pBody, { 0.5f,0.5f,0.5f });
    pBox->pData = new RenderingMaterial({ 1,0,1 }, true, true);
    pBody->angularVelocity = { 2,2,2 };
    pBody->transform.position = { 0,0,10 };

    pBody = simulation.createBody();
    GBSphereCollider* pSphere = simulation.attachSphereCollider(pBody, 0.65f);
    pBody->transform.position = { 0,5,10 };
    pSphere->pData = new RenderingMaterial({ 0.6,0.3,0.86 }, true, true);


    pBody = simulation.createBody(1.0f, true);
    pBox = simulation.attachBoxCollider(pBody, { 20,20, 0.05f });
    pBox->pData = new RenderingMaterial({ 1,1,1});
    pBody->transform.position = { 0,0,-0.025 };
    simulation.init();
}

Mesh cubeMesh;
Model cubeModel;
Mesh sphereMesh;
Model sphereModel;

int viewPosLoc;
int ambientLoc;
int colDiffuseLocation;
int scaleLoc;
int useTextureLoc;
int matModelLoc;

Shader shader;


void drawBoxEdges(const GBBoxCollider& box, Color color = ORANGE)
{
    Vector3 verts[8];
    for (int i = 0; i < 8; i++)
    {
        verts[i] = toRayVec(box.vertices[i]);
    }

    // back face
    DrawLine3D(verts[0], verts[1], color);
    DrawLine3D(verts[1], verts[2], color);
    DrawLine3D(verts[2], verts[3], color);
    DrawLine3D(verts[3], verts[0], color);
    DrawLine3D(verts[0], verts[2], color);
    DrawLine3D(verts[1], verts[3], color);

    // front face
    DrawLine3D(verts[4], verts[5], color);
    DrawLine3D(verts[5], verts[6], color);
    DrawLine3D(verts[6], verts[7], color);
    DrawLine3D(verts[7], verts[4], color);
    DrawLine3D(verts[4], verts[6], color);
    DrawLine3D(verts[5], verts[7], color);

    // connections
    DrawLine3D(verts[0], verts[4], color);
    DrawLine3D(verts[1], verts[5], color);
    DrawLine3D(verts[2], verts[6], color);
    DrawLine3D(verts[3], verts[7], color);
}

void drawSimulation()
{
    for (auto& bodyIt : simulation.rigidBodies)
    {
        for (GBCollider* pCol : bodyIt->colliders)
        {
            GBBoxCollider* pBox;
            GBCapsuleCollider* pCap;
            GBSphereCollider* pSphere;
            RenderingMaterial* pMat = (RenderingMaterial*)pCol->pData;
            switch (pCol->type)
            {
            case ColliderType::Sphere:
            {
                pSphere = (GBSphereCollider*)pCol;
                sphereModel.transform = makeTransform(pSphere->transform.position, pSphere->transform.rotation,
                     GBVector3::uniformSize(pSphere->radius * 2.0f));
                BeginShaderMode(shader);

                int useTexture = pMat->useTexture;

                SetShaderValue(
                    shader,
                    useTextureLoc,
                    &useTexture,
                    SHADER_UNIFORM_INT
                );


                DrawModel(sphereModel, { 0,0,0 }, 1.0f, pMat->getColor());
                EndShaderMode();

                break;
            }
            case ColliderType::Box:
            {
                pBox = (GBBoxCollider*)pCol;
                pBox->setVerts();
                pCol->pBody->updateColliders();
                cubeModel.transform = makeTransform(pBox->transform.position, pBox->transform.rotation, 2.0f * pBox->halfExtents);
                //SetShaderValueMatrix(shader, matModelLoc, cubeModel.transform);
                float maxX = GBMax(pBox->halfExtents.x, pBox->halfExtents.y);
                float maxY = GBMax(maxX, pBox->halfExtents.z);
                Vector2 s = {2.0f*maxX, 2.0f*maxY };
                SetShaderValue(
                    shader,
                    scaleLoc,
                    &s,
                    SHADER_UNIFORM_VEC2
                );

                BeginShaderMode(shader);

                int useTexture = pMat->useTexture;

                SetShaderValue(
                    shader,
                    useTextureLoc,
                    &useTexture,
                    SHADER_UNIFORM_INT
                );


                DrawModel(cubeModel, { 0,0,0 }, 1.0f, pMat->getColor());
                EndShaderMode();

                if(pMat->drawWireFrame)
                    drawBoxEdges(*pBox);

                break;
            }
            case ColliderType::Capsule:
                pCap = (GBCapsuleCollider*)pCol;
                break;
            }
        }
    }
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

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera freea");

    // Define the camera to look into our 3d world
    Camera3D camera = { 0 };
    camera.position = { 10.0f, 10.0f, 10.0f }; // Camera position
    camera.target = { 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 65.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type
    Texture2D crateTex = LoadTexture("resources/crate-diffuse.jpg");
    SetTextureWrap(crateTex, TEXTURE_WRAP_REPEAT);


    cubeMesh =  GenMeshCube(1.0f, 1.0f, 1.0f);
     cubeModel = LoadModelFromMesh(cubeMesh);
     sphereMesh = GenMeshSphere(0.5f, 20, 20);
     sphereModel = LoadModelFromMesh(sphereMesh);

   shader = LoadShader(
        "Resources/lighting.vs",
        "Resources/lighting.fs"
    );

    cubeModel.materials[0].shader = shader;
    cubeModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = crateTex;

    viewPosLoc = GetShaderLocation(shader, "viewPos");
    ambientLoc = GetShaderLocation(shader, "ambient");
    colDiffuseLocation = GetShaderLocation(shader, "colDiffuse");
    scaleLoc = GetShaderLocation(shader, "scale");
    useTextureLoc = GetShaderLocation(shader, "useTexture");
    matModelLoc = GetShaderLocation(shader, "matModel");

    float ambient[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
    SetShaderValue(shader, ambientLoc, ambient, SHADER_UNIFORM_VEC4);

    DisableCursor();                    // Limit cursor to relative movement inside the window

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    initSimulation();

    Vector4 color;
    color = { 1,0,0,1 };
    Vector2 scale = { 2.0f, 2.0f };

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        float dt = GetFrameTime();

        simulation.step(dt);
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera, CAMERA_FREE);

        SetShaderValue(
            shader,
            viewPosLoc,
            &camera.position,
            SHADER_UNIFORM_VEC3
        );



        if (IsKeyPressed(KEY_Z)) camera.target = { 0.0f, 0.0f, 0.0f };
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        drawSimulation();

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