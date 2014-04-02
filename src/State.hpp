#ifndef STATE_HPP
#define STATE_HPP

#include <fstream>
#include <string>
#include <cstring>


class State {
public:
    enum Direction {
        INITIAL, UP, DOWN, LEFT, RIGHT
    };

    // Static because all states shared the same dimensions
    static int NbLines;
    static int NbCols;
    static size_t GridSize;

    /**
     * Initialize NbLines, NbCols, and GridSize
     */
    static void init_dimensions(int lines, int cols);

    State();
    ~State();

    /**
     * @return true if state grid is the end state grid.
     */
    bool is_solved() const;

    /**
     * Get number of parent states (= number of moves)
     */
    int get_depth() const;

    /**
     * Generate a new state, by copying current state and moving the empty tile
     * @param dir: move empty tile in the given direction
     * @return new state if direction is valid, otherwise nullptr
     */
    State* derive(Direction dir) const;

    /**
     * Load initial state from a stream.
     * See README.md for expected format description.
     */
    void load_initial(std::istream& in);

    /**
     * Parent state, from which this state was generated.
     * @return ptr to parent, nullptr when state is initial state
     */
    const State* get_parent() const;

    /**
     * Check if state is closer to the end state than the given state
     * See /compute_heuristic for "closer" definition.
     * @param s: state to compare
     */
    bool closer_to_end_than(const State& s) const;

    // Ptr to next node in StateQueue (linked list)
    State* next;

private:
    friend struct StateGridCompare;
    friend std::ostream& operator<<(std::ostream&, const State&);

    struct Vector2 {
        int x;
        int y;
    };

    /**
     * Compute heuristic for "how close" the end state is.
     * States with a lesser heuristic value will be explored first.
     */
    void compute_heuristic();

    int* grid_;
    Vector2 empty_tile_;
    Direction direction_;
    const State* parent_;

    int depth_;
    int manhattan_;
    int heuristic_; // Cache value for depth + manhattan.
};

std::ostream& operator<<(std::ostream& stream, const State& state);

struct StateGridCompare {
    /**
     * LessThan compare functor, used for std::set.
     * State identity is based on grid values.
     */
    inline bool operator()(const State* a, const State* b) const
    {
        return std::memcmp(a->grid_, b->grid_, State::GridSize) < 0;
    }
};

#endif // STATE_HPP
