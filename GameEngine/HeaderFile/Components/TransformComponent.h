

#pragma once

#include <DirectXMath.h>

using namespace DirectX;

struct TransformComponent
{
	// DirectXMathの型を使う理由：GPUはベクトル計算が得意で、DirectXMathはその計算に最適化された型だから

	XMFLOAT3 position; // 位置（ｘ、ｙ、ｚ）
	XMFLOAT3 rotation; // 回転（ｘ、ｙ、ｚ）ーラジアン単位
	XMFLOAT3 scale; // 拡大縮小（ｘ、ｙ、ｚ）サイズ

	// コンストラクタで初期値を設定しておくと便利ー＞疑問
	TransformComponent()
	{
		position = XMFLOAT3(0.0f, 0.0f, 0.0f);
		rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
		scale = XMFLOAT3(1.0, 1.0, 1.0);

	}
};