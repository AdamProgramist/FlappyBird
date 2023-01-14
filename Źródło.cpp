#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>
#include <math.h>
#include <random>
#include <deque>
#include <windows.h>
#include "FlappyBird.h"


int ilosc_rur = 10;

int main() {
	
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "FlappyBird");
	system("dir");
	Ptak ptak({ 400.0f, 500.0f });
	ptak.load_texture("duch.png");

	sf::Texture texture_rura;
	texture_rura.loadFromFile("rura.png");

	std::random_device rd;
	std::mt19937 gen(rd());

	float max_rozmiar_szpary = 500.f;
	float min_rozmiar_szpary = 300.f;

	float max_y = 850.f;
	float min_y = 150.f;
	
	std::uniform_real_distribution <float> dis(min_rozmiar_szpary, max_rozmiar_szpary);
	std::uniform_real_distribution <float> dis1(min_y, max_y);
	//Rura rura({ 450.0f, 600.0f }, NORMAL);
	//rura.load_texture("rura.png");
	std::deque<std::pair<Rura, Rura>> Para_rur(ilosc_rur);
	for (int i = 0; i < ilosc_rur; i++)
	{
		Para_rur[i] = Wygeneruj_rury({ 1000.f + i * odleglosc_miedzy_rurami, dis1(gen) }, dis(gen));
		Para_rur[i].first.load_texture("rura.png");
		Para_rur[i].second.load_texture("rura.png");
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
		

		window.clear(sf::Color::Blue);

		elapsedTime = clock.restart();
		ptak.oblicz_pozycje(elapsedTime.asSeconds());

		window.draw(tlo);

		if (Para_rur.front().first) {
			Para_rur.pop_front();
			Para_rur.push_back(Wygeneruj_rury({ 1000.f + Rura::getIlosc_rur() * odleglosc_miedzy_rurami, {dis1(gen)} }, dis(gen)));
			Para_rur.back().first.load_texture("rura.png");
			Para_rur.back().second.load_texture("rura.png");
		}

		//rura.oblicz_przesuniecie(elapsedTime.asSeconds());
		for (int i = 0; i < ilosc_rur; i++)
		{
			Para_rur[i].first.oblicz_przesuniecie(elapsedTime.asSeconds());
			Para_rur[i].second.oblicz_przesuniecie(elapsedTime.asSeconds());

			window.draw(Para_rur[i].first);
			window.draw(Para_rur[i].second);

			if (ptak.getGlobalBounds().intersects(Para_rur[i].first.getGlobalBounds())) {
				std::cout << "Dotknalem dol" << std::endl;
			}
			else if (ptak.getGlobalBounds().intersects(Para_rur[i].second.getGlobalBounds())) {
				std::cout << "Dotknalem gore" << std::endl;
			}

		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			ptak.skok();
		}



		window.draw(ptak);



		//window.draw(rura);




		window.display();
	}

	return 0;
}