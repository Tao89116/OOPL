# 2026 OOPL Final Report

## 組別資訊

組別：T20

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
* 遊戲初始畫面
![image](https://github.com/Tao89116/OOPL/blob/main/sample/sample-0.png)
* 難易度選擇畫面
![image](https://github.com/Tao89116/OOPL/blob/main/sample/sample-1.png)
* 遊戲主畫面
![image](https://github.com/Tao89116/OOPL/blob/main/sample/sample-2.png)
* 選塔、建塔、升級塔、賣塔畫面
![image](https://github.com/Tao89116/OOPL/blob/main/sample/sample-3.gif)
* 遊玩畫面
![image](https://github.com/Tao89116/OOPL/blob/main/sample/sample-6.gif)
* 結算畫面(勝利)
![image](https://github.com/Tao89116/OOPL/blob/main/sample/sample-4.gif)
* 結算畫面(失敗)
![image](https://github.com/Tao89116/OOPL/blob/main/sample/sample-5.gif)
* 作弊模式啟動：在難易度選擇畫面/主畫面輸入konami code
![image](https://github.com/Tao89116/OOPL/blob/main/sample/Konami-Code.png)
## 程式設計

### 程式架構
#### 架構總覽：最外層FSM切換場景，場景皆為MVC架構
```mermaid
flowchart LR
    subgraph Scene["Scene"]
        direction TB
        C["Controller"] --> M["Model"]
        V["View"] --> M
    end

    SM["SceneManager<br/>State Machine"]

    Input["Input"] --> C
    V --> Output["Output"]

    C -->|"request"| SM
    SM -->|"switch"| Scene
```
#### FSM 架構：透過狀態機分離場景，避免過耦合
```mermaid
stateDiagram-v2
    [*] --> Start

    Start --> Difficulty
    Difficulty --> Game
    Game --> Result
    Result --> Start
    Result --> Difficulty

    Game --> Difficulty: Return
    Difficulty --> Start: Back

    state Start {
        [*] --> StartScene
    }

    state Difficulty {
        [*] --> DifficultyScene
    }

    state Game {
        [*] --> GameScene
    }

    state Result {
        [*] --> ResultScene
    }
```
#### 遊戲邏輯核心：負責管理遊戲狀態、地圖與放置資料、執行中的塔/敵人/子彈，以及提供給 View 顯示用的事件資料
```mermaid
flowchart LR
    GS["Game State<br/>Ready / Running / Paused / Win / Lose"] --> GM["GameModel"]

    WD["World Data<br/>Map / Placement / Wave"] --> GM

    GM --> RO["Runtime Objects<br/>Towers / Enemies / Projectiles"]

    GM --> EV["Events<br/>Pop / Hit / Count"]

    RO --> TB["IBuildable"]
    RO --> EN["EnemyModel"]
    RO --> PR["ProjectileModel"]
```

### 程式技術
1. MVC Pattern

整體架構採用HMVC(Hierarchical-Model-View-Controller)。最上層由 SceneManager 管理不同 Scene，而每個 Scene 內部再依職責拆成 Model、View、Controller。Controller 負責接收輸入並修改 Model，View 負責讀取 Model 狀態並更新畫面，Model 則只負責資料與邏輯，不依賴 View 或 Controller。這樣可以降低耦合，也讓不同場景能獨立維護。
```
Scene
├── Model      資料 / 狀態
├── View       畫面 / UI
└── Controller 輸入 / 指令
```
2. State Pattern

有兩層 State Pattern 的概念。第一層是 SceneManager 作為場景狀態機，負責在 Start、Difficulty、Game、Result 之間切換。第二層是 GameModel 內部的遊戲狀態，透過 IGameState 拆分成 ReadyState、RoundRunningState、PausedState、WinState、LoseState。

這樣可以避免用大量 if-else 或 switch 判斷目前遊戲狀態，讓每個狀態自己管理對應行為。
```
IGameState
├── ReadyState
├── RoundRunningState
├── PausedState
├── WinState
└── LoseState
```
3. Command Pattern

將玩家操作封裝成 Command 物件，例如 SelectBuildableCommand、StartRoundCommand、SellTowerCommand、UpgradeTowerCommand、ReturnToDifficultyCommand。Controller 不需要直接把所有操作邏輯寫在一起，而是根據輸入執行對應 Command。

這樣可以讓 UI、輸入處理與遊戲邏輯分離，未來若要新增快捷鍵或新按鈕，只需要新增對應 Command，較不會影響原本 Controller。
```
ICommand
├── SelectBuildableCommand
├── StartRoundCommand
├── SellTowerCommand
├── UpgradeTowerCommand
└── ReturnToDifficultyCommand
```

4. Factory Pattern + Singleton Pattern

使用 BuildableRegistry 管理所有可建造物的 id 與 factory function。玩家選擇塔時，GameModel 只需要根據 id 向 BuildableRegistry 查詢 factory，就能建立對應的 IBuildable 物件，不需要直接依賴 DartTower、CannonTower、GlueTrap 等具體類別。

BuildableRegistry 同時使用 Singleton Pattern，確保整個遊戲只有一份可建造物註冊表，避免不同系統各自建立 registry 造成資料不同步或重複註冊。這讓新增塔與陷阱時，只需要集中註冊新的 factory，提升擴充性與維護性。
```
BuildableRegistry
├── tower id
├── factory function
└── create IBuildable
```
>Singleton Pattern：這些物件全域唯一，包括資源管理、塔註冊資料、和波次設定。 避免重複載入資料，也方便不同系統共用同一份設定。
```
ResourceManager
BuildableRegistry
WaveConfig
```
其他的oop設計 ↓ (都舉部分)

5. Interface / Abstract Class

使用多個 interface 與 abstract class 來定義共同規格。例如 IScene 定義所有場景都必須實作 Update()，IGameState 定義遊戲狀態的 Enter、Update、Exit，ICommand 定義所有操作指令都必須能 Execute，而 IBuildable 則定義所有可建造物都必須具備 Update、GetId、GetCost、GetPosition 等方法。

透過介面與抽象類別，GameModel 和 SceneManager 不需要知道具體類別，只要依賴共同介面即可操作不同物件，達到低耦合與高擴充性。
```
   IScene
   IGameState
   ICommand
   IBuildable
   TowerBase
   AttackTowerBase
   TrapBase
   ProjectileModel
```
6. Inheritance

使用繼承來整理相似物件的共同行為。例如所有可建造物都繼承 IBuildable，塔類別再由 TowerBase 延伸出 AttackTowerBase 與 TrapBase。攻擊塔如 DartTower、CannonTower、SuperTower 繼承 AttackTowerBase；陷阱如 GlueTrap、SpikeTrap 則繼承 TrapBase。

子彈系統也使用 ProjectileModel 作為共同基底，再延伸出 CannonProjectile、IceBallProjectile、BoomerangProjectile、GlueProjectile 等不同子彈。透過繼承可以減少重複程式碼，並讓相似物件共用共同邏輯。

7. Polymorphism

本透過多型讓 GameModel 可以用同一個容器管理不同種類的塔。例如所有塔與陷阱都可以被視為 IBuildable，因此 GameModel 只需要呼叫 tower->Update()，實際執行時會依照物件真實型別執行 DartTower、CannonTower、GlueTrap 或 SpikeTrap 的更新邏輯。


同樣地，ProjectileModel 也透過多型讓不同子彈可以覆寫 Update() 或 OnHit()，例如砲彈可以造成範圍傷害，冰球可以凍結敵人，膠水子彈可以緩速敵人。這讓遊戲能用統一流程管理不同物件，又保留各自的特殊行為。 

8. Encapsulation

本專案透過封裝讓不同類別負責自己的資料與行為。GameModel 負責 HP、Gold、Round、塔、敵人、子彈與勝敗判斷；GameView 負責畫面物件、UI、音效與特效同步；GameController 負責玩家輸入與指令執行。EnemyModel、ProjectileModel、PlacementModel、MapModel 也各自封裝自己的狀態與操作方法。

透過封裝，外部不需要直接修改物件內部資料，而是透過方法進行操作，能降低錯誤發生機率，也讓程式更容易維護。

9. Smart Pointer / Resource Management

使用 C++ smart pointer 管理物件生命週期。例如 SceneManager 使用 unique_ptr 管理目前 Scene，GameModel 使用 unique_ptr 管理目前 GameState，並使用 shared_ptr 管理塔、敵人與子彈等遊戲物件。ProjectileModel 對目標敵人使用 weak_ptr，避免因互相持有 shared_ptr 而造成循環引用。

透過 smart pointer，可以減少手動 new/delete 造成的記憶體管理問題，也能降低 memory leak 的風險。

10. Manual Data Binding / View Synchronization

沒有做到自動 Data Binding，而是由 GameView 每一幀主動讀取 GameModel 的資料，並同步畫面上的塔、敵人與子彈物件。當 Model 中有新物件但 View 還沒有對應 GameObject 時，View 會建立新畫面物件；當 Model 物件狀態改變時，View 會更新位置、旋轉與顯示效果；當 Model 物件消失時，View 則移除對應畫面物件。

這種手動同步方式可以讓 Model 不依賴 View，同時保持畫面與遊戲資料一致。

11. Event System / Observer-like Design

Model 不會直接呼叫 View 播放音效或產生特效，而是將事件暫存在 GameModel 中，例如 PoppedEnemyEvent、HitEffectEvent、PoppedBloonCount。GameScene 在更新流程中取出這些事件，再交給 GameView 顯示特效或播放音效。

雖然這不是完整的 Observer Pattern，但具有事件傳遞的概念，可以降低 Model 與 View 之間的耦合，讓遊戲邏輯和畫面表現分離。
### 使用到 AI/AI Agent 的部分

開發過程中有使用codex和chatgpt輔助，主要架構設計、程式設計與整合仍由本人完成。AI Agent 主要用於架構分析、程式重構建議、設計模式判斷、Debug 方向整理，以及報告內容與架構圖的輔助產生。

1. **協助導入 Design Patterns**  
   設計的pattern都是由我構想，並透過AI Agent更快的完成部分需要花費大量時間除錯且複雜的pattern，例如 GameModel 的 State Pattern、UI 操作的 Command Pattern、BuildableRegistry 的 Factory Pattern。原本的設計就是以此為方向，但透過AI可以更快的完成這些設計，讓程式更容易維護與擴充。

2. **協助設計塔、陷阱與子彈的 OOP 繼承架構**  
   AI Agent 協助整理各種塔、陷阱與子彈的共通行為，並輔助設計 IBuildable、TowerBase、AttackTowerBase、TrapBase、ProjectileModel 等架構。透過繼承與多型，不同塔與子彈可以共用介面，但各自實作不同攻擊方式。

3. **協助 Debug 與修正遊戲流程問題**  
   在建塔、升級、賣塔、敵人生成、回合切換與勝敗判斷等功能上，AI Agent 協助分析錯誤原因並提供修正方向，幫助我更快定位問題，也讓 GameModel、GameView、GameController 的分工更清楚。

4. **協助產生報告架構圖與技術說明**  
   AI Agent 協助將程式碼整理成報告可用的架構圖與文字說明，例如 SceneManager 狀態機、HMVC、GameModel 組織架構、塔與陷阱繼承架構、Command Pattern 與 Factory Pattern。這讓報告內容更完整，也更容易讓讀者理解程式設計。
## 結語

### 問題與解決方法
#### 1. 降低耦合

一開始在實作遊戲功能時，許多邏輯都是直接用 if-else 或 switch-case 寫在一起。雖然這樣可以快速完成基本功能，但隨著場景切換、玩家操作、建塔、升級、賣塔、回合狀態等功能越來越多，程式碼開始變得混亂，也讓不同功能之間的耦合度變高。每次要修改一個功能時，都可能需要到很多地方檢查條件式，閱讀和維護上都變得比較困難。

後來我重新複習陳偉凱老師課堂中提到的 design pattern 與 孫老師的OOP，將這些大量條件判斷透過適合的設計模式來整理。因此我將部分邏輯重新拆分，讓不同類別負責不同職責，降低彼此之間的依賴。

例如，遊戲流程狀態拆成 ReadyState、RoundRunningState、PausedState、WinState、LoseState，避免 GameModel 裡充滿判斷目前狀態的 if-else；玩家操作則改成 Command Pattern，將選塔、開始回合、升級、賣塔、返回等操作封裝成不同 Command 類別；塔與陷阱的建立則透過 BuildableRegistry 搭配 Factory Pattern，使 GameModel 不需要直接知道每個具體塔類別。

這樣修改後，程式的責任分工變得比較清楚。GameModel 專注於遊戲資料與規則，Controller 負責輸入與指令，View 負責畫面顯示，SceneManager 負責場景切換。雖然程式類別數量變多，但每個類別的目的更明確，也讓後續新增功能或修改邏輯時比較不容易影響其他部分。
#### 2. 地圖路徑和回合設計
一開始在設計地圖路徑和回合時，我是直接用 case-switch 和 if-else 寫在主要遊戲邏輯中。初期關卡數量還不多時，這樣寫還可以接受，但後來要設計完整 50 回合時，程式裡開始出現大量地圖座標、氣球種類與生成順序，導致 GameModel 變得很雜亂，也讓後續調整難度或新增路線變得不方便。

為了解決這個問題，我後來將地圖路徑與回合資料從主要遊戲邏輯中分離出來，讓 GameModel 不直接寫死所有地圖座標與敵人波次，而是透過 MapModel 管理路徑資料，透過 WaveConfig 管理每回合的敵人組合。

MapModel 負責保存不同難度對應的路徑資料，例如 Easy、Medium、Hard 可以有不同路線設計；WaveConfig 則負責保存 50 回合中每一回合要生成的氣球種類與順序。GameModel 只需要在回合開始時取得目前回合的 wave 設定，並依照設定生成敵人即可。

這樣修改後，GameModel 的職責變得比較單純，只需要負責遊戲流程、敵人生成與物件更新，而地圖資料和回合資料則交給專門的類別管理。未來如果要調整某一關的氣球數量、修改地圖路線，或新增不同難度的地圖，就不需要在 GameModel 裡面翻找大量 if-else，只要修改 MapModel 或 WaveConfig 即可。
#### 3. 異常狀態重複紀錄
因為遊戲裡有冰凍、緩速、爆炸、穿透這些效果，如果沒有記錄同一個敵人是否已經被這次攻擊處理過，就可能出現重複扣血或重複產生特效的問題。所以我用 hit enemies 或 affected enemies 這類集合記錄已處理過的敵人，避免同一次攻擊重複作用。另外，Model 產生的事件在交給 View 後會被 consume 掉，避免下一幀又重複播放同一個特效。
### 自評

| 項次 | 項目 | 完成 |
|------|------|------|
| 1 | 完成 BTD2 基本塔防玩法：建塔、敵人移動、扣血、勝敗判斷 | V |
| 2 | 完成 Start、Difficulty、Game、Result 四個主要場景 | V |
| 3 | 完成 SceneManager 狀態機切換場景 | V |
| 4 | 每個 Scene 皆具有 MVC 分工概念 | V |
| 5 | 完成多種防禦塔與陷阱塔 | V |
| 6 | 完成塔的升級與賣塔功能 | V |
| 7 | 完成多種氣球敵人與分裂規則 | V |
| 8 | 完成不同難度與不同地圖設定 | V |
| 9 | 導入 State Pattern 管理遊戲流程狀態 | V |
| 10 | 導入 Command Pattern 管理玩家操作 | V |
| 11 | 導入 Factory / Singleton Pattern 管理塔的建立 | V |
| 12 | 使用繼承、多型、封裝等 OOP 技術設計塔與子彈系統 | V |
| 13 | 具有 Debug / Cheat Mode 輔助測試功能 | V |
| 14 | 報告包含程式架構圖與設計模式說明 | V |
| 15 | 專案權限已改為 public，並附上 README 與遊戲畫面 | V |

### 心得

從專案一開始，我就打算用 HMVC 的架構來完成這個遊戲。雖然看到有同學選擇大幅修改 PTSD 框架時有點被嚇到，也會擔心自己選的 Bloons Tower Defense 2 比起來太low，但我自己的想法是先降低風險，能做出來成績也過比較重要。因為這是個人專案，我更需要確保每個功能都能完成，而不是一開始就把範圍拉得太大，最後反而無法收尾。

在實作過程中，我發現真正困難的地方不只是把畫面做出來，而是當功能越來越多時，程式會很容易變得混亂。一開始有些場景切換、遊戲狀態、地圖路徑和回合資料都使用 if-else 或 switch-case 寫在一起，短期內可以運作，但後來要設計完整 50 回合、不同難度地圖、建塔、升級、賣塔和異常狀態時，就開始變得很難維護。因此我回頭複習課堂中提到的 OOP 與 design pattern，並把部分程式重新整理成 State Pattern、Command Pattern、Factory Pattern 等架構，讓不同類別各自負責不同工作。

這次專案讓我比較深刻體會到，OOP 不只是把程式拆成很多 class，而是要讓每個 class 的責任清楚。像 GameModel 應該專注在遊戲資料與規則，GameView 負責畫面同步，GameController 負責輸入與指令，SceneManager 則負責場景切換。當我把地圖資料交給 MapModel、回合資料交給 WaveConfig、玩家操作交給 Command 物件後，雖然類別數量變多，但整體反而更容易理解，也比較不會因為修改一個功能影響到其他部分。

完成這個專案後還是蠻有成就感的。雖然我選擇的是風險較低、規則相對明確的遊戲，但從實作過程中還是遇到了許多架構與邏輯問題，也透過重構一步步解決。這次專案讓我學到，設計模式不是為了讓程式看起來很厲害，而是在功能變多、程式開始變亂時，用來控制複雜度的方法。最後能把遊戲做完，並整理出一套自己能解釋的架構，是這次專案中我覺得最有收穫的地方。
### 貢獻比例
我獨自升級
