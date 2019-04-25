/*!
 *@brief	�ǂ��ӂ�
 */

/*!
 *@brief	���_�V�F�[�_�[�̓��́B
 */
struct VSInput{
	float4 pos : SV_Position;
	float2 uv  : TEXCOORD0;
};

/*!
 *@brief	�s�N�Z���V�F�[�_�[�̓��́B
 */
struct PSInput{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;
};

Texture2D<float4> bokeTexture : register(t0);			//�{�P�e�N�X�`���B
Texture2D<float4> depthInViewTexture : register(t1);	//�J������Ԃ̐[�x�e�N�X�`���B
sampler Sampler : register(s0);


/*!
 * @brief	���_�V�F�[�_�[�B
 */
PSInput VSMain(VSInput In) 
{
	PSInput psIn;
	psIn.pos = In.pos;
	psIn.uv = In.uv;
	return psIn;
}

/////////////////////////////////////////////////////////
// �ŏI�����p�̃s�N�Z���V�F�[�_�[�B
/////////////////////////////////////////////////////////
float4 PSFinal(PSInput psIn) : SV_Target0
{
	float depth = depthInViewTexture.Sample(Sampler, psIn.uv);
	float alpha = saturate((depth - 500.0f) / 1000.0f);
	return float4( bokeTexture.Sample(Sampler, psIn.uv).xyz, alpha);
}