#include <iostream>
#include <vector>
#include <string>
#include <array>

struct RowCol {
    size_t Row{ 0 };
    size_t Col{ 0 };
};

using u32 = unsigned int;
using Grid = std::vector<std::vector<u32>>;

constexpr std::array<RowCol, 4> GetNeighbors(RowCol pos) {
    return {
        RowCol{pos.Row - 1, pos.Col},
        RowCol{pos.Row + 1, pos.Col},
        RowCol{pos.Row, pos.Col - 1},
        RowCol{pos.Row, pos.Col + 1}
    };
}

constexpr void UpdateGrid(Grid& grid, RowCol start, auto target, auto next) {
    std::vector<RowCol> q{ start };
    while (!q.empty()) {
        auto pos = q.back();
        q.pop_back();
        grid[pos.Row][pos.Col] = next;
        for (auto n : GetNeighbors(pos)) {
            if (grid[n.Row][n.Col] == target) {
                q.push_back(n);
            }
        }
    }
}

int Main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    size_t rows, cols, count;
    std::cin >> rows >> cols;

    Grid grid(rows + 2, std::vector<u32>(cols + 2, 0));
    for(size_t i = 1; i < rows + 1; i++) {
        std::string line;
        std::cin >> line;
        for(size_t j = 1; j < cols + 1;  j++) {
            grid[i][j] = line[j - 1];
        }
    }
    std::cin >> count;

    u32 next0 = '2';
    u32 next1 = '3';
    for (size_t i = 0; i < count; i++) {
        RowCol start, end;
        std::cin >> start.Row >> start.Col >> end.Row >> end.Col;
        auto c = grid[start.Row][start.Col];
        if (c < '2') {
            auto& next = c == '1' ? next1 : next0;
            UpdateGrid(grid, start, c, next);
            next += 2;
        }
        auto s = grid[start.Row][start.Col];
        if (s != grid[end.Row][end.Col]) std::cout << "neither\n";
        else std::cout << (((s - '2') % 2 == 0) ? "binary\n" : "decimal\n");
    }
}