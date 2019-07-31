
/**
* @file Main.cpp
*/
#include "TitleScene.h"
#include "GLFWEW.h"
#include <Windows.h>
#include <iostream>
int main()
{
	GLFWEW::Window& window = GLFWEW::Window::Instance();
	if (!window.Init(800, 600, "Title")) {
		return 1;
	}

	SceneStack& sceneStack = SceneStack::Instance();
	sceneStack.Push(std::make_shared<TitleScene>());

	while (!window.ShouldClose())
	{
		//window.Update();

		const float deltaTime = 1.0f / 60.0f;
		sceneStack.Update(deltaTime);

		/**
		//ESCキーが押されたら終了ウィンドウを表示
		if (window.IsKeyPressed(GLFW_KEY_ESCAPE)) {
			if (MessageBox(nullptr, "ゲームを終了しますか？", "終了", MB_OKCANCEL) == IDOK) {
				break;
			}
		}
		*/



		//バックバッファを消去する
		glClearColor(0.8f, 0.2f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//GLコンテキストのパラメーターを設定
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);


		sceneStack.Render();
		window.SwapBuffers();
	}

	
}