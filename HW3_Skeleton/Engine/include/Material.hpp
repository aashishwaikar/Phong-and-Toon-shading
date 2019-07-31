#pragma once

#include <glm/glm.hpp>

#include <Program.hpp>

namespace Engine
{
    class Material
    {
    protected:
        Program* _program;
    public:
        Material();
        ~Material();
        Program *GetProgram() { return _program; }
    };
}