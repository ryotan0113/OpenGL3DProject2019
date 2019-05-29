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

#endif // SPRITE_H_ICLUDED