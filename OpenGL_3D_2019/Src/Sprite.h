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
* ãÈå`ç\ë¢ëÃ
*/
struct Rect
{
	glm::vec2 origin; ///< ç∂â∫å¥ì_
	glm::vec2 size;   ///< ècâ°ÇÃïù
};

#endif // SPRITE_H_ICLUDED