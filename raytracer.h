//        __            _      __               __    ____             
//   ____/ /___ _____  (_)__  / /___ ___  ___  / /_  / / /_  ___  _____
//  / __  / __ `/ __ \/ / _ \/ / __ `__ \/ _ \/ __ \/ / __ \/ _ \/ ___/
// / /_/ / /_/ / / / / /  __/ / / / / / /  __/ / / / / /_/ /  __/ /    
// \__,_/\__,_/_/ /_/_/\___/_/_/ /_/ /_/\___/_/ /_/_/_.___/\___/_/     
//  https://github.com/danielmehlber                                     

#pragma once
#include "math.h"
#include "timing.h"
#include <list>
#include <fstream>
#include <iostream>
#include <array>
//For displaying.
#ifdef _WIN32
#include <windows.h>
#endif

struct Color{
    float r = 0, g = 0, b = 0;
    inline friend std::ostream& operator<<(std::ostream& stream, const Color& c){
        stream << (int)(c.r * 255) << std::endl;
        stream << (int)(c.g * 255) << std::endl;
        stream << (int)(c.b * 255) << std::endl;
        return stream;
    }

    inline float brightness(){
        float max = r;
        if(g > max) max = g;
        if(b > max) max = b;
        return max;
    }

    inline void add(float f){
        r += f;
        g += f;
        b += f;
        clamp_color();
    }

    inline void add(const Color& col){
        r += col.r;
        g += col.g;
        b += col.b;
        clamp_color();
    }

    inline void sub(float f){
        r -= f;
        g -= f;
        b -= f;
        clamp_color();
    }

    inline void sub(const Color& col){
        r -= col.r;
        g -= col.g;
        b -= col.b;
        clamp_color();
    }

    inline void mult(float f){
        r *= f;
        g *= f;
        b *= f;
        clamp_color();
    }

    inline void mult(const Color& col){
        r *= col.r;
        g *= col.g;
        b *= col.b;
        clamp_color();
    }

    inline void div(float f){
        r /= f;
        g /= f;
        b /= f;
        clamp_color();
    }

    inline void div(const Color& col){
        r /= col.r;
        g /= col.g;
        b /= col.b;
        clamp_color();
    }

    inline void clamp_color(){
        Vec3<float> v = {r, g, b};
        if(v.length() > 1){
            auto _v = v.norm();
            r = _v.x;
            g = _v.y;    
            b = _v.z;
        }
    }

    inline void operator+=(const Color& col){ add(col); };
    inline void operator-=(const Color& col){ sub(col); };
    inline void operator*=(const Color& col){ mult(col); };
    inline void operator/=(const Color& col){ div(col); };
    inline void operator+=(float col){ add(col); };
    inline void operator-=(float col){ sub(col); };
    inline void operator*=(float col){ mult(col); };
    inline void operator/=(float col){ div(col); };

    inline Color operator+(const Color& col){ Color _c(*this); _c.add(col); return _c; };
    inline Color operator-(const Color& col){ Color _c(*this); _c.sub(col); return _c; };
    inline Color operator*(const Color& col){ Color _c(*this); _c.mult(col); return _c; };
    inline Color operator/(const Color& col){ Color _c(*this); _c.div(col); return _c; };
    inline Color operator+(float col){ Color _c(*this); _c.add(col); return _c; };
    inline Color operator-(float col){ Color _c(*this); _c.sub(col); return _c; };
    inline Color operator*(float col){ Color _c(*this); _c.mult(col); return _c; };
    inline Color operator/(float col){ Color _c(*this); _c.div(col); return _c; };
    
};

/**
 * @brief Transformation of an object (position, rotation, scale)
 */
struct Transform{
    Vec3<float> pos, rot, scale;
};

/**
 * @brief Raw camera data. View is calculated in rendering process.
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
    /**
     * @brief If false, light will be ignored in the rendering process.
     */
    bool    visible     {true};
    /**
     * @brief Color of light
     */
    Color   color       {1,1,1};
    /**
     * @brief Intensity of light. Controls how strong the light "shines".
     */
    float   intensity   {1}; 
    /**
     * @brief Max distance of light.
     */
    float   distance    {10};
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

struct SceneData;

/**
 * @brief View Vectors of Camera.
 */
struct View {
    Vec3<float> ul, ur, ll, lr;
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
    Color               m_color {1, 1, 1};
    /**
     * @brief closest intersection to the camera --> visible intersection.
     */
    Intersection        m_closest;
    /**
     * @brief Ignore an object for the next fire iteration. (Could be emitter)
     */
    Renderable*         m_ignore;

    /**
     * @brief Construct a new Ray object
     * @param max_bounces bounce limit.
     * @param start start point.
     * @param dir direction of Ray.
     */
    Ray(const size_t max_bounces = 0, Vec3<float> start = {0,0,0}, Vec3<float> dir = {1,0,0});

    /**
     * @brief Fire ray and check for intersections.
     * @param renderlist list of objects to render (= list of intersectable objects).
     * @return Color Result and final color of ray.
     */
    Color fire(const SceneData& scene);

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
    Color base_color      {1, 1, 1};
    /**
     * @brief diffuse-ness of surface. 1 = no reflections, 0 = only reflections.
     */
    float diffuseness   {1.0f}; 
};

/**
 * @brief Bounding Box of an objects.
 */
class BoundingBox{
public:
    /**
     * @brief points a and b defining box.
     */
    Vec3<float> a, b;
    /**
     * @brief Construct a new Bounding Box object by margins of object (measured from position)
     * 
     * @param x1 width in pos x-direction.
     * @param x2 width in neg x-direction.
     * @param y1 length in pos y-direction. 
     * @param y2 length in neg y-direction.
     * @param z1 height in pos z-direction.
     * @param z2 height in neg z-direction.
     */
    BoundingBox(float x1, float x2, float y1, float y2, float z1, float z2);
    /**
     * @brief Checks if Geometry is in some camera's view.
     * 
     * @param cam Camera.
     * @return true Bounding box and Geometry inside could be visible to the camera.
     * @return false Bounding box is outside view.
     */
    bool check_visibility(const Camera& cam, const View& view);
    /**
     * @brief Calculate and return vertex points of bounding box.
     * 
     * @return std::array<Vec3<float>, 8> array of vertex positions.
     */
    inline std::array<Vec3<float>, 8> get_points() noexcept;
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
    Material    material;
    /**
     * @brief Checks for intersections with ray. (= Intersection stage)
     * @param ray Ray.
     * @return true The Ray has intersected with the object.
     * @return false No intersection took place.
     */
    virtual bool intersect(Ray& ray) = 0;
    /**
     * @brief Process intersection using Material data, Scene data and View Data. (= Materialization stage)
     * @param intersection clostest visible intersection on object.
     * @param ray ray used for detecting the intersection (For view data).
     * @return Color Final Color.
     */
    virtual Color process(const SceneData& scene, const Vec3<float>& intersection, const Ray& ray) = 0;
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
    virtual Color process(const SceneData& scene, const Vec3<float>& intersection, const Ray& ray) override;
};

/**
 * @brief Represents scene, all objects in it and data.
 */
struct SceneData{
    /**
     * @brief List of objects in the scene / to render.
     */
    std::list<Renderable*>  m_render_list;
    /**
     * @brief Lights used by the scene.
     */
    std::list<Light*> light_list;

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
    /**
     * @brief Adds light to scene.
     * @param light light.
     */
    void add(Light* light);
    /**
     * @brief Remove light from scene.
     * @param light light.
     */
    void remove(Light* light);
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
    Image*              m_img;
public:
    /**
     * @brief Current Scene data.
     */
    SceneData           scene;
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
};

void display(const Image* img);
