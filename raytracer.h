
//        __            _      __               __    ____             
//   ____/ /___ _____  (_)__  / /___ ___  ___  / /_  / / /_  ___  _____
//  / __  / __ `/ __ \/ / _ \/ / __ `__ \/ _ \/ __ \/ / __ \/ _ \/ ___/
// / /_/ / /_/ / / / / /  __/ / / / / / /  __/ / / / / /_/ /  __/ /    
// \__,_/\__,_/_/ /_/_/\___/_/_/ /_/ /_/\___/_/ /_/_/_.___/\___/_/     
//  https://github.com/danielmehlber                                     

#include "math.h"
#include <list>
#include <fstream>

struct Color{
    int r = 0, g = 0, b = 0;
    inline friend std::ostream& operator<<(std::ostream& stream, const Color& c){
        stream << c.r << std::endl;
        stream << c.g << std::endl;
        stream << c.b << std::endl;
        return stream;
    }
};


struct Transform{
    Vec3<float> pos, rot, scale;
};

struct Camera : Transform {
    int         max_ray_bounces {3};
    Vec2<float> view_plane      {1, 1};
    float       distance        {0.2f};
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