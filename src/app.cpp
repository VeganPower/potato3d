#include "app.hpp"
#include <GLFW/glfw3.h>
#include "raytrace.hpp"

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
   on_resize(win_width, win_height);
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


}
App::~App()
{
   delete raytracer;
   delete rasterizer;
   delete gl_renderer;
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
   if( action == GLFW_REPEAT )
   {
      return;
   }
   switch( key )
   {
   case GLFW_KEY_Q:
   case GLFW_KEY_ESCAPE:
      glfwSetWindowShouldClose( window, 1 );
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
   delete raytracer;
   raytracer = new Raytracer(win_width, win_height);
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

   Mesh::ptr mesh = std::make_shared<Mesh>( tri, pos);
   scene.AddMesh(mesh);

   raytracer->bake_scene(scene);
   // rasterizer->bake_scene(scene);
   // gl_renderer->bake_scene(scene);
}

int App::run()
{
   if (zombie)
   {
      return 1;
   }
   double last_render_time = glfwGetTime();
   double elapsed_frame_time = 0.0;
   double average_render_time = 0.0;
   // need_redraw = true;
   while (!glfwWindowShouldClose(window))
   {
      double now = glfwGetTime();
      elapsed_frame_time = now - last_render_time;
      last_render_time = now;
      // Move camera?
      // move_camera(elapsed_frame_time);
      // float f_widht = win_width;
      // float f_height = win_height;
      // float ratio = f_widht / f_height;
      // vec3 cam_x = cam.x_axis() * ratio;
      // vec3 cam_y = cam.y_axis();
      // vec3 cam_z = cam.z_axis();
      // vec3 cam_p = cam.position();
      // auto new_z = -cam_z - cam_x * 0.5f - cam_y * 0.5f;
      // float rx = 1.0f / float(f_widht);
      // float ry = 1.0f / float(f_height);
      // ispc::Vec3f vx = glm_to_ispc(cam_x * rx);
      // ispc::Vec3f vy = glm_to_ispc(cam_y * ry);
      // ispc::Vec3f vz = glm_to_ispc(new_z);
      // ispc::Vec3f p  = glm_to_ispc(cam.position());
      {
         double start_time = glfwGetTime();

         raytracer->render(camera);
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

}


int main(int argc, char const *argv[])
{
   potato::App app(argc, argv);

   app.load_scene();
   return app.run();
}
