#include "raytrace.hpp"
#include <GLFW/glfw3.h>
#include "scene.hpp"

int align_int(int n, int alignment)
{
   return n + (n % alignment);
}

namespace potato
{

Raytracer::Raytracer(u16 w, u16 h)
   : width(w)
   , height(h)
{
   frame_buffer = static_cast<u32*>(malloc(sizeof(u32) * width * height));
   for (int j = 0; j < height; ++j)
   {
      int k = j * w;
      for (int i = 0; i < width; ++i)
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
}

Raytracer::~Raytracer()
{
   free(frame_buffer);
}

void Raytracer::bake_scene(Scene const& scene)
{
   // TODO: Rewrite, this is just temp and will work only for a mesh
   for(auto& m : scene)
   {
      std::copy(m.mesh->get_pos().begin(), m.mesh->get_pos().end(), std::back_insert_iterator<std::vector<vec3>>(positions));
      std::copy(m.mesh->get_faces().begin(), m.mesh->get_faces().end(), std::back_insert_iterator<std::vector<Face>>(triangles));
   }
}

void Raytracer::render(Camera const& viewpoint) const
{
   for (int j = 0; j < height; ++j)
   {
      int offset = j * width;
      for (int i = 0; i < width; ++i)
      {
         int idx = offset + i;
         int o = (i / 16) ^ ((j)/ 16);
         if (o & 0x1)
         {
            frame_buffer[idx] = 0xFFFFFF;
         }else
            frame_buffer[idx] = 0x000000;
      }
   }
   glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, frame_buffer);
}

}
