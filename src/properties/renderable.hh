#pragma once

#include "matrix4/matrix4.hh"

namespace pogl
{
    class Renderable
    {
    public:
        virtual void draw() = 0;

    private:
    };

} // namespace pogl
