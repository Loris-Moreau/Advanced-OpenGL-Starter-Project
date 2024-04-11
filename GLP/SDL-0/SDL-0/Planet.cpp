#include "Planet.h"
#include "MeshObject.h"
#include "Window.h"

Planet::Planet()
{}

void Planet::LoadShaders()
{
	
	m_vertexShader.LoadFrom("PlanetVertex.vert", VERTEX);
	m_fragmentShader.LoadFrom("PlanetFrag.frag", FRAGMENT);
	
	m_tessControl.LoadFrom("PlanetTessControl.tesc", TESSELLATION_CONTROL);
	m_tessEval.LoadFrom("PlanetTessEval.tese", TESSELLATION_EVALUATION);
}

void Planet::CreateShaderPrograms()
{
	m_Program.Compose
	(
		std::vector<Shader*>
		{
			& m_fragmentShader, 
			& m_vertexShader
		}
	);
	
	m_TessProgram.Compose
	(
		std::vector<Shader*>
		{
			& m_vertexShader, 
			& m_tessControl, 
			& m_tessEval, 
			& m_fragmentShader
		}
	);
}

void Planet::VerticeInformationSlicer()
{
	float* verticeArray = new float[m_vertices.size()]();
	std::copy(m_vertices.begin(), m_vertices.end(), verticeArray);

	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), verticeArray, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	delete [] verticeArray;
}


void Planet::SetupScene()
{
	LoadShaders();
	CreateShaderPrograms();
	
	
	// Create one ID to be given at object generation and store it in m_vao via reference
	glGenVertexArrays(1, &m_vao);
	// Binding the VAO 
	// /!\ If you have multiple VAOs, you should bind the correct VAO after each change
	glBindVertexArray(m_vao);

	// Binds the buffer linked to this ID to the vertex array buffer to be rendered. Put 0 instead of vbo to reset the value.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	VerticeInformationSlicer();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}

void Planet::UpdateScene()
{
	float timeValue = static_cast<float>(SDL_GetTicks()) / 1000;
	float sinusoidValue = (sin(timeValue)) / 2.0f;
	
	position.y = sinusoidValue / 2;

	mv = Matrix4::createTranslation(position - Vector3(0.5f,0.0f,0.0f));
	Matrix4 rotationX = Matrix4::createRotationX(0.01f );
	Matrix4 rotationY = Matrix4::createRotationY(0.005f );
	Matrix4 rotationZ = Matrix4::createRotationY(0.01f );
	rotation *= rotationX;
	rotation *= rotationY;
	rotation *= rotationZ;
	mv = mv * rotation;


	mv = Matrix4::createTranslation(position + Vector3(0.5f,0,0));
	mv = mv * rotation;

	glPointSize(5.0f);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_PATCHES, 0, 12);
}
