#include "Juego.h"

void main() {
	Console::SetWindowSize(anchoConsola, altoConsola);
	Console::CursorVisible = false;
	srand(time(0));
	Juego* juego = new Juego();
	juego->jugar();
}
#pragma once
#include "Terrestre.h"
class Bicicleta : public Terrestre
{
public:
	Bicicleta();
	~Bicicleta();
	void dibujar();
	void mover();
private:

};

Bicicleta::Bicicleta() : Terrestre(6, 2)
{
	Random r;
	tipo = 'I';
	x = r.Next(anchoConsola - 6);
	dx = r.Next(2) * 2 - 1;
}

void Bicicleta::dibujar() {
	if (dx < 0) {
		Console::SetCursorPosition(x, y);
		cout << "/_..";
		Console::SetCursorPosition(x, y + 1);
		cout << "(o)(o)";
	}
	else {
		Console::SetCursorPosition(x, y);
		cout << ".._\\";
		Console::SetCursorPosition(x, y + 1);
		cout << "(o)(o)";
	}
}

void Bicicleta::mover() {
	validarMovimiento(-dx, 0);
}

#pragma once
#include "Terrestre.h"
class Bus : public Terrestre
{
public:
	Bus();
	~Bus();
	void dibujar();
	void mover();
private:

};

Bus::Bus() : Terrestre(13, 3)
{
	tipo = 'B';
	x = anchoConsola - 13;
	dx = -2;
}

Bus::~Bus()
{
}

void Bus::dibujar() {
	Console::SetCursorPosition(x, y);
	cout << "_/_|[][][][]|";
	Console::SetCursorPosition(x, y + 1);
	cout << "[           |";
	Console::SetCursorPosition(x, y + 2);
	cout << "=--OO---OO-=";
}

void Bus::mover() {
	Random r;
	dx = -r.Next(3, 6);
	validarMovimiento(0, 0);
}

#pragma once
#include "Terrestre.h"
class Carro : public Terrestre
{
public:
	Carro();
	~Carro();
	void dibujar();
	void mover();
private:

};

Carro::Carro() : Terrestre(8, 3)
{
	tipo = 'C';
	x = 0;
	dx = 2;
}

Carro::~Carro()
{
}

void Carro::dibujar() {
	Console::SetCursorPosition(x, y);
	cout << "   | ~\_";
	Console::SetCursorPosition(x, y + 1);
	cout << "[| _ | -";
	Console::SetCursorPosition(x, y + 2);
	cout << " (_)(_) ";
}

void Carro::mover() {
	Random r;
	dx = r.Next(1, 4);
	validarMovimiento(0, 0);
}
#pragma once
#include "Bicicleta.h"
#include "Carro.h"
#include "Bus.h"
#include "Persona.h"
#include <vector>
#include <conio.h>
class Juego
{
public:
	Juego();
	~Juego();
	void gestionarTodo();
	bool colision();
	void jugar();
	void agregarTransporte();
	void borrar();
private:
	vector <Terrestre*> transportes;
	Persona* persona;
};

Juego::Juego()
{
	transportes.push_back(new Bicicleta());
	persona = new Persona();
}

Juego::~Juego()
{
}

void Juego::gestionarTodo() {
	for (int i = 0; i < transportes.size(); i++) {
		transportes[i]->borrar();
		transportes[i]->mover();
		transportes[i]->dibujar();
	}
	persona->borrar();
	persona->mover();
	persona->dibujar();
}
bool Juego::colision() {
	for (int i = 0; i < transportes.size(); i++)
		if (transportes[i]->getTipo() != 'I')
			if (transportes[i]->colision(persona->getX(), persona->getY(), persona->getAncho(), persona->getAlto()))
				return true;
	return false;
}

void  Juego::borrar() {
	for (int i = 0; i < transportes.size(); i++)
		if (transportes[i]->getDX() == 0) {
			transportes[i]->borrar();
			transportes.erase(transportes.begin() + i);
			i--;
		}
}
void Juego::jugar() {
	do
	{
		if (kbhit())
			persona->direccion(getch());
		gestionarTodo();
		agregarTransporte();
		borrar();
		//persona->direccion(NULL);
		_sleep(50);
	} while (!colision());
}
void Juego::agregarTransporte() {
	Random r;
	switch (r.Next(50))
	{
	case 1: case 2: transportes.push_back(new Carro()); break;
	case 3: case 4: transportes.push_back(new Bus()); break;
	case 5:
		if (r.Next(10) == 1)
			transportes.push_back(new Bicicleta());
		break;
	}
}

#pragma once
#include "Terrestre.h"
#define izquierda 75
#define derecha 77
#define arriba 72
#define abajo 80
class Persona : public Terrestre
{
public:
	Persona();
	~Persona();
	void dibujar();
	void mover();
	void direccion(char tecla);
private:
};

Persona::Persona() : Terrestre(1, 2)
{
	y = altoConsola - 2;
	x = anchoConsola / 2;
	dx = 0;
	dy = 0;
}

Persona::~Persona()
{
}

void Persona::dibujar() {
	Console::SetCursorPosition(x, y);
	cout << "0";
	Console::SetCursorPosition(x, y + 1);
	cout << "+";
}

void Persona::mover() {
	validarMovimiento(0, 0);
}

void Persona::direccion(char tecla) {
	dx = dy = 0;
	switch (tecla)
	{
	case arriba: dy = -1; break;
	case abajo: dy = 1; break;
	case izquierda: dx = -2; break;
	case derecha: dx = 2; break;
	}
}
#pragma once
#include <iostream>
#define anchoConsola 120
#define altoConsola 30

using namespace std;
using namespace System;

class Terrestre
{
public:
	Terrestre(int pancho, int palto);
	~Terrestre();
	void validarMovimiento(int pdx, int pdy);
	void borrar();
	virtual void mover() = 0;
	virtual void dibujar();
	bool colision(int ex, int ey, int eancho, int ealto);
	int getX();
	int getY();
	int getDX();
	int getDY();
	int getAncho();
	int getAlto();
	int getTipo();
protected:
	int x, y, dx, dy, ancho, alto;
	char tipo;
};

Terrestre::Terrestre(int pancho, int palto)
{
	ancho = pancho;
	alto = palto;
	y = rand() % (altoConsola - alto - 3);
	dy = 0;
}

Terrestre::~Terrestre()
{
}

void Terrestre::validarMovimiento(int pdx, int pdy) {
	if (x + dx < 0 || x + dx + ancho > anchoConsola)
		dx = pdx;
	if (y + dy < 0 || y + dy + alto> altoConsola)
		dy = pdy;
	x += dx;
	y += dy;
}
void Terrestre::borrar() {
	for (int i = 0; i < alto; i++)
		for (int j = 0; j < ancho; j++) {
			Console::SetCursorPosition(x + j, y + i);
			cout << " ";
		}
}
void Terrestre::dibujar() {}

bool Terrestre::colision(int ex, int ey, int eancho, int ealto) {
	return x < ex + eancho && ex < x + ancho
		&& y < ey + ealto && ey < y + alto;
}

int Terrestre::getX() { return x; }
int Terrestre::getY() { return y; }
int Terrestre::getDX() { return dx; }
int Terrestre::getDY() { return dy; }
int Terrestre::getAncho() { return ancho; }
int Terrestre::getAlto() { return alto; }
int Terrestre::getTipo() { return tipo; }