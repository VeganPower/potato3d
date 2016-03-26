#include "app.hpp"
#include <GLFW/glfw3.h>
#include "raytrace.hpp"
#include "gl_renderer.hpp"

#include "mesh.hpp"

#include <cstdlib>
#include <iostream>
#include <iomanip>


namespace potato
{

static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
   App* app = reinterpret_cast<App*>(glfwGetWindowUserPointer(window));
   app->on_cursor_pos(xpos, ypos);
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
   App* app = reinterpret_cast<App*>(glfwGetWindowUserPointer(window));
   app->on_mouse_button(button, action, mods);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   App* app = reinterpret_cast<App*>(glfwGetWindowUserPointer(window));
   app->on_key(key, scancode, action, mods);
}

static void character_callback(GLFWwindow* window, unsigned int codepoint)
{
   App* app = reinterpret_cast<App*>(glfwGetWindowUserPointer(window));
   app->on_character(codepoint);
}

static void resize_callback(GLFWwindow* window, int w, int h)
{
   App* app = reinterpret_cast<App*>(glfwGetWindowUserPointer(window));
   app->on_resize(w, h);
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
   App* app = reinterpret_cast<App*>(glfwGetWindowUserPointer(window));
   app->on_scroll(xoffset, yoffset);
}

App::App(int argc, char const* const* argv)
   : zombie(false)
   , win_width(800)
   , win_height(600)
   , window(nullptr)
   , raytracer(nullptr)
   , rasterizer(nullptr)
   , gl_renderer(nullptr)
{
   if (glfwInit() != GL_TRUE)
   {
      zombie = true;
      return;
   }
   // Parse options

   // Create window
   window = glfwCreateWindow(win_width, win_height, "Hello World", NULL, NULL);
   if (window == nullptr)
   {
      zombie = true;
      return;
   }

   raytracer = std::unique_ptr<Raytracer>( new Raytracer(win_width, win_height));
   rasterizer = std::unique_ptr<Rasterizer>( new Rasterizer(win_width, win_height));
   gl_renderer = std::unique_ptr<GlRenderer>( new GlRenderer());

   // Setup callback
   glfwSetCursorPosCallback(window, cursor_pos_callback);
   glfwSetMouseButtonCallback(window, mouse_button_callback);
   glfwSetKeyCallback(window, key_callback);
   glfwSetCharCallback(window, character_callback);
   glfwMakeContextCurrent(window);
   glfwSetWindowUserPointer(window, this);
   glfwSetFramebufferSizeCallback(window, resize_callback);
   glfwSetScrollCallback(window, scroll_callback);

//   #include "mesh.hpp"
   Transformation t = camera.transformation();
   t.position(vec3(0.f, 0.f, -1.f));
   camera.transformation(t);
}
App::~App()
{
   glfwTerminate();
}

void App::on_cursor_pos(double xpos, double ypos)
{

}

void App::on_mouse_button(int button, int action, int mods)
{

}

void App::on_key(int key, int scancode, int action, int mods)
{
   if (action == GLFW_REPEAT)
   {
      return;
   }
   switch (key)
   {
   case GLFW_KEY_Q:
   case GLFW_KEY_ESCAPE:
      glfwSetWindowShouldClose(window, 1);
      break;
   case GLFW_KEY_F5:
      active_renderer = eRaytracer;
      break;
   case GLFW_KEY_F6:
      active_renderer = eRasterizer;
      break;
   case GLFW_KEY_F7:
      active_renderer = eGl;
      break;
  /* case GLFW_KEY_W:
      directions = set_direction(directions, action == GLFW_PRESS, forward );
      break;
   case GLFW_KEY_A:
      directions = set_direction(directions, action == GLFW_PRESS, left );
      break;
   case GLFW_KEY_S:
      directions = set_direction(directions, action == GLFW_PRESS, backward );
      break;
   case GLFW_KEY_D:
      directions = set_direction(directions, action == GLFW_PRESS, right );
      break;*/
   }
}

void App::on_character(unsigned int codepoint)
{

}

void App::on_resize(int w, int h)
{
   win_width = static_cast<u16>(w);
   win_height = static_cast<u16>(h);

   raytracer->on_resize(win_width, win_height);
   // rasterizer->on_resize(win_width, win_height);
   gl_renderer->on_resize(win_width, win_height);
   GLsizei w_i = static_cast<GLsizei>(w);
   GLsizei h_i = static_cast<GLsizei>(h);
   glViewport(0, 0, w_i, h_i);
}

void App::on_scroll(double xoffset, double yoffset)
{

}

void App::load_scene()
{
   //Mesh::Ptr mesh = Mesh::load("")
   Face f = {0, 1, 2};
   std::vector<Face> tri;
   tri.push_back(f);

   std::vector<vec3> pos;
   vec3 a = { 0.0, 0.0, 0.0 };
   vec3 b = { 0.0, 1.0, 0.0 };
   vec3 c = { 1.0, 0.0, 0.0 };
   pos.push_back(a);
   pos.push_back(b);
   pos.push_back(c);

   Mesh::ptr mesh = std::make_shared<Mesh>(tri, pos);
   scene.AddMesh(mesh);

   raytracer->bake_scene(scene);
   // rasterizer->bake_scene(scene);
   gl_renderer->bake_scene(scene);
}

int App::run()
{
   if (zombie)
   {
      return 1;
   }
   double last_render_time = glfwGetTime();
   f32 elapsed_frame_time = 0.0;
   double average_render_time = 0.0;
   // need_redraw = true;
   while (!glfwWindowShouldClose(window))
   {
      double now = glfwGetTime();
      elapsed_frame_time = static_cast<f32>(now - last_render_time);
      last_render_time = now;
      // Move camera?
      move_camera(elapsed_frame_time);
      glClear(GL_COLOR_BUFFER_BIT);
      {
         double start_time = glfwGetTime();
         switch (active_renderer)
         {
         case eRaytracer:
            raytracer->render(camera);
            break;
         case eRasterizer:
            rasterizer->render(camera);
            break;
         case eGl:
            gl_renderer->render(camera);
            break;
         }
         double end_time = glfwGetTime();

         average_render_time = average_render_time * 0.90 + (end_time - start_time) * 0.10;
         //std::cout << "Elapsed time: " << std::setw(10) <<  std::left << elapsed_frame_time << "Render time: " << std::setw(10) << average_render_time << std::endl;
      }
      // swap buffer
      glfwSwapBuffers(window);
      glfwPollEvents();
   }
   return 0;
}

void App::move_camera(f32 dt)
{
   glm::vec2 mouse_delta = old_mouse_pos - mouse_pos;
   old_mouse_pos = mouse_pos;

   vec3 camera_pos = camera.transformation().position() + camera_speed * dt;
   //camera_speed +=
}

}


int main(int argc, char const *argv[])
{
   potato::App app(argc, argv);

   app.load_scene();
   return app.run();
}
