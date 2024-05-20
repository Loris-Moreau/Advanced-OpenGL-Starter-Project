#include "TerrainTessellationScene.h"


TerrainTessellationScene::TerrainTessellationScene():
	dmapDepth(8.0f), isFogEnabled(true), isDisplacementEnabled(true),
	wireframe(false), paused(false), totalTime(0){}


void TerrainTessellationScene::LoadShaders() {
	m_vertexShader.LoadFrom("terrainTessVertex.shader", VERTEX);
	m_fragmentShader.LoadFrom("terrainTessFragment.shader", FRAGMENT);
	m_tessControlShader.LoadFrom("terrainTessControl.shader", TESSELLATION_CONTROL);
	m_tessEvalShader.LoadFrom("terrainTessEvaluation.shader", TESSELLATION_EVALUATION);

}

void TerrainTessellationScene::SetupScene() {
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
	Assets::loadTextureKtx("./Textures/terragen1.ktx", "terragen1");
	texDisplacement = Assets::getTextureKtx("terragen1").id;
	glBindTexture(GL_TEXTURE_2D, texDisplacement);

	glActiveTexture(GL_TEXTURE2);
	Assets::loadTextureKtx("./Textures/terragen_color.ktx", "terragen_color");
	texColor = Assets::getTextureKtx("terragen_color").id;
	glBindTexture(GL_TEXTURE_2D, texColor);

}

void TerrainTessellationScene::UpdateScene() {
	totalTime = (float)SDL_GetTicks() / 1000;
	t = totalTime * 0.03f;
	r = sinf(t * 5.37f) * 15.0f + 16.0f;
	h = cosf(t * 4.79f) * 2.0f + 10.2f;
	static const GLfloat bgColor[] = { 0.0f, 0.0f, 0.2f, 1.0f };
	static const GLfloat one = 1.0f;
	glClearBufferfv(GL_COLOR, 0, bgColor);
	glClearBufferfv(GL_DEPTH, 0, &one);

	view = Matrix4::createLookAt(Vector3(sinf(t) * r, h, cosf(t) * r), Vector3(0.0f,0.0f,0.0f), Vector3(0.0f, 1.0f, 0.0f));
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
	glDrawArraysInstanced(GL_PATCHES, 0, 4, 64 * 64);
}

void TerrainTessellationScene::HandleInputs(SDL_Event& e)
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



