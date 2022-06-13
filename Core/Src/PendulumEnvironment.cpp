#include "PendulumEnvironment.h"

#ifndef STM32
#define _USE_MATH_DEFINES
#else
#define M_PI           3.14159265358979323846  /* pi */
#endif
#include <cmath>
#include <vector>
#include <cstdlib>
#include <cstdint>


#ifndef STM32
#include <iostream>
#endif


extern "C" {
    #include "pendulum.h"
    double* in1;    // Pointer shared with the TPG c code to access environment data
}

const double PendulumEnvironment::MAX_SPEED = 8.0;
const double PendulumEnvironment::MAX_TORQUE = 2.0;
const double PendulumEnvironment::TIME_DELTA = 0.05;
const double PendulumEnvironment::G = 9.81;
const double PendulumEnvironment::MASS = 1.0;
const double PendulumEnvironment::LENGTH = 1.0;

double PendulumEnvironment::generateRandDouble(double min, double max){
	double r = (double)rand() / RAND_MAX;
	return (r * (max - min)) + min;
}

void PendulumEnvironment::setAngle(double newValue) {
    this->currentState[0] = newValue;
}

void PendulumEnvironment::setVelocity(double newValue) {
    this->currentState[1] = newValue;
}

double PendulumEnvironment::getAngle() const {
    return this->currentState[0];
}

double PendulumEnvironment::getVelocity() const {
    return this->currentState[1];
}

void PendulumEnvironment::reset(size_t seed) {
    srand(seed);

    setAngle(generateRandDouble(-M_PI, M_PI));
    setVelocity(generateRandDouble(-1.0, 1.0));
}

double PendulumEnvironment::getActionFromID(const uint64_t &actionID) {
    double result = (actionID == 0) ? 0.0 : this->availableActions.at((actionID - 1) % availableActions.size());
    return (actionID <= availableActions.size()) ? result : -result;
}

void PendulumEnvironment::doAction(uint64_t& actionID) {
    // Get the action
    double currentAction = getActionFromID(actionID);
    currentAction *= PendulumEnvironment::MAX_TORQUE;

    // Get current state
    double angle = this->getAngle();
    double velocity = this->getVelocity();

    // Update angular velocity
    velocity = velocity + ((-3.0) * G / (2.0 * LENGTH) * (sin(angle + M_PI)) +
                           (3.f / (MASS * LENGTH * LENGTH)) * currentAction) * TIME_DELTA;
    velocity = std::fmin(std::fmax(velocity, -MAX_SPEED), MAX_SPEED);

    // Update angle
    angle = angle + velocity * TIME_DELTA;

    // Save new pendulum state
    this->setAngle(angle);
    this->setVelocity(velocity);
}


void PendulumEnvironment::startInference(int nbSteps){

	for(int i = 0; i < nbSteps; i++){
		uint64_t action = (uint64_t)inferenceTPG();
		this->doAction(action);

#ifndef STM32
		std::cout << *this << " === Step " << i << ", action : " << getActionFromID(action) << std::endl;
#endif
	}
}


#ifndef STM32
std::ostream& operator<<(std::ostream& os, const PendulumEnvironment& pendulum){
    return os << "Pendulum current state : { Angle = " << pendulum.getAngle() << ", Velocity = " << pendulum.getVelocity() << "}";
}
#endif
