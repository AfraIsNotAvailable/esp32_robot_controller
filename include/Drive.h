#pragma once
#include "Subsystem.h"
#include "Constants.h"
#include <Arduino.h>
#include <algorithm>
#include <cmath>

class Drive : public Subsystem {
public:
    static Drive& getInstance() {
        static Drive instance;
        return instance;
    }

    struct PeriodicIO {
        double des_vx = 0;
        double des_vy = 0;
        double des_omega = 0;

        // OUTPUTS
        double fl_demand = 0;
        double fr_demand = 0;
        double bl_demand = 0;
        double br_demand = 0;
    };

    void setOpenLoop(double vx, double vy, double omega);
    void readPeriodicInputs() override;
    void writePeriodicOutputs() override;
    void registerEnabledLoops(Looper* looper) override;
    void stop() override;

private:
    Drive();
    PeriodicIO periodicIO;

    void setMotor(int pwm_pin, int dir_pin, double speed);

    class DriveLoop : public Loop {
        Drive* drive_;
    public:
        DriveLoop(Drive* d) : drive_(d) {}
        void onStart(double timestamp) override {}
        void onLoop(double timestamp) override;
        void onStop(double timestamp) override { drive_->stop(); }
    };
};