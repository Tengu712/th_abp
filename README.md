# 射命丸文の弾幕稽古

## 諸注意

### ビルド環境

* Windows 10
* Build Tools for Visual Studio 2022 (14.30.30705)
* Windows Kits 10 (10.0.19041.0)

### DX11ラッパーについて

ヘッダーファイル名の先頭に`_`を付してある。

特にDirectXラッパーライブラリに関して、_dx11.hppは完全に外部用である。

責任者が_dx11private.hppをincludeし、適切に処理をしなければならない。

### モデルの描画について

唯一のモデルバッファをideaと名付けてAppで保有している都合上、四角形しか描画しえない。

## Class

| クラス | 説明 |
| ----- | ---------- |
| App | アプリ全体を調停する |
| Camera | カメラ |
| Model | モデル |
| Entity | エンティティすべてが継承する |
| Scene | 各シーンを管理する |

## UML

### 全体の流れ

![Initialize](uml/uml.png)

### 描画

![Draw](uml/uml_001.png)