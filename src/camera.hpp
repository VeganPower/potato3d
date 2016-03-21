#pragma once

#include <memory>
#include "transformation.hpp"

namespace potato
{

class Camera
{
public:
   typedef std::shared_ptr<Camera> ptr;
   Camera();

   void transformation(Transformation const& p);
   Transformation const& transformation() const;

   f32 tan_half_fov() const;
private:
   Transformation t_;
   f32 fov_;
   f32 tan_half_fov_;
};

inline Camera::Camera()
   : t_(Transformation::identity)
   , fov_(90.f)
   , tan_half_fov_(tan(fov_/2.f))
{
}

inline Transformation const& Camera::transformation() const
{
   return t_;
}

inline void Camera::transformation(Transformation const& p)
{
   t_ = p;
}

inline f32 Camera::tan_half_fov() const
{
   return tan_half_fov_;
}

}
