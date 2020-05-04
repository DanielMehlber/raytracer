
//        __            _      __               __    ____             
//   ____/ /___ _____  (_)__  / /___ ___  ___  / /_  / / /_  ___  _____
//  / __  / __ `/ __ \/ / _ \/ / __ `__ \/ _ \/ __ \/ / __ \/ _ \/ ___/
// / /_/ / /_/ / / / / /  __/ / / / / / /  __/ / / / / /_/ /  __/ /    
// \__,_/\__,_/_/ /_/_/\___/_/_/ /_/ /_/\___/_/ /_/_/_.___/\___/_/     
//                                                                    

#include "raytracer.h"

Image::Image(const size_t width, const size_t height)
: Matrix<Color>(width, height)
{}

void Image::write(const char * dest) const {
    std::ofstream file(dest);
    if(!file || file.bad()) throw "Cannot open file.";
    file << "P3\n" << width() << "\n" << height() << "\n" << "255\n"; 
    for(size_t x = 0; x < width(); x++)
        for(size_t y = 0; y < height(); y++)
            file << operator()(x, y);
    file.flush();
}

void Raytracer::render(){
    const size_t width = m_img->columns();
    const size_t height = m_img->rows();
    for(size_t x = 0; x < width; x++)
        for(size_t y = 0; y < height; y++){
           //TODO: Ray generation
        }
}

Raytracer::Raytracer(Image* img)
: m_img{img}
{
    if(!m_img) throw "Cannot create Raytracer with no image. img was nullptr.";
}

