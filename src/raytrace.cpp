#include "raytrace.hpp"
#include <GLFW/glfw3.h>
#include "scene.hpp"

int align_int(int n, int alignment)
{
   return n + (n % alignment);
}

#define EPSILON 1e-07

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
         }
         else
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
   for (auto& m : scene)
   {
      std::copy(m.mesh->get_pos().begin(), m.mesh->get_pos().end(), std::back_insert_iterator<std::vector<vec3>>(positions));
      std::copy(m.mesh->get_faces().begin(), m.mesh->get_faces().end(), std::back_insert_iterator<std::vector<Face>>(triangles));
   }
}

void Raytracer::render(Camera const& viewpoint) const
{
   glm::mat3 mat = glm::mat3_cast(viewpoint.transformation().orientation());
   vec3 cam_pos = viewpoint.transformation().position();
   f32 tan_half_fov = viewpoint.tan_half_fov();
   f32 ratio = static_cast<f32>(width) / static_cast<f32>(height);

   f32 f_width = static_cast<f32>(width);
   f32 f_height = static_cast<f32>(height);

   vec3 x_step = 2.f * ratio * tan_half_fov * mat[0];
   vec3 y_step = -2.f * tan_half_fov * mat[1];
   vec3 top_left = cam_pos + mat[2] - (x_step - y_step)*0.5f;
   for (int j = 0; j < height; ++j)
   {
      int offset = j * width;
      f32 y = static_cast<f32>(j) / f_height;
      vec3 row_plane_pos = top_left - (y_step * y);
      for (int i = 0; i < width; ++i)
      {
         f32 x = static_cast<f32>(i) / f_width;
         vec3 ray_direction = glm::normalize(row_plane_pos + x_step * x - cam_pos);

         Ray ray = { cam_pos, ray_direction };

         bool hit = false;
         vec3 t_coord;
         for (auto const& f : triangles)
         {
            if (intersect(ray, f, t_coord))
            {
               hit = true;
               break;
            }
         }
         if(ray_direction.y < 0)
         {
            assert( hit == false );
         }
         int idx = offset + i;
         u8 r = hit ? (t_coord.s * 255) : 0x00;
         u8 g = hit ? (t_coord.t * 255) : 0x00;
         u8 b = 0;//std::min(std::max(y, 0.f), 1.f) * 255;
         frame_buffer[idx] = r | g << 8 | b << 16 | 0xFF << 24;
      }
   }
   glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, frame_buffer);
}


// Moller- Trumbore algorithm
bool Raytracer::intersect(Ray const& r, Face const& f, vec3& coord) const
{
   vec3 a = positions[f.a];
   vec3 b = positions[f.b];
   vec3 c = positions[f.c];

   vec3 edge1 = b - a;
   vec3 edge2 = c - a;

   vec3 p_vec = glm::cross(r.dir, edge2);
   f32 det = glm::dot(p_vec, edge1);
   if (fabs(det) < EPSILON)
   {
      return false;
   }
   f32 inv_det = 1.f / det;
   vec3 t_vec = r.origin - a;
   f32 u = glm::dot(p_vec, t_vec) * inv_det; // out
   if (u < 0.f || u > 1.f)
   {
      return false;
   }
   vec3 q_vec = glm::cross(t_vec, edge1);
   f32 v = glm::dot(r.dir, q_vec) * inv_det; // out
   if (v < 0.f || (u+v) > 1.f)
   {
      return false;
   }
   f32 t = -dot(edge2, q_vec) * inv_det; // out
   coord = vec3(u, v, t);
   return true;
}

}
