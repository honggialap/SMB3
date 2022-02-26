#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__

#pragma region INCLUDE
#include "Input.h"
#include <map>
#pragma endregion

namespace Engine
{

	class Player : public KeyHandler
	{
	private:

	public:
		virtual void KeyState();
		virtual void OnKeyUp(int keyCode);
		virtual void OnKeyDown(int keyCode);
	};
	typedef Player* pPlayer;

}

#endif // !__PLAYER_H__
