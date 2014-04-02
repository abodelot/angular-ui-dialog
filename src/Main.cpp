/**
 * Solutionneur de jeux de taquin
 * @author Alexandre Bodelot <alexandre.bodelot@gmail.com>
 * @date 2011-02-11
 */
#include "SlidePuzzle.hpp"

#include <iostream>
#include <string>


int main(int argc, char *argv[])
{
    if (argc == 2) {
        std::string path = argv[1];

        SlidePuzzle solver;
        if (solver.open(path)) {
            if (solver.solve()) {
                solver.write_solution();
            }
        }
        else {
            std::cerr << "Cannot open file " << argv[1] << std::endl;
            return 1;
        }
    }
    else {
        std::cerr << "Usage: " << argv[0] << " <filepath>" << std::endl;
        return 1;
    }
    return 0;
}
