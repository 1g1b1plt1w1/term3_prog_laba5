#include "predator_prey.h"

namespace{
    const int PREDATOR_MAX_AGE = 20;
    const int PREY_MAX_AGE = 10;
}

Predator::Predator(int x, int y, int d, int k) : Animal(x, y, d, k){}

bool Predator::is_predator() const{
    return true;
}

int Predator::max_age() const{
    return PREDATOR_MAX_AGE;
}

void Predator::move(const Field& field){
    step(2, field);
}

void Predator::eat(){
    fed++;
}

std::unique_ptr<Animal> Predator::make_child(){
    if (fed < 2){
        return nullptr;
    }
    fed = 0;
    return std::make_unique<Predator>(pos.x, pos.y, static_cast<int>(direct), k);
}

Prey::Prey(int x, int y, int d, int k) : Animal(x, y, d, k){}

bool Prey::is_predator() const{
    return false;
}

int Prey::max_age() const{
    return PREY_MAX_AGE;
}

void Prey::move(const Field& field){
    step(1, field);
}

void Prey::eat(){
}

std::unique_ptr<Animal> Prey::make_child(){
    if (age != 5 && age != 10){
        return nullptr;
    }
    return std::make_unique<Prey>(pos.x, pos.y, static_cast<int>(direct), k);
}
