#include "Renderer.h"

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
        window.create("Generative Framework", 1024, 1024);
        window.addKeyListener(this);

        //fboTexture.create();
        //fboTexture.bind(TextureUnit::TEXTURE0);
        //fboTexture.setSampling(NEAREST, NEAREST);
        //fboTexture.setData(1024, 1024, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, 0);

        //fbo.create();
        //fbo.bind();
        //fbo.addColorTexture(0, fboTexture);
        //fbo.validate();
    }
    float rot = 0;
    void update()
    {
        //Geometry geom;
        //geom.vertices.push_back(Vector3f(-1, -1, 0));
        //geom.vertices.push_back(Vector3f(1, -1, 0));
        //geom.vertices.push_back(Vector3f(-1, 1, 0));
        //geom.vertices.push_back(Vector3f(1, 1, 0));

        //GLuint vao;
        //glGenVertexArrays(1, &vao);
        //glBindVertexArray(vao);
        //GLuint vbo;
        //glGenBuffers(1, &vbo);
        //glBindBuffer(GL_ARRAY_BUFFER, vbo);
        //glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * geom.vertices.size(), geom.vertices.data(), GL_STATIC_DRAW);
        //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        //glEnableVertexAttribArray(0);

        renderer.init();

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
                //targetPoint *= 5;

                velocity = velocity * 0.999f + normalize(targetPoint - position) * 0.001f;

                //Vector3f dir(distribution(generator)*cos(pow(time, 6)), distribution(generator)*sin(pow(time, 6)), distribution(generator));
                //dir.normalize();
                //dir *= 4;

                //dir += (Vector3f(0) - savedPoint);
                //dir.normalize();

                //Vector3f point = savedPoint + dir * 0.05f;
                //heading = dir;

                Vector3f newPosition = position + velocity * 0.01f;

                renderer.drawLine(position, newPosition);

                //savedPoint = point;
                position = newPosition;

                glClearColor(0, 0, 0, 1);
                glClear(GL_COLOR_BUFFER_BIT);

                if (it % 100 == 0)
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

    bool stop = false;
};

int main()
{
    Application application;
    application.init();
    application.update();
}
