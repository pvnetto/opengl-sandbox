#pragma once

#include <vector>

class Layer;
class Event;

// ===================================================
// ==== Runtime ======================================
// ===================================================
namespace spr { namespace runtime {

    void attachLayer(Layer* layer);
    void dettachLayer(Layer* layer);
    void replaceLayers(std::vector<Layer*> layers);
    
    void update();

    float getTime();
    float getDeltaTime();

    void handleEvent(Event &evt);

}}