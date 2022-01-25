#pragma once

class Layer;

namespace spr {

    void init();
    void clear();
    void shutdown();

    void createWindow(const char* windowName, int width, int height);

}

namespace spr { namespace runtime {
    void attachLayer(Layer* layer);
    void dettachLayer(Layer* layer);
    
    void update();
}}