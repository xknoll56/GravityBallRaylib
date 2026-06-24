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
    int playerLayer = 3;

    {
       GBBody* body = simulation.createBody();
        body->transform.position = { 0,0,6 };
        body->angularVelocity = { 100,10,10 };
        body->velocity = { 10,10,10 };
        body->layer = ~playerLayer;
        GBCapsuleCollider* cc = simulation.attachCapsuleCollider(body, 0.25f, 0.8f);
        cc->pData = new RenderingMaterial({ 1,1,1 }, true, false);


        GBBody* arm1 = simulation.createBody();
        GBCapsuleCollider* cc1 = simulation.attachCapsuleCollider(arm1, 0.1f, 0.45f);
        arm1->transform.position = body->transform.position +
            GBVector3::right() * (cc->radius + cc1->radius + cc1->height * 0.5f)
            + GBVector3::up() * 0.25f;
        arm1->transform.rotate(GBQuaternion::fromAxisAngle({ 1,0,0 }, GB_PI * 0.5f));
        arm1->updateColliders();
        arm1->layer = ~playerLayer;
        cc1->pData = new RenderingMaterial({ 0,1,0 }, true, false);


        GBBody* arm11 = simulation.createBody();
        GBCapsuleCollider* cc11 = simulation.attachCapsuleCollider(arm11, 0.1f, 0.45f);
        arm11->transform.position = arm1->transform.position + GBVector3::right() * (cc1->height * 0.5f + cc1->radius + cc11->height * 0.5f + cc11->radius);
        arm11->transform.rotate(GBQuaternion::fromAxisAngle({ 1,0,0 }, GB_PI * 0.5f));
        arm11->updateColliders();
        arm11->layer = ~playerLayer;
        cc11->pData = new RenderingMaterial({ 1,0,0 }, true, false);


        GBBody* arm2 = simulation.createBody();
        GBCapsuleCollider* cc2 = simulation.attachCapsuleCollider(arm2, 0.1f, 0.45f);
        arm2->transform.position = body->transform.position -
            GBVector3::right() * (cc->radius + cc2->radius + cc2->height * 0.5f)
            + GBVector3::up() * 0.25f;
        arm2->transform.rotate(GBQuaternion::fromAxisAngle({ 1,0,0 }, GB_PI * 0.5f));
        arm2->updateColliders();
        arm2->layer = ~playerLayer;
        cc2->pData = new RenderingMaterial({ 0,1,0 }, true, false);

        GBBody* arm22 = simulation.createBody();
        GBCapsuleCollider* cc22 = simulation.attachCapsuleCollider(arm22, 0.1f, 0.45f);
        arm22->transform.position = arm2->transform.position - GBVector3::right() * (cc2->height * 0.5f + cc2->radius + cc22->height * 0.5f + cc22->radius);
        arm22->transform.rotate(GBQuaternion::fromAxisAngle({ 1,0,0 }, GB_PI * 0.5f));
        arm22->updateColliders();
        arm22->layer = ~playerLayer;
        cc22->pData = new RenderingMaterial({ 1,0,0 }, true, false);


        GBBody* leg = simulation.createBody();
        GBCapsuleCollider* cc3 = simulation.attachCapsuleCollider(leg, 0.1f, 0.5f);
        leg->transform.position = body->transform.position +
            GBVector3::up() * (-cc->height * 0.5f - cc->radius - cc3->height * 0.5f - cc3->radius) +
            GBVector3::right() * (cc3->radius);
        leg->updateColliders();
        leg->layer = ~playerLayer;
        cc3->pData = new RenderingMaterial({ 0,1,1 }, true, false);

        GBBody* foot = simulation.createBody();
        GBCapsuleCollider* cc33 = simulation.attachCapsuleCollider(foot, 0.1f, 0.5);
        foot->transform.position = leg->transform.position +
            GBVector3::up() * (-cc3->height * 0.5f - cc3->radius - cc33->height * 0.5f - cc33->radius);
        foot->updateColliders();
        foot->layer = ~playerLayer;
        cc33->pData = new RenderingMaterial({ 1,0,1 }, true, false);

        GBBody* leg1 = simulation.createBody();
        GBCapsuleCollider* cc4 = simulation.attachCapsuleCollider(leg1, 0.1f, 0.5);
        leg1->transform.position = body->transform.position +
            GBVector3::up() * (-cc->height * 0.5f - cc->radius - cc3->height * 0.5f - cc3->radius) +
            GBVector3::left() * (cc3->radius);
        leg1->updateColliders();
        leg1->layer = ~playerLayer;
        cc4->pData = new RenderingMaterial({ 0,1,1 }, true, false);

        GBBody* foot1 = simulation.createBody();
        GBCapsuleCollider* cc44 = simulation.attachCapsuleCollider(foot1, 0.1f, 0.5);
        foot1->transform.position = leg1->transform.position +
            GBVector3::up() * (-cc4->height * 0.5f - cc4->radius - cc44->height * 0.5f - cc44->radius);
        foot1->updateColliders();
        foot1->layer = ~playerLayer;
        cc44->pData = new RenderingMaterial({ 1,0,1 }, true, false);

        GBBody* head = simulation.createBody();
        GBCapsuleCollider* cc5 = simulation.attachCapsuleCollider(head, 0.35f, 0.0f);
        head->transform.position = body->transform.position +
            GBVector3::up() * (cc->height * 0.5f + cc->radius + cc5->radius);
        head->updateColliders();
        head->layer = ~playerLayer;
        cc5->pData = new RenderingMaterial({0,0,1 }, true, false);


        GBBallJoint* s = simulation.attachCapsuleBallJoint(body, cc1);
        s = simulation.attachCapsuleBallJoint(arm1, cc11);
        s = simulation.attachCapsuleBallJoint(body, cc2);
        s = simulation.attachCapsuleBallJoint(arm2, cc22);
        s = simulation.attachCapsuleBallJoint(body, cc4);
        s = simulation.attachCapsuleBallJoint(leg1, cc44);
        s = simulation.attachCapsuleBallJoint(body, cc3);
        s = simulation.attachCapsuleBallJoint(leg, cc33);
        s = simulation.attachCapsuleBallJoint(body, cc5);
    }

    GBBody* pBody = simulation.createBody(1.0f, true);
    GBBoxCollider* pBox = simulation.attachBoxCollider(pBody, { 20,20, 0.05f });
    pBox->pData = new RenderingMaterial({ 1,1,1 });
    pBody->transform.position = { 0,0,-0.025 };

    //simulation.init();
}

Mesh cubeMesh;
Model cubeModel;
Mesh sphereMesh;
Model sphereModel;
Mesh cylinderMesh;
Model cylinderModel;

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

void drawSphereFrame(GBTransform sphereTrans, float radius, const RenderingMaterial& mat, Color color)
{

    for (int i = 0; i < 3; i++)
    {
        Vector3 axis(0, 0, 0);
        if (i == 0)
            axis.x = 1.0f;
        else if (i == 1)
            axis.y = 1.0f;
        else
            axis.z = 1.0f;

        if (mat.drawWireFrame)
        {
            DrawCircle3D(
                toRayVec(sphereTrans.position),
                radius,
                axis,
                270.0f/GB_PI ,
                color
            );
        }
    }
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

            if (pMat)
            {
                BeginShaderMode(shader);

                switch (pCol->type)
                {
                case ColliderType::Sphere:
                {
                    pSphere = (GBSphereCollider*)pCol;
                    sphereModel.transform = makeTransform(pSphere->transform.position, pSphere->transform.rotation,
                        GBVector3::uniformSize(pSphere->radius * 2.0f));

                    Vector2 s = { 2.0f , 2.0f };
                    SetShaderValue(
                        shader,
                        scaleLoc,
                        &s,
                        SHADER_UNIFORM_VEC2
                    );

                    

                    int useTexture = pMat->useTexture;

                    SetShaderValue(
                        shader,
                        useTextureLoc,
                        &useTexture,
                        SHADER_UNIFORM_INT
                    );


                    DrawModel(sphereModel, { 0,0,0 }, 1.0f, pMat->getColor());
                    
                    drawSphereFrame(pSphere->transform, pSphere->radius, *pMat, pCol->pBody->isSleeping ? RED : GREEN);

                    break;
                }
                case ColliderType::Box:
                {
                    pBox = (GBBoxCollider*)pCol;

                    // Setting the verts of the box does not happen everyframe, instead boxes convert to quads which get the positions anyway
                    // But this function is left over and still may be useful.
                    pBox->setVerts();
                    pCol->pBody->updateColliders();
                    cubeModel.transform = makeTransform(pBox->transform.position, pBox->transform.rotation, 2.0f * pBox->halfExtents);
                    float maxX = GBMax(pBox->halfExtents.x, pBox->halfExtents.y);
                    float maxY = GBMax(maxX, pBox->halfExtents.z);
                    Vector2 s = { 2.0f * maxX, 2.0f * maxY };
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

                    if (pMat->drawWireFrame)
                        drawBoxEdges(*pBox, pCol->pBody->isSleeping?RED:GREEN);

                    break;
                }
                case ColliderType::Capsule:
                    pCap = (GBCapsuleCollider*)pCol;


                    Vector2 s = { 2.0f , 2.0f };
                    SetShaderValue(
                        shader,
                        scaleLoc,
                        &s,
                        SHADER_UNIFORM_VEC2
                    );



                    int useTexture = pMat->useTexture;

                    SetShaderValue(
                        shader,
                        useTextureLoc,
                        &useTexture,
                        SHADER_UNIFORM_INT
                    );


                    GBVector3 upper, lower, up;
                    pCap->extractSphereLocations(upper, lower, &up);
                    sphereModel.transform = makeTransform(upper, pCap->transform.rotation,
                        GBVector3::uniformSize(pCap->radius * 2.0f));


                    DrawModel(sphereModel, { 0,0,0 }, 1.0f, pMat->getColor());

                    sphereModel.transform = makeTransform(lower, pCap->transform.rotation,
                        GBVector3::uniformSize(pCap->radius * 2.0f));


                    DrawModel(sphereModel, { 0,0,0 }, 1.0f, pMat->getColor());

                    cylinderModel.transform = makeTransform(pCap->transform.position - up*pCap->height*0.5f, pCap->transform.rotation,
                        {2.0f* pCap->radius, 2.0f*pCap->radius, pCap->height});
                    DrawModel(cylinderModel, { 0,0,0 }, 1.0f, pMat->getColor());


                    if (pMat->drawWireFrame)
                    {
                        drawSphereFrame(GBTransform(lower), pCap->radius, *pMat, pCol->pBody->isSleeping?RED:GREEN);
                        drawSphereFrame(GBTransform(upper), pCap->radius, *pMat, pCol->pBody->isSleeping ? RED : GREEN);
                    }

                    break;
                }

                EndShaderMode();
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
     cylinderMesh = GenMeshCylinder(0.5f, 1.0f, 20.0f);
     cylinderModel = LoadModelFromMesh(cylinderMesh);


   shader = LoadShader(
        "Resources/lighting.vs",
        "Resources/lighting.fs"
    );

    cubeModel.materials[0].shader = shader;
    cubeModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = crateTex;
    sphereModel.materials[0].shader = shader;
    sphereModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = crateTex;
    cylinderModel.materials[0].shader = shader;
    cylinderModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = crateTex;

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
        if (dt > 1.0f / 60.0f)
            dt = 1.0f / 60.0f;
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