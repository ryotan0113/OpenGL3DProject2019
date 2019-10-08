/**
* @file Collision.cpp
*/
#include "Collision.h"

namespace Collision {


/**
* 球と球が衝突しているか調べる
*
* @param s0 判定対象の球その１
* @param s1 判定対象の球その２
*
* @retval true 衝突している
* @retval false 衝突していない
*/
bool TestSphereSphere(const Sphere& s0, const Sphere& s1)
{
	const glm::vec3 m = s0.center - s1.center;
	const float radiusSum = s0.r + s1.r;
	return glm::dot(m, m) <= radiusSum * radiusSum;
}

}//namespace Collision