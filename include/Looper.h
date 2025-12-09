#pragma once
#include <vector>
#include "Loop.h"
#include <Arduino.h>

class Looper {
public:
    Looper(double period) : period_(period), running_(false) {}

    void registerLoop(Loop* loop) {
        loops_.push_back(loop);
    }

    void start() {
        if (running_) return;
        running_ = true;
        xTaskCreatePinnedToCore(task, "Looper", 8192, this, 1, &taskHandle_, 1);
    }

    void stop() {
        running_ = false;
    }

private:
    static void task(void* params) {
        Looper* looper = (Looper*)params;
        TickType_t xLastWakeTime = xTaskGetTickCount();
        const TickType_t xFrequency = pdMS_TO_TICKS(looper->period_ * 1000);

        // onStart
        double now = micros() / 1e6;
        for (auto loop : looper->loops_) loop->onStart(now);

        while (looper->running_) {
            now = micros() / 1e6;
            for (auto loop : looper->loops_) loop->onLoop(now);
            vTaskDelayUntil(&xLastWakeTime, xFrequency);
        }

        // onStop
        now = micros() / 1e6;
        for (auto loop : looper->loops_) loop->onStop(now);
        vTaskDelete(NULL);
    }

    double period_;
    bool running_;
    std::vector<Loop*> loops_;
    TaskHandle_t taskHandle_;
};