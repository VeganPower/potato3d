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

   std::vector<Face> const& get_faces() const;
   std::vector<vec3> const& get_pos() const;
private:
   std::vector<vec3> positions;
   std::vector<Face> triangles;
};

inline std::vector<Face> const& Mesh::get_faces() const
{
   return triangles;
}

inline std::vector<vec3> const& Mesh::get_pos() const
{
   return positions;
}


}
