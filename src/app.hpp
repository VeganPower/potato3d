#pragma once
#include "base.hpp"
#include "scene.hpp"

class GLFWwindow;

namespace potato
{

class Raytracer;
class Rasterizer;
class GlRenderer;

class Camera // Stub
{

};

class Rasterizer // Stub
{

};

class GlRenderer // Stub
{

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
   bool zombie;   // app not initialized correctly go in zombie state
   u16 win_width;
   u16 win_height;
   GLFWwindow* window;

   Camera camera;
   Scene scene;

   Raytracer*  raytracer;
   Rasterizer* rasterizer;
   GlRenderer* gl_renderer;
};

} // namespace
