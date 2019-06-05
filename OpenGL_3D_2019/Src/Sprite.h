/**
* @file Sprite.h
*/
#ifndef SPRITE_H_INCLUDED
#define SPRITE_H_INCLUDED
#include "BufferObject.h"
#include "Texture.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <vector>

/**
* ��`�\����
*/
struct Rect
{
	glm::vec2 origin; ///< �������_
	glm::vec2 size;   ///< �c���̕�
};

/**
* �X�v���C�g�N���X
*/
class Sprite 
{
public:
	Sprite() = default;
	explicit Sprite(const Texture::Image2Dptr&);
	~Sprite() = default;
	Sprite(const Sprite&) = default;
	Sprite& operator=(const Sprite&) = default;

	//���W�̐ݒ�E�擾
	void Position(const glm::vec3& p) { position = p; }
	const glm::vec3& Position() const { return position; }

	//��]�̐ݒ�E�擾
	void Rotation(float r) { rotation = r; }
	float Rotation() const { return rotation; }

	//�g�嗦�̐ݒ�E�擾
	void Scale(const glm::vec2& s) { scale = s; }
	const glm::vec2& Scale() const { return scale; }

	//�F�̐ݒ�E�擾
	void Color(const glm::vec4& c) { color = c; }
	const glm::vec4& Color() const { return color; }

	//��`�̐ݒ�E�擾
	void Rectangle(const Rect& r) { rect = r; }
	const Rect& Rectangle() const { return rect; }

	//�e�N�X�`���̐ݒ�E�擾
	void Texture(const Texture::Image2Dptr& tex);
	const Texture::Image2Dptr& Texture() const { return texture; }

private:
	glm::vec3 position = glm::vec3(0);
	glm::f32 rotation = 0;
	glm::vec2 scale = glm::vec2(1);
	glm::vec4 color = glm::vec4(1);
	Rect rect = { glm::vec2(0,0),glm::vec2(1,1) };
	Texture::Image2Dptr texture;
};

/**
* �X�v���C�g�`��N���X
*/
class SpriteRenderer
{
public:
	SpriteRenderer() = default;
	~SpriteRenderer() = default;
	SpriteRenderer(const SpriteRenderer&) = delete;
	SpriteRenderer& operator=(const SpriteRenderer&) = delete;

	bool Init(size_t maxSpriteCount, const char* vsPath, const char* fsPath);
	void BeginUpdate();
	bool AddVertices(const Sprite&);
	void EndUpdate();
	void Draw(const glm::vec2&) const;
	void Clear();

private:
	BufferObject vbo;
	BufferObject ibo;
	VertexArrayObject vao;
	Shader::ProgramPtr program;

	struct Vertex {
		glm::vec3 position; ///< ���W
		glm::vec4 color; ///<�F
		glm::vec2 texCoord; ///<�e�N�X�`�����W
	};
	std::vector<Vertex> vertices; //���_�f�[�^�z��

	struct Primitive {
		size_t count;
		size_t offset;
		Texture::Image2Dptr texture;
	};
	std::vector<Primitive> primitives;
};

#endif // SPRITE_H_ICLUDED