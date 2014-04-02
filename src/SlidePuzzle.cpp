#include "SlidePuzzle.hpp"
#include <ctime>
#include <cassert>
#include <iostream>


SlidePuzzle::SlidePuzzle():
    start_state_(nullptr),
    end_state_(nullptr)
{
}


SlidePuzzle::~SlidePuzzle()
{
    for (StateIndex::iterator it = states_.begin(); it != states_.end(); ++it) {
        delete *it;
    }
}


bool SlidePuzzle::open(const std::string& filename)
{
    std::ifstream file(filename.c_str());
    if (file) {
        int nb_lines = 0, nb_cols = 0;
        file >> nb_lines >> nb_cols;
        State::init_dimensions(nb_lines, nb_cols);
        start_state_ = new State();
        start_state_->load_initial(file);
        std::cout << *start_state_ << std::endl;
        file.close();
        return true;
    }
    return false;
}


bool SlidePuzzle::add_state(State* state)
{
    StateIndex::const_iterator it = states_.find(state);
    if (it == states_.end()) {
        // State contains an unknown grid, insert state in the exploration queue
        queue_.insert(state);
        states_.insert(state);
        return true;
    }
    // Discard state (already indexed in states_)
    delete state;
    return false;
}


bool SlidePuzzle::solve()
{
    clock_t t1 = std::clock();

    // Put initial state in the exploration queue
    queue_.prepend(start_state_);
    states_.insert(start_state_);

    while (!queue_.is_empty()) {
        State* current = queue_.pop();
        if (current->is_solved()) {
            end_state_ = current;
            break;
        }
        else {
            // Create new states to explore from the current state
            if (State* derived = current->derive(State::UP)) {
                add_state(derived);
            }
            if (State* derived = current->derive(State::RIGHT)) {
                add_state(derived);
            }
            if (State* derived = current->derive(State::DOWN)) {
                add_state(derived);
            }
            if (State* derived = current->derive(State::LEFT)) {
                add_state(derived);
            }
        }
    }
    clock_t t2 = std::clock();
    if (end_state_) {
        std::printf(
            "Solution found in %.2f seconds (%d moves)\n\n",
            (double) (t2 - t1) / CLOCKS_PER_SEC,
            end_state_->get_depth()
        );
    }
    else {
        std::cout << "No solution found for this puzzle." << std::endl;
    }
    return end_state_ != nullptr;
}


void SlidePuzzle::write_solution() const
{
    // Allocate memory to store path from initial state to final state
    const State** path = new const State* [end_state_->get_depth()];
    const State* current = end_state_;
    int i;
    for (i = 0; current != start_state_; ++i) {
        path[i] = current;
        current = current->get_parent();
    }

    // Read array backward to print solution in correct order
    for (--i; i >= 0; --i) {
        std::cout << *path[i];
    }
    delete [] path;
}
