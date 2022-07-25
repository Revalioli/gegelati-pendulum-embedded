#ifndef PENDULUM_H
#define PENDULUM_H

#include <gegelati.h>

/**
* \brief Inverted pendulum LearningEnvironment.
*
* The code of the class is adapted from Florian Arrestier's code released
* under CECILL-C License.
* Link: https://github.com/preesm/preesm-apps/tree/master/org.ietr.preesm.reinforcement_learning
*/
class Pendulum : public Learn::LearningEnvironment
{
private:
	// Constants for the pendulum behavior
	static const double MAX_SPEED;
	static const double MAX_TORQUE;
	static const double TIME_DELTA;
	static const double G;
	static const double MASS;
	static const double LENGTH;
	static const size_t REWARD_HISTORY_SIZE = 300;
	static const double STABILITY_THRESHOLD;

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

	/// Randomness control
	Mutator::RNG rng;

	/// Reward history for score computation
	double rewardHistory[REWARD_HISTORY_SIZE];

	/// Total reward accumulated since the last reset
	double totalReward = 0.0;

	/// Number of actions since the last reset
	uint64_t nbActionsExecuted = 0;

	/// Copy of current angle and velocity provided to the LearningAgent
	/// Current angle of the pendulum in [-M_PI; M_PI]
	/// Current velocity of the pendulum in [-1;1]
	Data::PrimitiveTypeArray<double> currentState;

protected:
	/// Setter for angle state
	void setAngle(double newValue);

	/// Setter for velocity
	void setVelocity(double newValue);

public:

	/**
	* \brief Default constructor.
	*
	* Attributes angle and velocity are set to 0.0 by default.
	*/
	Pendulum(const std::vector<double>& actions) :
		LearningEnvironment(actions.size() * 2 + 1), // see availableActions comment.
		availableActions{ actions },
		currentState{ 2 }
	{};

	/**
	* \brief Copy constructor for the Pendulum.
	*
	* Default copy constructor since all attributes are trivially copyable.
	*/
	Pendulum(const Pendulum& other) = default;

	/// Getter for angle state
	double getAngle() const;

	/// Getter for velocity
	double getVelocity() const;

	/// Inherited via LearningEnvironment
	virtual std::vector<std::reference_wrapper<const Data::DataHandler>> getDataSources() override;

	/// Inherited via LearningEnvironment
	virtual void reset(size_t seed = 0, Learn::LearningMode mode = Learn::LearningMode::TRAINING) override;

	/**
	* \brief Get the action from its associated ID.
	*
	* \param[in] actionID the integer representing the selected action.
	* \return the torque applied to the pendulum.
	*/
	double getActionFromID(const uint64_t& actionID);

	/// Inherited via LearningEnvironment
	virtual void doAction(uint64_t actionID) override;

	/// Inherited via LearningEnvironment
	virtual bool isCopyable() const override;

	/// Inherited via LearningEnvironment
	virtual LearningEnvironment* clone() const;

	/**
	* \brief Get a score for the pendulum stabilization.
	*
	* The score returned at any time can either be positive or negative.
	*
	* A positive score is returned if the pendulum has been stabilized, that is,
	* the isTerminal() method returns true.
	* In such a case, the returned score will be $10 / ln(nbActionExecuted)$
	* such that shorter convergence time leads to higher scores.
	*
	* A negative score is returned if the pendulum has not been stabilized
	* (yet).
	* In such a case, the returned score simply is the average reward since
	* the last reset.
	*
	* \return a double value corresponding to the score.
	*/
	virtual double getScore() const override;

	/**
	* \brief Is the pendulum considered stabilized.
	*
	* If the mean reward over the recent rewardHistory is lower than a fixed
	* threshold, then the pendulum is considered to be stable in the upward
	* position and the learningAgent has succeded in stabilizing it.
	*
	* \return true if the pendulum has been stabilized, false otherwise.
	*/
	virtual bool isTerminal() const override;
};

#endif // !PENDULUM_H