#pragma once

#include "matrix4/matrix4.hh"

namespace pogl
{
    class Renderable
    {
    public:
        virtual void draw() = 0;
        virtual void update_camera(const Matrix4 &camera_transform) = 0;

    private:
    };

} // namespace pogl
