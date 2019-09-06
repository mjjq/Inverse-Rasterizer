#include <iostream>
#include <SFML/Graphics.hpp>
#include "./include/VerticesFromBitmap.hpp"

void drawIslands(sf::RenderWindow & window,
                 std::vector<Island > const & islands)
{
    sf::CircleShape circ(3.1f);
    circ.setOrigin({3.1f, 3.1f});


    std::vector<sf::Vertex > verts;
    for(auto & island : islands)
    {
        verts.clear();
        for(int i=0; i<island.vertexPositions.size(); ++i)
        {
            Vec2 position = island.vertexPositions[i];
            verts.push_back({{position.x+0.5f, position.y+0.5f}, sf::Color::Green});
            circ.setPosition({position.x+0.5f, position.y+0.5f});
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

    sf::Sprite sprite;
    sf::Image image;
    image.loadFromFile("./test.png");
    sf::Texture texture;
    texture.loadFromFile("./test.png");
    sprite.setTexture(texture);



    VerticesFromBitmap bmp(image);

    std::vector<Island > islands = bmp.generateIslands();



    std::cout << islands.size() << "numIslands\n";
    for(int i=0; i<islands.size(); ++i)
    {
        std::cout << "island " << i << " has " << islands[i].vertexPositions.size() << " vertices\n";
    }
    //std::cout << islands[0].vertexPositions.size() << "numVerts\n";


    bool drawImage = false;
    float averageSpacing = 30.0f;
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
                        image.loadFromFile("./test.png");
                        bmp = VerticesFromBitmap(image, averageSpacing);
                        resetView(window, image);
                    }
                    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
                    {
                        averageSpacing /= 2.0f;
                        image.loadFromFile("./test.png");
                        bmp = VerticesFromBitmap(image, averageSpacing);
                        resetView(window, image);
                    }
                    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::R))
                    {
                        image.loadFromFile("./test.png");
                        bmp = VerticesFromBitmap(image, averageSpacing);
                        resetView(window, image);
                    }
                    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                    {
                        drawImage = !drawImage;
                    }


                    islands.clear();
                    islands = bmp.generateIslands();
                    std::cout << islands.size() << " numOfIslands\n";
                }
                default:
                    break;
            }
        }


        window.clear(sf::Color::Black);

        if(drawImage)
            window.draw(sprite);
        drawIslands(window, islands);

        sf::sleep(sf::milliseconds(16));

        window.display();
    }
}
