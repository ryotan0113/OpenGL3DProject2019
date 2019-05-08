/**
* @file Scene.cpp
*/
#include "Scene.h"
#include <iostream>

/**
* コンストラクタ
*
* @param name シーン名
*/
Scene::Scene(const char* name) : name(name)
{
	std::cout << "Scene コンストラクタ:" << name << "\n";
}

/**
* デストラクタ
*/
Scene::~Scene()
{
	Finalize();
	std::cout << "Scene デストラクタ: " << name << "\n";
}

/**
* シーンを活動状態にする
*/
void Scene::Play()
{
	isActive = true;
	std::cout << "Scene Play: " << name << "\n";
}

/**
* シーンを停止状態にする
*/
void Scene::Stop()
{
	isActive = false;
	std::cout << "Scene Stop: " << name << "\n";
}

/**
* シーンを表示する
*/
void Scene::Show()
{
	isVisible = true;
	std::cout << "Scene Show: " << name << "\n";
}

/**
* シーンを非表示にする
*/
void Scene::Hide()
{
	isVisible = false;
	std::cout << "Scene Hide: " << name << "\n";
}

/**
* シーン名を取得する
*
* @return シーン名
*/
const std::string& Scene::Name() const
{
	return name;
}

/**
* シーンの活動状態を調べる
*
* @retval true 活動している
* @retval false 停止している
*/
bool Scene::IsActive() const
{
	return isActive;
}

/**
* シーンの表示状態を調べる
*
* @retval true 表示状態
* @retval false 非表示状態
*/
bool Scene::IsVisible() const
{
	return isVisible;
}

/**
* シーンスタックを取得する
*
* @return シーンスタック
*/
SceneStack& SceneStack::Instance()
{
	static SceneStack instance;
	return instance;
}

/**
* コンストラクタ
*/
SceneStack::SceneStack()
{
	stack.reserve(16);
}

/**
* シーンをプッシュする
*
* @param p 新しいシーン
*/
void SceneStack::Push(ScenePtr p)
{
	if (!stack.empty())
	{
		Current().Stop();
	}

	stack.push_back(p);
	std::cout << "[シーンプッシュ] " << p->Name() << "\n";
	Current().Initialize();
	Current().Play();
}