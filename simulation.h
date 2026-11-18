#pragma once

#include <memory>
#include <vector>

#include "animal.h"

class Simulation{
private:
    Field field;
    int turns;
    std::vector<std::unique_ptr<Animal>> animals;
    std::vector<std::vector<std::vector<Animal*>>> forest;

    template<typename T>
    void read_animal();

    void read_input();
    void place_in_forest();
    void remove_dead();
    void eating_on_cell(std::vector<Animal*>& cell);
    void move_phase();
    void eating_phase();
    void aging_phase();
    void reproduction_phase();
    void extinction_phase();
    void draw_forest();

public:
    void start();
};
