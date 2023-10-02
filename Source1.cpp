#include "Manejador.h"
#include <conio.h>
void main() {
	char tecla;
	CManejador* objeto;
	Console::CursorVisible = false;
	objeto = new CManejador();
	while (1) {
		if (_kbhit()) {
			tecla = toupper(_getch());
			if (tecla == 'C' || tecla == 'T')objeto->Insertar(tecla);
			if (tecla == 'H')objeto->Cambiar_Direccion_Horizontal();
			if (tecla == 'V')objeto->Cambiar_Direccion_Vertical();
			if (tecla == 'R')objeto->Cambiar_Relleno();
		}
		objeto->Dibujar();
		_sleep(50);
		objeto->Borrar();
		objeto->Mover();
	}
}

#pragma once
#include "Figura.h"
class CCuadrado : public CFigura
{
private:
	int tipo;
	int relleno;
public:
	CCuadrado();
	CCuadrado(int px, int py, int pdx, int pdy, int plado, int ptipo);
	void Dibujar();
	void CambiaDx();
	void CambiaDy();
	void CambiaRelleno();
};

CCuadrado::CCuadrado()
{ }
CCuadrado::CCuadrado(int px, int py, int pdx, int pdy, int plado, int ptipo) :
	CFigura(px, py, pdx, pdy, plado)
{
	tipo = ptipo;
	relleno = 1;
}
void CCuadrado::Dibujar()
{
	if (tipo == 1)
		Console::ForegroundColor = ConsoleColor::Green;
	else
		Console::ForegroundColor = ConsoleColor::Yellow;
	int px = x, py = y;
	if (relleno == 1)
	{
		for (int i = 1; i <= lado; i++)
		{
			Console::SetCursorPosition(px, py);
			for (int k = 1; k <= lado; k++)
				cout << "*";
			py++;
		}
	}
	else
	{
		for (int i = 1; i <= lado; i++)
		{
			Console::SetCursorPosition(px, py);
			for (int k = 1; k <= lado; k++)
				if (i == 1 || i == lado || k == 1 || k == lado)
					cout << "*";
				else
					cout << " ";
			py++;
		}
	}
}
void CCuadrado::CambiaDx()
{
	dx = -dx;
}
void CCuadrado::CambiaDy()
{
	dy = -dy;
}
void CCuadrado::CambiaRelleno()
{
	if (relleno == 1) relleno = 0;
	else
		relleno = 1;
}

#pragma once
#include<iostream>
using namespace std;
using namespace System;
class CFigura
{
protected:
	int x, y, dx, dy, lado;
public:
	CFigura();
	CFigura(int px, int py, int pdx, int pdy, int plado);
	void Mover();
	void Borrar();
	virtual void Dibujar() {};
	virtual void CambiaDx() {};
	virtual void CambiaDy() {};
	virtual void CambiaRelleno() {};
};
CFigura::CFigura()
{ }
CFigura::CFigura(int px, int py, int pdx, int pdy, int plado)
{
	x = px; y = py; dx = pdx; dy = pdy; lado = plado;
}
void CFigura::Mover()
{
	if (x + dx < 0 || x + dx + lado > 79)
	{
		dx = -dx;
	}
	if (y + dy < 0 || y + dy + lado > 24)
	{
		dy = -dy;
	}
	x = x + dx;
	y = y + dy;
}
void CFigura::Borrar()
{
	int px = x, py = y;
	for (int i = 1; i <= lado; i++)
	{
		Console::SetCursorPosition(px, py);
		for (int k = 1; k <= lado; k++)
			cout << " ";
		py++;
	}
}

#pragma once
#include "Cuadrado.h"
#include "Triangulo.h"
#include<vector>
class CManejador
{
private:
	vector<CFigura*>vecfig;
public:
	CManejador();
	void Insertar(char letra);
	void Dibujar();
	void Mover();
	void Borrar();
	void Cambiar_Direccion_Vertical();
	void Cambiar_Direccion_Horizontal();
	void Cambiar_Relleno();
};

CManejador::CManejador() {}
void CManejador::Insertar(char letra)
{
	Random r;
	if (letra == 'C')
	{
		int tipo = r.Next(1, 3);
		if (tipo == 1)
			vecfig.push_back(new CCuadrado(r.Next(1, 70), r.Next(1, 15), 0, 1,
				r.Next(2, 6), tipo));
		else
			vecfig.push_back(new CCuadrado(r.Next(1, 70), r.Next(1, 15), 1, 0,
				r.Next(2, 6), tipo));
	}
	if (letra == 'T')
		vecfig.push_back(new CTriangulo(r.Next(1, 70), r.Next(7, 16), 1, 0,
			r.Next(2, 6), r.Next()));
}
void CManejador::Dibujar() {
	for (int i = 0; i < vecfig.size(); i++)
		vecfig[i]->Dibujar();
}
void CManejador::Mover() {
	for (int i = 0; i < vecfig.size(); i++)
		vecfig[i]->Mover();
}
void CManejador::Borrar() {
	for (int i = 0; i < vecfig.size(); i++)
		vecfig[i]->Borrar();
}
void CManejador::Cambiar_Direccion_Vertical() {
	for (int i = 0; i < vecfig.size(); i++)
		vecfig[i]->CambiaDy();
}
void CManejador::Cambiar_Direccion_Horizontal() {
	for (int i = 0; i < vecfig.size(); i++)
		vecfig[i]->CambiaDx();
}
void CManejador::Cambiar_Relleno() {
	for (int i = 0; i < vecfig.size(); i++)
		vecfig[i]->CambiaRelleno();
}

#pragma once
#include "Figura.h"
class CTriangulo : public CFigura
{
private:
	char car;
public:
	CTriangulo();
	CTriangulo(int px, int py, int pdx, int pdy, int plado, char pcar);
	void Dibujar();
};
CTriangulo::CTriangulo()
{ }
CTriangulo::CTriangulo(int px, int py, int pdx, int pdy, int plado, char pcar) :
	CFigura(px, py, pdx, pdy, plado)
{
	car = pcar;
}
void CTriangulo::Dibujar()
{
	int px = x, py = y;
	Console::ForegroundColor = ConsoleColor::Red;
	for (int fila = 1; fila <= lado; fila++)
	{
		Console::SetCursorPosition(px, py);
		for (int col = 1; col <= fila; col++)
			cout << car;
		py++;
	}
}