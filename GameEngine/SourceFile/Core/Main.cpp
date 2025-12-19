////////////////////////////////
// 
// Main.cpp
// 製作開始日：2025/12/07
// 
// 製作日：2025/12/07
// 作業内容：#1
// 　　　追加：Begin(画面クリア)とEnd(表示)を繰り返して描画するようにした。
// 
// 製作日：2025/12/14
// 作業内容：#2
// 　　　追加：#2:三角形描画処理の追加
// 
// 
// 
// 
////////////////////////////////




#include <Windows.h>
#include "Core/Window.h"
#include "Core/Main.h"
#include "Graphics/DX11Device.h" // 描画するためのもの

/* #2:COMオブジェクトの安全な解放用マクロ
　（Releaseし忘れを防ぐため、あれば便利）*/
template<typename T>
void SafeRelease(T*& ptr)
{
	if (ptr)
	{
		ptr->Release();
		ptr = nullptr;
	}
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Window window;
	DX11Device dx11; // #1

	// 初期化 (ウィンドウ作成)
	if (!window.Init(hInstance, SCREEEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE))
	{
		return -1;
	}

	// #1：DirectX初期化（ウィンドウのハンドルを渡す）
	if (!dx11.Init(window.GetHandle(), SCREEEN_WIDTH, SCREEN_HEIGHT))
	{
		MessageBox(NULL, L"DirectX Init Failed", L"Error", MB_OK);
		return -1;
	}

	// #2:ここから描画リソースの準備（Init）

	// #2:シェーダーの読み込み
	ID3D11VertexShader* vertexShader = nullptr;
	ID3DBlob* vsBlob = nullptr; // #2:InputLayout作成に必要
	if (!dx11.CreateVertexShader(L"SimpleVertexShader.cso", &vertexShader, &vsBlob))
	{
		return -1;
	}

	ID3D11PixelShader* pixelShader = nullptr;
	if (!dx11.CreatePixelShader(L"SimplePixelShader.cso", &pixelShader))
	{
		return -1;
	}

	/* #2:頂点データの定義（三角形）
	   画面画像系は中心が（0,0）,上が+Y、右が+X */
	Vertex vertices[] = {
		{ 0.0f, 0.5f, 0.0f}, // #2:上
		{0.5f, -0.5f, 0.0f}, // #2:右下
		{-0.5f, -0.5f, 0.0f}, // #2:左下


	};

	// #2:頂点バッファの作成
	ID3D11Buffer* vertexBuffer = nullptr;
	if (!dx11.CreateVertexBuffer(vertices, sizeof(vertices), &vertexBuffer))
	{
		return -1;
	}

	// #2:InputLayout（データの読み方）の作成
	ID3D11InputLayout* inputLayout = nullptr;
	if (!dx11.CreateInputLayout(vsBlob, &inputLayout))
	{
		return -1;
	}
	// #2:もうBlobは不要なので解放
	SafeRelease(vsBlob);
	




	// メインループ
	while (window.ProcessMessage())
	{

		// #1：描画開始（画面を濃い青色でクリア）
		dx11.Begin(0.1f, 0.2f, 0.1f); // ウィンドウの色設定

		/* #2:ここから描画命令（Render）
		   GPUは「状態」を持っているので、毎回セットしてから描くのが作法*/

		ID3D11DeviceContext* context = dx11.GetContext();

		/* #2:Input Assembler (IA)ステージの設定
		   "どんなデータを入力するか？"*/

		/* #2:InputLayoutをセット
		   「これから送るデータは、さっき決めたレイアウト（位置だけ）通りだよ」*/
		context->IASetInputLayout(inputLayout);

		/* #2:InputLayoutをセット
		   「このバッファに入っているデータを使ってね」*/
		UINT stride = sizeof(Vertex); // #2:1頂点あたりのサイズ
		UINT offset = 0; // #2:バッファの先頭から使う
		context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

		/* #2:トポロジー（形状）のセット
		  「点は3つずつセットで「三角形リスト」として扱う」*/
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		/* #2:Shader ステージの設定
		   どのシェーダプログラムを使うのか？*/

		// #2:頂点シェーダーをセット
		context->VSSetShader(vertexShader, nullptr, 0);

		// #2:ピクセルシェーダーをセット
		context->PSSetShader(pixelShader, nullptr, 0);

		/* #2:描画コマンド発行（Draw Call）
		   Draw(頂点数、開始位置)
		   「3つの頂点を使って描いて」 -> これで画面に絵が出る*/
		context->Draw(3, 0);



		// ここに将来以下の処理が入ります
		// Game.Update();
		// Game.Render();

		// #1：描画終了（画面を更新）
		dx11.End();
	}

	/*#2:終了処理（Release）
	作った順と逆順で解放するのが良い*/
	SafeRelease(inputLayout);
	SafeRelease(vertexBuffer);
	SafeRelease(pixelShader);
	SafeRelease(vertexShader);

	return 0;
}