//covid-19-
#include "Controlador.h"
void main() {
	Console::SetWindowSize(anchoConsola + 20, altoConsola);
	Console::CursorVisible = false;
	Controlador* controlador = new Controlador();
	controlador->jugar();}

#pragma once
#include "Figura.h"
class Casa : public Figura
{public:
	Casa();
	~Casa();
	void dibujar();
private:};
Casa::Casa()
{alto = 5;
	ancho = 7;
	x = anchoConsola - ancho;
	y = altoConsola - alto;
	dx = dy = 0;}
Casa::~Casa(){}
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
	cout << "^~^~^~^";}

#pragma once
#include "Virus.h"
#include "Vacuna.h"
#include "Monigote.h"
#include "Casa.h"
#include <conio.h>
#include <vector>
class Controlador
{public:
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
	time_t tiempoInicial;};
Controlador::Controlador()
{cantidadVacunas = 1;
	tiempoInicial = time(0);
	Random r;
	n = r.Next(3, 7);
	for (int i = 0; i < n; i++)
		figuras.push_back(new Virus());
	figuras.push_back(new Vacuna());

	casa = new Casa();
	monigote = new Monigote();}
Controlador::~Controlador(){}
void Controlador::gestionarTodo() {
	for (int i = 0; i < figuras.size(); i++) {
		figuras[i]->borrar();
		figuras[i]->mover();
		figuras[i]->dibujar();
	}
	casa->dibujar();
	monigote->borrar();
	monigote->mover();
	monigote->dibujar();}
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
	return false;}
void Controlador::jugar() {
	char tecla;
	do
	{tecla = NULL;
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
		}_sleep(50);
	} while (!colision());
	cout << "Cantidad de pasos del monigote: " << monigote->getPasos() << endl;
	getch();getch();getch();}

#pragma once
#include <iostream>
#define anchoConsola 100
#define altoConsola 30
using namespace std;
using namespace System;
class Figura
{public:
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
	string tipo;};
Figura::Figura(){}
Figura::~Figura(){}
void Figura::movimiento(int pdx, int pdy) {
	if (x + dx < 0 || x + dx + ancho > anchoConsola)
		dx = pdx;
	if (y + dy < 0 || y + dy + alto > altoConsola)
		dy = pdy;
	x += dx;
	y += dy;}
void Figura::borrar() {
	for (int i = 0; i < alto; i++)
		for (int j = 0; j < ancho; j++) {
			Console::SetCursorPosition(x + j, y + i);
			cout << " ";
		}}
void Figura::dibujar() {}
void Figura::mover() {}
bool Figura::colision(int ex, int ey, int eancho, int ealto) {
return x <= ex + eancho && ex <= x + ancho && y <= ey + ealto && ey <= y + alto;}
int Figura::getX() { return x; }
int Figura::getY() { return y; }
int Figura::getAncho() { return ancho; }
int Figura::getAlto() { return alto; }
string  Figura::getTipo() { return tipo; }

#pragma once
#include "Figura.h"
class Vacuna : public Figura
{public:
	Vacuna();
	~Vacuna();
	void dibujar();
	void mover();
private:};

Vacuna::Vacuna()
{tipo = "vacuna";
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
	dx *= 2;}
Vacuna::~Vacuna(){}
void Vacuna::dibujar() {
	Console::SetCursorPosition(x, y);
	cout << "|->|";}
void Vacuna::mover() {
	movimiento(-dx, -dy);}

#pragma once
#define izquierda 75
#define derecha 77
#define arriba 72
#define abajo 80
#include "Figura.h"
class Monigote : public Figura
{public:
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
	int vidas;};
Monigote::Monigote()
{cantPasos = 0;
	vidas = 3;
	alto = 2;
	ancho = 1;
	x = 0;
	y = 0;
	dx = dy = 0;
	inmune = false;
	cantMovimientos = 0;}
Monigote::~Monigote(){}
void Monigote::dibujar() {
	Console::SetCursorPosition(x, y);
	cout << "O";
	Console::SetCursorPosition(x, y + 1);
	cout << "+";}
void Monigote::mover() {
	movimiento(0, 0);}
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
		}}}
void Monigote::reposicionar() {x = y = 0;}
bool Monigote::getInmune() { return inmune; }
void Monigote::hacerInmune() { inmune = true; }
void Monigote::quitarVidas() { vidas--; }
int Monigote::getVidas() { return vidas; }
int Monigote::getPasos() { return cantPasos; }

#pragma once
#include "Figura.h"
class Virus : public Figura
{public:
	Virus();
	~Virus();
	void dibujar();
	void mover();
private:};
Virus::Virus()
{tipo = "virus";
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
	} while (dx == 0 || dy == 0);}
Virus::~Virus(){}
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
	cout << "    b";}
void Virus::mover() {
	movimiento(-dx, -dy);}
//Murcielago-
#include "Controlador.h"
void main() {Console::SetWindowSize(anchoConsola, altoConsola);
	Console::CursorVisible = false;
	srand(time(0));
	Juego* juego = new Juego();
	juego->jugar();}

#pragma once
#include "Murcielago.h"
#include "Monigote.h"
#include <vector>
#include <conio.h>
class Juego//controlador
{public:
	Juego();
	~Juego();
	void jugar();
	void validarMovimiento(int pdx, int pdy);
	void gestionarTodo();
	void borrar();
private:
	Monigote* monigote;
	Murcielago* murcielago;};
Juego::Juego()
{monigote = new Monigote();
	murcielago = new Murcielago(40, 21);}
Juego::~Juego(){}
void Juego::gestionarTodo() {
	monigote->borrar();
	monigote->mover();
	monigote->dibujar();
	murcielago->InsertaBala();
	murcielago->Dibujar();
	murcielago->DibujarBalas();
	_sleep(15);
	murcielago->Borrar();
	murcielago->BorrarBalas();
	murcielago->Mover();
	murcielago->MoverBalas();
	murcielago->AnalizayEliminarBala();}
void Juego::borrar(){}
void Juego::jugar() {
	do
	{if (kbhit())
			monigote->direccion(getch());
		gestionarTodo();
		borrar();
		monigote->direccion(NULL);
		_sleep(50);
	} while (true);}

#pragma once
#include <iostream>
#define anchoConsola 120
#define altoConsola 30
using namespace std;
using namespace System;
class Objeto
{public:
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
	char tipo;};
Objeto::Objeto(int pancho, int palto)
{ancho = pancho;
	alto = palto;
	y = rand() % (altoConsola - alto - 3);
	dy = 0;}
Objeto::~Objeto(){}
void Objeto::borrar() {
	for (int i = 0; i < alto; i++)
		for (int j = 0; j < ancho; j++) {
			Console::SetCursorPosition(x + j, y + i);
			cout << " ";
		}}
void Objeto::dibujar() {}
void Objeto::validarMovimiento(int pdx, int pdy) {
	if (x + dx < 0 || x + dx + ancho > anchoConsola)
		dx = pdx;
	if (y + dy < 0 || y + dy + alto > altoConsola)
		dy = pdy;
	x += dx;
	y += dy;}
int Objeto::getX() { return x; }
int Objeto::getY() { return y; }
int Objeto::getDX() { return dx; }
int Objeto::getDY() { return dy; }
int Objeto::getAncho() { return ancho; }
int Objeto::getAlto() { return alto; }
int Objeto::getTipo() { return tipo; }

#pragma once
#include "Objeto.h"
#define arriba 72
#define abajo 80
#define izquierda 75
#define derecha 77
class Monigote : public Objeto
{public:
	Monigote();
	~Monigote();
	void dibujar();
	void mover();
	void direccion(char tecla);
private:};
Monigote::Monigote() : Objeto(1, 2)
{y = 1;
	x = 2;
	dx = 0;
	dy = 0;}
Monigote::~Monigote()
{}
void Monigote::dibujar() {
	Console::SetCursorPosition(x, y);
	cout << "0";
	Console::SetCursorPosition(x, y + 1);
	cout << "+";}
void Monigote::mover() {
	validarMovimiento(0, 0);}
void Monigote::direccion(char tecla) {
	dx = dy = 0;
	switch (tecla)
	{
	case arriba: dy = -1; break;
	case abajo: dy = 1; break;
	case izquierda: dx = -2; break;
	case derecha: dx = 2; break;}}

#pragma once
#include"bala.h"
#include <vector>
class Murcielago {
private:
	int x, y, dy, alto, ancho, contador_eliminados;
	vector<CBala> vecbalas;
public:
	Murcielago(int px, int py);
	~Murcielago();
	void Dibujar();
	void Mover();
	void Borrar();
	void DibujarBalas();
	void MoverBalas();
	void BorrarBalas();
	void InsertaBala();
	void AnalizayEliminarBala();
	int GetContador_Eliminados();};
Murcielago::Murcielago(int px, int py) {
	x = 75; y = 14; dy = 1; alto = 3; ancho = 4; contador_eliminados = 0;}
Murcielago::~Murcielago() {};
void Murcielago::Dibujar() {
	Console::SetCursorPosition(x, y);     cout << " ###";
	Console::SetCursorPosition(x, y + 1); cout << "--* ";
	Console::SetCursorPosition(x, y + 2); cout << " ###";}
void Murcielago::Borrar() {
	Console::SetCursorPosition(x, y);     cout << "     ";
	Console::SetCursorPosition(x, y + 1); cout << "     ";
	Console::SetCursorPosition(x, y + 2); cout << "     ";}
void Murcielago::Mover() {
	if (y + dy < 0 || y + dy + alto>24)dy *= -1;
	y += dy;}
void Murcielago::DibujarBalas() {
	for each (CBala objeto in vecbalas) {
		objeto.Dibujar();}}
void Murcielago::BorrarBalas() {
	for each (CBala objeto in vecbalas) {
		objeto.Borrar();}}
void Murcielago::MoverBalas() {
	for (int i = 0; i < vecbalas.size(); i++) {
		vecbalas[i].Mover();}}
void Murcielago::InsertaBala() {
	vecbalas.push_back(CBala(x + 1, y - 1));}
void Murcielago::AnalizayEliminarBala() {
	if (vecbalas.size() > 0) {
		if (vecbalas[0].GetX() == 1) {
			vecbalas.erase(vecbalas.begin());
			contador_eliminados++;}}}
int Murcielago::GetContador_Eliminados() {
	return contador_eliminados;}

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
	void Borrar();};
CBala::CBala(int px, int py) {
	x = px; y = py; dx = -1;}
CBala::~CBala() {}
int CBala::GetX() {
	return x;}
void CBala::Dibujar() {
	Console::SetCursorPosition(x - 2, y + 2);
	cout << "*";}
void CBala::Mover() {
	x = x + dx;}
void CBala::Borrar() {
	Console::SetCursorPosition(x - 2, y + 2);
	cout << " ";}
//transporte-
#include "Juego.h"
void main() {
	Console::SetWindowSize(anchoConsola, altoConsola);
	Console::CursorVisible = false;
	srand(time(0));
	Juego* juego = new Juego();
	juego->jugar();}

#pragma once
#include "Terrestre.h"
class Bicicleta : public Terrestre
{public:
	Bicicleta();
	~Bicicleta();
	void dibujar();
	void mover();
private:};
Bicicleta::Bicicleta() : Terrestre(6, 2)
{Random r;
	tipo = 'I';
	x = r.Next(anchoConsola - 6);
	dx = r.Next(2) * 2 - 1;}
void Bicicleta::dibujar() {
	if (dx < 0) {
		Console::SetCursorPosition(x, y);
		cout << "/_..";
		Console::SetCursorPosition(x, y + 1);
		cout << "(o)(o)";}
	else {Console::SetCursorPosition(x, y);
		cout << ".._\\";
		Console::SetCursorPosition(x, y + 1);
		cout << "(o)(o)";}}
void Bicicleta::mover() {validarMovimiento(-dx, 0);}

#pragma once
#include "Terrestre.h"
class Bus : public Terrestre
{public:
	Bus();
	~Bus();
	void dibujar();
	void mover();
private:
};
Bus::Bus() : Terrestre(13, 3)
{tipo = 'B';
	x = anchoConsola - 13;
	dx = -2;}
Bus::~Bus(){}
void Bus::dibujar() {
	Console::SetCursorPosition(x, y);
	cout << "_/_|[][][][]|";
	Console::SetCursorPosition(x, y + 1);
	cout << "[           |";
	Console::SetCursorPosition(x, y + 2);
	cout << "=--OO---OO-=";}
void Bus::mover() {
	Random r;
	dx = -r.Next(3, 6);
	validarMovimiento(0, 0);}

#pragma once
#include "Terrestre.h"
class Carro : public Terrestre
{public:
	Carro();
	~Carro();
	void dibujar();
	void mover();
private:
};
Carro::Carro() : Terrestre(8, 3)
{tipo = 'C';
	x = 0;
	dx = 2;}
Carro::~Carro(){}
void Carro::dibujar() {
	Console::SetCursorPosition(x, y);
	cout << "   | ~\_";
	Console::SetCursorPosition(x, y + 1);
	cout << "[| _ | -";
	Console::SetCursorPosition(x, y + 2);
	cout << " (_)(_) ";}
void Carro::mover() {
	Random r;
	dx = r.Next(1, 4);
	validarMovimiento(0, 0);}

#pragma once
#include "Bicicleta.h"
#include "Carro.h"
#include "Bus.h"
#include "Persona.h"
#include <vector>
#include <conio.h>
class Juego
{public:
	Juego();
	~Juego();
	void gestionarTodo();
	bool colision();
	void jugar();
	void agregarTransporte();
	void borrar();
private:
	vector <Terrestre*> transportes;
	Persona* persona;};
Juego::Juego()
{transportes.push_back(new Bicicleta());
	persona = new Persona();}
Juego::~Juego(){}
void Juego::gestionarTodo() {
	for (int i = 0; i < transportes.size(); i++) {
		transportes[i]->borrar();
		transportes[i]->mover();
		transportes[i]->dibujar();}
	persona->borrar();
	persona->mover();
	persona->dibujar();}
bool Juego::colision() {
	for (int i = 0; i < transportes.size(); i++)
		if (transportes[i]->getTipo() != 'I')
			if (transportes[i]->colision(persona->getX(), persona->getY(), persona->getAncho(), persona->getAlto()))
				return true;
	return false;}
void  Juego::borrar() {
	for (int i = 0; i < transportes.size(); i++)
		if (transportes[i]->getDX() == 0) {
			transportes[i]->borrar();
			transportes.erase(transportes.begin() + i);
			i--;}}
void Juego::jugar() {
	do
	{if (kbhit())
			persona->direccion(getch());
		gestionarTodo();
		agregarTransporte();
		borrar();
		//persona->direccion(NULL);
		_sleep(50);
	} while (!colision());}
void Juego::agregarTransporte() {
	Random r;
	switch (r.Next(50))
	{case 1: case 2: transportes.push_back(new Carro()); break;
	case 3: case 4: transportes.push_back(new Bus()); break;
	case 5:
		if (r.Next(10) == 1)
			transportes.push_back(new Bicicleta());
		break;}}

#pragma once
#include "Terrestre.h"
#define izquierda 75
#define derecha 77
#define arriba 72
#define abajo 80
class Persona : public Terrestre
{public:
	Persona();
	~Persona();
	void dibujar();
	void mover();
	void direccion(char tecla);
private:
};
Persona::Persona() : Terrestre(1, 2)
{y = altoConsola - 2;
	x = anchoConsola / 2;
	dx = 0;
	dy = 0;}
Persona::~Persona(){}
void Persona::dibujar() {
	Console::SetCursorPosition(x, y);
	cout << "0";
	Console::SetCursorPosition(x, y + 1);
	cout << "+";}
void Persona::mover() {validarMovimiento(0, 0);}
void Persona::direccion(char tecla) {
	dx = dy = 0;
	switch (tecla)
	{case arriba: dy = -1; break;
	case abajo: dy = 1; break;
	case izquierda: dx = -2; break;
	case derecha: dx = 2; break;}}

#pragma once
#include <iostream>
#define anchoConsola 120
#define altoConsola 30
using namespace std;
using namespace System;
class Terrestre
{public:
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
	char tipo;};

Terrestre::Terrestre(int pancho, int palto)
{ancho = pancho;
	alto = palto;
	y = rand() % (altoConsola - alto - 3);
	dy = 0;}
Terrestre::~Terrestre(){}
void Terrestre::validarMovimiento(int pdx, int pdy) {
	if (x + dx < 0 || x + dx + ancho > anchoConsola)
		dx = pdx;
	if (y + dy < 0 || y + dy + alto> altoConsola)
		dy = pdy;
	x += dx;
	y += dy;}
void Terrestre::borrar() {
	for (int i = 0; i < alto; i++)
		for (int j = 0; j < ancho; j++) {
			Console::SetCursorPosition(x + j, y + i);
			cout << " ";}}
void Terrestre::dibujar() {}
bool Terrestre::colision(int ex, int ey, int eancho, int ealto) {
	return x < ex + eancho && ex < x + ancho
		&& y < ey + ealto && ey < y + alto;}
int Terrestre::getX() { return x; }
int Terrestre::getY() { return y; }
int Terrestre::getDX() { return dx; }
int Terrestre::getDY() { return dy; }
int Terrestre::getAncho() { return ancho; }
int Terrestre::getAlto() { return alto; }
int Terrestre::getTipo() { return tipo; }
//protones-
#include "Manejador.h"
#include <conio.h>
void main()
{CManejador objman;
	objman.Generar();
	while (1)
	{Console::CursorVisible = false;
		objman.DibujarTodos();
		_sleep(200);
		objman.BorrarTodos();
		objman.MoverTodos();
		objman.EliminarParticula();
		objman.ColisionProton();
		objman.ColisionNeutron();
		Console::SetCursorPosition(0, 25);
		cout << "Numero de protones: " << objman.getProton();
		Console::SetCursorPosition(0, 26);
		cout << "Numero de neutrones: " << objman.getNeutron();}}

#pragma once
#include "Particula.h"
#include "QArriba.h"
#include "QAbajo.h"
#include "Proton.h"
#include "Neutron.h"
#include <vector>
class CManejador
{private:
	vector<CParticula*>vecpar;
public:
	CManejador();
	void Generar();
	void DibujarTodos();
	void BorrarTodos();
	void MoverTodos();
	void EliminarParticula();
	void ColisionProton();
	void ColisionNeutron();
	int getProton();
	int getNeutron();
};
CManejador::CManejador() {}
void CManejador::Generar()
{Random f;
	for (int i = 0; i <= 10; i++)
	{vecpar.push_back(new CQArriba(f.Next(0, 70), f.Next(0, 15)));
		_sleep(10);}
	for (int i = 0; i <= 10; i++)
	{vecpar.push_back(new CQAbajo(f.Next(0, 70), f.Next(0, 15)));
		_sleep(10);}}
void CManejador::DibujarTodos()
{for (int i = 0; i < vecpar.size(); i++)
		vecpar[i]->Dibujar();}
void CManejador::BorrarTodos()
{for (int i = 0; i < vecpar.size(); i++)
		vecpar[i]->Borrar();}
void CManejador::MoverTodos()
{for (int i = 0; i < vecpar.size(); i++)
		vecpar[i]->Mover();}
void CManejador::EliminarParticula()
{int x, y;
	if (vecpar.size() > 0)
	{for (int i = 0; i < vecpar.size(); i++)
		{if (vecpar[i]->getTipo() == 1 || vecpar[i]->getTipo() == 2)
			{x = vecpar[i]->getX(); y = vecpar[i]->getY();
				if (x == 0 || x == 79 || y == 0 || y == 23)
					vecpar.erase(vecpar.begin() + i);
			}}}}
void CManejador::ColisionProton()
{if (vecpar.size() > 3)
	{for (int i = 0; i < vecpar.size() - 2; i++)
			for (int j = i + 1; j < vecpar.size() - 1; j++)
				for (int k = j + 1; k < vecpar.size(); k++)
				{if (vecpar[i]->getTipo() == 1 && vecpar[j]->getTipo() == 1 && vecpar[k]->getTipo() == 2 ||
						vecpar[i]->getTipo() == 1 && vecpar[j]->getTipo() == 2 && vecpar[k]->getTipo() == 1 ||
						vecpar[i]->getTipo() == 2 && vecpar[j]->getTipo() == 1 && vecpar[k]->getTipo() == 1)
{int x1 = vecpar[i]->getX();
						int y1 = vecpar[i]->getY();
						int w1 = vecpar[i]->getancho();
						int h1 = vecpar[i]->getalto();
						int x2 = vecpar[j]->getX();
						int y2 = vecpar[j]->getY();
						int w2 = vecpar[j]->getancho();
						int h2 = vecpar[j]->getalto();
						int x3 = vecpar[k]->getX();
						int y3 = vecpar[k]->getY();
						int w3 = vecpar[k]->getancho();
						int h3 = vecpar[k]->getalto();
						if (!((x1 + w1<x2 || x1>x2 + w2 || y1 + h1<y2 || y1>y2 + h2) ||
							(x2 + w2<x3 || x2>x3 + w3 || y2 + h2<y3 || y2>y3 + h3) ||
							(x3 + w3<x1 || x3>x1 + w1 || y3 + h3<y1 || y3>y1 + h1)))
						{vecpar.push_back(new CProton(x1, y1));
							vecpar.erase(vecpar.begin() + k);
							vecpar.erase(vecpar.begin() + j);
							vecpar.erase(vecpar.begin() + i);
						}}}}}
void CManejador::ColisionNeutron()
{if (vecpar.size() > 3)
	{for (int i = 0; i < vecpar.size() - 2; i++)
			for (int j = i + 1; j < vecpar.size() - 1; j++)
				for (int k = j + 1; k < vecpar.size(); k++)
				{if (vecpar[i]->getTipo() == 2 && vecpar[j]->getTipo() == 2 && vecpar[k]->getTipo() == 1 ||
						vecpar[i]->getTipo() == 2 && vecpar[j]->getTipo() == 1 && vecpar[k]->getTipo() == 2 ||
						vecpar[i]->getTipo() == 1 && vecpar[j]->getTipo() == 2 && vecpar[k]->getTipo() == 2)
					{int x1 = vecpar[i]->getX();
						int y1 = vecpar[i]->getY();
						int w1 = vecpar[i]->getancho();
						int h1 = vecpar[i]->getalto();
						int x2 = vecpar[j]->getX();
						int y2 = vecpar[j]->getY();
						int w2 = vecpar[j]->getancho();
						int h2 = vecpar[j]->getalto();
						int x3 = vecpar[k]->getX();
						int y3 = vecpar[k]->getY();
						int w3 = vecpar[k]->getancho();
						int h3 = vecpar[k]->getalto();
						if (!((x1 + w1<x2 || x1>x2 + w2 || y1 + h1<y2 || y1>y2 + h2) ||
							(x2 + w2<x3 || x2>x3 + w3 || y2 + h2<y3 || y2>y3 + h3) ||
							(x3 + w3<x1 || x3>x1 + w1 || y3 + h3<y1 || y3>y1 + h1)))
						{vecpar.push_back(new CNeutron(x1, y1));
							vecpar.erase(vecpar.begin() + k);
							vecpar.erase(vecpar.begin() + j);
							vecpar.erase(vecpar.begin() + i);
						}}}}}
int CManejador::getProton()
{int cprot = 0;
	if (vecpar.size() > 0)
		for (int i = 0; i < vecpar.size(); i++)
			if (vecpar[i]->getTipo() == 3)
				cprot++;
	return cprot;}
int CManejador::getNeutron()
{int cneu = 0;
	if (vecpar.size() > 0)
		for (int i = 0; i < vecpar.size(); i++)
			if (vecpar[i]->getTipo() == 4)
				cneu++;
	return cneu;}

#pragma once
#include <iostream>
using namespace std;
using namespace System;
class CParticula
{protected:
	int x, y, dx, dy;
	int ancho, alto;
	int tipo;
public:
	CParticula(int x, int y);
	virtual void Dibujar() {};
	void Borrar();
	virtual void Mover() {}
	int getX();
	int getY();
	int getTipo();
	int getancho();
	int getalto();};
CParticula::CParticula(int px, int py){x = px; y = py;}
void CParticula::Borrar()
{int py = y;
	for (int fila = 1; fila <= alto; fila++)
	{Console::SetCursorPosition(x, py);
		for (int col = 1; col <= ancho; col++)
			cout << ' ';
		py++;}}
int CParticula::getX() { return x; }
int CParticula::getY() { return y; }
int CParticula::getTipo() { return tipo; }
int CParticula::getalto() { return alto; }
int CParticula::getancho() { return ancho; }

#pragma once
#include "Particula.h"
class CProton :public CParticula
{public:
	CProton(int px, int py);
	void Dibujar();
	void Rebota();
	void Mover();};
CProton::CProton(int px, int py) :CParticula(px, py)
{Random f;
	dx = f.Next(-1, 2); dy = f.Next(-1, 2);
	if (dx == 0 && dy == 0) { dx = 1; dy = 1; }
	ancho = 7; alto = 4;
	tipo = 3;}
void CProton::Dibujar()
{Console::SetCursorPosition(x, y);		cout << " ***** ";
	Console::SetCursorPosition(x, y + 1);	cout << "* U U *";
	Console::SetCursorPosition(x, y + 2);	cout << "*  D  *";
	Console::SetCursorPosition(x, y + 3);	cout << " ***** ";}
void CProton::Rebota(){dx *= -1; dy *= -1;}
void CProton::Mover()
{if (x + dx < 0 || x + dx + ancho>79) dx *= -1;
	if (y + dy < 0 || y + dy + alto>24) dy *= -1;
	x += dx;
	y += dy;}

#pragma once
#include "Particula.h"
class CNeutron :public CParticula
{public:
	CNeutron(int px, int py);
	void Dibujar();
	void Rebota();
	void Mover();};
CNeutron::CNeutron(int px, int py) :CParticula(px, py)
{Random f;
	dx = f.Next(-1, 2); dy = f.Next(-1, 2);
	if (dx == 0 && dy == 0) { dx = 1; dy = 1; }
	ancho = 7; alto = 4;
	tipo = 4;}
void CNeutron::Dibujar()
{Console::SetCursorPosition(x, y);		cout << " ***** ";
	Console::SetCursorPosition(x, y + 1);	cout << "* D D *";
	Console::SetCursorPosition(x, y + 2);	cout << "*  U  *";
	Console::SetCursorPosition(x, y + 3);	cout << " ***** ";}
void CNeutron::Rebota(){dx *= -1; dy *= -1;}
void CNeutron::Mover()
{if (x + dx < 0 || x + dx + ancho>79) dx *= -1;
	if (y + dy < 0 || y + dy + alto>24) dy *= -1;
	x += dx;
	y += dy;}

#pragma once
#include "Particula.h"
class CQArriba :public CParticula
{public:
	CQArriba(int px, int py);
	void Dibujar();
	void Mover();};
CQArriba::CQArriba(int px, int py) :CParticula(px, py)
{Random f;
	ancho = 5; alto = 3;
	dx = 1;
	dy = 0;
	tipo = 1;}
void CQArriba::Dibujar()
{Console::SetCursorPosition(x, y);	  cout << " *** ";
	Console::SetCursorPosition(x, y + 1); cout << "| U |";
	Console::SetCursorPosition(x, y + 2); cout << " *** ";}
void CQArriba::Mover()
{Random f;
	if (x + dx < 0 || x + dx + ancho>79) x = f.Next(0, 60);
	if (y + dy < 0 || y + dy + alto>24) y = f.Next(0, 60);
	x += dx;
	y += dy;}

#pragma once
#include "Particula.h"
class CQAbajo :public CParticula
{public:
	CQAbajo(int px, int py);
	void Dibujar();
	void Mover();};
CQAbajo::CQAbajo(int px, int py) :CParticula(px, py)
{Random f;
	ancho = 5; alto = 3;
	dx = 0;
	dy = 1;
	tipo = 2;}
void CQAbajo::Dibujar()
{Console::SetCursorPosition(x, y);	  cout << " *** ";
	Console::SetCursorPosition(x, y + 1); cout << "| D |";
	Console::SetCursorPosition(x, y + 2); cout << " *** ";}
void CQAbajo::Mover()
{Random f;
	if (x + dx < 0 || x + dx + ancho>79) x = f.Next(10, 60);
	if (y + dy < 0 || y + dy + alto>24) y = f.Next(0, 10);
	x += dx;
	y += dy;}