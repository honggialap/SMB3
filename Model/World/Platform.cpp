#pragma region INCLUDE
#include "Platform.h"
#include "../SMB3.h"
#pragma endregion

void CPlatform::Load() {
	CGameObject::Load();

	/* Read file */
	pugi::xml_document prefab;
	prefab.load_file(_source.c_str());

	/* Stats */
	pugi::xml_node statsNode = prefab.child("Prefab").child("Stats");
	_solid = statsNode.attribute("solid").as_bool();

	/* Body */
	pugi::xml_node bodyNode = prefab.child("Prefab").child("Body");
	_renderBody = bodyNode.attribute("render").as_bool();
	BODY_WIDTH = bodyNode.attribute("width").as_float();
	BODY_HEIGHT = bodyNode.attribute("height").as_float();
	BODY_OFFSETX = bodyNode.attribute("offsetX").as_float();
	BODY_OFFSETY = bodyNode.attribute("offsetY").as_float();
}

void CPlatform::Start() {
	CGameObject::Start();
}

void CPlatform::Update(
	float elapsedMs
) {
	if (!_start) {
		Start();
	}
}

void CPlatform::Render() {
	if (_renderBody) {
		_sprites[SPR_BBOX]->Render(_x, _y);
	}
}

int CPlatform::IsCollidable() {
	return 1;
}

int CPlatform::IsBlocking() {
	if (_solid) {
		return 1;
	}
	else {
		return 0;
	}
}

void CPlatform::GetBoundingBox(
	float& left, 
	float& top, 
	float& right,
	float& bottom
) {
	left = _x + BODY_OFFSETX - (BODY_WIDTH / 2);
	right = _x + BODY_OFFSETX + (BODY_WIDTH / 2);
	top = _y + BODY_OFFSETY + (BODY_HEIGHT / 2);
	bottom = _y + BODY_OFFSETY - (BODY_HEIGHT / 2);
}
