# 射命丸文の弾幕稽古

## 諸注意

ヘッダーファイル名の先頭に`_`を付してある。

特にDirectXラッパーライブラリに関して、_dx11.hppは完全に外部用である。

責任者が_dx11private.hppをincludeし、適切に処理をしなければならない。

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