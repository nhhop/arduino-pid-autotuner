#include <pidautotuner.h>

float targetValue = 60;
long loopInterval = 2000;

long outputMin = 0;
long outputMax = 255;

void setup() {

    PIDAutotuner tuner = PIDAutotuner();

    // Set the target value to tune to
    // This will depend on what you are tuning. This should be set to a value within
    // the usual range of the setpoint. For low-inertia systems, values at the lower
    // end of this range usually give better results. For anything else, start with a
    // value at the middle of the range.
    tuner.setTargetInputValue(targetValue);

    // Set the loop interval in microseconds
    // This must be the same as the interval the PID control loop will run at
    tuner.setLoopInterval(loopInterval);

    // Set the output range
    // These are the minimum and maximum possible output values of whatever you are
    // using to control the system (Arduino analogWrite, for example, is 0-255)
    tuner.setOutputRange(outputMin, outputMax);

    // Set the Ziegler-Nichols tuning mode
    // Set it to either PIDAutotuner::ZNModeBasicPID, PIDAutotuner::ZNModeLessOvershoot,
    // or PIDAutotuner::ZNModeNoOvershoot. Defaults to ZNModeNoOvershoot as it is the
    // safest option.
    tuner.setZNMode(PIDAutotuner::ZNModeBasicPID);

    // This must be called immediately before the tuning loop
    // Must be called with the current time in microseconds
    tuner.startTuningLoop(micros());

    // Run a loop until tuner.isFinished() returns true
    long microseconds;
    while (!tuner.isFinished()) {

        // This loop must run at the same speed as the PID control loop being tuned
        long prevMicroseconds = microseconds;
        microseconds = micros();

        // Get input value here (temperature, encoder position, velocity, etc)
        double input = doSomethingToGetInput();

        // Call tunePID() with the input value and current time in microseconds
        double output = tuner.tunePID(input, microseconds);

        // Set the output - tunePid() will return values within the range configured
        // by setOutputRange(). Don't change the value or the tuning results will be
        // incorrect.
        doSomethingToSetOutput(output);

        // This loop must run at the same speed as the PID control loop being tuned
        while (micros() - microseconds < loopInterval) delayMicroseconds(1);
    }

    // Turn the output off here.
    doSomethingToSetOutput(0);

    // Get PID gains - set your PID controller's gains to these
    double kp = tuner.getKp();
    double ki = tuner.getKi();
    double kd = tuner.getKd();
}

void loop() {

    // ...
}


double doSomethingToGetInput()
{
  //  ...
  return 0;
}

void doSomethingToSetOutput(double setValue)
{
  // ...
}