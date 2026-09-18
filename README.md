# 概要
* DirectX11のゲームテンプレートです。

# 環境
* VisualStudio 2026
* Windows 10

# ライブラリ
* ImGUI ( [license](https://github.com/mochinekoo/DirectX11_Game_Template/blob/main/LICENSE) )
* fbxsdk ( [license](https://github.com/mochinekoo/DirectX11_Game_Template/blob/main/fbxsdk_License) )

# クラス
* 詳細な説明は：
## ルート
| クラス/ヘッダー名 | 説明 |
| --- | --- |
| Main.cpp | メインクラス。 |
| Framework.h | |

## データ
| クラス/ヘッダー名 | 説明 |
| --- | --- |
| Camera.h | カメラのデータクラス。 |
| Texture.cpp/h | テクスチャのデータクラス。 |
| Transform.h | 移動（座標・スケール・回転・ベクトル）のデータクラス。|
| VideoData.cpp/h | ビデオのデータクラス。|

## シーン
| クラス/ヘッダー名 | 説明 |
| --- | --- |
| BaseScene.h | シーンの基底クラス。 |
| DebugScene.cpp/h | デバック用のシーン。|

## オブジェクト
| クラス/ヘッダー名 | 説明 |
| --- | --- |
| BaseObject.h | オブジェクトの基底クラス。 |
| Box.cpp/h | 2D四角形を描画するクラス。 |
| FBX.cpp/h | FBXを描画するクラス。 |
| FBXChild.cpp/h | FBXの子を描画するクラス。 |
| FontText.cpp/h | フォントを使って文字を描画するクラス。 |
| Image.cpp/h | 画像を描画するクラス |
| ImageButton.cpp/h | 画像のボタンを描画するクラス。 |
| VideoObject.cpp/h | ビデオを描画するクラス。 |

## システムオブジェクト
| クラス/ヘッダー名 | 説明 |
| --- | --- |
| BaseColllider.h | コライダーの基底クラス。 |
| BoxCollider.cpp/h | 四角形のコライダークラス。|
| SphereCollider.cpp.h | 球のコライダークラス。|

## マネージャー
| クラス/ヘッダー名 | 説明 |
| --- | --- |
| CameraManager.cpp/h | カメラを制御するクラス。複数のカメラを設定できる。 |
| DX2DManager.cpp/h | Direct2Dを管理するクラス。|
| DX3DManager.cpp/h | Direct3Dを管理するクラス。 |
| ImageManager.cpp/h | 画像データを管理するクラス。 |
| SoundManager.cpp/h | 音声データを管理するクラス。 |
| InputManager.cpp/h | 入力装置を管理するクラス。マウス・キーボード・コントローラーに対応している。|
| SceneManager.cpp/h | シーンを管理するクラス。|
| ShaderManager.cpp/h | シェーダーを管理するクラス。 |

## ユーティリティ
| クラス/ヘッダー名 | 説明 |
| --- | --- |
|  |  |

# 関連項目
* pygame（Python）によるテンプレート： https://github.com/mochinekoo/PyGame_Template
* DirectX11（C++）によるテンプレート： https://github.com/mochinekoo/DirectX11_Game_Template
* DxLib（C++）によるテンプレート： https://github.com/mochinekoo/C-PlusPlus_DxLib_GameTemplate
* Vortice（C#）によるテンプレート： https://github.com/mochinekoo/VorticeDirectX11_Template
* DxLib（C#）によるテンプレート： https://github.com/mochinekoo/C-Sharp_DxLib_GameTemplate
