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

struct Drop
{
    Vector2f pos;
    float radius;
};

class Application : public KeyListener
{
public:
    void init()
    {
        window.create("EvoTest", 1024, 1024);
        window.addKeyListener(this);
        window.enableVSync(true);
    }

    void update()
    {
        renderer.init();

        Camera& camera = renderer.getCamera();
        camera.zNear = 0.1f;
        camera.zFar = 30.0f;
        camera.fovy = 60;
        camera.aspect = 1;
        camera.position = Vector3f(0, 0, 2);

        while (!window.shouldClose())
        {
            glClearColor(0, 0, 0, 1);
            glClear(GL_COLOR_BUFFER_BIT);
            if (stop)
            {
                window.update();
                continue;
            }
            // Generate new drops
            for (int i = 0; i < 40; i++)
            {
                float u1 = distribution(generator) * 2 - 1;
                float u2 = distribution(generator) * 2 - 1;
                drops.push_back({ Vector2f(u1, u2), 1 });
            }

            // Merge drops
            for (int i = 0; i < drops.size(); i++)
            {
                Drop& a = drops[i];
                if (a.radius < 0.5f)
                    continue;

                for (int j = i + 1; j < drops.size(); j++)
                {
                    Drop& b = drops[j];

                    if ((a.pos - b.pos).length() < (a.radius + b.radius) * 0.005f)
                    {
                        a.radius += b.radius / a.radius;
                        b.radius = 0;
                    }
                }
            }

            // Delete merged drops
            auto it = drops.begin();
            while (it != drops.end())
            {
                if ((*it).radius < 0.5f)
                {
                    drops.erase(it);
                }
                else
                    ++it;
            }

            // Draw drops
            for (const Drop& drop : drops)
            {
                renderer.drawCircle(drop.pos, drop.radius * 0.005f);
            }

            renderer.update();

            window.update();
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

    PolyLine polyLine;

    std::vector<Drop> drops;

    bool stop = true;
};

int main()
{
    Application application;
    application.init();
    application.update();
}
