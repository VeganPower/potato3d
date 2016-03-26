#pragma once
#include "base.hpp"
#include "scene.hpp"
#include "camera.hpp"

#include <memory>

class GLFWwindow;

namespace potato
{

class Raytracer;
class Rasterizer;
class GlRenderer;

class Rasterizer // Stub
{
public:
   Rasterizer(u16 w, u16 h){}
   void bake_scene(Scene const& scene){}
   void on_resize(u16 w, u16 h){}
   void render(Camera const&){}
};

enum RendererType
{
   eRaytracer,
   eRasterizer,
   eGl
};

class App
{
public:
   App(int argc, char const* const* argv);
   ~App();

   void load_scene();
   int run();

   void on_cursor_pos(double xpos, double ypos);
   void on_mouse_button(int button, int action, int mods);
   void on_key(int key, int scancode, int action, int mods);
   void on_character(unsigned int codepoint);
   void on_resize(int w, int h);
   void on_scroll(double xoffset, double yoffset);
private:
   void move_camera(f32 dt);
   bool zombie;   // app not initialized correctly go in zombie state
   u16 win_width;
   u16 win_height;
   GLFWwindow* window;

   Camera camera;
   // Transformation ;
   Scene scene;

   std::unique_ptr<Raytracer>  raytracer;
   std::unique_ptr<Rasterizer> rasterizer;
   std::unique_ptr<GlRenderer> gl_renderer;

   glm::vec2 mouse_pos;
   glm::vec2 old_mouse_pos;
   vec3 camera_speed;

   RendererType active_renderer;
};

} // namespace
