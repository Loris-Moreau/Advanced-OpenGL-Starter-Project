#pragma once
#include "BaseScene.h"
#include "ScreensaverScene.h"
#include "TessellationModesScene.h"
#include "TerrainTessellationScene.h"

enum SceneID {
	BASE_SCENE,
	DEMO_SCENE,
	TESSELLATION,
	TESSELLATION_MODES,
	TERRAIN_TESSELLATION,
	MIP_MAP
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
		break;
	}
	default: {
		return nullptr;
	}
	}
}