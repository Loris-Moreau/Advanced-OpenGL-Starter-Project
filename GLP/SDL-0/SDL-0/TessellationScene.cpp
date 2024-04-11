#include "TessellationScene.h"

using namespace std;

const bool ENABLE_GEOMETRY_SHADER = false;
const bool IsQuad = true;

TessellationScene::TessellationScene() {}

//Load the correct shaders from your files
void TessellationScene::LoadShaders()
{
	if(!IsQuad) m_vertexShader.LoadFrom("tessellationVertex.shader", VERTEX);
	if(IsQuad) m_vertexShader.LoadFrom("QuadTessellationVertex.shader", VERTEX);
	
	m_fragmentShader.LoadFrom("tessellationFragment.shader", FRAGMENT);
	
	if(!IsQuad) m_tessControlShader.LoadFrom("tessellationControl.shader", TESSELLATION_CONTROL);
	if(IsQuad) m_tessControlShader.LoadFrom("QuadTessellationControl.shader", TESSELLATION_CONTROL);
	
	 if(!IsQuad) m_tessEvalShader.LoadFrom("tessellationEval.shader", TESSELLATION_EVALUATION);
	 if(IsQuad) m_tessEvalShader.LoadFrom("QuadTessellationEval.shader", TESSELLATION_EVALUATION);
	
	if(ENABLE_GEOMETRY_SHADER) m_geometryShader.LoadFrom("tessellationGeometry.shader", GEOMETRY);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void TessellationScene::CreateShaderPrograms()
{
	m_shaderProgram.Compose
	(
		vector<Shader*>
		{
		& m_vertexShader, 
		& m_fragmentShader,
		& m_tessControlShader,
		& m_tessEvalShader,
		& m_geometryShader
		}
	);
}

void TessellationScene::VerticeInformationSlicer()
{
	//No slicing necessary, everything is handled in shaders
}

void TessellationScene::SetupScene()
{
	LoadShaders();
	CreateShaderPrograms();
	
	//Create one ID to be given at object generation and store it in m_vao via reference
	glGenVertexArrays(1, &m_vao);
	//Binding the VAO 
	//   /!\ If you have multiple VAOs, you should bind the correct VAO after each change
	glBindVertexArray(m_vao);

	//Binds the buffer linked to this ID to the vertex array buffer to be rendered. Put 0 instead of vbo to reset the value.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	
	//VerticeInformationSlicer();
}

void TessellationScene::UpdateScene()
{
	float timeValue = static_cast<float>(SDL_GetTicks()) / 500;
	float sinusoidValue = 2 * (sin(timeValue));
	
	float timeValue2 = static_cast<float>(SDL_GetTicks()) / 1000;
	float sinusoidValue2 = 2 * (sin(timeValue2));
	
	int outerVertexRatio = glGetUniformLocation(m_shaderProgram.GetID(), "outerRatio");
	glUniform1f(outerVertexRatio, 2 * (sinusoidValue + 4));
	
	int innerVertexRatio = glGetUniformLocation(m_shaderProgram.GetID(), "innerRatio");
	glUniform1f(innerVertexRatio, 2 * (sinusoidValue2 + 4));
	
	m_shaderProgram.Use();

	glPointSize(5.0f);
	if(IsQuad)glPatchParameteri(GL_PATCH_VERTICES, 4);
	glDrawArrays(GL_PATCHES, 0, 4);
}
