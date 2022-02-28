#pragma region INCLUDE
#include "Player.h"
#include "Ultilities.h"
#pragma endregion

namespace Engine
{

	void Player::KeyState()
	{
		_previousButtonState = _currentButtonState;
	}

	void Player::OnKeyUp(int keyCode)
	{
		if (_currentButtonState.find(keyCode) != _currentButtonState.end())
		{
			_currentButtonState[keyCode] = false;
		}
	}

	void Player::OnKeyDown(int keyCode)
	{
		if (_currentButtonState.find(keyCode) != _currentButtonState.end())
		{
			_currentButtonState[keyCode] = true;
		}
	}

	void Player::BindKey(int keyCode)
	{
		_previousButtonState[keyCode] = false;
		_currentButtonState[keyCode] = false;
	}

	bool Player::IsKeyDown(int keyCode)
	{
		return _currentButtonState[keyCode];
	}

	bool Player::IsKeyPressed(int keyCode)
	{
		return
			!_previousButtonState[keyCode]
			&& _currentButtonState[keyCode];
	}

	bool Player::IsKeyReleased(int keyCode)
	{
		return
			_previousButtonState[keyCode]
			&& !_currentButtonState[keyCode];
	}

}