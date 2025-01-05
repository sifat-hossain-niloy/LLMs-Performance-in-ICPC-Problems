#include <iostream>
#include <vector>
#include <algorithm>

struct Plate {
    int size;
    int stack_num;
    int index_in_stack;
    int plate_id;
};

bool plate_cmp(const Plate& a, const Plate& b) {
    if (a.size != b.size)
        return a.size < b.size;
    if (a.stack_num != b.stack_num)
        return a.stack_num < b.stack_num;
    return a.index_in_stack < b.index_in_stack;
}

int main() {
    int test_case = 0;
    int n;
    while (std::cin >> n) {
        test_case++;
        int total_stacks = n;
        int total_splits = 0;
        int plate_id_counter = 0;
        std::vector<Plate> plates;
        std::vector<std::vector<int>> stack_plate_positions(n);
        for (int s = 0; s < n; s++) {
            int h;
            std::cin >> h;
            for (int i = 0; i < h; i++) {
                int size;
                std::cin >> size;
                plates.push_back({size, s, i, plate_id_counter++});
            }
        }

        // Sort plates to get desired sequence positions
        std::sort(plates.begin(), plates.end(), plate_cmp);

        // Assign positions in desired sequence
        for (int i = 0; i < plates.size(); i++) {
            Plate& plate = plates[i];
            // Positions are 0-based
            int position = i;
            stack_plate_positions[plate.stack_num].push_back(position);
        }

        // For each stack, compute splits
        for (int s = 0; s < n; s++) {
            int splits = 0;
            const std::vector<int>& positions = stack_plate_positions[s];
            for (int i = 1; i < positions.size(); i++) {
                if (positions[i] != positions[i - 1] + 1) {
                    splits++;
                }
            }
            total_splits += splits;
        }

        int total_resulting_stacks = total_stacks + total_splits;
        int total_joins = total_resulting_stacks - 1;
        int total_operations = total_splits + total_joins;

        std::cout << "Case " << test_case << ": " << total_operations << std::endl;
    }
    return 0;
}
