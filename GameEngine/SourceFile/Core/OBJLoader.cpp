////////////////////////////////
// 
// OBJLoader.cpp
// 
// 役割：ファイルを読み込んで解析する場所
// 
// 作成開始日：2025/12/30
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

#include "Core/OBJLoader.h"
#include <fstream>
#include <sstream>

using namespace DirectX;
using namespace std;

bool OBJLoader::Load(const char* filename, std::vector<Vertex>& outVertices)
{
	ifstream file(filename);
	if (!file.is_open()) return false;

	std::vector<XMFLOAT3> positions;
	string line;

	while (getline(file, line))
	{
		stringstream ss(line);
		string prefix;
		ss >> prefix;

		if (prefix == "v")
		{
			float x, y, z;
			ss >> x >> y >> z;
			positions.push_back(XMFLOAT3(x, y, z));
		}
		else if (prefix == "f")
		{
			int v1, v2, v3;
			ss >> v1 >> v2 >> v3;

			// OBJファイルのインデックスは1始まりなので0始まりに直す
			v1--; v2--; v3--;

			// 範囲チェック（念のため）
			if (v1 < 0 || v1 >= (int)positions.size()) continue;

			// 色は白で統一
			Vertex v;
			v.r = 1.0f; v.g = 1.0f; v.b = 1.0f; v.a = 1.0f;

			// 三角形の3頂点を作成して追加
			v.x = positions[v1].x; v.y = positions[v1].y; v.z = positions[v1].z;
			outVertices.push_back(v);

			v.x = positions[v2].x; v.y = positions[v2].y; v.z = positions[v2].z;
			outVertices.push_back(v);

			v.x = positions[v3].x; v.y = positions[v3].y; v.z = positions[v3].z;
			outVertices.push_back(v);
		}
	}

	return !outVertices.empty();
}