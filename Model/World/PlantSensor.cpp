#pragma region INCLUDE
#include "PlantSensor.h"
#include "../SMB3.h"
#include "Mario.h"
#pragma endregion

void CPlantSensor::Load() {
	CGameObject::Load();

	/* Read file */
	pugi::xml_document prefabDoc;
	prefabDoc.load_file(_source.c_str());

	/* Body */
	auto bodyNode = prefabDoc.child("Prefab").child("Body");
	_renderBody = bodyNode.attribute("render").as_bool();
	BODY_WIDTH = bodyNode.attribute("width").as_float();
	BODY_HEIGHT = bodyNode.attribute("height").as_float();
	BODY_OFFSETX = bodyNode.attribute("offsetX").as_float();
	BODY_OFFSETY = bodyNode.attribute("offsetY").as_float();
}

void CPlantSensor::Start() {
	CGameObject::Start();
}

void CPlantSensor::Update(float elapsedMs) {
	if (!_start) {
		Start();
	}

	_triggered = false;

	std::vector<pGameObject> collidables = _game->GetLocal(_ID);
	_collider->Process(elapsedMs, &collidables);
}

void CPlantSensor::Render() {
	if (_renderBody) {
		_sprites[SPR_BBOX]->Render(_x, _y);
	}
}

int CPlantSensor::IsCollidable() {
	return 1;
}

int CPlantSensor::IsBlocking() {
	return 0;
}

void CPlantSensor::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = _x + BODY_OFFSETX - (BODY_WIDTH / 2);
	right = _x + BODY_OFFSETX + (BODY_WIDTH / 2);
	top = _y + BODY_OFFSETY + (BODY_HEIGHT / 2);
	bottom = _y + BODY_OFFSETY - (BODY_HEIGHT / 2);
}

void CPlantSensor::OnNoCollision(float elapsedMs) {
}

void CPlantSensor::OnCollisionWith(pCollision collision) {
	if (dynamic_cast<pMario>(collision->_target))
		OnCollisionWithMario(collision);
}

void CPlantSensor::OnCollisionWithMario(pCollision collision) {
	_triggered = true;
}
