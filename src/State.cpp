#include "State.hpp"

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>


int State::NbLines = 0;
int State::NbCols = 0;
size_t State::GridSize = 0;


void State::init_dimensions(int nb_lines, int nb_cols)
{
    State::NbLines = nb_lines;
    State::NbCols = nb_cols;
    State::GridSize = sizeof (int) * nb_lines * nb_cols;
}


State::State():
    next(nullptr),
    direction_(INITIAL),
    parent_(nullptr),
    depth_(0),
    manhattan_(0),
    heuristic_(0)
{
    grid_ = new int [NbCols * NbLines];
}


State::~State()
{
    delete [] grid_;
}


bool State::is_solved() const
{
    return manhattan_ == 0;
}


int State::get_depth() const
{
    return depth_;
}


void State::load_initial(std::istream& in)
{
    for (int i = 0; i < NbLines; ++i) {
        for (int j = 0; j < NbCols; ++j) {
            int index = NbCols * i + j;
            in >> grid_[index];
            if (in.fail()) {
                // Parsing fails on '#' character, which is the empty slot
                grid_[index] = 0;
                empty_tile_.y = i;
                empty_tile_.x = j;
                in.clear();
                in.ignore();
            }
        }
    }

    parent_ = nullptr;
    depth_ = 0;
    direction_ = INITIAL;
    compute_heuristic();
}


State* State::derive(Direction dir) const
{
    Vector2 dest = empty_tile_;
    // Apply given direction if it's a valid move
    switch (dir) {
    case UP:
        if (empty_tile_.y > 0) {
            --dest.y;
            break;
        }
        return nullptr;
    case DOWN:
        if (empty_tile_.y < NbLines - 1) {
            ++dest.y;
            break;
        }
        return nullptr;
    case LEFT:
        if (empty_tile_.x > 0) {
            --dest.x;
            break;
        }
        return nullptr;
    case RIGHT:
        if (empty_tile_.x < NbCols - 1) {
            ++dest.x;
            break;
        }
        return nullptr;
    default:
        std::cerr << "Unhandled direction " << dir << std::endl;
        return nullptr;
    }

    State* derived = new State();
    derived->parent_ = this;
    std::memcpy(derived->grid_, grid_, GridSize);

    // Copy value from empty_tile_ to dest
    int index_dest = NbCols * dest.y + dest.x;
    derived->grid_[NbCols * empty_tile_.y + empty_tile_.x] = derived->grid_[index_dest];
    // Set dest as the new state empty tile
    derived->grid_[index_dest] = 0;
    derived->empty_tile_ = dest;
    derived->direction_ = dir;

    // Increase number of performed moves
    derived->depth_ = depth_ + 1;
    derived->compute_heuristic();
    return derived;
}


const State* State::get_parent() const
{
    return parent_;
}


bool State::closer_to_end_than(const State& other) const
{
    return heuristic_ < other.heuristic_
        || (heuristic_ == other.heuristic_ && manhattan_ < other.manhattan_);
}


void State::compute_heuristic()
{
    // Sum of manhattan distance for each tile:
    // https://en.wikipedia.org/wiki/Taxicab_geometry
    manhattan_ = 0;
    for (int y = 0; y < NbLines; ++y) {
        for (int x = 0; x < NbCols; ++x) {
            int value = grid_[NbCols * y + x];
            // No cost for empty tile (value 0)
            if (value) {
                // Value N should be located at index N-1 in the final state
                --value;
                // Delta between coords in this state, and coords in the final state
                manhattan_ += abs(y - (value / NbCols)) + abs(x - (value % NbCols));
            }
        }
    }

    // Add depth_ in the heuristic to find optimal solutions
    // Removing depth increases solving speed, but gives non-optimal solutions
    heuristic_ = manhattan_ + depth_;
}


std::ostream& operator<<(std::ostream& out, const State& state)
{
    out << '[' << state.depth_ << "] ";
    switch (state.direction_) {
    case State::INITIAL:
        out << "Initial\n";
        break;
    case State::UP:
        out << "Up\n";
        break;
    case State::RIGHT:
        out << "Right\n";
        break;
    case State::DOWN:
        out << "Down\n";
        break;
    case State::LEFT:
        out << "Left\n";
        break;
    }

    for (int y = 0; y < State::NbLines; ++y) {
        for (int x = 0; x < State::NbCols; ++x) {
            int value = state.grid_[State::NbCols * y + x];
            if (value) {
                out.width(3);
                out << value;
            }
            else {
                out << "  #";
            }
        }
        out << '\n';
    }
    return out;
}
