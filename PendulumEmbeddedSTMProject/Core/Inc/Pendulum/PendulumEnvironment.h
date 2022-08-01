#ifndef PENDULUMENVIRONMENT_H
#define PENDULUMENVIRONMENT_H

#include <vector>
#include <cstdint>

#include "ina219.h"

#ifndef STM32
#include <iostream>
#endif

/**
 * \brief Pendulum environment.
 * 
 * This environment is based on the pendulum from the gegelati-apps repository.
 * It only keeps  the pendulum simulation part and use a CodeGen TPG to
 * do action inferences.
 * CodeGen files (pendulum.h/.c and pendulum_program.h/.c) are thus required
 * for this environment.
 */
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
	* \brief Available actions.
	*
	* Each number a in this list, with a in ]0.0;1.0], corresponds to two
	* actions : a * MAX_TORQUE and -a*MAX_TORQUE.
	* An additional action 0.0 is always available.
	*
	* A total of availableAction.size()*2 + 1 actions are thus available
	* through the doAction() method.
	*/
    const std::vector<double> availableActions;

    /// Generate a random double number between min and max using rand().
	double generateRandDouble(double min, double max);

	/// Current step during inference, -1 otherwise. Used by interrupt code and for debug.
	volatile int currentStep;

public:
    /// Setter for angle state.
    void setAngle(double newValue);

    /// Setter for velocity.
    void setVelocity(double newValue);

    /// Current angle and velocity provided to the LearningAgent :
    /// - currentState[0] --> Current angle of the pendulum in [-M_PI; M_PI]
    /// - currentState[1] --> Current velocity of the pendulum in [-1;1]
    /// This must be public to be exposed to the inference program.
    double currentState[2];

    /**
    * \brief Basic constructor.
    *
    * Attributes angle and velocity are set to 0.0.
    */
    PendulumEnvironment(const std::vector<double>& actions) :
            availableActions(actions), currentStep(-1)
    {
			currentState[0] = 0.0;
			currentState[1] = 0.0;
	};

    /// Getter for angle state.
    double getAngle() const;

    /// Getter for velocity.
    double getVelocity() const;

    /// Return the current inference step.
    int getCurrentStep() const;

    /// Reset the environment.
    void reset(size_t seed = 0);

	/**
	* \brief Get the action from its associated ID.
	*
	* \param[in] actionID the integer representing the selected action.
	* \return the torque applied to the pendulum.
	*/
    double getActionFromID(const uint64_t& actionID);

    /**
     * \brief Perform an action on the environment.
     * 
     * \param actionID the index of the action in availableActions. 
     */
    void doAction(uint64_t& actionID);

    /// Start nbSteps inferences and simulation steps using the code gen TPG.
    void startInference(int nbSteps);


};

inline int PendulumEnvironment::getCurrentStep() const { return this->currentStep; }


#ifdef PENDULUM_TRACE
/// Print a PendulumEnvironment in a stream.
std::ostream& operator<<(std::ostream& os, const PendulumEnvironment& pendulum);
#endif

#endif //PENDULUMENVIRONMENT_H
