//        __            _      __               __    ____             
//   ____/ /___ _____  (_)__  / /___ ___  ___  / /_  / / /_  ___  _____
//  / __  / __ `/ __ \/ / _ \/ / __ `__ \/ _ \/ __ \/ / __ \/ _ \/ ___/
// / /_/ / /_/ / / / / /  __/ / / / / / /  __/ / / / / /_/ /  __/ /    
// \__,_/\__,_/_/ /_/_/\___/_/_/ /_/ /_/\___/_/ /_/_/_.___/\___/_/     
//  https://github.com/danielmehlber                                     

#include "raytracer.h"

Image::Image(const size_t width, const size_t height)
: Matrix<Color>(width, height)
{}

void Image::write(const char * dest) const {
    std::ofstream file(dest);
    if(!file || file.bad()) throw "Cannot open file.";
    //Basic Header data for .ppm image files.
    file << "P3\n" << width() << "\n" << height() << "\n" << "255\n"; 
    //Data    ^mode     ^width             ^height             ^color value
    for(size_t x = 0; x < width(); x++)
        for(size_t y = 0; y < height(); y++)
            file << operator()(x, y);
    file.flush();
}

void Raytracer::render(){
    const size_t width = m_img->width();
    const size_t height = m_img->height();

    //Calculate View
    
    Vec3<float> ul, ll, ur, lr;

    //Create camera setup from data
    ul = {camera.distance, -camera.view_plane.x / 2, camera.view_plane.y / 2};
    ll = {camera.distance, ul.y, camera.view_plane.y / 2};
    ur = {camera.distance, camera.view_plane.x / 2, ul.z};
    lr = {camera.distance, ur.y, ll.z};

    //Rotate Camera
    rotate(ul, camera.rot.x, camera.rot.y, camera.rot.z); //    ul ------------ ur
    rotate(ll, camera.rot.x, camera.rot.y, camera.rot.z); //    |               |
    rotate(ur, camera.rot.x, camera.rot.y, camera.rot.z); //    |               |
    rotate(lr, camera.rot.x, camera.rot.y, camera.rot.z); //    ll ------------ lr


    for(size_t y = 0; y < height; y++)
        for(size_t x = 0; x < width; x++){
            Vec3<float> vx = ul + (ur - ul) * ((float)x / (float)width);
            Vec3<float> v = vx + (ur-lr) * ((float)y / (float)height);
            RayCast raycast(camera.max_ray_bounces, camera.pos, v);
            m_img->operator()(x, y) = raycast.fire();
        }
}

Raytracer::Raytracer(Image* img)
: m_img{img}
{
    if(!m_img) throw "Cannot create Raytracer with no image. img was nullptr.";
}


RayCast::RayCast(const size_t max_bounces, Vec3<float> start, Vec3<float> dir)
: m_start{start}, m_dir{dir}, m_max_bounces{max_bounces}
{

}

Color RayCast::fire() {
    return {1, 0, 0};
}
