#pragma once
#include "BaseScene.h"
#include "ScreensaverScene.h"
#include "CubeScene.h"
#include "CubeTessellationScene.h"
#include "TessellationModesScene.h"
#include "TerrainTessellationScene.h"
#include "TextureScene.h"
#include "BeachScene.h"

enum SceneID {
	BASE_SCENE,
	DEMO_SCENE,
	TESSELLATION,
	TESSELLATION_MODES,
	TERRAIN_TESSELLATION,
	MIP_MAP,
	CUBES,
	CUBES_TESSELLATION,
	BEACH_SCENE
};

Scene* SceneSelector(SceneID id) {
	switch (id) {
	case BASE_SCENE: {
		return new BaseScene();
		break;
	}	
	case DEMO_SCENE: {
		return new ScreensaverScene();
		break;
	}
	case TESSELLATION: {
		return new TessellationScene();
		break;
	}
	case TESSELLATION_MODES: {
		return new TessellationModesScene();
		break;
	}
	case TERRAIN_TESSELLATION: {
		return new TerrainTessellationScene();
		break;
	}
	case MIP_MAP: {
		return new TextureScene();
		break;
	}
	case CUBES: {
		return new CubeScene();
		break;
	}
	case CUBES_TESSELLATION: {
		return new CubeTessellationScene();
		break;
	}
	case BEACH_SCENE: {
		return new BeachScene();
		break;
	}
	default: {
		return nullptr;
	}
	}
}