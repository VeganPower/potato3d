#include "base.hpp"

#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include "mesh.hpp"
#include "scene.hpp"

int align_int(int n, int alignment)
{
   return n + (n % alignment);
}

namespace potato
{

static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void character_callback(GLFWwindow* window, unsigned int codepoint);
static void resize_callback(GLFWwindow* window, int w, int h);
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


class RayApp
{
public:
   RayApp(int argc, char const* const* argv)
      : zombie(false)
      , win_width(800)
      , win_height(600)
      , window(nullptr)
      , frame_buffer(nullptr)
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

   }
   ~RayApp()
   {
      free(frame_buffer);
      glfwTerminate();
   }

   void load_scene()
   {
      Mesh::ptr blonde = Mesh::load("data/Blonde/Blonde.obj");
      scene.AddObj(blonde);

   }
   int run();

   void on_cursor_pos(double xpos, double ypos);
   void on_mouse_button(int button, int action, int mods);
   void on_key(int key, int scancode, int action, int mods);
   void on_character(unsigned int codepoint);
   void on_resize(int w, int h);
   void on_scroll(double xoffset, double yoffset);
private:
   void render() const;


   bool zombie;   // app not initialized correctly go in zombie state
   uint win_width;
   uint win_height;
   uint buffer_stride;
   GLFWwindow* window;
   u32* frame_buffer;

   uint k = 0;

   Scene scene;
};

int RayApp::run()
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
         k++;
         render();
         double end_time = glfwGetTime();

         average_render_time = average_render_time * 0.90 + (end_time - start_time) * 0.10;
         //std::cout << "Elapsed time: " << std::setw(10) <<  std::left << elapsed_frame_time << "Render time: " << std::setw(10) << average_render_time << std::endl;
      }
      // swap buffer
      glfwSwapBuffers(window);
      glfwPollEvents();
   }
   render();
   return 0;
}

void RayApp::render() const
{

   for (int j = 0; j < win_height; ++j)
   {
      int offset = j * win_width;
      for (int i = 0; i < win_width; ++i)
      {
         int idx = offset + i;
         int o = (i / 16) ^ ((j + k)/ 16);
         if (o & 0x1)
         {
            frame_buffer[idx] = 0xFFFFFF;
         }else
            frame_buffer[idx] = 0x000000;
      }
   }
   glDrawPixels(win_width, win_height, GL_RGBA, GL_UNSIGNED_BYTE, frame_buffer);
}

void RayApp::on_cursor_pos(double xpos, double ypos)
{

}

void RayApp::on_mouse_button(int button, int action, int mods)
{

}

void RayApp::on_key(int key, int scancode, int action, int mods)
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

void RayApp::on_character(unsigned int codepoint)
{

}

void RayApp::on_resize(int w, int h)
{
   free(frame_buffer);
   frame_buffer = static_cast<u32*>(malloc(sizeof(u32) * w * h));
   for (int j = 0; j < h; ++j)
   {
      int k = j * w;
      for (int i = 0; i < w; ++i)
      {
         int idx = k + i;
         int o = (i / 16) ^ (j / 16);
         if (o & 0x1)
         {
            frame_buffer[idx] = 0xFFFFFF;
         }else
            frame_buffer[idx] = 0x000000;
      }
   }
   win_width = w;
   win_height = h;
}

void RayApp::on_scroll(double xoffset, double yoffset)
{

}

void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
   RayApp* app = reinterpret_cast<RayApp*>(glfwGetWindowUserPointer(window));
   app->on_cursor_pos(xpos, ypos);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
   RayApp* app = reinterpret_cast<RayApp*>(glfwGetWindowUserPointer(window));
   app->on_mouse_button(button, action, mods);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   RayApp* app = reinterpret_cast<RayApp*>(glfwGetWindowUserPointer(window));
   app->on_key(key, scancode, action, mods);
}

void character_callback(GLFWwindow* window, unsigned int codepoint)
{
   RayApp* app = reinterpret_cast<RayApp*>(glfwGetWindowUserPointer(window));
   app->on_character(codepoint);
}

void resize_callback(GLFWwindow* window, int w, int h)
{
   RayApp* app = reinterpret_cast<RayApp*>(glfwGetWindowUserPointer(window));
   app->on_resize(w, h);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
   RayApp* app = reinterpret_cast<RayApp*>(glfwGetWindowUserPointer(window));
   app->on_scroll(xoffset, yoffset);
}

}

int main(int argc, char const *argv[])
{
   potato::RayApp app(argc, argv);

   app.load_scene();
   return app.run();
}
