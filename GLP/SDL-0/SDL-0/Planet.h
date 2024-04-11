#pragma once

#include "Scene.h"

class Planet : public Scene
{
protected:
	void LoadShaders() override;
	void CreateShaderPrograms() override;
	void VerticeInformationSlicer() override;
	
public:
	Planet();
	~Planet();
	void SetupScene() override;
	void UpdateScene() override;
	
private:
	Shader m_vertexShader, m_fragmentShader, m_tessControl, m_tessEval;
	
	ShaderProgram m_Program, m_TessProgram;
	
	Matrix4 projection, mv, rotation;
	Vector3 position;
};
