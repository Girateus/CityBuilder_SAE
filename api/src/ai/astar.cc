//
// Created by noahs on 10.06.2026.
//

#include <iostream>
#include <mdspan>
#include <vector>

void Print(std::mdspan<int, std::dextents<std::size_t, 2>> m) {
  for (std::size_t r = 0; r < m.extent(0); ++r) {
    for (std::size_t c = 0; c < m.extent(1); ++c) {
      std::cout << m[r, c] << '\t';
    }
    std::cout << '\n';
  }
}