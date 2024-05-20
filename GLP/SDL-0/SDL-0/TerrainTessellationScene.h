#pragma once
#include "TessellationScene.h"
#include <Matrix4.h>
#include "Assets.h"
class TerrainTessellationScene :public TessellationScene
{
public:
    TerrainTessellationScene();
    ~TerrainTessellationScene(){}
    void SetupScene() override;
    void UpdateScene() override;
    void HandleInputs(SDL_Event& e) override;

private:
    void LoadShaders();

    // Uniforms
    Matrix4 mvp;
    Matrix4 view;
    Matrix4 proj;
    float dmapDepth;
    bool isFogEnabled;

    GLuint texDisplacement;
    GLuint texColor;
    bool isDisplacementEnabled;
    bool wireframe;
    bool paused;

    float totalTime;
    float t, r, h;
};

