/* Hello world program in c++
* Compilar: g++ -o ejecutable demo_classes.cc
* Se genera el ejecutable
* Ejecutar: ./ejecutable
*/

// Program to illustrate the working of
// public and private in C++ Class

#include <iostream>
using namespace std; // esto es para que delante de la funcion perteneciente a este include ^ en vez de poner ''std::cout <<'' pones directamente ''cout << ''

class Room {

   private:
    double length;
    double breadth;
    double height;

   public:

    // function to initialize private variables
    void initData(double len, double brth, double hgt) {
        length = len;
        breadth = brth;
        height = hgt;
    }

    double calculateArea() {
        return length * breadth;
    }

    double calculateVolume() {
        return length * breadth * height;
    }
};

int main() {

    // create object of Room class
    Room room1;

    // pass the values of private variables as arguments
    room1.initData(42.5, 30.8, 19.2);

    cout << "Area of Room =  " << room1.calculateArea() << endl;
    cout << "Volume of Room =  " << room1.calculateVolume() << endl;

    return 0;
}