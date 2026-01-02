////////////////////////////////
// 
// Input.cpp
// 
// 役割：入力を管理する場所
// 
// 作成開始日：2025/12/29
// 
// 作成日：2025/
// 
// 
// 
// 
// 
////////////////////////////////

////////////////////----- 単語帳 -----////////////////////////////////////////////////////////////////////////////////
//
// 
// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Core/Input.h"

#include <Windows.h> // GetKystate等に必要

void Input::Init()
{
	// 配列の中身を全て　false　（押されていない）にリセット
	for (int i = 0; i < KEY_NUM; ++i)
	{
		m_currentKeys[i] = false;
		m_prevKeys[i] = false;
	}
}

void Input::Update()
{
	// 全てのキーについて調べる
	for (int i = 0; i < KEY_NUM; ++i)
	{
		// 現在の状態を「過去の状態」として保存
		m_prevKeys[i] = m_currentKeys[i];

		// Windowsから現在のキー状態を取得
		// (0x8000は「現在押されている」ビットグラフ)
		if (GetAsyncKeyState(i) & 0x8000)
		{
			m_currentKeys[i] = true;
		}
		else
		{
			m_currentKeys[i] = false;
		}
	}
}

bool Input::GetKey(int keycode)
{
	return m_currentKeys[keycode];
}

bool Input::GetKyDown(int keycode)
{
	// 「今押されている」かつ「前は押されていなかった」場合 = 押された瞬間
	return m_currentKeys[keycode] && !m_prevKeys[keycode];
}

bool Input::GetKeyUp(int keycode)
{
	return !m_currentKeys[keycode] && m_prevKeys[keycode];
}