#ifndef PENDULUM_EMBEDDED_PENDULUMENVIRONMENT_H
#define PENDULUM_EMBEDDED_PENDULUMENVIRONMENT_H

#include <vector>
#include <cstdint>

#ifndef STM32
#include <iostream>
#endif

class PendulumEnvironment {
private:

    // Constants for the pendulum behavior
    static const double MAX_SPEED;
    static const double MAX_TORQUE;
    static const double TIME_DELTA;
    static const double G;
    static const double MASS;
    static const double LENGTH;

		/**
		* \brief Available actions for the LearningAgent.
		*
		* Each number $a$ in this list, with $a \in ]0.0;1.0], corresponds to two
		* actions available for the LearningAgent: $a*MAX_TORQUE$ and
		* $-a*MAX_TORQUE$.
		* An additional action 0.0 is always available to the LearningAgent.
		*
		* A total of availableAction.size()*2 + 1 actions are thus available to
		* the LearningAgent, through the doAction() method.
		*/
    const std::vector<double> availableActions;
		
		double generateRandDouble(double min, double max);

public:
    /// Setter for angle state
    void setAngle(double newValue);

    /// Setter for velocity
    void setVelocity(double newValue);

    /// Current angle and velocity provided to the LearningAgent
    /// currentState[0] --> Current angle of the pendulum in [-M_PI; M_PI]
    /// currentState[1] --> Current velocity of the pendulum in [-1;1]
    /// This must be public to be exposed to the inference program
    double currentState[2];

    /**
    * \brief Default constructor.
    *
    * Attributes angle and velocity are set to 0.0 by default.
    */
    PendulumEnvironment(const std::vector<double>& actions) :
            availableActions(actions)
    {
			currentState[0] = 0.0;
			currentState[1] = 0.0;
		};

    /// Getter for angle state
    double getAngle() const;

    /// Getter for velocity
    double getVelocity() const;

    /// Resets the environment
    void reset(size_t seed = 0);

		/**
		* \brief Get the action from its associated ID.
		*
		* \param[in] actionID the integer representing the selected action.
		* \return the torque applied to the pendulum.
		*/
    double getActionFromID(const uint64_t& actionID);

    /// Inherited via LearningEnvironment
    void doAction(uint64_t& actionID);

    /// Starts the inference for nbSteps
    void startInference(int nbSteps);

};

#ifndef STM32
std::ostream& operator<<(std::ostream& os, const PendulumEnvironment& pendulum);
#endif

#endif //PENDULUM_EMBEDDED_PENDULUMENVIRONMENT_H
