#include "Controlador.h"

void main() {

	Console::SetWindowSize(anchoConsola, altoConsola);
	Console::CursorVisible = false;
	srand(time(0));
	Juego* juego = new Juego();
	juego->jugar();
}
//contro.h
#pragma once
#include "Murcielago.h"
#include "Monigote.h"
#include <vector>
#include <conio.h>
class Juego
{
public:
	Juego();
	~Juego();
	void jugar();
	void validarMovimiento(int pdx, int pdy);
	void gestionarTodo();
	void borrar();
private:
	Monigote* monigote;
	Murcielago* murcielago;
};
Juego::Juego()
{
	monigote = new Monigote();
	murcielago = new Murcielago(40, 21);
}
Juego::~Juego()
{
}
void Juego::gestionarTodo() {
	monigote->borrar();
	monigote->mover();
	monigote->dibujar();
	//bala
	murcielago->InsertaBala();
	murcielago->Dibujar();
	murcielago->DibujarBalas();
	_sleep(15);
	murcielago->Borrar();
	murcielago->BorrarBalas();
	murcielago->Mover();
	murcielago->MoverBalas();
	murcielago->AnalizayEliminarBala();
}
void Juego::borrar()
{
}
void Juego::jugar() {
	do
	{
		if (kbhit())
			monigote->direccion(getch());
		gestionarTodo();
		borrar();
		monigote->direccion(NULL);
		_sleep(50);
	} while (true);
}
//obj.h
#pragma once
#include <iostream>
#define anchoConsola 120
#define altoConsola 30

using namespace std;
using namespace System;

class Objeto
{
public:
	Objeto(int pancho, int palto);
	~Objeto();
	void borrar();
	virtual void mover() = 0;
	virtual void dibujar();
	void validarMovimiento(int pdx, int pdy);
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

Objeto::Objeto(int pancho, int palto)
{
	ancho = pancho;
	alto = palto;
	y = rand() % (altoConsola - alto - 3);
	dy = 0;
}
Objeto::~Objeto()
{
}
void Objeto::borrar() {
	for (int i = 0; i < alto; i++)
		for (int j = 0; j < ancho; j++) {
			Console::SetCursorPosition(x + j, y + i);
			cout << " ";
		}
}
void Objeto::dibujar() {}
void Objeto::validarMovimiento(int pdx, int pdy) {
	if (x + dx < 0 || x + dx + ancho > anchoConsola)
		dx = pdx;
	if (y + dy < 0 || y + dy + alto > altoConsola)
		dy = pdy;
	x += dx;
	y += dy;
}
int Objeto::getX() { return x; }
int Objeto::getY() { return y; }
int Objeto::getDX() { return dx; }
int Objeto::getDY() { return dy; }
int Objeto::getAncho() { return ancho; }
int Objeto::getAlto() { return alto; }
int Objeto::getTipo() { return tipo; }
//monigo.h
#pragma once
#include "Objeto.h"
#define arriba 72
#define abajo 80
#define izquierda 75
#define derecha 77
class Monigote : public Objeto
{
public:
	Monigote();
	~Monigote();
	void dibujar();
	void mover();
	void direccion(char tecla);
private:
};
Monigote::Monigote() : Objeto(1, 2)
{
	y = 1;
	x = 2;
	dx = 0;
	dy = 0;
}
Monigote::~Monigote()
{
}
void Monigote::dibujar() {
	Console::SetCursorPosition(x, y);
	cout << "0";
	Console::SetCursorPosition(x, y + 1);
	cout << "+";
}
void Monigote::mover() {
	validarMovimiento(0, 0);
}
void Monigote::direccion(char tecla) {
	dx = dy = 0;
	switch (tecla)
	{
	case arriba: dy = -1; break;
	case abajo: dy = 1; break;
	case izquierda: dx = -2; break;
	case derecha: dx = 2; break;
	}
}
//murcie.h
#pragma once
#include"bala.h"
#include <vector>

class Murcielago {
private:
	//atributo del barco
	int x, y, dy, alto, ancho, contador_eliminados;
	//atributo relacionado a las balas
	vector<CBala> vecbalas;
public:
	Murcielago(int px, int py);
	~Murcielago();
	//propios movimientos del barco
	void Dibujar();
	void Mover();
	void Borrar();
	//propio de las balas
	void DibujarBalas();
	void MoverBalas();
	void BorrarBalas();
	void InsertaBala();
	void AnalizayEliminarBala();
	int GetContador_Eliminados();
};
Murcielago::Murcielago(int px, int py) {
	x = 75; y = 14; dy = 1; alto = 3; ancho = 4; contador_eliminados = 0;
}
Murcielago::~Murcielago() {};
void Murcielago::Dibujar() {
	Console::SetCursorPosition(x, y);     cout << " ###";
	Console::SetCursorPosition(x, y + 1); cout << "--* ";
	Console::SetCursorPosition(x, y + 2); cout << " ###";
}
void Murcielago::Borrar() {
	Console::SetCursorPosition(x, y);     cout << "     ";
	Console::SetCursorPosition(x, y + 1); cout << "     ";
	Console::SetCursorPosition(x, y + 2); cout << "     ";
}
void Murcielago::Mover() {
	if (y + dy < 0 || y + dy + alto>24)dy *= -1;
	y += dy;
}
//impl balas
void Murcielago::DibujarBalas() {
	for each (CBala objeto in vecbalas) {
		objeto.Dibujar();
	}
}
void Murcielago::BorrarBalas() {
	for each (CBala objeto in vecbalas) {
		objeto.Borrar();
	}
}
void Murcielago::MoverBalas() {
	for (int i = 0; i < vecbalas.size(); i++) {
		vecbalas[i].Mover();
	}
}
void Murcielago::InsertaBala() {
	vecbalas.push_back(CBala(x + 1, y - 1));
}
void Murcielago::AnalizayEliminarBala() {
	if (vecbalas.size() > 0) {
		if (vecbalas[0].GetX() == 1) {
			vecbalas.erase(vecbalas.begin());
			contador_eliminados++;
		}
	}
}
int Murcielago::GetContador_Eliminados() {
	return contador_eliminados;
}
//bala.h
#pragma once
#include <iostream>
using namespace std;
using namespace System;
class CBala {
private:
	int x, y, dx;
public:
	CBala(int px, int py);
	~CBala();
	int GetX();
	void Dibujar();
	void Mover();
	void Borrar();
};
CBala::CBala(int px, int py) {
	x = px; y = py; dx = -1;
}
CBala::~CBala() {}
int CBala::GetX() {
	return x;
}
void CBala::Dibujar() {
	Console::SetCursorPosition(x - 2, y + 2);
	cout << "*";
}
void CBala::Mover() {
	x = x + dx;
}
void CBala::Borrar() {
	Console::SetCursorPosition(x - 2, y + 2);
	cout << " ";
}