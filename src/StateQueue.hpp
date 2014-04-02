#ifndef STATE_QUEUE_HPP
#define STATE_QUEUE_HPP

class State;

/**
 * Priority queue for State objects
 */
class StateQueue
{
public:
    StateQueue();
    ~StateQueue();

    /**
     * Prepend given state in queue (state becomes the first element)
     * @param state: state to insert
     */
    void prepend(State* state);

    /**
     * Insert given state in queue. States are kept ordered in the queue (sorted
     * on state heuristic value).
     * @param state: state to insert
     */
    void insert(State* state);

    /**
     * Remove and return first element in queue.
     * @return first element if queue is not empty, otherwise nullptr
     */
    State* pop();

    /**
     * @return true if queue is empty, otherwise false
     */
    bool is_empty() const;

private:
    State* head_;
};

#endif // STATE_QUEUE_HPP
