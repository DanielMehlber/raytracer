#ifndef __RAYTRACER_H__
#define __RAYTRACER_H__

//        __            _      __               __    ____             
//   ____/ /___ _____  (_)__  / /___ ___  ___  / /_  / / /_  ___  _____
//  / __  / __ `/ __ \/ / _ \/ / __ `__ \/ _ \/ __ \/ / __ \/ _ \/ ___/
// / /_/ / /_/ / / / / /  __/ / / / / / /  __/ / / / / /_/ /  __/ /    
// \__,_/\__,_/_/ /_/_/\___/_/_/ /_/ /_/\___/_/ /_/_/_.___/\___/_/     
//  https://github.com/danielmehlber                                     

#pragma once
#include "math.h"
#include <list>
#include <fstream>
#include <iostream>
#include <chrono>

/**
 * @brief Measures time. Used for render time.
 */
class Clock {
public:
    std::chrono::system_clock::time_point last_start;
    std::chrono::system_clock::time_point last_stop;
    /**
     * @brief Construct a new Clock object and starts time measurement.
     */
    Clock();
    /**
     * @brief Starts time measurement.
     * @return double time of starting measurement.
     */
    double start();
    /**
     * @brief Stops time measurement.
     * @return double measured time in nano-seconds.
     */
    double stop();
};

struct Color{
    uint8_t r = 0, g = 0, b = 0;
    inline friend std::ostream& operator<<(std::ostream& stream, const Color& c){
        stream << +c.r << std::endl;
        stream << +c.g << std::endl;
        stream << +c.b << std::endl;
        return stream;
    }
    /**
     * @brief Multiplying Color by a scalar + clamping values.
     * @tparam T Type of scalar.
     * @param scalar value of scalar.
     * @return Color new Color after multiplication.
     */
    template <typename T> inline Color operator*(T scalar){
        int _r = ((int)r) * scalar, _b = ((int)b) * scalar, _g = ((int)g) * scalar;
        if(_r > 255) _r = 255; else if (_r < 0) _r = 0;
        if(_g > 255) _g = 255; else if (_g < 0) _g = 0;
        if(_b > 255) _b = 255; else if (_b < 0) _b = 0;
        return {(uint8_t)(_r), (uint8_t)(_g), (uint8_t)(_b)};
    }
};

/**
 * @brief Transformation of an object (position, rotation, scale)
 */
struct Transform{
    Vec3<float> pos, rot, scale;
};

/**
 * @brief Camera and view
 * 
 */
struct Camera : Transform {
    /**
     * @brief Max amount of ray bounces before Ray terminates. Higher values result in better graphics.
     */
    int         max_ray_bounces {3};
    /**
     * @brief Height and Width of view plane. Used for calculating camera rays.
     */
    Vec2<float> view_plane      {1, 1};
    /**
     * @brief Distance from camera origin to view plane. (=Field of view)
     */
    float       distance        {1.0f};
};

/**
 * @brief Basic light object.
 */
struct Light : Transform {
    Color color;
};

/**
 * @brief Image. is basically a matrix of colors. Necessary for rendering.
 */
class Image : public Matrix<Color> {
protected:
public:
    Image() = delete;
    /**
     * @brief Construct a new Image object.
     * @param width width of image.
     * @param height height of image.
     */
    Image(const size_t width, const size_t height);

    /**
     * @brief Output image to .ppm file.
     * @param dest file path (file will be created or overwritten).
     */
    void write(const char* dest) const;

    /**
     * @brief Get width of image.
     * @return const size_t width.
     */
    inline const size_t width() const { return m_colums; }
    /**
     * @brief Get height of image.
     * @return const size_t height.
     */
    inline const size_t height() const { return m_rows; } 
};

struct Renderable;
/**
 * @brief Intersection between a renderable object and a ray on a specific point in space.
 */
struct Intersection {
    /**
     * @brief location of intersection.
     */
    Vec3<float> point;
    /**
     * @brief intersected object.
     */
    Renderable* object {nullptr};
};

/**
 * @brief Ray.
 */
struct Ray{
    /**
     * @brief May bounces of ray.
     */
    const size_t        m_max_bounces;
    /**
     * @brief Start point of ray.
     */
    const Vec3<float>   m_start;
    /**
     * @brief Direction of ray.
     */
    const Vec3<float>   m_dir;
    /**
     * @brief color of ray.
     */
    Color               m_color {255, 255, 255};
    /**
     * @brief closest intersection to the camera --> visible intersection.
     */
    Intersection        m_closest;

    /**
     * @brief Construct a new Ray object
     * @param max_bounces 
     * @param start 
     * @param dir 
     */
    Ray(const size_t max_bounces = 0, Vec3<float> start = {0,0,0}, Vec3<float> dir = {1,0,0});

    /**
     * @brief Fire ray and check for intersections.
     * @param renderlist list of objects to render (= list of intersectable objects).
     * @return Color Result and final color of ray.
     */
    Color fire(std::list<Renderable*>* renderlist);

    /**
     * @brief Register another intersection. Intersection will be automatically filtered.
     * @param inter Intersection-
     */
    void intersection(const Intersection& inter);
};

/**
 * @brief Material of renderable object. Gives basic information about its surface.
 */
struct Material {
    /**
     * @brief Base diffuse color of surface.
     */
    Color base      {255, 0, 255};
    /**
     * @brief diffuse-ness of surface. 1 = no reflections, 0 = only reflections.
     */
    float diffuse   {1.0f}; 
};

/**
 * @brief An object that can be rendered.
 */
struct Renderable {
    /**
     * @brief visibility of object. If false, it will be ignored in the rendering process.
     */
    bool        m_visible  {true};
    /**
     * @brief Material and surface information of object.
     */
    Material    m_material;
    /**
     * @brief Checks for intersections with ray. (= Intersection stage)
     * @param ray Ray.
     * @return true The Ray has intersected with the object.
     * @return false No intersection took place.
     */
    virtual bool intersect(Ray& ray) = 0;
    /**
     * @brief Process intersection using Material data. (= Materialization stage)
     * @param intersection clostest visible intersection on object.
     * @param ray ray used for detecting the intersection (For view data).
     * @return Color Final Color.
     */
    virtual Color process(const Vec3<float>& intersection, const Ray& ray) = 0;
};

/**
 * @brief Sphere object.
 */
struct Sphere : Renderable, Transform{
    /**
     * @brief Radius of Sphere
     */
    float radius {1.0f};
    virtual bool intersect(Ray& ray) override;
    virtual Color process(const Vec3<float>& intersection, const Ray& ray);
};

/**
 * @brief Central raytracing unit.
 * 
 */
class Raytracer{
protected:
    /**
     * @brief Image in which the result will be saved. Cannot be null.
     */
    Image*                  m_img;
    /**
     * @brief List of objects in the scene / to render.
     * 
     */
    std::list<Renderable*>  m_render_list;
public:
    /**
     * @brief Currently used camera.
     */
    Camera camera;

    Raytracer() = delete;
    /**
     * @brief Construct a new Raytracer object.
     * @param img Image.
     */
    Raytracer(Image* img);

    /**
     * @brief renders the scene and stores data in image.
     */
    void render();
    /**
     * @brief Add renderable object to scene.
     * @param obj renderable object.
     */
    void add(Renderable* obj);
    /**
     * @brief Remove renderable object from scene.
     * @param obj renderable object.
     */
    void remove(Renderable* obj);
};
#endif // __RAYTRACER_H__