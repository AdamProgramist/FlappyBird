#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>


const float gravity = 15.0f;
const float pipe_velocity = -100.f;
const float odleglosc_miedzy_rurami = 1000.f;

class Asset {
public:


};

class Ptak {
public:

	Ptak(sf::Vector2f p);

	void load_texture(std::string t);
	void oblicz_pozycje(float dt);
	void skok();


	operator sf::Sprite& () { update(); return sprite_ptak; };

	sf::FloatRect getGlobalBounds();

private:
	float masa = 10.0f;
	float kat_pochylenia; //obracanie duszka
	sf::Vector2f pozycja;
	sf::Vector2f predkosc;
	sf::Vector2f przyspieszenie;
	sf::Texture texture_ptak;
	sf::Sprite sprite_ptak;
	sf::FloatRect bounds_ptak;


	void update();
};

sf::FloatRect Ptak::getGlobalBounds() {
	return sprite_ptak.getGlobalBounds();
}


Ptak::Ptak(sf::Vector2f p) {
	pozycja = p;
	predkosc = { 0,0 };
	przyspieszenie = { 0,0 };
}


void Ptak::load_texture(std::string t) {
	texture_ptak.loadFromFile(t);
	sprite_ptak.setTexture(texture_ptak);

	sf::Vector2f rozmiar_sprite;
	rozmiar_sprite.x = texture_ptak.getSize().x;
	rozmiar_sprite.y = texture_ptak.getSize().y;

	sprite_ptak.setOrigin(rozmiar_sprite.x * 0.5f, rozmiar_sprite.y * 0.5f);
}


void Ptak::oblicz_pozycje(float dt) {
	sf::Vector2f przesuniecie = { 0, 0 };

	przyspieszenie.y = gravity * masa;
	predkosc.x += przyspieszenie.x * dt;
	predkosc.y += przyspieszenie.y * dt;

	przesuniecie.x += predkosc.x * dt;
	przesuniecie.y += predkosc.y * dt;

	pozycja.x += przesuniecie.x;
	pozycja.y += przesuniecie.y;


	kat_pochylenia = atan2(predkosc.y, -pipe_velocity) * 180.f / M_PI;
}


void Ptak::skok() {
	predkosc.y = -200.f;
}


void Ptak::update() {
	sprite_ptak.setPosition(pozycja);
	sprite_ptak.setRotation(kat_pochylenia);
}

//////////////////////////////////////////////////////////////////////
enum pipe_type {
	NORMAL = 0, ROTATED = 1
};

class Rura {
public:
	~Rura();

	Rura();
	Rura(sf::Vector2f p, pipe_type r);

	void load_texture(std::string t);
	void load_texture(sf::Texture *t);
	void oblicz_przesuniecie(float dt);

	static unsigned int getIlosc_rur() { return ilosc_rur; };

	operator sf::Sprite& () { update(); return sprite_rura; };
	operator bool() { return czy_wyszla; };

	sf::FloatRect getGlobalBounds();


protected:
	static unsigned int last_id;
	static unsigned int ilosc_rur;

private:
	pipe_type rodzaj;
	unsigned int id;
	sf::Texture texture_rura;
	sf::Vector2f pozycja;
	sf::Sprite sprite_rura;
	sf::FloatRect bounds_rura;

	bool czy_wyszla = 0;

	void update();

};

unsigned int Rura::last_id = 0;
unsigned int Rura::ilosc_rur = 0;

sf::FloatRect Rura::getGlobalBounds() {
	return sprite_rura.getGlobalBounds();
}

Rura::~Rura() {
	ilosc_rur--;
}

Rura::Rura() {
	pozycja = { 2000.f, 1000.f };

	id = last_id++;

	ilosc_rur++;
}

Rura::Rura(sf::Vector2f p, pipe_type r) {
	pozycja = p;
	rodzaj = r;




	id = last_id++;

	ilosc_rur++;
}

void Rura::load_texture(std::string t) {
	texture_rura.loadFromFile(t);
	sprite_rura.setTexture(texture_rura);
	sprite_rura.setScale({ 0.5f, 1.f });

	sf::Vector2f rozmiar_sprite;
	rozmiar_sprite.x = texture_rura.getSize().x;
	rozmiar_sprite.y = texture_rura.getSize().y;

	sprite_rura.setOrigin(rozmiar_sprite.x * 0.5f, rozmiar_sprite.y * 0.5f);

	if (rodzaj == ROTATED) {
		sprite_rura.setRotation(180.f);
		pozycja.y -= texture_rura.getSize().y * 0.5f;
	}
	else if (rodzaj == NORMAL)
		pozycja.y += texture_rura.getSize().y * 0.5f;

}

void Rura::load_texture(sf::Texture *t) {
	sprite_rura.setTexture(*t);
	sprite_rura.setScale({ 0.5f, 1.f });

	sf::Vector2f rozmiar_sprite;
	rozmiar_sprite.x = texture_rura.getSize().x;
	rozmiar_sprite.y = texture_rura.getSize().y;

	std::cout << "x: " << rozmiar_sprite.x << "y: " << rozmiar_sprite.y << std::endl;

	sprite_rura.setOrigin(rozmiar_sprite.x * 0.5f, rozmiar_sprite.y * 0.5f);

	if (rodzaj == ROTATED) {
		sprite_rura.setRotation(180.f);
		pozycja.y -= texture_rura.getSize().y * 0.5f;
	}
	else if (rodzaj == NORMAL)
		pozycja.y += texture_rura.getSize().y * 0.5f;

}

void Rura::oblicz_przesuniecie(float dt) {
	pozycja.x += pipe_velocity * dt;

	if (pozycja.x <= -200.f)
		czy_wyszla = 1;
}

void Rura::update() {
	sprite_rura.setPosition(pozycja);
}

std::pair<Rura, Rura> Wygeneruj_rury(sf::Vector2f pozycja, float rozmiar_szpary = 300.f) {

	std::pair<Rura, Rura> Para_rur;
	Para_rur.first = Rura{ {pozycja.x, pozycja.y + rozmiar_szpary * 0.5f }, NORMAL };
	Para_rur.second = Rura{ {pozycja.x, pozycja.y - rozmiar_szpary * 0.5f }, ROTATED };

	return Para_rur;
}
////////////////////////////////////////////////////////////////////////////////////
class Tlo {
public:
	Tlo(sf::Vector2f p);

	void load_texture(std::string t);


	operator sf::Sprite& () { update(); return sprite_tlo; };

private:
	sf::Vector2f pozycja;
	sf::Texture texture_tlo;
	sf::Sprite sprite_tlo;

	void update();
};

Tlo::Tlo(sf::Vector2f p) {
	pozycja = p;
}

void Tlo::load_texture(std::string t) {
	texture_tlo.loadFromFile(t);
	sprite_tlo.setTexture(texture_tlo);

	sf::Vector2f rozmiar_sprite;
	rozmiar_sprite = { 1000.f, 1000.f };
	rozmiar_sprite.x = texture_tlo.getSize().x;
	rozmiar_sprite.y = texture_tlo.getSize().y;


	sprite_tlo.setOrigin(rozmiar_sprite.x * 0.5f, rozmiar_sprite.y * 0.5f);
}

void Tlo::update() {
	sprite_tlo.setPosition(pozycja);
}

/////////////////////////////////////////////
