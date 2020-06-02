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

    //Empty view of scene. 
    View view;

    //Calculate view plane (=perspective) of view.
    view.ul = {camera.distance, -camera.view_plane.x / 2, camera.view_plane.y / 2};
    view.ll = {camera.distance, view.ul.y, -camera.view_plane.y / 2};
    view.ur = {camera.distance, camera.view_plane.x / 2, view.ul.z};
    view.lr = {camera.distance, view.ur.y, view.ll.z};

    //Rotate Camera: Rotate view.
    rotate(view.ul, camera.rot.x, camera.rot.y, camera.rot.z); //    ul ------------ ur
    rotate(view.ll, camera.rot.x, camera.rot.y, camera.rot.z); //    |               |
    rotate(view.ur, camera.rot.x, camera.rot.y, camera.rot.z); //    |               |
    rotate(view.lr, camera.rot.x, camera.rot.y, camera.rot.z); //    ll ------------ lr

    //Iterate through pixels and calculate their color => Rendering.
    for(size_t y = 0; y < height; y++)
        for(size_t x = 0; x < width; x++){
            //For each pixel:
            //1. Calculate ray direction vector from view plane and current pixel position.
            Vec3<float> _ray_direction_x_only   = view.ul + (view.ur - view.ul) * ((float)x / (float)width);
            Vec3<float> ray_direction           = _ray_direction_x_only + ((view.lr - view.ur) * ((float)y / (float)height));

            //2. Cast ray from camera position, generated direction and bounce limit.
            Ray raycast(camera.max_ray_bounces, camera.pos, ray_direction.norm());
            m_img->operator()(x, y) = raycast.fire(scene);
            //                ^Pixel                ^Visible data
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

Color Ray::fire(const SceneData& scene) {
    //Stage 1: Intersection phase - calculate all possible intersections (with visible objects).
    for(Renderable* object : scene.m_render_list){
        if(object->m_visible && std::find(m_ignore.begin(), m_ignore.end(), object) == m_ignore.end())
            object->intersect(*this);
    }
    
    //Check if any intersections were registered.
    if(m_closest.object){
        //Process intersection
        m_color = m_closest.object->process(scene, m_closest.point, *this);
    } // else: Skip processing stage.

    return m_color;
}

void Ray::intersection(const Intersection& inter){
    //Step 1: Is intersection positive (=visible to the view)?
    float dist;
    if(m_dir.x != 0)
        dist = (inter.point.x - m_start.x) / m_dir.x;
    else if(m_dir.y != 0)
        dist = (inter.point.y - m_start.y) / m_dir.y;
    else
        dist = (inter.point.z - m_start.z) / m_dir.z;

    //Step 2: Check if factor along direction vector of ray is positive.
    if(dist > 0){
        //Step 3: Compare to last intersection or set as closest intersection if there is no other
        if(!m_closest.object){
            m_closest = inter;
        }else{
            //Step 4: Compare both distances from the camera. Set to closest.
            float _dist = (m_start - m_closest.point).length();
            if(abs(dist) < _dist) m_closest = inter;
        }
    }// else: Don't register intersection
    
    
}

BoundingBox::BoundingBox(float x1, float x2, float y1, float y2, float z1, float z2) 
{
    a = {x1, y1, z1};
    b = {-x2, -y2, -z2};
}

bool BoundingBox::check_visibility(const Camera& cam, const View& view){
    auto arr = get_points();
    for(Vec3<float>& vertex : arr){
        Vec3<float> cam2vertex = (vertex - cam.pos).norm();
        
    }
};

std::array<Vec3<float>, 8> BoundingBox::get_points(){
    std::array<Vec3<float>, 8> arr;
    arr[0] = a;
    arr[1] = {a.x, a.y, b.z};
    arr[2] = {a.x, b.x, a.z};
    arr[3] = {b.x, a.y, a.z};
    arr[4] = b;
    arr[5] = {b.x, b.y, a.z};
    arr[6] = {b.x, a.y, b.z};
    arr[7] = {a.x, b.y, b.z};
    return arr;
};

bool Sphere::intersect(Ray& ray){
    Vec3<float> camera2center = pos - ray.m_start; 
    float dist = std::sin(angle(camera2center, ray.m_dir)) * camera2center.length(); 
    //Detect if ray hits the sphere. If so, calculate the intersecting points...
    if(dist <= radius){
        
        float midpoint_dist = std::sqrt(std::pow(camera2center.length(), 2) - std::pow(dist, 2));
        //Vec3<float> midpoint = ray.m_start + ray.m_dir * midpoint_dist;

        float delta = sin(0.5f * PI* ((radius - dist)/radius));
        float alpha = delta * radius;
        
        Vec3<float> inter1 = ray.m_start + ray.m_dir * (midpoint_dist - alpha);
        Vec3<float> inter2 = ray.m_start + ray.m_dir * (midpoint_dist + alpha);
        
        //Check if that point is in front of the camera by caluclating the distance to sphere center.
        if((inter1 - pos).length() <= radius) ray.intersection({inter1, this});
        if((inter2 - pos).length() <= radius) ray.intersection({inter2, this});

        return true;
    } else return false;
}

Color Sphere::process(const SceneData& scene, const Vec3<float>& point, const Ray& ray){
    Vec3<float> normal = (point - pos).norm();

    //If no more bounces allowed, use diffuse color to 100%
    float diffuseness = ray.m_max_bounces == 0 ? 1 : material.diffuseness;
    Color pixel_color = {0,0,0};

    //Reflection calculation
    if(diffuseness != 1) {
        Color reflection_color = {0,0,0};
        Vec3<float> reflect = (ray.m_dir - (normal * normal.dot(ray.m_dir) * 2)).norm();
        
        Ray reflection_ray(ray.m_max_bounces - 1, point, reflect);
        reflection_ray.m_ignore.push_back(this);
        //m_visible = false;
        reflection_color = reflection_ray.fire(scene);
        //m_visible = true;

        pixel_color += reflection_color * (1 - diffuseness);
    }

    //Diffuse calculation
    if(diffuseness != 0) {
        Color diffuse_color = material.base_color;

        Color light_color = {0,0,0};
        for(Light* current_light : scene.light_list){
            Vec3<float> point_to_light      = current_light->pos - point;
            float       distance_to_light   = point_to_light.length();
            //Check for Shadows here.

            if(distance_to_light <= current_light->distance){
                float _dot_product      = point_to_light.norm().dot(normal);
                float angle_factor      = _dot_product > 1 ? 1 : (_dot_product < 0 ? 0 : _dot_product);
                float distance_factor   = (current_light->distance - distance_to_light) / current_light->distance;

                light_color += current_light->color * sqrt(angle_factor) * sqrt(distance_factor) * current_light->intensity;
                //                                    ^ To counteract quadratic falloff
            }
        }

        //Mix with pixel color
        diffuse_color *= light_color;
        pixel_color += diffuse_color * diffuseness;
    }

    return pixel_color;
    
}

void SceneData::add(Renderable* ren){
    if(!ren) throw "Cannot add nullptr as renderable.";
    m_render_list.push_back(ren);
}

void SceneData::remove(Renderable* ren){
    if(!ren) throw "Cannot remove nullptr from renderable list.";
    m_render_list.remove(ren); 
}

void SceneData::add(Light* light){
    if(!light) throw "Cannot add nullptr as light.";
    light_list.push_back(light);
}

void SceneData::remove(Light* light){
    if(!light) throw "Cannot remove nullptr from lights list.";
    light_list.remove(light);
}



