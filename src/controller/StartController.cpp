#include "controller/StartController.h"

#include "model/StartModel.h"

void StartController::Update(StartModel& model, float deltaTimeMs) {
    model.Update(deltaTimeMs);
}
