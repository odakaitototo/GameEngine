////////////////////////////////
// 
// GeometryGenerator.h
// 
// 役割：頂点データを生成する専用の場所
// 
// 作成開始日：2026/02/10
// 
// 作成日：2026/02/10
// 
// 
// 
// 
// 
////////////////////////////////

////////////////////----- 単語帳 -----////////////////////////////////////////////////////////////////////////////////
//
// ・Vertex：頂点
// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <vector>
#include <DirectXMath.h>
#include "Components/Components.h" // Vertex構造体を使用するため

using namespace DirectX;

/***
* @brief 定型的な形状(ジオメトリ)を生成するクラス	
*/
class GeometoryGenerator {
public:
	/**
	* @brief 立方体の頂点データを生成します
	* @param size 一辺の長さ
	* @return 生成された頂点データのリスト
	*/
	static std::vector<Vertex> CreateCube(float size = 1.0f)
	{
		float h = size / 2.0f; //半径
        std::vector<Vertex> vertices =
        {
            // 前面 (Z = -h)
            { {-h,  h, -h} }, { { h,  h, -h} }, { { h, -h, -h} },
            { {-h,  h, -h} }, { { h, -h, -h} }, { {-h, -h, -h} },
            // 背面 (Z = h)
            { { h,  h,  h} }, { {-h,  h,  h} }, { {-h, -h,  h} },
            { { h,  h,  h} }, { {-h, -h,  h} }, { { h, -h,  h} },
            // 上面 (Y = h)
            { {-h,  h,  h} }, { { h,  h,  h} }, { { h,  h, -h} },
            { {-h,  h,  h} }, { { h,  h, -h} }, { {-h,  h, -h} },
            // 下面 (Y = -h)
            { {-h, -h, -h} }, { { h, -h, -h} }, { { h, -h,  h} },
            { {-h, -h, -h} }, { { h, -h,  h} }, { {-h, -h,  h} },
            // 左面 (X = -h)
            { {-h,  h,  h} }, { {-h,  h, -h} }, { {-h, -h, -h} },
            { {-h,  h,  h} }, { {-h, -h, -h} }, { {-h, -h,  h} },
            // 右面 (X = h)
            { { h,  h, -h} }, { { h,  h,  h} }, { { h, -h,  h} },
            { { h,  h, -h} }, { { h, -h,  h} }, { { h, -h, -h} },

        };
        return vertices;
	}

};