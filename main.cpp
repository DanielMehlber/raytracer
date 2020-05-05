//        __            _      __               __    ____             
//   ____/ /___ _____  (_)__  / /___ ___  ___  / /_  / / /_  ___  _____
//  / __  / __ `/ __ \/ / _ \/ / __ `__ \/ _ \/ __ \/ / __ \/ _ \/ ___/
// / /_/ / /_/ / / / / /  __/ / / / / / /  __/ / / / / /_/ /  __/ /    
// \__,_/\__,_/_/ /_/_/\___/_/_/ /_/ /_/\___/_/ /_/_/_.___/\___/_/     
//  https://github.com/danielmehlber                                     

#include <iostream>
#include "raytracer.h"

int main(int argc, char** argv){
    const char* out_location = argv[1];

    if(!out_location) out_location = "result.ppm";

    std::cout << "Raytracer started" << std::endl;
    

    Vec3<float> vec {1, 0, 0};
    auto v = rotateX(rotateY(rotateZ(vec, 90), 90), 90);

    Image img(1080, 720);

    std::cout << "Rendering started...";
    Raytracer tracer(&img);
    tracer.render();
    std::cout << " finished." << std::endl;

    std::cout << "Saving Image to location '" << out_location << "'... ";
    _TRY_BEGIN img.write(out_location); _CATCH(const char* e) std::cerr << e << std::endl; _CATCH_END; 

    std::cout << "finished. Raytracer is terminating..." << std::endl;
    
    return 0;
}