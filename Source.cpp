#include "Controlador.h"
void main() {
	Console::SetWindowSize(anchoConsola + 20, altoConsola);
	Console::CursorVisible = false;
	Controlador* controlador = new Controlador();
	controlador->jugar();
}
#pragma once
#include "Figura.h"
class Casa : public Figura
{
public:
	Casa();
	~Casa();
	void dibujar();
private:

};
Casa::Casa()
{
	alto = 5;
	ancho = 7;
	x = anchoConsola - ancho;
	y = altoConsola - alto;
	dx = dy = 0;
}
Casa::~Casa()
{
}
void Casa::dibujar() {
	Console::SetCursorPosition(x, y);
	cout << "  +";
	Console::SetCursorPosition(x, y + 1);
	cout << "  A_";
	Console::SetCursorPosition(x, y + 2);
	cout << " /\\-\\";
	Console::SetCursorPosition(x, y + 3);
	cout << "_||\"|_";
	Console::SetCursorPosition(x, y + 4);
	cout << "^~^~^~^";
}
#pragma once
#include "Virus.h"
#include "Vacuna.h"
#include "Monigote.h"
#include "Casa.h"
#include <conio.h>
#include <vector>
class Controlador
{
public:
	Controlador();
	~Controlador();
	void gestionarTodo();
	bool colision();
	void jugar();
private:
	vector<Figura*> figuras;
	Casa* casa;
	Monigote* monigote;
	int n;
	int cantidadVacunas;
	time_t tiempoInicial;
};

Controlador::Controlador()
{
	cantidadVacunas = 1;
	tiempoInicial = time(0);
	Random r;
	n = r.Next(3, 7);
	for (int i = 0; i < n; i++)
		figuras.push_back(new Virus());
	figuras.push_back(new Vacuna());

	casa = new Casa();
	monigote = new Monigote();
}

Controlador::~Controlador()
{
}

void Controlador::gestionarTodo() {
	for (int i = 0; i < figuras.size(); i++) {
		figuras[i]->borrar();
		figuras[i]->mover();
		figuras[i]->dibujar();
	}
	casa->dibujar();
	monigote->borrar();
	monigote->mover();
	monigote->dibujar();
}
bool Controlador::colision() {
	for (int i = 0; i < figuras.size(); i++) {
		if (figuras[i]->colision(monigote->getX(), monigote->getY(), monigote->getAncho(), monigote->getAlto())) {
			if (monigote->getInmune() == false) {
				if (figuras[i]->getTipo() == "virus") {
					monigote->borrar();
					monigote->reposicionar();
					monigote->quitarVidas();
				}
				else
					monigote->hacerInmune();
				figuras[i]->borrar();
				figuras.erase(figuras.begin() + i);
				i--;
			}
		}
	}
	if (casa->colision(monigote->getX(), monigote->getY(), monigote->getAncho(), monigote->getAlto()))
		return true;
	if (monigote->getVidas() == 0)
		return true;
	return false;
}
void Controlador::jugar() {
	char tecla;
	do
	{
		tecla = NULL;
		Console::SetCursorPosition(anchoConsola, 1);
		cout << "Vidas: " << monigote->getVidas();
		Console::SetCursorPosition(anchoConsola, 2);
		cout << "Inmunidad: " << monigote->getInmune();
		Console::SetCursorPosition(anchoConsola, 3);
		cout << "Pasos: " << monigote->getPasos();
		if (kbhit())
			tecla = getch();
		monigote->desplazamiento(tecla);
		gestionarTodo();
		if (difftime(time(0), tiempoInicial) > 10 && cantidadVacunas < 5) {
			figuras.push_back(new Vacuna());
			tiempoInicial = time(0);
			cantidadVacunas++;
		}
		_sleep(50);
	} while (!colision());
	cout << "Cantidad de pasos del monigote: " << monigote->getPasos() << endl;
	getch();
	getch();
	getch();
}

#pragma once
#include <iostream>
#define anchoConsola 100
#define altoConsola 30

using namespace std;
using namespace System;

class Figura
{
public:
	Figura();
	~Figura();
	void movimiento(int pdx, int pdy);
	void borrar();
	virtual void dibujar();
	virtual void mover();
	bool colision(int ex, int ey, int eancho, int ealto);
	int getX();
	int getY();
	int getAncho();
	int getAlto();
	string getTipo();
protected:
	int x, y, dx, dy, ancho, alto;
	string tipo;
};

Figura::Figura()
{

}

Figura::~Figura()
{
}

void Figura::movimiento(int pdx, int pdy) {
	if (x + dx < 0 || x + dx + ancho > anchoConsola)
		dx = pdx;
	if (y + dy < 0 || y + dy + alto > altoConsola)
		dy = pdy;
	x += dx;
	y += dy;
}
void Figura::borrar() {
	for (int i = 0; i < alto; i++)
		for (int j = 0; j < ancho; j++) {
			Console::SetCursorPosition(x + j, y + i);
			cout << " ";
		}
}
void Figura::dibujar() {}
void Figura::mover() {}
bool Figura::colision(int ex, int ey, int eancho, int ealto) {
	return x <= ex + eancho && ex <= x + ancho && y <= ey + ealto && ey <= y + alto;
}

int Figura::getX() { return x; }
int Figura::getY() { return y; }
int Figura::getAncho() { return ancho; }
int Figura::getAlto() { return alto; }
string  Figura::getTipo() { return tipo; }

#pragma once
#include "Figura.h"
class Vacuna : public Figura
{
public:
	Vacuna();
	~Vacuna();
	void dibujar();
	void mover();
private:

};

Vacuna::Vacuna()
{
	tipo = "vacuna";
	Random r;
	alto = 1;
	ancho = 4;
	x = r.Next(0, anchoConsola - ancho);
	y = r.Next(0, altoConsola - alto);
	do
	{
		dx = r.Next(-1, 2); //-1 0 1
		dy = r.Next(-1, 2); //-1 0 1
	} while (abs(dx) == abs(dy));
	dx *= 2;
}

Vacuna::~Vacuna()
{
}

void Vacuna::dibujar() {
	Console::SetCursorPosition(x, y);
	cout << "|->|";
}

void Vacuna::mover() {
	movimiento(-dx, -dy);
}
#pragma once
#define izquierda 75
#define derecha 77
#define arriba 72
#define abajo 80
#include "Figura.h"
class Monigote : public Figura
{
public:
	Monigote();
	~Monigote();
	void dibujar();
	void mover();
	void desplazamiento(char tecla);
	void reposicionar();
	bool getInmune();
	void hacerInmune();
	void quitarVidas();
	int getVidas();
	int getPasos();
private:
	int cantMovimientos;
	int cantPasos;
	bool inmune;
	int vidas;
};

Monigote::Monigote()
{
	cantPasos = 0;
	vidas = 3;
	alto = 2;
	ancho = 1;
	x = 0;
	y = 0;
	dx = dy = 0;
	inmune = false;
	cantMovimientos = 0;
}

Monigote::~Monigote()
{
}

void Monigote::dibujar() {
	Console::SetCursorPosition(x, y);
	cout << "O";
	Console::SetCursorPosition(x, y + 1);
	cout << "+";
}

void Monigote::mover() {
	movimiento(0, 0);
}

void Monigote::desplazamiento(char tecla) {
	dx = dy = 0;
	if (izquierda == tecla || tecla == derecha || tecla == abajo || tecla == arriba) {
		if (inmune) cantMovimientos++;
		if (cantMovimientos > 10) {
			inmune = false;
			cantMovimientos = 0;
		}
		cantPasos++;
		switch (tecla)
		{
		case izquierda: dx = -2; break;
		case derecha: dx = 2; break;
		case arriba: dy = -1; break;
		case abajo: dy = 1; break;
		}
	}
}

void Monigote::reposicionar() {
	x = y = 0;
}

bool Monigote::getInmune() { return inmune; }
void Monigote::hacerInmune() { inmune = true; }
void Monigote::quitarVidas() { vidas--; }
int Monigote::getVidas() { return vidas; }
int Monigote::getPasos() { return cantPasos; }

#pragma once
#include "Figura.h"
class Virus : public Figura
{
public:
	Virus();
	~Virus();
	void dibujar();
	void mover();
private:

};

Virus::Virus()
{
	tipo = "virus";
	Random r;
	alto = 5;
	ancho = 9;
	x = r.Next(0, anchoConsola - ancho);
	y = r.Next(0, altoConsola - alto);
	do
	{
		dx = r.Next(-4, 5); //[-5 -4 -3- 2 -1 0 1 2 3 4 5]
		dy = r.Next(-2, 3); //-3 0 3
		_sleep(10);
	} while (dx == 0 || dy == 0);
}

Virus::~Virus()
{
}

void Virus::dibujar() {
	Console::SetCursorPosition(x, y);
	cout << "    q";
	Console::SetCursorPosition(x, y + 1);
	cout << "  0-0-0";
	Console::SetCursorPosition(x, y + 2);
	cout << "0-0 0 0-0";
	Console::SetCursorPosition(x, y + 3);
	cout << "  0-0-0";
	Console::SetCursorPosition(x, y + 4);
	cout << "    b";
}

void Virus::mover() {
	movimiento(-dx, -dy);
}
