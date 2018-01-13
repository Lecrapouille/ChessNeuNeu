#include "Board.hpp"
#include "Stockfish.hpp"

int main()
{
  sf::RenderWindow window(sf::VideoMode(504, 504), "NeuNeuChess");
  States states;
  Board board(states, window);
  Stockfish IA(states);

  while (window.isOpen())
    {
      sf::Event event;

      board.mousePosition(sf::Mouse::getPosition(window));
      while (window.pollEvent(event))
        {
          switch (event.type)
            {
            case sf::Event::Closed:
              window.close();
              break;

            case sf::Event::MouseButtonPressed:
              board.takeFigure();
              break;

            case sf::Event::MouseButtonReleased:
              board.releaseFigure();
              break;

            case sf::Event::KeyPressed:
              if (event.key.code == sf::Keyboard::Space)
                {
                  auto movement = IA.nextMove();
                  board.move(movement);
                }
              else if (event.key.code == sf::Keyboard::BackSpace)
                {
                  board.moveBack();
                }
              break;

            default:
              break;
            }
        }
      window.clear();
      board.draw();
    }

  return 0;
}
