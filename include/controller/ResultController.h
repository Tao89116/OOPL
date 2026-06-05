//
// Created by polyunicorn on 2026/3/13.
//

#ifndef RESULT_CONTROLLER_H
#define RESULT_CONTROLLER_H

class SceneManager;
class ResultModel;

class ResultController {
public:
    void Update(ResultModel& model, SceneManager& sceneManager);

private:
    bool m_ReturnRequested = false;
};

#endif
