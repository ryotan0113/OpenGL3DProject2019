/**
* @file StatusScene.cpp
*/
#include "GLFWEW.h"
#include "StatusScene.h"



bool StatusScene::Initialize() {
	fontRenderer.Init(1000);
	fontRenderer.LoadFromFile("Res/font.fnt");

	return true;
}

/**
* プレイヤーの入力を処理する
*/
void StatusScene::ProcessInput()
{
	GLFWEW::Window& window = GLFWEW::Window::Instance();
	const GamePad gamepad = window.GetGamePad();
	if (gamepad.buttonDown & GamePad::B) {
		SceneStack::Instance().Pop();
	}


}

void StatusScene::Update(float deltaTime) {

	GLFWEW::Window& window = GLFWEW::Window::Instance();

	const float w = window.Width();
	const float h = window.Height();
	const float lineHeight = fontRenderer.LineHeight();
	fontRenderer.BeginUpdate();
	fontRenderer.AddString(glm::vec2(-w * 0.5f + 32, h * 0.5f - lineHeight), L"ステータス画面");
	fontRenderer.EndUpdate();
}

void StatusScene::Render() {

	const GLFWEW::Window& window = GLFWEW::Window::Instance();
	const glm::vec2 screenSize(window.Width(), window.Height());
	spriteRenderer.Draw(screenSize);
	fontRenderer.Draw(screenSize);
}