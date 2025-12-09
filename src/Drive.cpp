#include "Drive.h"

Drive::Drive() {
    ledcAttach(MOTOR_FL_PWM, PWM_FREQ, PWM_RES);
    ledcAttach(MOTOR_FR_PWM, PWM_FREQ, PWM_RES);
    ledcAttach(MOTOR_BL_PWM, PWM_FREQ, PWM_RES);
    ledcAttach(MOTOR_BR_PWM, PWM_FREQ, PWM_RES);
    
    pinMode(MOTOR_FL_DIR, OUTPUT);
    pinMode(MOTOR_FR_DIR, OUTPUT);
    pinMode(MOTOR_BL_DIR, OUTPUT);
    pinMode(MOTOR_BR_DIR, OUTPUT);
}

void Drive::setOpenLoop(double vx, double vy, double omega) {
    periodicIO.des_vx = vx;
    periodicIO.des_vy = vy;
    periodicIO.des_omega = omega;
}

void Drive::readPeriodicInputs() {}

void Drive::DriveLoop::onLoop(double timestamp) {
    // Mecanum Kinematics
    double vx = drive_->periodicIO.des_vx;
    double vy = drive_->periodicIO.des_vy;
    double omega = drive_->periodicIO.des_omega;

    double fl = vx - vy - omega;
    double fr = vx + vy + omega;
    double bl = vx + vy - omega;
    double br = vx - vy + omega;

    // Normalize
    double max_speed = std::max({std::abs(fl), std::abs(fr), std::abs(bl), std::abs(br)});
    if (max_speed > 1.0) {
        fl /= max_speed; fr /= max_speed; bl /= max_speed; br /= max_speed;
    }

    drive_->periodicIO.fl_demand = fl;
    drive_->periodicIO.fr_demand = fr;
    drive_->periodicIO.bl_demand = bl;
    drive_->periodicIO.br_demand = br;
}

void Drive::writePeriodicOutputs() {
    // v3.0: Pass the PIN to the helper
    setMotor(MOTOR_FL_PWM, MOTOR_FL_DIR, periodicIO.fl_demand);
    setMotor(MOTOR_FR_PWM, MOTOR_FR_DIR, periodicIO.fr_demand);
    setMotor(MOTOR_BL_PWM, MOTOR_BL_DIR, periodicIO.bl_demand);
    setMotor(MOTOR_BR_PWM, MOTOR_BR_DIR, periodicIO.br_demand);
}

void Drive::setMotor(int pwm_pin, int dir_pin, double speed) {
    bool direction = speed > 0;
    int pwm_val = std::abs(speed) * 255; 
    
    digitalWrite(dir_pin, direction);
    ledcWrite(pwm_pin, pwm_val); 
}

void Drive::stop() {
    setOpenLoop(0, 0, 0);
    writePeriodicOutputs();
}

void Drive::registerEnabledLoops(Looper* looper) {
    looper->registerLoop(new DriveLoop(this));
}