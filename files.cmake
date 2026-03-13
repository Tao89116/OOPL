set(SRC_FILES
        App.cpp
        ResourceManager.cpp

        model/EnemyModel.cpp
        model/TowerModel.cpp
        model/ProjectileModel.cpp
        model/MapModel.cpp
        model/GameModel.cpp

        view/StartView.cpp
        view/DifficultyView.cpp
        view/GameView.cpp
        view/ResultView.cpp
        view/UIView.cpp

        controller/StartController.cpp
        controller/DifficultyController.cpp
        controller/GameController.cpp
        controller/ResultController.cpp

        scene/StartScene.cpp
        scene/DifficultyScene.cpp
        scene/GameScene.cpp
        scene/ResultScene.cpp
        scene/SceneManager.cpp
)

set(INCLUDE_FILES
        App.h
        GameConfig.h
        ResourceManager.h
        SceneType.h

        scene/IScene.h
        scene/SceneManager.h
        scene/StartScene.h
        scene/DifficultyScene.h
        scene/GameScene.h
        scene/ResultScene.h

        model/EnemyModel.h
        model/TowerModel.h
        model/ProjectileModel.h
        model/MapModel.h
        model/GameModel.h

        view/StartView.h
        view/DifficultyView.h
        view/GameView.h
        view/ResultView.h
        view/UIView.h

        controller/StartController.h
        controller/DifficultyController.h
        controller/GameController.h
        controller/ResultController.h
)

set(TEST_FILES)