#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>

using namespace std;

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

namespace{
    const int DX[4] = {0, 1, 0, -1};
    const int DY[4] = {-1, 0, 1, 0};
    const int PREY_MAX_AGE = 10;
    const int PREDATOR_MAX_AGE = 20;

    int wrap(int a, int n){
        return ((a % n) + n) % n;
    }
}

class Animal{
protected:
    int age = 0;
    int fed = 0;
    Position pos;
    Direction direct;
    int k;
    bool alive = true;

    bool is_need_change_direction() const{
        return (age + 1) % k == 0;
    }

    void step(int steps, const Field& field){
        int i = static_cast<int>(direct);
        pos.x = wrap(pos.x + DX[i] * steps, field.width);
        pos.y = wrap(pos.y + DY[i] * steps, field.height);
        if (is_need_change_direction()){
            direct = static_cast<Direction>((i + 1) % 4);
        }
    }

public:
    Animal(int x, int y, int d, int k) :
            pos{x, y}, direct(static_cast<Direction>(d)), k(k){}
    virtual ~Animal() = default;

    bool is_alive() const{
        return alive;
    }

    void kill(){
        alive = false;
    }

    int get_fed() const{
        return fed;
    }

    Position get_pose() const{
        return pos;
    }

    void aging(){
        age++;
    }

    bool is_expired() const{
        return age >= max_age();
    }

    virtual bool is_predator() const = 0;
    virtual int max_age() const = 0;
    virtual void move(const Field& field) = 0;
    virtual void eat() = 0;
    virtual unique_ptr<Animal> make_child() = 0;
};

class Predator : public Animal{
public:
    Predator(int x, int y, int d, int k) : Animal(x, y, d, k){}

    bool is_predator() const override{
        return true;
    }

    int max_age() const override{
        return PREDATOR_MAX_AGE;
    }

    void move(const Field& field) override{
        step(2, field);
    }

    void eat() override{
        fed++;
    }

    unique_ptr<Animal> make_child() override{
        if (fed < 2){
            return nullptr;
        }
        fed = 0;
        return make_unique<Predator>(pos.x, pos.y, static_cast<int>(direct), k);
    }
};

class Prey : public Animal{
public:
    Prey(int x, int y, int d, int k) : Animal(x, y, d, k){}

    bool is_predator() const override{
        return false;
    }

    int max_age() const override{
        return PREY_MAX_AGE;
    }

    void move(const Field& field) override{
        step(1, field);
    }

    void eat() override{
    }

    unique_ptr<Animal> make_child() override{
        if (age != 5 && age != 10){
            return nullptr;
        }
        return make_unique<Prey>(pos.x, pos.y, static_cast<int>(direct), k);
    }
};

class Simulation{
private:
    Field field;
    int turns;
    vector<unique_ptr<Animal>> animals;
    vector<vector<vector<Animal*>>> forest;

    template<typename T>
    void read_animal(){
        int x, y, d, k;
        cin >> x >> y >> d >> k;
        animals.push_back(make_unique<T>(x, y, d, k));
    }

    void read_input(){
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

    void place_in_forest(){
        forest.assign(field.height, vector<vector<Animal*>>(field.width));
        for (const auto& creature : animals){
            Position p = creature->get_pose();
            forest[p.y][p.x].push_back(creature.get());
        }
    }

    void remove_dead(){
        animals.erase(
            remove_if(animals.begin(), animals.end(),
                [](const unique_ptr<Animal>& a){ return !a->is_alive(); }),
            animals.end());
    }

    void eating_on_cell(vector<Animal*>& cell){
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

    void move_phase(){
        for (auto& creature : animals){
            creature->move(field);
        }
    }

    void eating_phase(){
        place_in_forest();
        for (auto& row : forest){
            for (auto& cell : row){
                eating_on_cell(cell);
            }
        }
        remove_dead();
    }

    void aging_phase(){
        for (auto& creature : animals){
            creature->aging();
        }
    }

    void reproduction_phase(){
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

    void extinction_phase(){
        for (auto& creature : animals){
            if (creature->is_expired()){
                creature->kill();
            }
        }
        remove_dead();
    }

    void draw_forest(){
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

public:
    void start(){
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
};

int main(){
    Simulation s;
    s.start();
}
