#include <iostream>
#include "raytracer.h"

int main(int argc, char** argv){
    const char* out_location = argv[1];

    Image img(500, 500);
    img(100, 100);
    
    return 0;
}