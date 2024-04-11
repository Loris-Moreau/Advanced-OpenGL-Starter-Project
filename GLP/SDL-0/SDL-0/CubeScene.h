#pragma once
#include "BaseScene.h"
#include "Texture.h"

class CubeScene : public Scene
{
protected:
    void LoadShaders() override;
    void CreateShaderPrograms() override;
    void VerticeInformationSlicer() override;
    
public:
    CubeScene();
    ~CubeScene() = default;
    void SetupScene() override;
    void UpdateScene() override;
    
private:
    Texture2D texture;
    Shader m_vertexShader, m_fragmentShader, m_fragmentShader2;
    Shader m_tessControl, m_tessEval;
    ShaderProgram m_Program;
    ShaderProgram m_TessProgram;
    Matrix4 projection, mv, rotation;
    Vector3 position;
    bool wireframe;
    
};
