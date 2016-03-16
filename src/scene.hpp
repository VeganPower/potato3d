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
   void AddMesh(Mesh::ptr );

   std::vector<Node>::const_iterator begin() const;
   std::vector<Node>::const_iterator end() const;
private:
   // plain tree?
   std::vector<Node> nodes;
   // triangle soup
};

inline void Scene::AddMesh(Mesh::ptr m)
{
   Node node = { 0, m};
   nodes.push_back(node);
}

inline std::vector<Node>::const_iterator Scene::begin() const
{
   nodes.begin();
}

inline std::vector<Node>::const_iterator Scene::end() const
{
   nodes.end();
}


}
