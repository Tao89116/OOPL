#ifndef START_CONTROLLER_H
#define START_CONTROLLER_H

class StartModel;

class StartController {
public:
    int Update(StartModel& model, float deltaTimeMs);
};

#endif
