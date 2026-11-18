#include "animal.h"

namespace{
    const int DX[4] = {0, 1, 0, -1};
    const int DY[4] = {-1, 0, 1, 0};

    int wrap(int a, int n){
        return ((a % n) + n) % n;
    }
}

Animal::Animal(int x, int y, int d, int k) :
        pos{x, y}, direct(static_cast<Direction>(d)), k(k){}

bool Animal::is_need_change_direction() const{
    return (age + 1) % k == 0;
}

void Animal::step(int steps, const Field& field){
    int i = static_cast<int>(direct);
    pos.x = wrap(pos.x + DX[i] * steps, field.width);
    pos.y = wrap(pos.y + DY[i] * steps, field.height);
    if (is_need_change_direction()){
        direct = static_cast<Direction>((i + 1) % 4);
    }
}

bool Animal::is_alive() const{
    return alive;
}

void Animal::kill(){
    alive = false;
}

int Animal::get_fed() const{
    return fed;
}

Position Animal::get_pose() const{
    return pos;
}

void Animal::aging(){
    age++;
}

bool Animal::is_expired() const{
    return age >= max_age();
}
