/**
* @file Sprite.cpp
*/
#include "Sprite.h"
#include <vector>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

/**
* Sprite�R���X�g���N�^
*
* @param tex �X�v���C�g�Ƃ��ĕ\������e�N�X�`���[
*/
Sprite::Sprite(const Texture::Image2Dptr& tex) :
	texture(tex), 
	rect(Rect{ glm::vec2(),glm::vec2(tex->Width(),tex->Height())})
{

}

/**
* �`��Ɏg�p����e�N�X�`�����w�肷��
*
* @param tex �`��Ɏg�p����e�N�X�`��
*/
void Sprite::Texture(const Texture::Image2Dptr& tex)
{
	texture = tex;
	Rectangle(Rect{ glm::vec2(0),glm::vec2(tex->Width(),tex->Height()) });
}

/**
* �X�v���C�g�`��N���X������������
*
* @param maxSpriteCount �`��\�ȍő�X�v���C�g��
* @param vsPath			���_�V�F�[�_�[�t�@�C����
* @param fsPath			�t���O�����g�V�F�[�_�[�t�@�C����
*
* @retval true ����������
* @retval false ���������s
*/
bool SpriteRenderer::Init(size_t maxSpriteCount, const char* vsPath, const char* fsPath)
{
	vbo.Create(GL_ARRAY_BUFFER, sizeof(Vertex) * maxSpriteCount * 4, nullptr, GL_STREAM_DRAW);

	//�l�p�`��maxSpriteCount���
	std::vector<GLushort> indices;
	indices.resize(maxSpriteCount * 6); //�l�p�`���ƂɃC���f�b�N�X�͂U�K�v
	for (GLushort i = 0; i < maxSpriteCount; ++i) {
		indices[i * 6 + 0] = (i * 4) + 0;
		indices[i * 6 + 1] = (i * 4) + 1;
		indices[i * 6 + 2] = (i * 4) + 2;
		indices[i * 6 + 3] = (i * 4) + 2;
		indices[i * 6 + 4] = (i * 4) + 3;
		indices[i * 6 + 5] = (i * 4) + 0;
	}
	ibo.Create(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW);

	//Vertex�\���̂ɍ��킹�Ē��_�A�g���r���[�g��ݒ肷��
	vao.Create(vbo.Id(), ibo.Id());
	vao.Bind();
	vao.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, position));
	vao.VertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, color));
	vao.VertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, texCoord));
	vao.Unbind();

	program = Shader::Program::Create(vsPath, fsPath);

	primitives.reserve(64); //32�ł͑���Ȃ����Ƃ����邩������Ȃ��̂�64�\��

	//�������ƒ�̂ǂꂩ�̃I�u�W�F�N�g�̍쐬�Ɏ��s���Ă�����A���̊֐����̂����s�Ƃ���
	if (!vbo. Id() || !ibo.Id() || !vao.Id() || program->IsNull()) {
		return false;
	}
	return true;
}

/**
* ���_�f�[�^�̍쐬���J�n����
*/
void SpriteRenderer::BeginUpdate()
{
	primitives.clear();
	vertices.clear();
	vertices.reserve(vbo.Size() / sizeof(Vertex));
}
/**
* ���_�f�[�^��ǉ�����
*
* @param sprite ���_�f�[�^�̌��ɂȂ�X�v���C�g
*
* @retval true �ǉ�����
* @retval false ���_�o�b�t�@�����t�Œǉ��ł��Ȃ�
*/
bool SpriteRenderer::AddVertices(const Sprite& sprite)
{
	if (vertices.size() * sizeof(Vertex) >= static_cast<size_t>(vbo.Size())) {
		std::cerr << "[�x��] " << __func__ << ": �ő�\�����𒴂��Ă��܂�\n";
		return false;
	}
	return true;
}
