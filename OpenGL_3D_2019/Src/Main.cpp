
/**
* @file Main.cpp
*/
#include "TitleScene.h"
#include "GLFWEW.h"
#include <Windows.h>
#include <iostream>
int main()
{
	auto& window = GLFWEW::Window::Instance();
	window.Init(800, 600, "Title");

	SceneStack& sceneStack = SceneStack::Instance();
	sceneStack.Push(std::make_shared<TitleScene>());

	for (;!window.ShouldClose();) {
		const float deltaTime = 1.0f / 60.0f;
		sceneStack.Update(deltaTime);
		sceneStack.Render();
		window.SwapBuffers();
	}
}