#pragma once
#include <vector>
#include "mesh.hpp"
#include "ray.hpp"

namespace potato
{

struct Node
{
   uint parent;
   Mesh::ptr mesh;
};

class Scene
{
public:
   void AddObj(Mesh::ptr );
   bool Occluded(Ray const&);
   // iterate triangle?
private:
   // plain tree?
   std::vector<Node> nodes;
   // triangle soup
};

inline void Scene::AddObj(Mesh::ptr m)
{
   Node node = { 0, m};
   nodes.push_back(node);
}

}
