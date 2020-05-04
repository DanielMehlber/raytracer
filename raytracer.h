#include "math.h"
#include <list>

union Color{
    int rgb_value;
    char data[4];
};


struct Transform{
    Vec3<float> pos, rot, scale;
};

struct Camera : Transform {
    int         max_ray_bounces {3};
};

struct Light : Transform {
    Color color;
};


class Image : public Matrix<Color> {
protected:
public:
    Image() = delete;
    Image(const size_t width, const size_t height);
};


class Renderable {
public:
    bool m_visible  {true};

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