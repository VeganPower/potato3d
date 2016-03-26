#pragma once

namespace potato
{

class Scene;
class Camera;

class GlRenderer
{
public:
   GlRenderer();
   void bake_scene(Scene const& scene);
   void render(Camera const&);
   void on_resize(u16 w, u16 h);
};

}
