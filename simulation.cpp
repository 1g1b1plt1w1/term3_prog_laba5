#include "simulation.h"

#include <iostream>
#include <algorithm>

#include "predator_prey.h"

using namespace std;

template<typename T>
void Simulation::read_animal(){
    int x, y, d, k;
    cin >> x >> y >> d >> k;
    animals.push_back(make_unique<T>(x, y, d, k));
}

void Simulation::read_input(){
    int r, w;
    cin >> field.width >> field.height >> turns;
    cin >> r >> w;
    for (int i = 0; i < r; i++){
        read_animal<Prey>();
    }
    for (int i = 0; i < w; i++){
        read_animal<Predator>();
    }
}

void Simulation::place_in_forest(){
    forest.assign(field.height, vector<vector<Animal*>>(field.width));
    for (const auto& creature : animals){
        Position p = creature->get_pose();
        forest[p.y][p.x].push_back(creature.get());
    }
}

void Simulation::remove_dead(){
    animals.erase(
        remove_if(animals.begin(), animals.end(),
            [](const unique_ptr<Animal>& a){ return !a->is_alive(); }),
        animals.end());
}

void Simulation::eating_on_cell(vector<Animal*>& cell){
    Animal* predator = nullptr;
    for (Animal* a : cell){
        if (a->is_predator()){
            predator = a;
            break;
        }
    }
    if (predator == nullptr){
        return;
    }
    for (Animal* a : cell){
        if (!a->is_predator()){
            a->kill();
            predator->eat();
        }
    }
}

void Simulation::move_phase(){
    for (auto& creature : animals){
        creature->move(field);
    }
}

void Simulation::eating_phase(){
    place_in_forest();
    for (auto& row : forest){
        for (auto& cell : row){
            eating_on_cell(cell);
        }
    }
    remove_dead();
}

void Simulation::aging_phase(){
    for (auto& creature : animals){
        creature->aging();
    }
}

void Simulation::reproduction_phase(){
    vector<unique_ptr<Animal>> newborns;
    for (auto& creature : animals){
        unique_ptr<Animal> child = creature->make_child();
        if (child){
            newborns.push_back(move(child));
        }
    }
    for (auto& child : newborns){
        animals.push_back(move(child));
    }
}

void Simulation::extinction_phase(){
    for (auto& creature : animals){
        if (creature->is_expired()){
            creature->kill();
        }
    }
    remove_dead();
}

void Simulation::draw_forest(){
    place_in_forest();
    for (const auto& row : forest){
        for (const auto& cell : row){
            int prey = 0;
            int predators = 0;
            for (const Animal* a : cell){
                if (a->is_predator()){
                    predators++;
                } else{
                    prey++;
                }
            }
            if (predators > 0){
                cout << '-' << predators;
            } else if (prey > 0){
                cout<< prey;
            } else{
                cout << '*';
            }
        }
        cout << '\n';
    }
}

void Simulation::start(){
    read_input();
    for (int i = 0; i < turns; i++){
        move_phase();
        eating_phase();
        aging_phase();
        reproduction_phase();
        extinction_phase();
    }
    draw_forest();
}
