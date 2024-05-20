#pragma once
#include <string>
#include <vector>
#include "Shader.h"
#include "ShaderProgram.h"

class Scene
{
protected:
	std::string m_name;
	unsigned int m_vbo, m_vao;
	std::vector<float> m_vertices;
	virtual void LoadShaders() = 0;
	virtual void CreateShaderPrograms() = 0;
	//Splits informations of the array into attributes
	virtual void VerticeInformationSlicer() = 0;

public:
	Scene(std::string nameP = "Demo");
	~Scene();
	
	virtual void SetupScene() = 0;
	virtual void UpdateScene() = 0;
	virtual void HandleInputs(SDL_Event& e){}
};