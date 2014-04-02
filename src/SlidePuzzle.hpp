#ifndef SLIDE_PUZZLE_HPP
#define SLIDE_PUZZLE_HPP

#include "State.hpp"
#include "StateQueue.hpp"

#include <set>

class SlidePuzzle {
public:
    SlidePuzzle();
    ~SlidePuzzle();

    /**
     * Load puzzle from file
     * @param filename: path to text file
     */
    bool open(const std::string& filename);

    /**
     * Solve puzzle (find moves from initial to final state)
     * @return true if solution found, otherwise false
     */
    bool solve();

    /**
     * Print solution
     */
    void write_solution() const;

private:
    /**
     * Insert given state in the exploration queue, granting the state grid has
     * not been generated yet.
     * If a state with the same grid already exists, the given state will be deleted.
     * @param state: new state to insert
     * @return true if state was inserted, otherwise false
     */
    bool add_state(State* state);

    State* start_state_; // Initial state, from which the solution must be found
    const State* end_state_; // Final state (grid is ordered)
    StateQueue queue_; // Priority queue containing states to explore

    typedef std::set<const State*, StateGridCompare> StateIndex;
    StateIndex states_; // All generated states
};

#endif // SLIDE_PUZZLE_HPP
