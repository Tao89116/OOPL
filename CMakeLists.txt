cmake_minimum_required(VERSION 3.16)
project(BLOONSTD)

set(CMAKE_CXX_STANDARD 17)

cmake_policy(SET CMP0135 NEW)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(
    ptsd

    URL         https://github.com/ntut-open-source-club/practical-tools-for-simple-design/archive/refs/tags/v0.2.zip

    # Or alternatively if you want to use a specific git commit instead of a release:
    # GIT_REPOSITORY  https://github.com/ntut-open-source-club/practical-tools-for-simple-design.git
    # GIT_TAG         <git_tag>

    SOURCE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/PTSD
)

FetchContent_MakeAvailable(ptsd)

include(files.cmake)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
list(TRANSFORM SRC_FILES PREPEND ${SRC_DIR}/)
list(TRANSFORM INCLUDE_FILES PREPEND ${INCLUDE_DIR}/)

# file(GLOB_RECURSE SRC_FILES src/*)
# file(GLOB_RECURSE HEADER_FILES include/*)

add_executable(${PROJECT_NAME}
    ${SRC_DIR}/main.cpp
    ${SRC_FILES}
    ${HEADER_FILES}
        include/ResourceManager.h
        src/ResourceManager.cpp
        include/GameConfig.h
        include/model/EnemyModel.h
        include/model/HitEffectEvent.h
        include/model/HitEffectEmitter.h
        src/model/EnemyModel.cpp
        include/model/ProjectileModel.h
        src/model/ProjectileModel.cpp
        include/model/MapModel.h
        src/model/MapModel.cpp
        include/model/GameModel.h
        src/model/GameModel.cpp
        include/model/StartModel.h
        src/model/StartModel.cpp
        include/View/UIView.h
        src/View/UIView.cpp
        include/View/GameView.h
        include/view/GameUILayout.h
        src/View/GameView.cpp
        include/Controller/GameController.h
        src/Controller/GameController.cpp
        include/Scene/GameScene.h
        src/Scene/GameScene.cpp
        include/SceneType.h
        include/scene/IScene.h
        include/scene/SceneManager.h
        src/scene/SceneManager.cpp
        src/View/StartView.cpp
        include/View/StartView.h
        include/Controller/StartController.h
        src/Controller/StartController.cpp
        include/scene/StartScene.h
        src/scene/StartScene.cpp
        include/View/DifficultyView.h
        src/View/DifficultyView.cpp
        include/Controller/DifficultyController.h
        src/Controller/DifficultyController.cpp
        include/scene/DifficultyScene.h
        src/scene/DifficultyScene.cpp
        include/View/ResultView.h
        src/View/ResultView.cpp
        include/Controller/ResultController.h
        src/Controller/ResultController.cpp
        include/scene/ResultScene.h
        src/scene/ResultScene.cpp
        include/model/MapData.h
        include/model/MapFactory.h
        include/model/PlacementModel.h
        src/model/MapFactory.cpp
        src/model/PlacementModel.cpp
        include/model/BuildableRegistry.h
        include/model/TowerBase.h
        include/model/AttackTowerBase.h
        include/model/TrapBase.h
        src/model/BuildableRegistry.cpp
        src/model/TowerBase.cpp
        src/model/AttackTowerBase.cpp
        src/model/TrapBase.cpp
        include/model/ConcreteTowers.h
        src/model/ConcreteTowers.cpp
        include/model/DifficultyModel.h
        src/model/DifficultyModel.cpp
        include/model/IBuildable.h
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/Resources")
else()
    message(FATAL_ERROR "relative RESOURCE_DIR is WIP, Please use `-DCMAKE_BUILD_TYPE=Debug` build for now.")
    # target_compile_definitions(${PROJECT_NAME} PRIVATE RESOURCE_DIR="${CMAKE_INSTALL_PREFIX}/Resources")
endif()

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${DEPENDENCY_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/PTSD/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME}
    SDL2::SDL2main
    PTSD
)
