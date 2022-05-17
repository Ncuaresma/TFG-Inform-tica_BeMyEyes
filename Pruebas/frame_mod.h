#ifndef FRAME_MOD_H
#define FRAME_MOD_H

#include "box_mod.h"
#include "frame_mod.cc"

// function to initialize private variables
//Frame(int numero, double len, double hgt, std::string n, cv::Mat image);
// function to set or change each value of the frame (we don't need it)
void set_length(double len);
void set_height(double hgt);
void show_name();
// functions to get each value
double get_length();
double get_height();

// your declarations (and certain types of definitions) here

#endif