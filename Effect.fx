//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ��� ���� ����
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cbuffer cbViewProjectionMatrix : register(b0)
{
	matrix		gmtxView : packoffset(c0);
	matrix		gmtxProjection : packoffset(c4);
};

cbuffer cbWorldMatrix : register(b1)
{
	matrix		gmtxWorld : packoffset(c0);
};

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float4 position : POSITION;
	float4 color : COLOR;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR0;
};

// ����-���̴� 
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.position = mul(input.position, gmtxWorld);
	output.position = mul(output.position, gmtxView);
	output.position = mul(output.position, gmtxProjection);
	output.color = input.color;
	//�ԷµǴ� ������ ������ �״�� ����Ѵ�. 
	return output;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ���� ���̴�
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
float4 VS(float4 position : POSITION) : SV_POSITION
{
	float4 output = (float4)0;
	output = mul(position, gmtxWorld);
	output = mul(output, gmtxView);
	output = mul(output, gmtxProjection);
	return output;
}

VS_OUTPUT VS1(float4 position : POSITION)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.position = mul(position, gmtxWorld);
	output.position = mul(output.position, gmtxView);
	output.position = mul(output.position, gmtxProjection);
	return output;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// �ȼ� ���̴�
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

float4 PS(VS_OUTPUT input) : SV_Target
{
	return input.color;
	//�ԷµǴ� ������ ������ �״�� ����Ѵ�. 
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// �ν��Ͻ��� ���� ���̴� ����
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 
struct VS_INSTANCED_COLOR_INPUT
{
	float3 position : POSITION;
	float4 color : COLOR0;
	column_major float4x4 mtxTransform : INSTANCEPOS;
	float4 instanceColor : INSTANCECOLOR;
};
struct VS_INSTANCED_COLOR_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR0;
	//�ý��� ���� ������ ���� ���̴��� ���޵Ǵ� ��ü �ν��Ͻ��� ID�� �ȼ� ���̴��� �����Ѵ�.
	float4 instanceID : INDEX;
};

VS_INSTANCED_COLOR_OUTPUT VSInstancedDiffusedColor(VS_INSTANCED_COLOR_INPUT input, uint instanceID : SV_InstanceID)
{
	VS_INSTANCED_COLOR_OUTPUT output = (VS_INSTANCED_COLOR_OUTPUT)0;
	output.position = mul(float4(input.position, 1), input.mtxTransform);
	output.position = mul(output.position, gmtxView);
	output.position = mul(output.position, gmtxProjection);

	output.color = input.color;
//	output.color = (instanceID % 5) ? input.color : (input.color * 0.3f + input.instanceColor * 0.7f);
	output.instanceID = instanceID;
	return output;
}

float4 PSInstancedDiffusedColor(VS_INSTANCED_COLOR_OUTPUT input) : SV_Target
{
	return input.color;
}