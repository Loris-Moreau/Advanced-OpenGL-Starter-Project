#include "BeachScene.h"

BeachScene::BeachScene():
	dmapDepth(8.0f), isFogEnabled(true), isDisplacementEnabled(true),
	wireframe(false), paused(false), totalTime(0){}


void BeachScene::LoadShaders() {
	m_vertexShader.LoadFrom("beachVertex.shader", VERTEX);
	m_fragmentShader.LoadFrom("beachFragment.shader", FRAGMENT);
	m_tessControlShader.LoadFrom("beachControl.shader", TESSELLATION_CONTROL);
	m_tessEvalShader.LoadFrom("beachEvaluation.shader", TESSELLATION_EVALUATION);

	m_waterVertexShader.LoadFrom("waterVertex.shader", VERTEX);
	m_waterFragmentShader.LoadFrom("waterFragment.shader", FRAGMENT);
	m_waterControlShader.LoadFrom("waterControl.shader", TESSELLATION_CONTROL);
	m_waterEvalShader.LoadFrom("waterEvaluation.shader", TESSELLATION_EVALUATION);
}

void BeachScene::CreateShaderPrograms() {
	m_shaderProgram.Compose(
		vector<Shader*>{
		& m_vertexShader,
		& m_fragmentShader,
		& m_tessControlShader,
		& m_tessEvalShader
	});

	m_waterShaderProgram.Compose(
		vector<Shader*>{
		& m_waterVertexShader,
		& m_waterFragmentShader,
		& m_waterControlShader,
		& m_waterEvalShader
	});
}

void BeachScene::SetupScene() {
	LoadShaders();
	CreateShaderPrograms();

	//Create one ID to be given at object generation and store it in m_vao via reference
	glGenVertexArrays(1, &m_vao);
	//Binding the VAO 
	//   /!\ If you have multiple VAOs, you should bind the correct VAO after each change
	glBindVertexArray(m_vao);

	//Binds the buffer linked to this ID to the vertex array buffer to be rendered. Put 0 instead of vbo to reset the value.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glPatchParameteri(GL_PATCH_VERTICES, 4);

	glEnable(GL_CULL_FACE);
	//TEXTURES
	glActiveTexture(GL_TEXTURE1);
	Assets::loadTexture("./Textures/sand_height.png", "terragen1");
	texDisplacement = Assets::getTexture("terragen1").id;
	glBindTexture(GL_TEXTURE_2D, texDisplacement);

	glActiveTexture(GL_TEXTURE2);
	Assets::loadTexture("./Textures/sand.png", "terragen_color");
	beachColor = Assets::getTexture("terragen_color").id;
	glBindTexture(GL_TEXTURE_2D, beachColor);

	glActiveTexture(GL_TEXTURE3);
	Assets::loadTexture("./Textures/water.jpeg", "water_color");
	waterColor = Assets::getTexture("water_color").id;
	glBindTexture(GL_TEXTURE_2D, waterColor);
}

void BeachScene::UpdateScene() {
	totalTime = (float)SDL_GetTicks() / 1000;
	t = totalTime * 0.3f;
	r = 70.0F;
	h = 50.F;
	static const GLfloat bgColor[] = { 0.0f, 0.0f, 0.2f, 1.0f };
	static const GLfloat one = 1.0f;
	glClearBufferfv(GL_COLOR, 0, bgColor);
	glClearBufferfv(GL_DEPTH, 0, &one);

	view = Matrix4::createLookAt(Vector3(sinf(t) * r, h, cosf(t) * r), Vector3(0.0f,20.0f,20.0f), Vector3(0.0f, 1.0f, 0.0f));
	proj = Matrix4::createPerspectiveFOV(45.0f, Window::Dimension.x, Window::Dimension.y, 0.1f, 1000.0f);

	m_shaderProgram.Use();
	m_shaderProgram.setMatrix4("mv_matrix", view);
	m_shaderProgram.setMatrix4("proj_matrix", proj);
	m_shaderProgram.setMatrix4("mvp_matrix", proj * view);
	m_shaderProgram.setFloat("dmap_depth", isDisplacementEnabled ? dmapDepth : 0.0f);
	m_shaderProgram.setFloat("enable_fog", isFogEnabled ? 1.0f : 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArraysInstanced(GL_PATCHES, 0, 4, 64*64);

	float sinAnim = sin(t*16) * 2;
	
	m_waterShaderProgram.Use();
	m_waterShaderProgram.setMatrix4("mv_matrix", view);
	m_waterShaderProgram.setMatrix4("proj_matrix", proj);
	m_waterShaderProgram.setMatrix4("mvp_matrix", proj * view);
	m_waterShaderProgram.setFloat("dmap_depth", isDisplacementEnabled ? dmapDepth : 0.0f);
	m_waterShaderProgram.setFloat("enable_fog", isFogEnabled ? 1.0f : 0.0f);
	m_waterShaderProgram.setFloat("time", totalTime);
	m_waterShaderProgram.setFloat("sin", sinAnim);

	float minHeight = 5;
	float maxHeight = dmapDepth;

	m_waterShaderProgram.setFloat("minHeight", minHeight);
	m_waterShaderProgram.setFloat("maxHeight", maxHeight);


	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArraysInstanced(GL_PATCHES, 0, 4, 64 * 64);
}

void BeachScene::HandleInputs(SDL_Event& e)
{
	switch (e.type)
	{
	case SDL_KEYDOWN:
		if(e.key.keysym.sym == SDLK_w)
		{
			//press W to switch with wireframe view
			wireframe = !wireframe;
		}
	}
}



