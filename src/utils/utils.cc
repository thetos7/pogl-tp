#include "utils.hh"

#include <cmath>
#include <ostream>

namespace utils
{
    namespace
    {
        inline int get_compact()
        {
            static int i = std::ios_base::xalloc();
            return i;
        }

        inline int get_pretty()
        {
            static int p = std::ios_base::xalloc();
            return p;
        }
    } // namespace
    std::ostream &compact_on(std::ostream &out)
    {
        out.iword(get_compact()) = 1;
        return out;
    }

    std::ostream &compact_off(std::ostream &out)
    {
        out.iword(get_compact()) = 0;
        return out;
    }

    bool compact_enabled(std::ostream &out)
    {
        return out.iword(get_compact()) == 1;
    }
    
    std::ostream &pretty_print_on(std::ostream &out)
    {
        out.iword(get_pretty()) = 1;
        return out;
    }

    std::ostream &pretty_print_off(std::ostream &out)
    {
        out.iword(get_pretty()) = 0;
        return out;
    }

    bool pretty_print_enabled(std::ostream &out)
    {
        return out.iword(get_pretty()) == 1;
    }

    double fract(double x)
    {
        return x - std::floor(x);
    }
} // namespace utils
