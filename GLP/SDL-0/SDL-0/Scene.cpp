#include "Scene.h"
#include <SDL.h>
#include "glew.h"

Scene::Scene(std::string nameP)
{
	m_name = nameP; 
	glGenBuffers(1, &m_vbo);
}

Scene::~Scene()
{
	glDeleteVertexArrays(1, &m_vao); 
}
