# Slide Puzzle Solver

## Presentation

This is a C++ solver for 15-puzzle games (also known as *slide puzzle* or *taquin*):
https://en.wikipedia.org/wiki/15_puzzle

Solver is not limited to 4x4 puzzles and works with any size of grids.

Objective is to find the smallest sequence of moves leading to the winning grid (all numbers are ordered).

Grid exploration is based on A\* pathfinding (find path from the given starting grid to the ordered grid).

Each grid can generate up to 4 child grids (up, down, right, left). New grids are
placed in a priority queue, ordered by heuristic (grids closer to being ordered
will be explored first).

## Usage

    ./slide-puzzle <file-path>

*file-path* is a text file describing the grid to solve (see format description below).

The sequence of moves will be printer to stdout.

## File format

- Values (number) are separater by whitespace characters
- First line contains number of lines and number of columns
- Then, one file line for each grid line
- The empty tile is represented by the '#' character

Example for a 3x4 grid:

```
3 4
  7  9  8  6
  2  3 10 11
  #  4  5  1
```

## About

- Author: Alexandre Bodelot <alexandre.bodelot@gmail.com>
- License: MIT
