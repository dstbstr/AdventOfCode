#include <iostream>
#include <array>
#include <format>
enum Dir { Left = 0, Up = 1, Right = 2, Down = 3 };
using Grid = std::array<std::array<int, 4>, 4>;

constexpr bool Shift(int& lhs, int& rhs) {
    if (rhs == 0) return false;
    if (lhs == 0) {
        std::swap(lhs, rhs);
        return false;
    }
    else if (lhs == rhs) {
        lhs += rhs;
        rhs = 0;
    }
    return true;
}
constexpr void MoveLeft(Grid& grid) {
    for (size_t row = 0; row < grid.size(); row++) {
        for (size_t col = 0; col < grid[row].size(); col++) {
            auto& lhs = grid[row][col];
            for (size_t n = col + 1; n < grid[row].size(); n++) {
                auto& rhs = grid[row][n];
                if (Shift(lhs, rhs)) break;
            }
        }
    }
}
constexpr void MoveRight(Grid& grid) {
    for (size_t row = 0; row < grid.size(); row++) {
        for (size_t col = grid[row].size(); col > 0; col--) {
            auto& lhs = grid[row][col - 1];
            for (size_t n = col - 1; n > 0; n--) {
                auto& rhs = grid[row][n - 1];
                if (Shift(lhs, rhs)) break;
            }
        }
    }
}
constexpr void MoveUp(Grid& grid) {
    for (size_t col = 0; col < grid[0].size(); col++) {
        for (size_t row = 0; row < grid.size(); row++) {
            auto& lhs = grid[row][col];
            for (size_t n = row + 1; n < grid.size(); n++) {
                auto& rhs = grid[n][col];
                if (Shift(lhs, rhs)) break;
            }
        }
    }
}

constexpr void MoveDown(Grid& grid) {
    for (size_t col = 0; col < grid[0].size(); col++) {
        for (size_t row = grid.size(); row > 0; row--) {
            auto& lhs = grid[row - 1][col];
            for (size_t n = row - 1; n > 0; n--) {
                auto& rhs = grid[n - 1][col];
                if (Shift(lhs, rhs)) break;
            }
        }
    }
}

constexpr void Move(Grid& grid, Dir dir) {
    switch (dir) {
    case Left: MoveLeft(grid); break;
    case Right: MoveRight(grid); break;
    case Up: MoveUp(grid); break;
    case Down: MoveDown(grid); break;
    }
}

static constexpr Grid TestGrid = { {
    { 2, 0, 0, 2},
    { 4, 16, 8, 2},
    { 2, 64, 32, 4},
    { 1024, 1024, 64, 0}
} };
constexpr bool TestLeft() {
    Grid grid = TestGrid;
    Move(grid, Left);
    return grid == Grid{ {
        { 4, 0, 0, 0 },
        { 4, 16, 8, 2 },
        { 2, 64, 32, 4 },
        { 2048, 64, 0, 0 }
	} };
}
static_assert(TestLeft());

constexpr bool TestRight() {
    Grid grid = TestGrid;
    Move(grid, Right);
    return grid == Grid{ {
        { 0, 0, 0, 4 },
        { 4, 16, 8, 2 },
        { 2, 64, 32, 4 },
        { 0, 0, 2048, 64 }
    } };
}
static_assert(TestRight());

constexpr bool TestUp() {
    Grid grid = TestGrid;
    Move(grid, Up);
    return grid == Grid{ {
        { 2, 16, 8, 4 },
        { 4, 64, 32, 4 },
        { 2, 1024, 64, 0 },
        { 1024, 0, 0, 0 }
    } };
}
static_assert(TestUp());
constexpr bool TestDown() {
    Grid grid = TestGrid;
    Move(grid, Down);
    return grid == Grid{ {
        { 2, 0, 0, 0},
        { 4, 16, 8, 0 },
        { 2, 64, 32, 4 },
        { 1024, 1024, 64, 4 }
    } };
}
static_assert(TestDown());

constexpr bool Sample5() {
    Grid grid = { {
        { 2, 2, 4, 8},
        { 4, 0, 4, 4},
        { 16, 16, 16, 16},
        { 32, 16, 16, 32}
    } };
    Move(grid, Left);
    return grid == Grid{ {
        {4, 4, 8, 0 },
        { 8, 4, 0, 0 },
        { 32, 32, 0, 0 },
        { 32, 32, 32, 0 }
    } };
}
static_assert(Sample5());

constexpr bool Sample6() {
    Grid grid = { {
    { 2, 2, 4, 8},
    { 4, 0, 4, 4},
    { 16, 16, 16, 16},
    { 32, 16, 16, 32}
} };
    Move(grid, Right);
    return grid == Grid{ {
        { 0, 4, 4, 8 },
        { 0, 0, 4, 8 },
        { 0, 0, 32, 32 },
        { 0, 32, 32, 32 }
    } };
}
static_assert(Sample6());

void Print(const Grid& grid) {
    for (size_t r = 0; r < 4; r++) {
        const auto& row = grid[r];
        std::cout << std::format("{} {} {} {}\n", row[0], row[1], row[2], row[3]);
    }
}

int Main2048() {
    Grid grid;
    for (size_t r = 0; r < 4; r++) {
        for (size_t c = 0; c < 4; c++) {
            std::cin >> grid[r][c];
        }
    }

    int dir;
    std::cin >> dir;

    Move(grid, static_cast<Dir>(dir));
    Print(grid);
    return 0;
}