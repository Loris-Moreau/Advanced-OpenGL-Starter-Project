#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <SDL.h>
#include "glew.h"

enum ShaderType
{
	VERTEX,
	FRAGMENT,
	TESSELLATION_CONTROL,
	TESSELLATION_EVALUATION,
	GEOMETRY
};

class Shader
{
protected:
	std::string m_code;
	unsigned int m_id;
	ShaderType m_type;
	void Load(std::string fileName);
	//Feel free to change the shader path  if needed
	const std::string SHADER_PATH = "./Shaders/";

public:
	Shader() { m_id = 0; m_type = VERTEX; };
	Shader(std::string fileName, ShaderType shaderType);
	~Shader();

	void LoadFrom(std::string fileName, ShaderType shaderType);

	int GetID() { return m_id; }

	//Shader operator=(Shader const& origin);

	std::string GetCode();
};

