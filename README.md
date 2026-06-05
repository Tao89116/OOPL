# 2026 OOPL Final Report

## 組別資訊

組別：20

組員：111590450 陶冠丞

復刻遊戲：bloons tower defense 2

## 專案簡介
### 遊戲簡介
Bloons Tower Defense 2，簡稱 BTD2，是 Ninja Kiwi 製作的經典塔防遊戲，也是 Bloons TD 系列的第二代作品。遊戲最早在 2007 年 10 月左右以 Flash 網頁遊戲形式推出，屬於早期很有代表性的瀏覽器塔防遊戲。

遊戲的核心玩法是：玩家要在地圖道路旁放置猴子塔、防禦塔或道具，阻止一波又一波的氣球，也就是 Bloons，走到終點。每打破氣球可以獲得金錢，再用這些錢購買新的塔或升級現有防禦。
### 組別分工
我獨自升級
## 遊戲介紹
### 遊戲規則
玩家要阻止氣球沿著固定路線走到終點。
如果氣球成功走到終點，玩家會扣生命值；生命值歸零就遊戲失敗。玩家必須撐過全部 50 回合 才算勝利。
#### 生命值規則
不同難度的初始生命值不同 ：
| 難度     | 地圖特色        | 初始生命值 |
| ------ | ----------- | ----: |
| Easy   | 路線較彎曲，較容易防守 |   100 |
| Medium | 路線稍短，難度中等   |    75 |
| Hard   | 雙路線且較難防守    |    50 |

#### 氣球規則
|氣球類型|英文|敘述|RBE|
|-----|----|-------|--|
|紅氣球 |red bloon| 最基本，一下就破|1|
|藍氣球 |blue bloon| 打破後變紅氣球|2|
|綠氣球 |green bloon|打破後變藍氣球|3|
|黃氣球 |yellow bloon| 速度非常快，打破後變綠氣球|4|
|黑氣球 |black bloon| 較小，打破後分裂成兩顆黃氣球，免疫爆炸|9|
|白氣球 |white bloon|較小且速度較快，打破後分裂成兩顆黃氣球，免疫冰凍|9|
|鉛氣球 |lead bloon| 一般飛鏢打不破，需要炸彈爆破，打破後分裂成兩顆黑氣球|19|
|彩虹氣球 |rainbow bloon| 速度較快，打破後會分裂成兩顆黑氣球和白氣球|37|
>RBE=Red Bloon Equivalent 等價紅氣球數量
#### 防禦塔規則
| 防禦塔               | 簡介                |特殊設定|
| ----------------- | ----------------- |--------|
| Dart Monkey       | 基本塔，向氣球丟飛鏢|N/A|
| Tack Shooter      | 向多方向發射釘子，適合放在彎道|N/A|
| Ice Tower         | 凍結氣球，讓氣球暫時停止移動|被凍結的氣球無法被打破|
| Cannon     | 發射砲彈，適合對付鉛氣球或群體氣球|可以打破鉛氣球和凍結的氣球|
| Boomerang | 丟出迴旋鏢，可攻擊路線上的氣球|可以打破兩顆氣球|
| Super Monkey      | 高攻速、高輸出，但價格較高|射速飛快|
>防禦塔可以升級
#### 陷阱塔規則
|陷阱塔|簡介|
|----|------|
|Road Spikes|擺在路上的尖刺，刺破氣球，可以觸發10次|
|Monkey Glue|猴子在路上的...額(膠水)，對氣球緩速，可以觸發20次|
>陷阱塔不能升級，每回合結束後失效

#### 塔升級規則
| 防禦塔        | 升級選項       | 效果                           |
| ------------ | ------------------ | -------------------------------- |
| Dart Monkey  | Piercing Darts     | +1 pierce                        |
|              | Long Range Darts   | +25 range                        |
| Tack Shooter | Faster Shooting    | -15 cooldown                     |
|              | Extra Range Tacks  | +10 range, +30% projectile size  |
| Ice Ball     | Long Freeze Time   | +20 freeze duration              |
|              | Wide Freeze Radius | +15 range  |
| Cannon | Bigger Bombs       | +50% projectile size |
|              | Extra Range Bombs  | +range                        |
| Boomerang    | Multi Target       | +3 pierce                        |
|| Sonic Boom         | 可以打破凍結狀態的氣球                |
| Super Monkey | Epic Range         | +100 range                       |
|| Laser Vision       | +1 pierce，可以打破凍結狀態的氣球      |


### 遊戲畫面

## 程式設計

### 程式架構
### 程式技術
### 使用到 AI/AI Agent 的部分

## 結語

### 問題與解決方法
### 自評

| 項次 | 項目                   | 完成 |
|------|------------------------|-------|
| 1    | 這是範例 |  V  |
| 2    | 完成專案權限改為 public |    |
| 3    | 具有 debug mode 的功能  |    |
| 4    | 解決專案上所有 Memory Leak 的問題  |    |
| 5    | 報告中沒有任何錯字，以及沒有任何一項遺漏  |    |
| 6    | 報告至少保持基本的美感，人類可讀  |    |

### 心得
### 貢獻比例
我獨自升級
