#pragma once
#include <string>
#include <DirectXMath.h>
#include "Transform.h"
#include "ImGUI/imgui.h"
#include <vector>
#include "BaseCollider.h"

/// <summary>
/// オブジェクトの親クラス
/// </summary>
class BaseObject {
private:
	int drawHighOrder_;	// オブジェクトの優先度。高いほど手前になる。（※Zバッファが有効な場合は使われません）
	std::string name_;	// オブジェクトの名前
protected:
	std::string tag_;	// オブジェクトのタグ
	bool isDead_;		// オブジェクトが死んでいるかどうか
	bool showImGUI_;	// デバック用のImGUIを表示するかどうか
	std::vector<BaseCollider*> colliderList_;	// コライダーのリスト

	BaseObject* parent_;					// 親のオブジェクト
	std::vector<BaseObject*> childList_;	// 子供のオブジェクトのリスト
	DirectX::XMMATRIX worldMatrix_;			// ワールド行列

	Transform transform_;	// トランスフォーム（移動/回転/スケール/ベクトル）
public:

	BaseObject(const std::string& name) {
		name_ = name;
		tag_ = "";
		isDead_ = false;
		transform_ = Transform();
		drawHighOrder_ = 0;
		showImGUI_ = true;
		parent_ = nullptr;
	};
	virtual ~BaseObject() {};

	/// <summary>
	/// オブジェクトを初期化する関数
	/// </summary>
	virtual void Init() {};

	/// <summary>
	/// オブジェクトを更新する関数
	/// </summary>
	virtual void Update() {};

	/// <summary>
	/// オブジェクトを描画する関数
	/// </summary>
	virtual void Draw() {};

	/// <summary>
	/// デバック用のImGUIを描画する関数
	/// </summary>
	virtual void DrawImGUI() {
		if (!showImGUI_) return;
		ImGui::Begin(name_.c_str(), nullptr, ImGuiWindowFlags_NoDocking);
		if (ImGui::BeginTabBar("Tab")) {
			if (ImGui::BeginTabItem("ObjectInfo")) {
				ImGui::SliderFloat("Location X", &transform_.location_.x, -10.0f, 10.0f);
				ImGui::SliderFloat("Location Y", &transform_.location_.y, -10.0f, 10.0f);
				ImGui::SliderFloat("Location Z", &transform_.location_.z, -10.0f, 10.0f);
				ImGui::SliderFloat("Velocity X", &transform_.velocity_.x, -10.0f, 10.0f);
				ImGui::SliderFloat("Velocity Y", &transform_.velocity_.y, -10.0f, 10.0f);
				ImGui::SliderFloat("Velocity Z", &transform_.velocity_.z, -10.0f, 10.0f);
				ImGui::SliderFloat("Rotation X", &transform_.rotation_.x, -DirectX::XM_PI, DirectX::XM_PI);
				ImGui::SliderFloat("Rotation Y", &transform_.rotation_.y, -DirectX::XM_PI, DirectX::XM_PI);
				ImGui::SliderFloat("Rotation Z", &transform_.rotation_.z, -DirectX::XM_PI, DirectX::XM_PI);
				ImGui::SliderFloat("Scale X", &transform_.scale_.x, 0.1f, 10.0f);
				ImGui::SliderFloat("Scale Y", &transform_.scale_.y, 0.1f, 10.0f);
				ImGui::SliderFloat("Scale Z", &transform_.scale_.z, 0.1f, 10.0f);

				ImGui::EndTabItem();
			}
		}
		ImGui::EndTabBar();
		ImGui::End();
	};
	virtual void Release() {};

	/// <summary>
	/// オブジェクトの親
	/// </summary>
	/// <returns>ポインタで返します。</returns>
	BaseObject* GetParent() const { return parent_; }

	/// <summary>
	/// オブジェクトの親を設定する関数
	/// </summary>
	/// <param name="object"></param>
	void SetParent(BaseObject* object) { parent_ = object; }

	/// <summary>
	/// オブジェクトの子供を追加する関数
	/// </summary>
	/// <param name="object"></param>
	void AddChild(BaseObject* object) { 
		childList_.push_back(object); 
		object->parent_ = this;
	}

	DirectX::XMMATRIX GetWorldMatrix() const { return worldMatrix_;}
	void SetWorldMatrix(const DirectX::XMMATRIX matrix) { worldMatrix_ = matrix; }

	void UpdateTransform() {
		if (parent_ != nullptr) {
			DirectX::XMMATRIX parentWorld = parent_->GetWorldMatrix();
			worldMatrix_ = parentWorld * transform_.GetWorldMatrix();
		}
		else {
			worldMatrix_ = transform_.GetWorldMatrix();
		}

		for (BaseObject* child : childList_) {
			if (child != nullptr) {
				child->UpdateTransform();
			}
		}
	}

	/// <summary>
	/// オブジェクトの名前
	/// </summary>
	std::string GetName() const { return name_; }

	/// <summary>
	/// オブジェクトのタグ
	/// </summary>
	/// <returns></returns>
	std::string GetTag() const { return tag_; }
	void SetTag(const std::string& tag) { tag_ = tag; }

	/// <summary>
	/// 自分をキルする関数
	/// </summary>
	void KillMe() { isDead_ = true; }

	/// <summary>
	/// オブジェクトが死んでいるかどうかを返す関数
	/// </summary>
	/// <returns>死んでいるならtrue、死んでいない場合はfalseを返す。</returns>
	bool IsDead() const { return isDead_; }
	Transform GetTransform() const { return transform_; }
	void SetTransform(const Transform& transform) { transform_ = transform; }
	int GetDrawOrder() const { return drawHighOrder_; }
	void SetDrawOrder(const int order) { drawHighOrder_ = order;  }
	bool IsShowImGUI() const { return showImGUI_; }
	void SetShowImGUI(const bool flag) { showImGUI_ = flag; }

	/// <summary>
	/// コライダーのリスト
	/// </summary>
	/// <returns></returns>
	std::vector<BaseCollider*>& GetColliderList() { return colliderList_; }
};