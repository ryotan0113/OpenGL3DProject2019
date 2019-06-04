/**
* @file Sprite.cpp
*/
#include "Sprite.h"
#include <vector>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

/**
* Spriteコンストラクタ
*
* @param tex スプライトとして表示するテクスチャー
*/
Sprite::Sprite(const Texture::Image2Dptr& tex) :
	texture(tex), 
	rect(Rect{ glm::vec2(),glm::vec2(tex->Width(),tex->Height())})
{

}

/**
* 描画に使用するテクスチャを指定する
*
* @param tex 描画に使用するテクスチャ
*/
void Sprite::Texture(const Texture::Image2Dptr& tex)
{
	texture = tex;
	Rectangle(Rect{ glm::vec2(0),glm::vec2(tex->Width(),tex->Height()) });
}

/**
* スプライト描画クラスを初期化する
*
* @param maxSpriteCount 描画可能な最大スプライト数
* @param vsPath			頂点シェーダーファイル名
* @param fsPath			フラグメントシェーダーファイル名
*
* @retval true 初期化成功
* @retval false 初期化失敗
*/
bool SpriteRenderer::Init(size_t maxSpriteCount, const char* vsPath, const char* fsPath)
{
	vbo.Create(GL_ARRAY_BUFFER, sizeof(Vertex) * maxSpriteCount * 4, nullptr, GL_STREAM_DRAW);

	//四角形をmaxSpriteCount個作る
	std::vector<GLushort> indices;
	indices.resize(maxSpriteCount * 6); //四角形ごとにインデックスは６個必要
	for (GLushort i = 0; i < maxSpriteCount; ++i) {
		indices[i * 6 + 0] = (i * 4) + 0;
		indices[i * 6 + 1] = (i * 4) + 1;
		indices[i * 6 + 2] = (i * 4) + 2;
		indices[i * 6 + 3] = (i * 4) + 2;
		indices[i * 6 + 4] = (i * 4) + 3;
		indices[i * 6 + 5] = (i * 4) + 0;
	}
	ibo.Create(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW);

	//Vertex構造体に合わせて頂点アトリビュートを設定する
	vao.Create(vbo.Id(), ibo.Id());
	vao.Bind();
	vao.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, position));
	vao.VertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, color));
	vao.VertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, texCoord));
	vao.Unbind();

	program = Shader::Program::Create(vsPath, fsPath);

	primitives.reserve(64); //32個では足りないことがあるかもしれないので64個予約

	//初期化家庭のどれかのオブジェクトの作成に失敗していたら、この関数自体も失敗とする
	if (!vbo. Id() || !ibo.Id() || !vao.Id() || program->IsNull()) {
		return false;
	}
	return true;
}

/**
* 頂点データの作成を開始する
*/
void SpriteRenderer::BeginUpdate()
{
	primitives.clear();
	vertices.clear();
	vertices.reserve(vbo.Size() / sizeof(Vertex));
}
/**
* 頂点データを追加する
*
* @param sprite 頂点データの元になるスプライト
*
* @retval true 追加成功
* @retval false 頂点バッファが満杯で追加できない
*/
bool SpriteRenderer::AddVertices(const Sprite& sprite)
{
	if (vertices.size() * sizeof(Vertex) >= static_cast<size_t>(vbo.Size())) {
		std::cerr << "[警告] " << __func__ << ": 最大表示数を超えています\n";
		return false;
	}
	return true;
}
