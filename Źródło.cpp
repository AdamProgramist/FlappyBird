#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "FlappyBird.h"

int ilosc_rur = 10;

int main() {
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "FlappyBird");

	Ptak ptak({ 400.0f, 500.0f });
	ptak.load_texture("duch.png");
	

	

	Rura rura({ 450.0f, 600.0f }, NORMAL);
	rura.load_texture("rura.png");
	std::vector<std::pair<Rura, Rura>> Para_rur(ilosc_rur);
	for (int i = 0; i < ilosc_rur ; i++)
	{
	Wygeneruj_rury(Para_rur[i], {1000.f+i*500.f, 500.f}, "rura.png");
	}
	
	Tlo tlo({ 500.0f, 500.0f });
	tlo.load_texture("tlo.png");
	

	sf::Clock clock;
	sf::Time elapsedTime;

	
	

	clock.restart();
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
		/* funkcja na kolizje obiektu ptak z obiektem rura
		bool collision(const ptak, const Rura); {
			return ptak.getBounds().intersects(Rura.getBounds());
		}
		*/
		/* lub tak mozna napisac:
		bool collision(sf::Sprite sprite1, sf::Sprite sprite2){
			sf::Rect<float> rect1 = sprite1.getGlobalBounds();
			sf::Rect<float> rect2 = sprite2.getGlobalBounds();
			return rect1.intersects(rect2);
		}*/
		//napisac if kolizji uzywajac getGlobalBounds().instects(obiekt.getGlobalBounds());
		
		window.clear(sf::Color::Blue);

		elapsedTime = clock.restart();
		ptak.oblicz_pozycje(elapsedTime.asSeconds());

		window.draw(tlo);


		//rura.oblicz_przesuniecie(elapsedTime.asSeconds());
		for (int i = 0; i < ilosc_rur; i++)
		{
		Para_rur[i].first.oblicz_przesuniecie(elapsedTime.asSeconds());
		Para_rur[i].second.oblicz_przesuniecie(elapsedTime.asSeconds());

		window.draw(Para_rur[i].first);
		window.draw(Para_rur[i].second);

		if (ptak.getGlobalBounds().intersects(Para_rur[i].getGlobalBounds())) {
			std::cout << "Dotknalem" << std::endl;
		}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			ptak.skok();
		}
		

		
		window.draw(ptak);

		

		window.draw(rura);

		
		

		window.display();
	}

	return 0;
}