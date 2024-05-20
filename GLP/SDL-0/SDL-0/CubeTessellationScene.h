#pragma once
#include "TessellationScene.h"
#include "Texture.h"

class CubeTessellationScene : public TessellationScene
{
private:
    Texture2D texture;
    Shader m_vertexShader, m_fragmentShader;
    ShaderProgram m_Program;
    Matrix4 projection, mv, rotation;
    Vector3 position;
    bool wireframe;
protected:
    void LoadShaders() override;
    void CreateShaderPrograms() override;
    void VerticeInformationSlicer() override;
public:
    CubeTessellationScene();
    ~CubeTessellationScene() = default;
    void SetupScene() override;
    void UpdateScene() override;
protected:

};
