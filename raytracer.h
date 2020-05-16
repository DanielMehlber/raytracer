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

struct Color{
    int r = 0, g = 0, b = 0;
    inline friend std::ostream& operator<<(std::ostream& stream, const Color& c){
        stream << c.r << std::endl;
        stream << c.g << std::endl;
        stream << c.b << std::endl;
        return stream;
    }
    template <typename T> inline Color operator*(T scalar){
        return {(int)(r * scalar), (int)(g * scalar), (int)(b * scalar)};
    }
};


struct Transform{
    Vec3<float> pos, rot, scale;
};

struct Camera : Transform {
    int         max_ray_bounces {3};
    Vec2<float> view_plane      {1, 1};
    float       distance        {1.0f};
};

struct Light : Transform {
    Color color;
};


class Image : public Matrix<Color> {
protected:
public:
    Image() = delete;
    Image(const size_t width, const size_t height);

    void write(const char* dest) const;

    inline const size_t width() const { return m_colums; }
    inline const size_t height() const { return m_rows; } 
};

struct Renderable;

struct Intersection {
    float dist = -1;
    Color color;
};

struct Ray{

    const size_t        m_max_bounces;
    const Vec3<float>   m_start;
    const Vec3<float>   m_dir;
    Color               m_color {255, 255, 255};
    Intersection        m_closest;

    Ray(const size_t max_bounces = 0, Vec3<float> start = {0,0,0}, Vec3<float> dir = {1,0,0});

    Color fire(std::list<Renderable*>* renderlist);

    void intersection(const Intersection& inter);
};


struct Material {
    Color base      {255, 0, 255};
    float diffuse   {1.0f}; 
};

struct Renderable {
    bool        m_visible  {true};
    Material    m_material;
    virtual bool intersect(Ray& ray) = 0;
};

struct Sphere : Renderable, Transform{
    float radius {1.0f};
    virtual bool intersect(Ray& ray) override;
};

class Raytracer{
protected:
    Image*                  m_img;
    std::list<Renderable*>  m_render_list;
public:

    Camera camera;

    Raytracer() = delete;
    Raytracer(Image* img);

    void render();
    void add(Renderable* obj);
    void remove(Renderable* obj);
};
#endif // __RAYTRACER_H__