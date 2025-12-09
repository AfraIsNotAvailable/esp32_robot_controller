#pragma once

class Loop {
public:
    virtual void onStart(double timestamp) = 0;
    virtual void onLoop(double timestamp) = 0;
    virtual void onStop(double timestamp) = 0;
    virtual ~Loop() {}
};