#include "pid.h"

pid::pid()
{
    p_limit = 0;
    p_gain = 0;
    i_limit = 0;
    i_gain = 0;
    d_limit = 0;
    d_gain = 0;
    i_sum = 0;
    d_error_prev = 0;
}

float pid::output(float error)
{
    //calculate initial errors
    float p_error = error * p_gain;
    float i_error = i_sum * i_gain;
    float d_error = error / (float)(millis() / 1000 - last_cycle_time) - d_error_prev;

    //apply limits
    p_error = apply_limit(p_error, p_limit);
    d_error = apply_limit(d_error, d_limit);

    //set parameters for next cycle
    d_error_prev = error;
    last_cycle_time = millis();
    i_sum = apply_limit(i_sum + error * (float)(millis() / 1000 - last_cycle_time), i_limit);

    return p_error + i_error + d_error;
}

float pid::apply_limit(float value, float limit)
{
    if (value > limit)
    {
        return limit;
    }
    if (value < -limit)
    {
        return -limit;
    }
    return value;
}