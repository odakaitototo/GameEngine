///////////////////////////////////////////////////////////////////////////////////////////////////
// 
// SimpleVertexShader.hlsl
// 
// 役割：頂点シェーダー「形を決める場所」の命令書
// 製作開始日：2025/12/11
// 
// 作成日：2025/12/30
// 作業内容：#1
// 　　　追加：色データの受け渡しをできるようにする
//
// 作成日：2026/01/03
// 作業内容：#2
// 　　　追加：法線のデータの受け渡しをできるようにする
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
    
    float4 color : COLOR; // #1:CPUから色を受け取る
    
    float3 normal : NORMAL; // #2:法線データを受け取る
};


// 出力データ（ピクセルシェーダーに渡すデータ）の型を定義
struct VS_OUTPUT
{
    float4 pos : SV_Position; // スクリーン上の位置（必須）
    float4 color : COLOR; // #1:ピクセルシェーダーに色を足す
    float3 normal : NORMAL; // #2:ピクセルシェーダーに渡す
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
    
    output.color = input.color; // #1:受け取った色をそのまま次へ渡す
    
    //　#2:法線の計算
    // 物体が回転したら、法線も一緒に回転させる必要があります。
    // (float3x3)World は「ワールド行列の回転成分だけ」を取り出しています。
    float3 normal = normalize(mul(input.normal, (float3x3) World));
    output.normal = normal;
    
    return output;
}