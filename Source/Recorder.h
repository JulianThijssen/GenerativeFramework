#pragma once

#include "Renderer.h"

#include <string>

class Recorder
{
public:
    void record(Renderer& renderer, std::string filePath);
};
