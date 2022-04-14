#pragma region INCLUDE
#include "Platform.h"
#include "../SMB3.h"
#pragma endregion

void CPlatform::Load() {
}

void CPlatform::Start() {
}

void CPlatform::Update(float elapsedMs) {
}

void CPlatform::Render() {
}

int CPlatform::IsCollidable() {
	return 0;
}

int CPlatform::IsBlocking() {
	return 0;
}

void CPlatform::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
}
