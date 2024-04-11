#include "Window.h"
#include <iostream>

Vector2 Window::Dimension = Vector2(0,0);

Window::Window(float width, float height, Color colorP)
{
	/////////SETTING UP SDL///
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cout << "SDL initialization failed. SDL Error: " << SDL_GetError();
	}
	else
	{
		std::cout << "SDL initialization succeeded!";
	}

	//Create a simple window
	Dimension = Vector2(width, height);
	unsigned int center = SDL_WINDOWPOS_CENTERED;
	m_window = SDL_CreateWindow("OpenGl Scene", center, center, (int)Dimension.x, (int)Dimension.y, SDL_WINDOW_OPENGL);

	//Create an OpenGL compatible context to let glew draw on it
	m_context = SDL_GL_CreateContext(m_window);

	/////////SETTING UP OPENGL WITH GLEW///
	//Initialize glew
	glewExperimental = GL_TRUE;
	if (glewInit() == GLEW_OK) {
		std::cout << "Glew initialized successfully\n";
	}

	//Set the viewing frame through which we will see the objects
	glViewport(0, 0, (int)width, (int)height);

	//Use depth management
	glEnable(GL_DEPTH_TEST);

	//0 is our origin, the higher the z, the farther the object
	glDepthFunc(GL_LESS);
	ChangeBackgroundColor(colorP);
	PaintBackground();
}

void Window::PaintBackground()
{
	glClearColor(m_backgroundColor.r, m_backgroundColor.g, m_backgroundColor.b, m_backgroundColor.a);
}

void Window::ChangeBackgroundColor(Color color)
{
	m_backgroundColor = color;
}


void Window::ChangeBackgroundColor(float rP, float gP, float bP, float aP)
{
	m_backgroundColor = Color(rP, gP, bP, aP);
}

void Window::Update()
{
	SDL_GL_SwapWindow(m_window); // Swapbuffer
}

void Window::Close()
{
	SDL_DestroyWindow(m_window);
	SDL_GL_DeleteContext(m_context);
}
