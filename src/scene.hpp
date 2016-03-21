#pragma once
#include <vector>
#include "mesh.hpp"
#include "camera.hpp"
#include "ray.hpp"
#include "transformation.hpp"

namespace potato
{

struct Node
{
   uint parent;
   Transformation t;
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

   // std::vector<Mesh::ptr>   meshes;
   // std::vector<Light>     lights;

};

inline void Scene::AddMesh(Mesh::ptr m)
{
   Node node = { 0, Transformation::identity, m};
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
