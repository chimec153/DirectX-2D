#include "Setting.h"
#include "Input.h"

bool Setting()
{
	GET_SINGLE(CInput)->AddAxisKey("MoveSide", DIK_A, -1.f);
	GET_SINGLE(CInput)->AddAxisKey("MoveSide", DIK_D, 1.f);
	GET_SINGLE(CInput)->AddAxisKey("MoveVertical", DIK_W, 1.f);
	GET_SINGLE(CInput)->AddAxisKey("MoveVertical", DIK_S, -1.f);
	GET_SINGLE(CInput)->AddAxisKey("Rotate", DIK_Q, 1.f);
	GET_SINGLE(CInput)->AddAxisKey("Rotate", DIK_E, -1.f);
	GET_SINGLE(CInput)->AddAxisKey("FireCon", DIK_Z, 1.f);

	GET_SINGLE(CInput)->AddActionKey("fire", DIK_SPACE);
	GET_SINGLE(CInput)->AddActionKey("fire2", DIK_F1);
	GET_SINGLE(CInput)->AddActionKey("fire3", DIK_F2);
	GET_SINGLE(CInput)->AddActionKey("Color", DIK_LCONTROL);
	GET_SINGLE(CInput)->AddActionKey("X", DIK_X);

	return true;
}