#include "raytracer.h"

Image::Image(const size_t width, const size_t height)
: Matrix<Pixel>(width, height)
{}

void Raytracer::render(){
    const size_t width = m_img->columns();
    const size_t height = m_img->rows();
    for(size_t x = 0; x < width; x++)
        for(size_t y = 0; y < height; y++){
           Vec3<float> start = camera.pos;
           Vec3<float> dir = ;
        }
}

Raytracer::Raytracer(Image* img)
: m_img{img}
{
    if(!m_img) throw "Cannot create Raytracer with no image. img was nullptr.";
}