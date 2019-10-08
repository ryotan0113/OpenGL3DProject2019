/**
* @file MainGameScene.cpp
*/
#include "GLFWEW.h"
#include "MainGameScene.h"
#include "StatusScene.h"
#include "GameOverScene.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <random>

/**
* 衝突を解決する
*
* @param a 衝突したアクターその１
* @param b 衝突したアクターその２
* @param p 衝突位置
*/
void PlayerCollisionHandler(const ActorPtr& a, const ActorPtr& b, const glm::vec3& p)
{
	const glm::vec3 v = a->colWorld.center - p;
	//衝突位置との距離が近づきすぎないか調べる
	if (dot(v, v) > FLT_EPSILON) {
		//aをbに重ならない位置まで移動
		const glm::vec3 vn = normalize(v);
		const float radiusSum = a->colWorld.r + b->colWorld.r;
		const float distance = radiusSum - glm::length(v) + 0.01f;
		a->position += vn * distance;
		a->colWorld.center += vn * distance;
	}else {
		//移動を取り消す(距離が近すぎる場合の例外処理)
		const float deltaTime = static_cast<float>(GLFWEW::Window::Instance().DeltaTime());
		const glm::vec3 deltaVelocity = a->velocity * deltaTime;
		a->position -= deltaVelocity;
		a->colWorld.center -= deltaVelocity;
	}
}

/**
* シーンを初期化する
*
* retval true 初期化成功
* retval false 初期化失敗
*/
bool MainGameScene::Initialize() {
	spriteRenderer.Init(1000, "Res/Sprite.vert", "Res/Sprite.frag");
	fontRenderer.Init(1000);
	fontRenderer.LoadFromFile("Res/font.fnt");
	Sprite spr(Texture::Image2D::Create("Res/TitleBg.tga"));
	spr.Scale(glm::vec2(2));
	sprites.push_back(spr);
	meshBuffer.Init(1'000'000 * sizeof(Mesh::Vertex), 3'000'000 * sizeof(GLushort));

	meshBuffer.LoadMesh("Res/red_pine_tree.gltf");
	meshBuffer.LoadMesh("Res/bikuni.gltf");
	meshBuffer.LoadMesh("Res/oni_small.gltf");

	//ハイトマップを作成する
	if (!heightMap.LoadFromFile("Res/Terrain.tga", 20.0f, 0.5f)) {
		return false;
	}
	if (!heightMap.CreateMesh(meshBuffer, "Terrain")) {
		return false;
	}

	glm::vec3 startPos(100, 0, 100);
	startPos.y = heightMap.Height(startPos);
	player = std::make_shared<StaticMeshActor>(
		meshBuffer.GetFile("Res/bikuni.gltf"), "Player", 20, startPos);
	player->colLocal = Collision::Sphere{ glm::vec3(0),0.5f };

	std::mt19937 rand;
	rand.seed(0);

	//敵を配置
	{
		const size_t oniCount = 100;
		enemies.Reserve(oniCount);
		const Mesh::FilePtr mesh = meshBuffer.GetFile("Res/oni_small.gltf");
		for (size_t i = 0; i < oniCount; ++i) {
			//敵の位置を(50,50)-(150,150)の範囲からランダムに選択
			glm::vec3 position(0);
			position.x = std::uniform_real_distribution<float>(50, 150)(rand);
			position.z = std::uniform_real_distribution<float>(50, 150)(rand);
			position.y = heightMap.Height(position);
			//敵の向きをランダムに選択
			glm::vec3 rotation(0);
			rotation.y = std::uniform_real_distribution<float>(0, 6.3f)(rand);
			StaticMeshActorPtr p = std::make_shared<StaticMeshActor>(
				mesh, "Kooni", 13, position, rotation);
			p->colLocal = Collision::Sphere{ glm::vec3(0), 1.0f };
			enemies.Add(p);
		}
	}

	std::mt19937 treesrand;
	treesrand.seed(0);
	//木を配置
	{
		const size_t treeCount = 1000;
		trees.Reserve(treeCount);
		const Mesh::FilePtr mesh = meshBuffer.GetFile("Res/red_pine_tree.gltf");
		for (size_t i = 0; i < treeCount; ++i) {
			//木の位置を(80,80)-(180,180)の範囲からランダムに選択
			glm::vec3 position(0);
			position.x = std::uniform_real_distribution<float>(40, 140)(treesrand);
			position.z = std::uniform_real_distribution<float>(40, 140)(treesrand);
			position.y = heightMap.Height(position);
			//木の向きをランダムに選択
			glm::vec3 rotation(0);
			rotation.y = std::uniform_real_distribution<float>(0, 6.3f)(treesrand);
			StaticMeshActorPtr t = std::make_shared<StaticMeshActor>(
				mesh, "Tree", 13, position, rotation);
			t->colLocal = Collision::Sphere{ glm::vec3(0), 1.0f };
			trees.Add(t);
		}
	}

	return true;
}
	


/**
* プレイヤーの入力を処理する
*/
void MainGameScene::ProcessInput()
{
	GLFWEW::Window& window = GLFWEW::Window::Instance();

	//プレイヤー操作
	const GamePad gamepad = window.GetGamePad();

	glm::vec3 velocity(0);
	if (gamepad.buttons & GamePad::DPAD_LEFT) {
		velocity.x = -1;
	}
	else if (gamepad.buttons & GamePad::DPAD_RIGHT) {
		velocity.x = 1;
	}
	if (gamepad.buttons & GamePad::DPAD_DOWN) {
		velocity.z = 1;
	}
	else if (gamepad.buttons & GamePad::DPAD_UP) {
		velocity.z = -1;
	}
	if (velocity.x || velocity.z) {
		velocity = normalize(velocity);
		player->rotation.y = std::atan2(-velocity.z, velocity.x) + glm::radians(90.0f);
		velocity *= 6.0f;
	}
	player->velocity = velocity;
	

	
	if (!flag) {
		if (window.GetGamePad().buttonDown & GamePad::X) {
			flag = true;
			SceneStack::Instance().Push(std::make_shared<StatusScene>());
		}

	}
	/**
	else {
		SceneStack::Instance().Replace(std::make_shared<GameOverScene>());
	}
	*/
	
}


void MainGameScene::Update(float deltaTime) {

	spriteRenderer.BeginUpdate();
	for (const Sprite& e : sprites) {
		spriteRenderer.AddVertices(e);
	}
	spriteRenderer.EndUpdate();

	GLFWEW::Window& window = GLFWEW::Window::Instance();

	//カメラの状態を更新
	{
		camera.target = player->position;
		camera.position = camera.target + glm::vec3(0, 30, 30);
	}
	player->Update(deltaTime);
	enemies.Update(deltaTime);
	trees.Update(deltaTime);

	player->position.y = heightMap.Height(player->position);
	DetectCollision(player, enemies, PlayerCollisionHandler);
	DetectCollision(player, trees, PlayerCollisionHandler);
	player->position.y = heightMap.Height(player->position);

	player->UpdateDrawData(deltaTime);
	enemies.UpdateDrawData(deltaTime);
	trees.UpdateDrawData(deltaTime);

	const float w = window.Width();
	const float h = window.Height();
	const float lineHeight = fontRenderer.LineHeight();
	fontRenderer.BeginUpdate();
	fontRenderer.AddString(glm::vec2(-w * 0.5f + 32, h * 0.5f - lineHeight), L"メイン画面");
	fontRenderer.EndUpdate();
}



void MainGameScene::Render() {
	const GLFWEW::Window& window = GLFWEW::Window::Instance();
	const glm::vec2 screenSize(window.Width(), window.Height());
	spriteRenderer.Draw(screenSize);
	fontRenderer.Draw(screenSize);

	const glm::mat4 matView = glm::lookAt(camera.position, camera.target, camera.up);
	const float aspectRatio =
		static_cast<float>(window.Width()) / static_cast<float>(window.Height());
	const glm::mat4 matProj =
		glm::perspective(glm::radians(30.0f), aspectRatio, 1.0f, 1000.0f);
	glm::vec3 cubePos(100, 0, 100);
	cubePos.y = heightMap.Height(cubePos);
	const glm::mat4 matModel = glm::translate(glm::mat4(1), cubePos);
	meshBuffer.SetViewProjectionMatrix(matProj * matView);
	Mesh::Draw(meshBuffer.GetFile("Cube"), matModel);
	Mesh::Draw(meshBuffer.GetFile("Terrain"), glm::mat4(1));

	player->Draw();
	enemies.Draw();
	trees.Draw();
}
