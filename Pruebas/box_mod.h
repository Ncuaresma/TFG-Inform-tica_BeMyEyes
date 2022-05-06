#ifndef BOX_MOD_H
#define BOX_MOD_H

#include "box_mod.cc"

/// function to initialize private variables
void ini_box(double len, double hgt, string name);
void add_box(double len, double hgt);
// function to set or change each value of the box (we don't need it)
void add_length(double len);
void add_height(double hgt);
/* void set_position(double pos){
    position = pos;
}*/

// functions to get each value
double get_length();
double get_height();

/*double get_position(){
    return position;
}*/
double calculate_area();

// function to compare two boxes
double size_difference(Box box1, Box box2);

// your declarations (and certain types of definitions) here

#endif