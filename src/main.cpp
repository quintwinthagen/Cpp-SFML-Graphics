#include "SFML/Graphics.hpp"
#include "AstarQ.hpp"
#include <iostream>


int main()
{
    const int ww = 800;
    const int wh = 800;

    astar Game(40, 40, ww, wh);
    Game.InitVertices();

    sf::RenderWindow window(sf::VideoMode(ww, wh), "A* Pathfinding on Grid");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::A)
                {
                    Game.SolveStep();
                }
            }
        }

        window.clear(sf::Color::White);

        Game.SolveStep();
        Game.UpdateVertices();
        window.draw(Game);

        window.display();
        //Sleep(500);
    }

}

#if 0
int main()
{
    const std::string fragmentShader = \
        "void main()" \
        "{" \
        "    gl_FragColor = vec4(255, 0, 0, 1.0);" \
        "}";

    sf::RenderWindow window(sf::VideoMode(800, 800), "Shaders");

    sf::VertexArray quad(sf::Quads, 4);
    quad[0] = sf::Vertex(sf::Vector2f(0.0f, 0.0f), sf::Color(0, 0, 0));
    quad[1] = sf::Vertex(sf::Vector2f(800.0f, 0.0f), sf::Color(255, 0, 0));
    quad[2] = sf::Vertex(sf::Vector2f(800.0f, 800.0f), sf::Color(255, 255, 0));
    quad[3] = sf::Vertex(sf::Vector2f(0.0f, 800.0f), sf::Color(0, 255, 0));

    sf::Shader shader;
    
    if (!shader.loadFromMemory(fragmentShader, sf::Shader::Fragment))
    {
        std::cout << "Error Ocurred" << std::endl;
    }

    sf::Shader shader2;
    shader2.loadFromFile("shader.glsl", sf::Shader::Fragment);

    shader2.setUniform("resolution", sf::Vector2f(800, 800));

    sf::Clock clk;
    clk.restart(); // start the timer

    float circlesize = 50.0f;

    while (window.isOpen())
            {
                sf::Event event;
                while (window.pollEvent(event))
                {
                    if (event.type == sf::Event::Closed)
                        window.close();
                    if (event.type == sf::Event::KeyPressed)
                    {
                        if (event.key.code == sf::Keyboard::Up)
                            circlesize *= 1.5f;
                        if (event.key.code == sf::Keyboard::Down)
                            circlesize *= 2.0f/3.0f;
                    }
                }
        
                shader2.setUniform("time", clk.getElapsedTime().asSeconds()*5);

                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                shader2.setUniform("mouse", sf::Vector2f(mousePos.x, mousePos.y));
                shader2.setUniform("size", circlesize);

                window.clear(sf::Color::White);
                
                window.draw(quad, &shader2);

                window.display();
                //Sleep(1000);
            }

}


//#include <iostream>
//#include "GameOfLife.hpp"
//#include "SFML/Graphics.hpp"
//#include <windows.h>

//int main()
//{
//    
//
//
//	GOL game(200, 200, 800, 800);
//	game.Init();
//
//    const char* map =   "00000000000000000000000000000000000000"
//                        "00000000000000000000000001000000000000"
//                        "00000000000000000000000101000000000000"
//                        "00000000000001100000011000000000000110"
//                        "00000000000010001000011000000000000110"
//                        "01100000000100000100011000000000000000"
//                        "01100000000100010110000101000000000000"
//                        "00000000000100000100000001000000000000"
//                        "00000000000010001000000000000000000000"
//                        "00000000000001100000000000000000000000"
//                        "00000000000000000000000000000000000000";
//
//    //const char* map = "1111111111";
//    game.ImportStringMap(map, 38, 11);
//    //game.InitRandom();
//
//    sf::RenderWindow window(sf::VideoMode(800, 800 ), "Conway's Game of Life: IMPROVED");
//
//    float fps;
//    sf::Clock clock = sf::Clock::Clock();
//    sf::Time previousTime = clock.getElapsedTime();
//    sf::Time currentTime;
//
//
//    while (window.isOpen())
//    {
//        sf::Event event;
//        while (window.pollEvent(event))
//        {
//            if (event.type == sf::Event::Closed)
//                window.close();
//
//            if (event.type == sf::Event::KeyPressed)
//                if (event.key.code == sf::Keyboard::A)
//                {
//                    std::cout << "FPS: " << fps << std::endl;
//                }
//        }
//
//        window.clear(sf::Color::White);
//
//        currentTime = clock.getElapsedTime();
//        fps = 1.0f / (currentTime.asSeconds() - previousTime.asSeconds()); // the asSeconds returns a float
//        previousTime = currentTime;
//
//        //game.PrintGrid();
//
//        game.ClearVertices();
//        game.NextGen();
//        window.draw(game.m_Vertices);
//
//
//        window.display();
//        //Sleep(1000);
//    }
//}
#endif
