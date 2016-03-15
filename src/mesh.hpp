#pragma once
#include "base.hpp"
#include <vector>
#include <memory>

namespace potato
{

struct Ray;

struct Face
{
   uint a, b, c;
};

class Mesh
{
public:
   typedef std::shared_ptr<Mesh> ptr;
   static ptr load(char const* path);

   Mesh(std::vector<Face> tri, std::vector<vec3> pos);

   uint intersect_triangle(Ray const& ray);
private:
   std::vector<vec3> positions;
   std::vector<Face> triangles;
};


}
