#include "Renderer.h"
#include "PolyLine.h"

#include <GDT/Window.h>

#include <GDT/Framebuffer.h>
#include <GDT/Texture.h>
#include <GDT/Vector2f.h>
#include <GDT/Vector3f.h>
#include <GDT/OpenGL.h>


//////////////////////////
#include "Geometry.h"
#include <random>
//////////////////////////

std::default_random_engine generator;
std::uniform_real_distribution<float> distribution;

class Application : public KeyListener
{
public:
    void init()
    {
        window.create("OrbitLine", 1024, 1024);
        window.addKeyListener(this);
        window.enableVSync(false);
    }

    float rot = 0;
    void update()
    {
        renderer.init(window.getWidth(), window.getHeight());

        Camera& camera = renderer.getCamera();
        camera.zNear = 0.1f;
        camera.zFar = 30.0f;
        camera.fovy = 60;
        camera.aspect = 1;
        camera.position = Vector3f(0, 0, 10);

        float time = 0;
        int it = 0;
        heading.set(0, 1, 0);
        while (!window.shouldClose())
        {
            if (!stop) {
                it++;
                time += 0.02f;

                if ((targetPoint - position).length() < 0.1)
                    targetPoint.set(distribution(generator)*6-3, distribution(generator)*6-3, distribution(generator)*6-3);

                velocity = velocity * 0.999f + normalize(targetPoint - position) * 0.001f;

                Vector3f newPosition = position + velocity * 0.01f;

                if (it % 100 == 0)
                    polyLine.addVertex(newPosition);

                renderer.drawPolyline(polyLine);

                position = newPosition;

                glClearColor(0, 0, 0, 1);
                glClear(GL_COLOR_BUFFER_BIT);

                if (it % 1000 == 0)
                {
                    rot += 0.5f;
                    renderer.modelMatrix.setIdentity();
                    renderer.modelMatrix.rotate(rot, 0, 1, 0);

                    renderer.update();

                    window.update();
                }
            }
            else {
                window.update();
            }
        }
    }

    virtual void onKeyPressed(int key, int mods) override
    {
        stop = !stop;
    }

    virtual void onKeyReleased(int key, int mods) override
    {

    }

private:
    Window window;
    Renderer renderer;

    Framebuffer fbo;
    Texture2D fboTexture;

    Vector3f savedPoint = Vector3f(0, 0, 0);
    Vector3f heading;
    Vector3f targetPoint;
    Vector3f position;
    Vector3f velocity;

    PolyLine polyLine;

    bool stop = false;
};

int main()
{
    Application application;
    application.init();
    application.update();
}
