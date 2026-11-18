#pragma once

#include <memory>

struct Position{
    int x;
    int y;
};

struct Field{
    int width;
    int height;
};

enum class Direction{
    Up = 0,
    Right = 1,
    Down = 2,
    Left = 3
};

class Animal{
protected:
    int age = 0;
    int fed = 0;
    Position pos;
    Direction direct;
    int k;
    bool alive = true;

    bool is_need_change_direction() const;
    void step(int steps, const Field& field);

public:
    Animal(int x, int y, int d, int k);
    virtual ~Animal() = default;

    bool is_alive() const;
    void kill();
    int get_fed() const;
    Position get_pose() const;
    void aging();
    bool is_expired() const;

    virtual bool is_predator() const = 0;
    virtual int max_age() const = 0;
    virtual void move(const Field& field) = 0;
    virtual void eat() = 0;
    virtual std::unique_ptr<Animal> make_child() = 0;
};
