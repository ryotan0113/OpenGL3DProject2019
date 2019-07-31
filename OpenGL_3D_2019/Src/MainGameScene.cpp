/**
* @file MainGameScene.cpp
*/
#include "GLFWEW.h"
#include "MainGameScene.h"
#include "StatusScene.h"
#include "GameOverScene.h"
#include <glm/gtc/matrix_transform.hpp>

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

	//TODO: 必要なくなったら消しとけ
	auto mesh = meshBuffer.GetFile("Cube");
	mesh->materials[0].texture = spr.Texture();

	return true;
}
	


/**
* プレイヤーの入力を処理する
*/
void MainGameScene::ProcessInput()
{
	/**
	if (!flag) {
		flag = true;
		SceneStack::Instance().Push(std::make_shared<StatusScene>());
	}
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

	const GLFWEW::Window& window = GLFWEW::Window::Instance();
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

	const glm::vec3 cameraPos(-5, 3, 5);
	const glm::vec3 targetPos(0, 0, 0);
	const glm::mat4 matView = glm::lookAt(cameraPos, targetPos, glm::vec3(0, 1, 0));
	const float aspectRatio =
		static_cast<float>(window.Width()) / static_cast<float>(window.Height());
	const glm::mat4 matProj =
		glm::perspective(glm::radians(30.0f), aspectRatio, 1.0f, 1000.0f);
	const glm::mat4 matModel(1);
	Mesh::Draw(meshBuffer.GetFile("Cube"), matProj * matView, matModel);
}
