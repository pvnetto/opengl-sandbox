#pragma once

#include <GLFW/glfw3.h>

namespace spr { namespace input {

    bool pressedKey(int key);

}}

enum SPREvents {
    SPR_RELEASE = GLFW_RELEASE,
    SPR_PRESS = GLFW_PRESS,
    SPR_REPEAT = GLFW_REPEAT,
};

enum SPRKeys {
    SPR_KEY_SPACE = GLFW_KEY_SPACE,           
    SPR_KEY_APOSTROPHE = GLFW_KEY_APOSTROPHE,
    SPR_KEY_COMMA = GLFW_KEY_COMMA,
    SPR_KEY_MINUS = GLFW_KEY_MINUS,
    SPR_KEY_PERIOD = GLFW_KEY_PERIOD,
    SPR_KEY_SLASH = GLFW_KEY_SLASH,
    SPR_KEY_0 = GLFW_KEY_0,               
    SPR_KEY_1 = GLFW_KEY_1,               
    SPR_KEY_2 = GLFW_KEY_2,               
    SPR_KEY_3 = GLFW_KEY_3,               
    SPR_KEY_4 = GLFW_KEY_4,               
    SPR_KEY_5 = GLFW_KEY_5,               
    SPR_KEY_6 = GLFW_KEY_6,               
    SPR_KEY_7 = GLFW_KEY_7,               
    SPR_KEY_8 = GLFW_KEY_8,               
    SPR_KEY_9 = GLFW_KEY_9,               
    SPR_KEY_SEMICOLON = GLFW_KEY_SEMICOLON,
    SPR_KEY_EQUAL = GLFW_KEY_EQUAL,
    SPR_KEY_A = GLFW_KEY_A,               
    SPR_KEY_B = GLFW_KEY_B,               
    SPR_KEY_C = GLFW_KEY_C,               
    SPR_KEY_D = GLFW_KEY_D,               
    SPR_KEY_E = GLFW_KEY_E,               
    SPR_KEY_F = GLFW_KEY_F,               
    SPR_KEY_G = GLFW_KEY_G,               
    SPR_KEY_H = GLFW_KEY_H,               
    SPR_KEY_I = GLFW_KEY_I,               
    SPR_KEY_J = GLFW_KEY_J,               
    SPR_KEY_K = GLFW_KEY_K,               
    SPR_KEY_L = GLFW_KEY_L,               
    SPR_KEY_M = GLFW_KEY_M,               
    SPR_KEY_N = GLFW_KEY_N,               
    SPR_KEY_O = GLFW_KEY_O,               
    SPR_KEY_P = GLFW_KEY_P,               
    SPR_KEY_Q = GLFW_KEY_Q,               
    SPR_KEY_R = GLFW_KEY_R,               
    SPR_KEY_S = GLFW_KEY_S,               
    SPR_KEY_T = GLFW_KEY_T,               
    SPR_KEY_U = GLFW_KEY_U,               
    SPR_KEY_V = GLFW_KEY_V,               
    SPR_KEY_W = GLFW_KEY_W,               
    SPR_KEY_X = GLFW_KEY_X,               
    SPR_KEY_Y = GLFW_KEY_Y,               
    SPR_KEY_Z = GLFW_KEY_Z,               
};