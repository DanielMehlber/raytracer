//        __            _      __               __    ____             
//   ____/ /___ _____  (_)__  / /___ ___  ___  / /_  / / /_  ___  _____
//  / __  / __ `/ __ \/ / _ \/ / __ `__ \/ _ \/ __ \/ / __ \/ _ \/ ___/
// / /_/ / /_/ / / / / /  __/ / / / / / /  __/ / / / / /_/ /  __/ /    
// \__,_/\__,_/_/ /_/_/\___/_/_/ /_/ /_/\___/_/ /_/_/_.___/\___/_/     
//  https://github.com/danielmehlber                                     

#include "raytracer.h"

Clock::Clock() {
    start();
}

double Clock::start(){
    last_start = std::chrono::system_clock::now();
    return last_start.time_since_epoch().count();
}

double Clock::stop(){
    last_stop = std::chrono::system_clock::now();
    auto delta = last_stop.time_since_epoch().count() - last_start.time_since_epoch().count();
    return delta;
}

Image::Image(const size_t width, const size_t height)
: Matrix<Color>(width, height)
{}

void Image::write(const char * dest) const {
    std::ofstream file(dest);
    if(!file || file.bad()) throw "Cannot open file.";
    //Basic Header data for .ppm image files.
    file << "P3\n" << width() << "\n" << height() << "\n" << "255\n"; 
    //Data    ^mode     ^width             ^height             ^color value
    for(size_t y = 0; y < height(); y++){
        for(size_t x = 0; x < width(); x++)
            file << operator()(x, y);
    }
    file.flush();
}

void Raytracer::render(){
    Clock render_clock;
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
    auto time = render_clock.stop();
    std::cout << "Elapsed time: " << (int)time << "ns = " << (time/1000000) << "ms" << std::endl;
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
    //Stage 1: Interseption phase
    for(Renderable* ren : *renderlist){
        if(ren->m_visible)
            ren->intersect(*this);
    }
    
    if(m_closest.object){
        //Process intersection
        m_color = m_closest.object->process(m_closest.point, *this);
    }else{
        //No intersection.
    }

    return m_color;
}

void Ray::intersection(const Intersection& inter){
    //Step 1: Calculate distance to point on ray (=direction factor)
    float dist;
    if(m_dir.x != 0)
        dist = (inter.point.x - m_start.x) / m_dir.x;
    else if(m_dir.y != 0)
        dist = (inter.point.y - m_start.y) / m_dir.y;
    else
        dist = (inter.point.z - m_start.z) / m_dir.z;
    //Step 2: Check if point is in front of camera
    if(dist > 0){
        //Step 3: Compare to last intersection or set as closest intersection if there is no other
        if(!m_closest.object){
            m_closest = inter;
        }else{
            //Step 4: Compare both distances from the camera. Set to closest.
            float _dist = (m_start - m_closest.point).length();
            if(abs(dist) < _dist) m_closest = inter;
        }
    }
    
    
}

bool Sphere::intersect(Ray& ray){
    Vec3<float> camera2center = pos - ray.m_start; 
    float dist = std::sin(angle(camera2center, ray.m_dir)) * camera2center.length(); 
    //Detect if ray hits the sphere. If so, calculate the intersecting points...
    if(dist <= radius){
        
        float midpoint_dist = std::sqrt(std::pow(camera2center.length(), 2) - std::pow(dist, 2));
        Vec3<float> midpoint = ray.m_start + ray.m_dir * midpoint_dist;

        float delta = sin(0.5f * PI* ((radius - dist)/radius));
        float alpha = delta * radius;
        
        Vec3<float> inter1 = ray.m_start + ray.m_dir * (midpoint_dist - alpha);
        Vec3<float> inter2 = ray.m_start + ray.m_dir * (midpoint_dist + alpha);
        
        ray.intersection({inter1, this});
        ray.intersection({inter2, this});

        return true;
    } else return false;
}

Color Sphere::process(const Vec3<float>& point, const Ray& ray){
    Color ret = m_material.base;
    Vec3<float> normal = (point - pos).norm();
    float value = -normal.dot(ray.m_dir);
    ret = ret * value;

    Vec3<float> reflect = ((normal * normal.dot(ray.m_dir) * 2) - ray.m_dir).norm();

    //Light check
    //Reflections

    return ret;
}

void Raytracer::add(Renderable* ren){
    m_render_list.push_back(ren);
}

void Raytracer::remove(Renderable* ren){
    m_render_list.remove(ren); 
}

