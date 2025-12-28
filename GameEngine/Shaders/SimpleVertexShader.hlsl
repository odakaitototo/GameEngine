///////////////////////////////////////////////////////////////////////////////////////////////////
// 
// SimpleVertexShader.hlsl
// 
// 役割：頂点シェーダー「形を決める場所」の命令書
// 製作開始日：2025/12/11
// 
// 
// 
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////----- 単語帳 -----////////////////////////////
//
// ・
// 
// 
///////////////////////////////////////////////////////////////////


cbuffer ConstantBuffer : register(b0)
{
    matrix World;  // ワールド行列
    matrix View; // ビュー行列
    matrix Projection; // プロジェクション行列
}


// 入力データ（CPUから送られてくるデータ）の型を定義
struct VS_INPUT
{
    float4 pos : POSITION; // 頂点の位置（x,y,z,w）
    // 色や法線等はまだ使わないので、まずは位置だけ
};


// 出力データ（ピクセルシェーダーに渡すデータ）の型を定義
struct VS_OUTPUT
{
    float4 pos : SV_Position; // スクリーン上の位置（必須）
    
};

// メイン関数
// 役割：入ってきた頂点１つ1つに対して呼ばれる関数
VS_OUTPUT main(VS_INPUT input)
{

    VS_OUTPUT output;
    
    float4 pos = input.pos;
    
    pos = mul(pos, World); // 世界のどこにあるか
    pos = mul(pos, View); // カメラがどう見えるか
    pos = mul(pos, Projection); // 遠近感をつける
    
    output.pos = pos;
    
    return output;
}