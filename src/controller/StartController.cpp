#include "controller/StartController.h"

#include "model/StartModel.h"

int StartController::Update(StartModel& model, float deltaTimeMs) {
    return model.Update(deltaTimeMs);
}
