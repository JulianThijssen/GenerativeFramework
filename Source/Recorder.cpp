#include "Recorder.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image_write.h"

void Recorder::record(Renderer& renderer, std::string filePath)
{
    std::vector<unsigned char> pixels;
    renderer.saveFramebufferContents(pixels);
    std::cout << (int)pixels[0] << " " << (int)pixels[1] << " " << (int)pixels[2] << " " << (int)pixels[3] << " " << (int)pixels[4] << std::endl;
    stbi_write_png(filePath.c_str(), 1024, 1024, 4, (void*) pixels.data(), 1024*4);
}
