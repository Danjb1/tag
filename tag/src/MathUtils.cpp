#include "MathUtils.h"

namespace MathUtils {

float clamp(float val, float min, float max)
{
    if (val < min)
    {
        return min;
    }
    return val > max ? max : val;
}

}  // namespace MathUtils
