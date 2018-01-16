sampler		samplerTexture;
sampler		samplerShadow01;
sampler		samplerShadow02;
sampler		samplerShadow03;
sampler		samplerShadow04;
float4		offset;
float		bias;

struct IN_VERTEX
{
 	float4	color : COLOR0;
 	float2	texcoord : TEXCOORD0;
 	float4	positionLight : TEXCOORD1;
	float4	shadowCoord01 : TEXCOORD2;
	float4	shadowCoord02 : TEXCOORD3;
	float4	shadowCoord03 : TEXCOORD4;
	float4	shadowCoord04 : TEXCOORD5;
	float4	splitPosition : TEXCOORD6;
	float	depth : TEXCOORD7;
};

float4 main( IN_VERTEX input ) : COLOR0
{
	// シャドウマップから値の取得
	float2 shadowUV = 0.5f * input.positionLight.xy / input.positionLight.w + float2( 0.5f , 0.5f );
	shadowUV.y = 1.0f - shadowUV.y;
	shadowUV.x = shadowUV.x + offset.x;
	shadowUV.y = shadowUV.y + offset.y;
	float shadowValue =  tex2D( samplerShadow , shadowUV ).r;

	float4 returnColor = tex2D( samplerTexture , input.texcoord ) * input.color;

    float sdwThreshold = 1.0f;      // シャドウにするかどうかの閾値です.
    float sdwBias      = 0.01f;     // シャドウバイアスです.
    float sdwColor     = 0.1f;      // シャドウのカラーです.

    if ( input.depth < input.SplitPos.x )
    {
        float2 coord = input.SdwCoord01.xy / input.SdwCoord01.w;
        float  depth = input.SdwCoord01.z  / input.SdwCoord01.w;
        sdwThreshold = samplerShadow01.SampleCmpLevelZero( ShadowSmp, coord, depth - sdwBias );
        sdwThreshold = saturate( sdwThreshold + sdwColor );
    }
    else if ( input.depth < input.SplitPos.y )
    {
        float2 coord = input.SdwCoord02.xy / input.SdwCoord02.w;
        float  depth = input.SdwCoord02.z  / input.SdwCoord02.w;
        sdwThreshold = samplerShadow02.SampleCmpLevelZero( ShadowSmp, coord, depth - sdwBias );
        sdwThreshold = saturate( sdwThreshold + sdwColor );
    }
    else if ( input.depth < input.SplitPos.z )
    {
        float2 coord = input.SdwCoord03.xy / input.SdwCoord03.w;
        float  depth = input.SdwCoord03.z  / input.SdwCoord03.w;
        sdwThreshold = samplerShadow03.SampleCmpLevelZero( ShadowSmp, coord, depth - sdwBias );
        sdwThreshold = saturate( sdwThreshold + sdwColor );
    }
    else
    {
        float2 coord = input.SdwCoord04.xy / input.SdwCoord04.w;
        float  depth = input.SdwCoord04.z  / input.SdwCoord04.w;
        sdwThreshold = samplerShadow04.SampleCmpLevelZero( ShadowSmp, coord, depth - sdwBias );
        sdwThreshold = saturate( sdwThreshold + sdwColor );
    }

	return returnColor;
}