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
    for(size_t y = 0; y < height(); y++)
        for(size_t x = 0; x < width(); x++)
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
    ll = {camera.distance, ul.y, -camera.view_plane.y / 2};
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
            Vec3<float> v = vx + ((lr - ur) * ((float)y / (float)height));
            Ray raycast(camera.max_ray_bounces, camera.pos, v.norm());
            m_img->operator()(x, y) = raycast.fire(&m_render_list);
        }
}

Raytracer::Raytracer(Image* img)
: m_img{img}
{
    if(!m_img) throw "Cannot create Raytracer with no image. img was nullptr.";
}


Ray::Ray(const size_t max_bounces, Vec3<float> start, Vec3<float> dir)
: m_start{start}, m_dir{dir}, m_max_bounces{max_bounces}
{

}

Color Ray::fire(std::list<Renderable*>* renderlist) {
    for(Renderable* ren : *renderlist){
        if(ren->m_visible)
            if(ren->intersect(*this)) break;
    }
    return m_color;
}

void Ray::intersection(const Intersection& inter){
    if(inter.dist >= 0){
        if(inter.dist > m_closest.dist)
            m_closest = inter;
    }
}

bool Sphere::intersect(Ray& ray){
    Vec3<float> camera2center = pos - ray.m_start; 
    float dist = std::sin(angle(camera2center, ray.m_dir)) * camera2center.length(); 
    //Detect if ray hits the sphere. If so, calculate the intersecting points...
    if(dist <= radius){
        /* Vector v is perpindicular to ray.
        *   1. Generate normal of auxiliary plane
        *   2. Cross product of normal and ray results in Vector v
        */
        
        float midpoint_dist = std::sqrt(std::pow(camera2center.length(), 2) - std::pow(dist, 2));
        Vec3<float> midpoint = ray.m_start + ray.m_dir * midpoint_dist;

        float delta = radius * sin(radius - dist);
        
        Vec3<float> inter1 = ray.m_start + ray.m_dir * (midpoint_dist - delta);
        Vec3<float> inter2 = ray.m_start + ray.m_dir * (midpoint_dist + delta);

        ray.m_color = m_material.base * delta;
        return true;
    } else return false;
}

void Raytracer::add(Renderable* ren){
    m_render_list.push_back(ren);
}

void Raytracer::remove(Renderable* ren){
    m_render_list.remove(ren); 
}

