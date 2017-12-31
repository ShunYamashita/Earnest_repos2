float4x4	worldMatrix;					//  ���[���h�s��
float4x4	viewMatrix;						//  �r���[�s��
float4x4	projectionMatrix;				//  �v���W�F�N�V�����s��
float4x4	worldInverseTransposeMatrix;	//  ���[���h�ϊ��s��̋t�s��̓]�u�s��

struct IN_VERTEX
{
	float3 localPosition : POSITION0;
	float3 normal : NORMAL0;
	float4 color : COLOR0;
	float2 texcoord : TEXCOORD0;
};

struct OUT_VERTEX
{
	float4 localPosition : POSITION0;		//  worldViewProjectionMatrix���K�v
	float2 texcoord : TEXCOORD0;			//  texcoord
	float3 worldNormal : TEXCOORD1;			//  worldInverseTransposeMatrix���K�v
	float3 worldPosition : TEXCOORD2;		//  mtxWorld���K�v
};

OUT_VERTEX main( IN_VERTEX input )
{
	OUT_VERTEX output = ( OUT_VERTEX )0;

	output.localPosition = mul( float4( input.localPosition , 1.0f ) , worldMatrix );
	output.localPosition = mul( output.localPosition , viewMatrix );
	output.localPosition = mul( output.localPosition , projectionMatrix );
	output.texcoord = input.texcoord;
	output.worldNormal = mul( float4( input.normal , 0.0f ) , worldInverseTransposeMatrix ).xyz;
	output.worldPosition = mul( float4( input.localPosition , 1.0f ) , worldMatrix ).xyz;

	return output;
}