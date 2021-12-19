//**************************************************************************
// Práctica 1 usando objetos
//**************************************************************************

#include <vector>
#include <GL/gl.h>
#include "vertex.h"
#include <stdlib.h>


const float AXIS_SIZE=5000;
typedef enum{POINTS,EDGES,SOLID_CHESS,SOLID, SOLID_ILLUMINATED_FLAT,
             SOLID_ILLUMINATED_GOURAUD,SELECT} _modo;

//*************************************************************************
// clase punto
//*************************************************************************

class _puntos3D
{
public:

  
	_puntos3D();
void 	draw_puntos(float r, float g, float b, int grosor);

vector<_vertex3f> vertices;
};

//*************************************************************************
// clase triángulo
//*************************************************************************

class _triangulos3D: public _puntos3D
{
public:

	_triangulos3D();
void 	draw_aristas(float r, float g, float b, int grosor);
void    draw_solido(float r, float g, float b);
void 	draw_solido_ajedrez(float r1, float g1, float b1, float r2, float g2, float b2);
void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

void 	draw_seleccion(int r, int g, int b);

void  	draw_iluminacion_plana( );
void 	draw_iluminacion_suave( );

void	calcular_normales_caras();
void 	calcular_normales_vertices();

vector<_vertex3i> caras;

vector<_vertex3f> normales_caras;
vector<_vertex3f> normales_vertices;

bool b_normales_caras;
bool b_normales_vertices;

_vertex4f ambiente_difusa;     //coeficientes ambiente y difuso
_vertex4f especular;           //coeficiente especular
float brillo;                  //exponente del brillo 



};


//*************************************************************************
// clase cubo
//*************************************************************************

class _cubo: public _triangulos3D
{
public:

	_cubo(float tam=0.5);
};


//*************************************************************************
// clase piramide
//*************************************************************************

class _piramide: public _triangulos3D
{
public:

	_piramide(float tam=0.5, float al=0.75);
};

//*************************************************************************
// clase objeto ply
//*************************************************************************

class _objeto_ply: public _triangulos3D
{
public:
   _objeto_ply();

int   parametros(char *archivo);
};

//************************************************************************
// objeto por revolución
//************************************************************************

class _rotacion: public _triangulos3D
{
public:
       _rotacion();
void  parametros(vector<_vertex3f> perfil1, int num1, int tipo); //0=cilindro, 1=cono, 2=esfera

vector<_vertex3f> perfil; 
int num;
};

//************************************************************************
// Esfera, cilindro y cono
//************************************************************************


class _esfera: public _rotacion
{
	public:
		_esfera();
		_esfera(float radio, int n, int m); //n=nº lados transversales m=nº lados longitudinales
};

class _cono: public _rotacion
{
	public:
		_cono(float radio, float altura, int m); //altura m=nº lados
};

class _cilindro: public _rotacion
{
	public:
		_cilindro(float radio, float altura, int m); //altura m=nº lados
};


//************************************************************************
// objeto articulado : Homerbot
//************************************************************************

//He añadido parámetros a los draw de algunos objetos para poder dibujar de varios colores piezas de una misma clase y que luego se cambien en el modo SELECT

class _chasis: public _triangulos3D{
	public:
		_chasis();
	void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float r3, float g3, float b3, float r4, float g4, float b4, float grosor);


protected:
	_rotacion cilindro;
	_cubo cuerpo;
};

class _brazoMazo: public _triangulos3D{
	public:
		_brazoMazo();
		float moviCodo;
	void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float r3, float g3, float b3, float r4, float g4, float b4, float r5, float g5, float b5, float r6, float g6, float b6, float grosor);

private:
	_rotacion codo;

//protected:
	_rotacion mazo;
	
};


class _brazoVacio: public _triangulos3D{
	public:
		_brazoVacio();
		float moviCodo;
	void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float r3, float g3, float b3, float r4, float g4, float b4, float grosor);

private:
	_rotacion codo;

//protected:
	_rotacion vacio;
	
};

class _detalles: public _triangulos3D{
	public:
		_detalles();
	void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float r3, float g3, float b3, float r4, float g4, float b4, float grosor);

protected:
	_rotacion detalle;
};

class _robotHomer: public _triangulos3D{
	public:
		_robotHomer();
	void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);
	void 	seleccion();


//Respecto eje X
float giroHombroM, giroHombroV;

float giroHombroM_min, giroHombroM_max, 
	  giroHombroV_min, giroHombroV_max;
//Respecto eje Z
float abrirHombroM, abrirHombroV;
float abrirHombroM_min, abrirHombroM_max,
	  abrirHombroV_min, abrirHombroV_max;

//Para el codo
float movCodo, movCodo_min, movCodo_max;

float  color_pick[3];
int    color_selec[3][3];
int    activo[3];
int    piezas;

protected:
	_chasis chasis;
	_brazoMazo brazoM;
	_brazoVacio brazoV;
	_detalles detalles;
};