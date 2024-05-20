#pragma once
#include "TessellationScene.h"
#include <Matrix4.h>
#include "Assets.h"
class BeachScene :public TessellationScene
{
public:
	BeachScene();
	~BeachScene(){}
	void SetupScene() override;
	void UpdateScene() override;
	void HandleInputs(SDL_Event& e) override;

private:
	void LoadShaders() override;
	void CreateShaderPrograms() override;

	// Uniforms
	Matrix4 mvp;
	Matrix4 view;
	Matrix4 proj;
	float dmapDepth;
	bool isFogEnabled;

	GLuint texDisplacement;
	GLuint beachColor;
	GLuint waterColor;
	bool isDisplacementEnabled;
	bool wireframe;
	bool paused;

	float totalTime;
	float t, r, h;

	Shader m_waterVertexShader, m_waterFragmentShader, m_waterControlShader, m_waterEvalShader;
	ShaderProgram m_waterShaderProgram;
};