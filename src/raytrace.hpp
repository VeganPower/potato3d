#pragma once

#include "base.hpp"
#include "mesh.hpp" // TODO: Put face in another file

#include <vector>

namespace potato
{

class Scene;
class Camera;

// TODO: common interface for renderer?
class Raytracer
{
public:
   Raytracer(u16 width, u16 height);
   ~Raytracer();

   void bake_scene(Scene const& scene);
   void render(Camera const& viewpoint) const;
private:
   bool intersect(Ray const& r, Face const& f, vec3& coord) const;
   u16 width;
   u16 height;
   u32* frame_buffer;

   std::vector<vec3> positions;
   std::vector<Face> triangles;
};

}
