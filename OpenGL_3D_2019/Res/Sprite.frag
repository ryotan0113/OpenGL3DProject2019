/**
* @file Sprite.frag
*/
layout(location=0) in vec4 inColor;
layout(location=1) in vec2 inTexCoord;

out vec4 fragColor;

uniform sample2D texColor;

/**
* �X�v���C�g�p�t���O�����g�V�F�[�_�[
*/
void main()
{
	fragColor = inColor * texture(texColor, inTexCoord);
}