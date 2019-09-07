#include <iostream>
#include <SFML/Graphics.hpp>
#include "./include/VerticesFromBitmap.hpp"

void drawIslands(sf::RenderWindow & window,
                 std::vector<Island > const & islands,
                 float circleRadius = 1.0f)
{
    sf::CircleShape circ(circleRadius);
    circ.setOrigin({circleRadius, circleRadius});


    std::vector<sf::Vertex > verts;
    for(auto & island : islands)
    {
        verts.clear();
        for(unsigned int i=0; i<island.vertexPositions.size(); ++i)
        {
            Vec2 position = island.vertexPositions[i];
            verts.push_back({{(float)(position.x+0.5), (float)(position.y+0.5)}, sf::Color::Green});
            circ.setPosition({(float)(position.x+0.5), (float)(position.y+0.5)});
            window.draw(circ);
        }

        window.draw(verts.data(), verts.size(), sf::LineStrip);
    }
}

void resetView(sf::RenderWindow & window,
               sf::Image & image)
{
    sf::View view;
    view.setCenter({image.getSize().x/2.0f, image.getSize().y/2.0f});
    float minSize = image.getSize().x < image.getSize().y ? image.getSize().x : image.getSize().y;
    view.zoom(0.8f*minSize/window.getSize().x);
    window.setView(view);
}

int main()
{
    sf::RenderWindow window;
    sf::VideoMode mode(800, 800);
    window.create(mode, "test");


    VerticesFromBitmap bmp;
    sf::Sprite sprite;
    sf::Image image;
    image.loadFromFile("./test.png");
    sf::Texture texture;
    bmp.applyContrast(image);
    texture.loadFromImage(image);
    sprite.setTexture(texture);



    std::vector<Island > islands = bmp.generateIslands(image);



    std::cout << islands.size() << "numIslands\n";
    for(unsigned int i=0; i<islands.size(); ++i)
    {
        std::cout << "island " << i << " has " << islands[i].vertexPositions.size() << " vertices\n";
    }
    //std::cout << islands[0].vertexPositions.size() << "numVerts\n";


    bool drawImage = false;
    float averageSpacing = 30.0f;
    float circleRadius = 2.0f;
    resetView(window, image);

    while(window.isOpen())
    {
        sf::Event currEvent;

        while(window.pollEvent(currEvent))
        {
            switch(currEvent.type)
            {
                case(sf::Event::Closed):
                    window.close();
                    break;
                case(sf::Event::KeyPressed):
                {
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
                    {
                        averageSpacing *= 2.0f;
                        bmp.setAverageSpacing(averageSpacing);
                        image.loadFromFile("./test.png");
                        texture.loadFromFile("./test.png");
                        sprite.setTexture(texture);
                        resetView(window, image);
                    }
                    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
                    {
                        averageSpacing /= 2.0f;
                        bmp.setAverageSpacing(averageSpacing);
                        image.loadFromFile("./test.png");
                        texture.loadFromFile("./test.png");
                        sprite.setTexture(texture);
                        resetView(window, image);
                    }
                    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::R))
                    {
                        image.loadFromFile("./test.png");
                        texture.loadFromFile("./test.png");
                        sprite.setTexture(texture);
                        resetView(window, image);
                    }
                    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                    {
                        drawImage = !drawImage;
                    }
                    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::PageUp))
                    {
                        circleRadius *= 2.0f;
                    }
                    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::PageDown))
                    {
                        circleRadius /= 2.0f;
                    }
                    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::K))
                    {
                        bmp.setColinearThreshold(bmp.getColinearThreshold() - 0.05f);
                    }
                    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::L))
                    {
                        bmp.setColinearThreshold(bmp.getColinearThreshold() + 0.05f);
                    }


                    islands.clear();
                    islands = bmp.generateIslands(image);
                    std::cout << islands.size() << " numOfIslands\n";
                }
                default:
                    break;
            }
        }


        window.clear(sf::Color::Black);

        if(drawImage)
            window.draw(sprite);
        drawIslands(window, islands, circleRadius);

        sf::sleep(sf::milliseconds(16));

        window.display();
    }
}
