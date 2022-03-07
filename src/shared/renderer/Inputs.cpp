#include "Inputs.h"
#include "Context.h"

namespace spr { namespace input {
    bool pressedKey(int key) {
        return glfwGetKey(spr::getWindow(), key);
    }
}}