#pragma once
#include <glm/gtc/quaternion.hpp>

namespace potato
{

class Transformation
{
public:
   enum Identity{ identity };

   Transformation();
   Transformation(Identity);

   vec3 position() const;
   void position(vec3 const& p);
   glm::quat orientation() const;
   void orientation(glm::quat const& o);
private:
   vec3 offset;
   glm::quat pose;
};


inline Transformation::Transformation()
{
}

inline Transformation::Transformation(Identity )
   : offset(0.f, 0.f, 0.f)
   , pose(1.f, 0.f, 0.f, 0.f)
{
}

inline vec3 Transformation::position() const
{
   return offset;
}

inline void Transformation::position(vec3 const& p)
{
   offset = p;
}

inline glm::quat Transformation::orientation() const
{
   return pose;
}

inline void Transformation::orientation(glm::quat const& o)
{
   pose = o;
}

}
