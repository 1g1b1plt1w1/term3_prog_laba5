#pragma once

#include "animal.h"

class Predator : public Animal{
public:
    Predator(int x, int y, int d, int k);

    bool is_predator() const override;
    int max_age() const override;
    void move(const Field& field) override;
    void eat() override;
    std::unique_ptr<Animal> make_child() override;
};

class Prey : public Animal{
public:
    Prey(int x, int y, int d, int k);

    bool is_predator() const override;
    int max_age() const override;
    void move(const Field& field) override;
    void eat() override;
    std::unique_ptr<Animal> make_child() override;
};
