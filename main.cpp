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

    Image img(500,500);

    Raytracer tracer(&img);

    Sphere sphere1; sphere1.radius = 1.5f;
    sphere1.pos = {6, -1.5f, 0};
    sphere1.material.base_color = {1, 0, 0};
    sphere1.material.diffuseness = 0;
    tracer.scene.add(&sphere1);

    Sphere sphere2; sphere2.radius = 1.5f;
    sphere2.pos = {6.5f, 1.5f, 0};
    sphere2.material.base_color = {0, 0, 1};
    sphere2.material.diffuseness = 1;
    tracer.scene.add(&sphere2);

    Sphere sphere3; sphere3.radius = 0.1f;
    sphere3.pos = {4, 0.8f, 0.8f};
    sphere3.material.base_color = {0, 1, 0};
    tracer.scene.add(&sphere3);

    Light light1; light1.pos = {3,2,1};
    light1.color = {1, 1, 1};
    light1.distance = 20;
    light1.intensity = 1.0f;
    tracer.scene.add(&light1);

    Light light2; light2.pos = {0, 0.5f, 1};
    light2.color = {1, 0, 0};
    light2.distance = 20;
    //tracer.scene.add(&light2);

    //std::cout << "Rendering started...";
    while(true){
        tracer.render();
    }
    //std::cout << " finished." << std::endl;

    //std::cout << "Saving Image to location '" << out_location << "'... ";
    //_TRY_BEGIN img.write(out_location); _CATCH(const char* e) std::cerr << e << std::endl; _CATCH_END; 

    //std::cout << "finished. Raytracer is terminating..." << std::endl;
    
    return 0;
}