//**************************************************************************
// Práctica 1 usando objetos
//**************************************************************************

#include "objetos_B2.h"
#include "file_ply_stl.hpp"


//*************************************************************************
// _puntos3D
//*************************************************************************

_puntos3D::_puntos3D()
{
}

//*************************************************************************
// dibujar puntos
//*************************************************************************

void _puntos3D::draw_puntos(float r, float g, float b, int grosor)
{
int i;
glPointSize(grosor);
glColor3f(r,g,b);
glBegin(GL_POINTS);
for (i=0;i<vertices.size();i++){
	glVertex3fv((GLfloat *) &vertices[i]);
	}
glEnd();
}


//*************************************************************************
// _triangulos3D
//*************************************************************************

_triangulos3D::_triangulos3D()
{
    b_normales_caras = false;
    b_normales_vertices = false;

    ambiente_difusa = _vertex4f(0.2, 0.4, 0.9, 1.0); //coeficientes ambiente y difuso
    especular = _vertex4f(0.5, 0.5, 0.5, 1.0);       //coeficiente especular
    brillo = 50; 
}


//*************************************************************************
// dibujar en modo arista
//*************************************************************************

void _triangulos3D::draw_aristas(float r, float g, float b, int grosor)
{
int i;
glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
glLineWidth(grosor);
glColor3f(r,g,b);
glBegin(GL_TRIANGLES);
for (i=0;i<caras.size();i++){
	glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
	}
glEnd();
}

//*************************************************************************
// dibujar en modo sólido
//*************************************************************************

void _triangulos3D::draw_solido(float r, float g, float b)
{
int i;
glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
glLineWidth(0.5);
glColor3f(r,g,b);
glBegin(GL_TRIANGLES);
for (i=0;i<caras.size();i++){
	glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
	}
glEnd();
}

//***************************************************************************
// dibujar iluminacion suave
//***************************************************************************


void _triangulos3D::draw_iluminacion_suave( ){
  int i;
  if (b_normales_caras==false) calcular_normales_caras(); //Calculamos lasnormales alas caras s i no se han calcuado ys
  if(b_normales_vertices==false) calcular_normales_vertices();

  glEnable (GL_LIGHTING);
  glShadeModel(GL_SMOOTH);  //GL_SMOOTH
  glEnable(GL_NORMALIZE); //En caso de scale, si no, lo podemos quitar

  //Fijar color
  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,(GLfloat *) &ambiente_difusa);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,(GLfloat *) &especular);
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,brillo);

  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glBegin(GL_TRIANGLES);
  for (i=0;i<caras.size();i++){
    glNormal3fv((GLfloat *) &normales_vertices[caras[i]._0]);
    glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
    glNormal3fv((GLfloat *) &normales_vertices[caras[i]._1]);
    glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
    glNormal3fv((GLfloat *) &normales_vertices[caras[i]._2]);
    glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
    }
  glEnd();

  glDisable(GL_LIGHTING);
}

//***************************************************************************
// dibujar iluminacion plana
//***************************************************************************

void     _triangulos3D::draw_iluminacion_plana( ){
  int i;
  if (b_normales_caras==false) calcular_normales_caras(); //Calculamos lasnormales alas caras s i no se han calcuado ys

  glEnable (GL_LIGHTING);
  glShadeModel(GL_FLAT);  //GL_SMOOTH
  glEnable(GL_NORMALIZE); //En caso de scale, si no, lo podemos quitar

  //Fijar color
  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,(GLfloat *) &ambiente_difusa);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,(GLfloat *) &especular);
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,brillo);

  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glBegin(GL_TRIANGLES);
  for (i=0;i<caras.size();i++){
    glNormal3fv((GLfloat *) &normales_caras[i]);
    glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
    glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
    glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
    }
  glEnd();

  glDisable(GL_LIGHTING);
}

//***************************************************************************
// dibujar seleccion de color
//***************************************************************************


void _triangulos3D::draw_seleccion(int r, int g, int b){
  int i;

  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glColor3ub(r,g,b);
  glBegin(GL_TRIANGLES);
  for (i=0;i<caras.size();i++){
	  glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
	  glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
	  glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
	}
  glEnd();
}

//*************************************************************************
// calcular normales a caras
//*************************************************************************

void _triangulos3D::calcular_normales_caras (){

  _vertex3f a1, a2, n;
  normales_caras.resize(caras.size());

  for(int i=0; i<caras.size(); i++){
    // obtener dos vectores en el triángulo y calcular el producto vectorial
    a1=vertices[caras[i]._1]-vertices[caras[i]._0];
           a2=vertices[caras[i]._2]-vertices[caras[i]._0];
        n=a1.cross_product(a2);
    // modulo
    float m=sqrt(n.x*n.x+n.y*n.y+n.z*n.z);
    // normalización
        normales_caras[i]= _vertex3f(n.x/m, n.y/m, n.z/m);
    }

  b_normales_caras=true;
}

//*************************************************************************
// calcular normales a vertices
//*************************************************************************
void _triangulos3D::calcular_normales_vertices(){
  int n,m,i;
  n=vertices.size();
  m=caras.size();
  normales_vertices.resize(n);

  for(i=0;i<n;i++){
    normales_vertices[i].x=0.0;
    normales_vertices[i].y=0.0;
    normales_vertices[i].z=0.0;
  }

  for(i=0;i<m;i++){
    normales_vertices[caras[i]._0]+=normales_caras[i];
    normales_vertices[caras[i]._1]+=normales_caras[i];
    normales_vertices[caras[i]._2]+=normales_caras[i];
  }

  b_normales_vertices=true;
}

//*************************************************************************
// dibujar en modo sólido con apariencia de ajedrez
//*************************************************************************

void _triangulos3D::draw_solido_ajedrez(float r1, float g1, float b1, float r2, float g2, float b2)
{
int i;
glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
glLineWidth(0.5);

glBegin(GL_TRIANGLES);
for (i=0;i<caras.size();i++){
	if(i%2==0)
		glColor3f(r1,g1,b1);
		else
		glColor3f(r2,g2,b2);
	
	glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
	}
glEnd();
}

//*************************************************************************
// dibujar con distintos modos
//*************************************************************************

void _triangulos3D::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{
switch (modo){
	case POINTS:draw_puntos(r1, g1, b1, grosor);break;
	case EDGES:draw_aristas(r1, g1, b1, grosor);break;
	case SOLID_CHESS:draw_solido_ajedrez(r1, g1, b1, r2, g2, b2);break;
	case SOLID:draw_solido(r1, g1, b1);break;
  case SOLID_ILLUMINATED_FLAT:
            draw_iluminacion_plana();
            break;
        case SOLID_ILLUMINATED_GOURAUD:
            draw_iluminacion_suave();
            break;
    case SELECT:draw_seleccion(r1,g1,b1);break;
	}
}

//*************************************************************************
// clase cubo
//*************************************************************************

_cubo::_cubo(float tam)
{
//vertices
vertices.resize(8); //Los 5 puntos de la piramide 
vertices[0].x=0;vertices[0].y=0;vertices[0].z=tam;
vertices[1].x=tam;vertices[1].y=0;vertices[1].z=tam;
vertices[2].x=tam;vertices[2].y=0;vertices[2].z=0;
vertices[3].x=0;vertices[3].y=0;vertices[3].z=0;
vertices[4].x=0;vertices[4].y=tam;vertices[4].z=tam;
vertices[5].x=tam;vertices[5].y=tam;vertices[5].z=tam;
vertices[6].x=tam;vertices[6].y=tam;vertices[6].z=0;
vertices[7].x=0;vertices[7].y=tam;vertices[7].z=0;

// triangulos
caras.resize(12);
caras[0]._0=0;caras[0]._1=2;caras[0]._2=1;
caras[1]._0=0;caras[1]._1=3;caras[1]._2=2;
caras[2]._0=0;caras[2]._1=3;caras[2]._2=7;
caras[3]._0=0;caras[3]._1=7;caras[3]._2=4;
caras[4]._0=4;caras[4]._1=5;caras[4]._2=6;
caras[5]._0=4;caras[5]._1=6;caras[5]._2=7;
caras[6]._0=1;caras[6]._1=2;caras[6]._2=6;
caras[7]._0=1;caras[7]._1=6;caras[7]._2=5;
caras[8]._0=3;caras[8]._1=6;caras[8]._2=2;
caras[9]._0=3;caras[9]._1=7;caras[9]._2=6;
caras[10]._0=0;caras[10]._1=1;caras[10]._2=5;
caras[11]._0=0;caras[11]._1=5;caras[11]._2=4;
}


//*************************************************************************
// clase piramide
//*************************************************************************

_piramide::_piramide(float tam, float al)
{

//vertices 
vertices.resize(5); 
vertices[0].x=-tam;vertices[0].y=0;vertices[0].z=tam;
vertices[1].x=tam;vertices[1].y=0;vertices[1].z=tam;
vertices[2].x=tam;vertices[2].y=0;vertices[2].z=-tam;
vertices[3].x=-tam;vertices[3].y=0;vertices[3].z=-tam;
vertices[4].x=0;vertices[4].y=al;vertices[4].z=0;

caras.resize(6);
caras[0]._0=0;caras[0]._1=1;caras[0]._2=4;
caras[1]._0=1;caras[1]._1=2;caras[1]._2=4;
caras[2]._0=2;caras[2]._1=3;caras[2]._2=4;
caras[3]._0=3;caras[3]._1=0;caras[3]._2=4;
caras[4]._0=3;caras[4]._1=1;caras[4]._2=0;
caras[5]._0=3;caras[5]._1=2;caras[5]._2=1;
}

//*************************************************************************
// clase objeto ply
//*************************************************************************


_objeto_ply::_objeto_ply() 
{
   // leer lista de coordenadas de vértices y lista de indices de vértices
  
}



int _objeto_ply::parametros(char *archivo)
{
int n_ver,n_car,i;

vector<float> ver_ply ;
vector<int>   car_ply ;
 
_file_ply::read(archivo, ver_ply, car_ply );

n_ver=ver_ply.size()/3;
n_car=car_ply.size()/3;

printf("Number of vertices=%d\nNumber of faces=%d\n", n_ver, n_car);

vertices.resize(n_ver);

//El vector vertices es del tipo: xyz xyz xyz ...

for(i=0;i<n_ver;i++){
  vertices[i].x=ver_ply[3*i];
  vertices[i].y=ver_ply[3*i+1];
  vertices[i].z=ver_ply[3*i+2];
}

caras.resize(n_car);

for(i=0;i<n_car;i++){
  caras[i]._0=car_ply[3*i];
  caras[i]._1=car_ply[3*i+1];
  caras[i]._2=car_ply[3*i+2];
}


return(0);
}


//************************************************************************
// objeto por revolucion
//************************************************************************

_rotacion::_rotacion()
{

}


void _rotacion::parametros(vector<_vertex3f> perfil, int num, int tipo)
{
int i,j,c;
_vertex3f vertice_aux;
_vertex3i cara_aux;
int num_aux;
float radio=0, altura=0;

num_aux=perfil.size();

if(tipo==2){
  radio=sqrt(perfil[0].x*perfil[0].x+perfil[0].y*perfil[0].y);
}

if(tipo==1){
  num_aux=1;
  altura=perfil[1].y;
}

vertices.resize(num_aux*num+2); //+2 por los vertices de las tapas

// tratamiento de los vértices

  for (j=0;j<num;j++){
    for (i=0;i<num_aux;i++)
     {
      vertice_aux.x=perfil[i].x*cos(2.0*M_PI*j/(1.0*num))+
                    perfil[i].z*sin(2.0*M_PI*j/(1.0*num));
      vertice_aux.z=-perfil[i].x*sin(2.0*M_PI*j/(1.0*num))+
                    perfil[i].z*cos(2.0*M_PI*j/(1.0*num));
      vertice_aux.y=perfil[i].y;
      vertices[i+j*num_aux]=vertice_aux;
     }
  }

// tratamiento de las caras 

caras.resize(2*(num_aux-1)*num+2*num);

c=0;

if(tipo==0 || tipo==2){
  for (j = 0; j < num-1; j++){ //Bucle para los triangulos
    
    for (i = 0; i < num_aux - 1; i++){
      //Con num_aux nos estamos saltando hasta el punto necesario
      caras[c]._0=j*num_aux+i;
      caras[c]._2=(j+1)*num_aux+i+1;
      caras[c]._1=(j+1)*num_aux+i;
      c+=1;
      caras[c]._0=j*num_aux+i;
      caras[c]._2=j*num_aux+i+1;
      caras[c]._1=(j+1)*num_aux+i+1;
      c+=1;
  }
  }

  //Ultima cara
  for(i=0; i<(num_aux-1);i++){
    caras[c]._0=(num-1)*num_aux+i;
    caras[c]._2=i+1;
    caras[c]._1=i;
    c++;
    caras[c]._0=(num-1)*num_aux+i;
    caras[c]._2=(num-1)*num_aux+i+1;
    caras[c]._1=i+1;
    c++;
  }
}

//Tapas

 // tapa inferior

  vertices[num_aux*num].x=0.0;
  if(tipo==0) vertices[num_aux*num].y=perfil[0].y;
  else if(tipo==1) vertices[num_aux*num].y=0.0;
  else if(tipo==2) vertices[num_aux*num].y=-radio;
  vertices[num_aux*num].z=0.0;

if (fabs(perfil[0].x)>0.0)
  {
    //Generar caras:
    for(j=0;j<num-1;j++){
      caras[c]._0=num_aux*num;
      caras[c]._2=j*num_aux;
      caras[c]._1=(j+1)*num_aux;
      c++;
    }
    caras[c]._0=num*num_aux;  //Vertice centro de la cara inferior
    caras[c]._2=num*num_aux-(num_aux);
    caras[c]._1=0;
    c++;
  }
 
 // tapa superior

  vertices[num_aux*num+1].x=0.0;
  if(tipo==0) vertices[num_aux*num+1].y=perfil[num_aux-1].y; //Ultimo vertice del perfil->num_aux-1
  else if(tipo==1) vertices[num_aux*num+1].y=altura;
  else if(tipo==2) vertices[num_aux*num+1].y=radio;
  vertices[num_aux*num+1].z=0.0;
 
if (fabs(perfil[num_aux-1].x)>0.0)
  {
    //Generar caras:
    for (j = 0; j < num-1; j++){
      caras[c]._0 = num_aux * num + 1;
      caras[c]._2 = j * num_aux + (num_aux-1);
      caras[c]._1 = (j + 1) * num_aux + (num_aux-1);
      c++;
    }
    caras[c]._0=num*num_aux+1;  //Vertice centro de la cara superior
    caras[c]._2=num*num_aux-1;
    caras[c]._1=num_aux-1;
  }

}

 
//************************************************************************
// Esfera
//************************************************************************

_esfera::_esfera(float radio, int n, int m){
  vector<_vertex3f> perfil1;
  _vertex3f aux;
  int i;
  for(i=1;i<n;i++){
    aux.x=radio*cos(M_PI*i/n-M_PI/2.0);
    aux.y=radio*sin(M_PI*i/n-M_PI/2.0);
    aux.z=0.0;
    perfil1.push_back(aux);
  }

  parametros(perfil1, m, 2); //Tipo: 2

  normales_vertices.resize(vertices.size());

  for(i=0;i<vertices.size();i++){
    normales_vertices[i].x=vertices[i].x;
    normales_vertices[i].y=vertices[i].y;
    normales_vertices[i].z=vertices[i].z;
  }

  b_normales_vertices=true;
}

//************************************************************************
// Cono
//************************************************************************

_cono::_cono(float radio, float altura, int m){
  vector<_vertex3f> perfil1;
  _vertex3f aux;

  aux.x=radio;aux.y=0.0;aux.z=0.0;
  perfil1.push_back(aux);
  aux.x=0.0;aux.y=altura;aux.z=0.0;
  perfil1.push_back(aux);

  parametros(perfil1,m,1);
}

//************************************************************************
// Cilindro
//************************************************************************

_cilindro::_cilindro(float radio, float altura, int m){
  vector<_vertex3f> perfil1;
  _vertex3f aux;

  aux.x=radio;aux.y=-altura/2.0;aux.z=0.0;
  perfil1.push_back(aux);
  aux.x=radio;aux.y=altura/2.0;aux.z=0.0;
  perfil1.push_back(aux);
  parametros(perfil1,m,0);
}



//************************************************************************
// objeto articulado: Homerbot
//************************************************************************



_chasis::_chasis(){
  // perfil para un cilindro
  vector<_vertex3f> perfil;
  _vertex3f aux;
  aux.x=0.175;aux.y=0;aux.z=0.0;
  perfil.push_back(aux);
  aux.x=0.175;aux.y=1;aux.z=0.0;
  perfil.push_back(aux);
  cilindro.parametros(perfil,12,0);

  cilindro.ambiente_difusa = _vertex4f(0.5508, 0.2118, 0.066, 1.0); //coeficientes ambiente y difuso
  cilindro.especular = _vertex4f(0.580594, 0.223257, 0.0695701, 1.0);       //coeficiente especular
  cilindro.brillo = 51.2; 

  cuerpo.ambiente_difusa = _vertex4f(0.5508, 0.2118, 0.066, 1.0); //coeficientes ambiente y difuso
  cuerpo.especular = _vertex4f(0.580594, 0.223257, 0.0695701, 1.0);       //coeficiente especular
  cuerpo.brillo = 51.2; 

/*   Polished copper
float[] mat_ambient ={ 0.2295f, 0.08825f, 0.0275f, 1.0f };
float[] mat_diffuse ={0.5508f, 0.2118f, 0.066f, 1.0f };
float[] mat_specular ={0.580594f, 0.223257f, 0.0695701f, 1.0f };
float shine =51.2f ;
 */

/*  Cyan rubber
float[] mat_ambient ={ 0.0f,0.05f,0.05f,1.0f };
float[] mat_diffuse ={0.4f,0.5f,0.5f,1.0f };
float[] mat_specular ={0.04f,0.7f,0.7f,1.0f };
float shine = 10.0f;
 */
};

void _chasis::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float r3, float g3, float b3, float r4, float g4, float b4, float grosor)
{
  //Cuerpo
  glPushMatrix();
  glTranslatef(-0.005,0,-0.15);
  glScalef(2.02,2.01,0.7);
  cuerpo.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();
  //CilindroSuperior
  glPushMatrix();
  glTranslatef(1,1,0.02);
  glRotatef(90,0,0,1.0);
  cilindro.ambiente_difusa = _vertex4f(0.5508, 0.2118, 0.066, 1.0); //coeficientes ambiente y difuso
  cilindro.especular = _vertex4f(0.580594, 0.223257, 0.0695701, 1.0);       //coeficiente especular
  cilindro.brillo = 51.2; 
  cilindro.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();
  //Visor
  glPushMatrix();
  glTranslatef(0.7,0.9,0.175);
  glRotatef(90,0,0,1.0);
  glScalef(0.4,0.4,0.4);
  cilindro.ambiente_difusa = _vertex4f(0.4f,0.5f,0.5f,1.0f ); //coeficientes ambiente y difuso
  cilindro.especular = _vertex4f(0.04f,0.7f,0.7f,1.0f);       //coeficiente especular
  cilindro.brillo = 10.0;
  cilindro.draw(modo, r3, g3, b3, r4, g4, b4, grosor);
  glPopMatrix();
  //RuedaDelantera
  glPushMatrix();
  glPushMatrix();
  glTranslatef(0.52,0,0.175);
  glRotatef(90,0,0,1.0);
  glScalef(1.1,0.04,1.1);
  cilindro.ambiente_difusa = _vertex4f(0.5508, 0.2118, 0.066, 1.0); //coeficientes ambiente y difuso
  cilindro.especular = _vertex4f(0.580594, 0.223257, 0.0695701, 1.0);       //coeficiente especular
  cilindro.brillo = 51.2; 
  cilindro.draw(modo, r3, g3, b3, r4, g4, b4, grosor);
  glPopMatrix();
  //Rueda1
  glPushMatrix();
  glPushMatrix();
  glTranslatef(0,0,0);
  glRotatef(90,0,0,1.0);
  glScalef(1.1,0.04,1.1);
  cilindro.ambiente_difusa = _vertex4f(0.5508, 0.2118, 0.066, 1.0); //coeficientes ambiente y difuso
  cilindro.especular = _vertex4f(0.580594, 0.223257, 0.0695701, 1.0);       //coeficiente especular
  cilindro.brillo = 51.2; 
  cilindro.draw(modo, r3, g3, b3, r4, g4, b4, grosor);
  glPopMatrix();
  //Rueda2
  glPushMatrix();
  glPushMatrix();
  glTranslatef(1.04,0,0);
  glRotatef(90,0,0,1.0);
  glScalef(1.1,0.04,1.1);
  cilindro.ambiente_difusa = _vertex4f(0.5508, 0.2118, 0.066, 1.0); //coeficientes ambiente y difuso
  cilindro.especular = _vertex4f(0.580594, 0.223257, 0.0695701, 1.0);       //coeficiente especular
  cilindro.brillo = 51.2; 
  cilindro.draw(modo, r3, g3, b3, r4, g4, b4, grosor);
  glPopMatrix();
  //antena1
  glPushMatrix();
  glTranslatef(0.45,1.2,0);
  glRotatef(45,0,0,1);
  glScalef(0.04,0.4,0.04);
  cilindro.ambiente_difusa = _vertex4f(0.5508, 0.2118, 0.066, 1.0); //coeficientes ambiente y difuso
  cilindro.especular = _vertex4f(0.580594, 0.223257, 0.0695701, 1.0);       //coeficiente especular
  cilindro.brillo = 51.2; 
  cilindro.draw(modo, r3, g3, b3, r4, g4, b4, grosor);
  glPopMatrix();
  //antena2
  glPushMatrix();
  glTranslatef(0.6,1.2,0);
  glRotatef(-45,0,0,1);
  glScalef(0.04,0.4,0.04);
  cilindro.ambiente_difusa = _vertex4f(0.5508, 0.2118, 0.066, 1.0); //coeficientes ambiente y difuso
  cilindro.especular = _vertex4f(0.580594, 0.223257, 0.0695701, 1.0);       //coeficiente especular
  cilindro.brillo = 51.2; 
  cilindro.draw(modo, r3, g3, b3, r4, g4, b4, grosor);
  glPopMatrix();
};

//************************************************************************

_brazoMazo::_brazoMazo(){
  vector<_vertex3f> perfil2;
  _vertex3f aux1;
  aux1.x=0.12;aux1.y=0;aux1.z=0.0;
  perfil2.push_back(aux1);
  aux1.x=0.12;aux1.y=0.3;aux1.z=0.0;
  perfil2.push_back(aux1);
  mazo.parametros(perfil2,12,0);

  //Codo
  vector<_vertex3f> perfil1;
  _vertex3f aux;
  int i;
  for(i=1;i<16;i++){
    aux.x=0.15*cos(M_PI*i/16-M_PI/2.0);
    aux.y=0.15*sin(M_PI*i/16-M_PI/2.0);
    aux.z=0.0;
    perfil1.push_back(aux);
  }

  codo.parametros(perfil1,16,2);

  moviCodo=0;

  codo.ambiente_difusa = _vertex4f(0.25f, 0.25f, 0.25f, 1.0f); //coeficientes ambiente y difuso
  codo.especular = _vertex4f(0.774597f, 0.774597f, 0.774597f, 1.0f );       //coeficiente especular
  codo.brillo = 76.8; 

  
/*   //Gold
float[] mat_ambient ={ 0.24725f, 0.1995f, 0.0745f, 1.0f };
float[] mat_diffuse ={0.75164f, 0.60648f, 0.22648f, 1.0f };
float[] mat_specular ={0.628281f, 0.555802f, 0.366065f, 1.0f };
float shine =51.2f ;
    //Bronze
float[] mat_ambient ={ 0.2125f, 0.1275f, 0.054f, 1.0f };
float[] mat_diffuse ={ 0.714f, 0.4284f, 0.18144f, 1.0f };
float[] mat_specular ={ 0.393548f, 0.271906f, 0.166721f, 1.0f };
float shine = 25.6f;

 */ 

};

void _brazoMazo::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float r3, float g3, float b3, float r4, float g4, float b4, float r5, float g5, float b5, float r6, float g6, float b6, float grosor)
{
  //Antebrazo
  glPushMatrix();
  glTranslatef(0,-0.2,0);
  glRotatef(-45,0,0,1);
  mazo.ambiente_difusa = _vertex4f(0.75164f, 0.60648f, 0.22648f, 1.0f); //coeficientes ambiente y difuso
  mazo.especular = _vertex4f(0.628281f, 0.555802f, 0.366065f, 1.0f);       //coeficiente especular
  mazo.brillo = 51.2;
  mazo.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();
  //Codo
  glPushMatrix();
  glRotatef(-moviCodo,1,0,0);
  glTranslatef(0,-0.2,0);
  glRotatef(45,0,0,1);
  codo.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();
  //movimiento del codo
  glPushMatrix();
  //glRotatef(-moviCodo,1,0,0);
    //brazo
    glPushMatrix();
    glRotatef(-moviCodo,1,0,0);
    glTranslatef(0,-0.5,0);
    mazo.ambiente_difusa = _vertex4f(0.75164f, 0.60648f, 0.22648f, 1.0f); //coeficientes ambiente y difuso
    mazo.especular = _vertex4f(0.628281f, 0.555802f, 0.366065f, 1.0f);       //coeficiente especular
    mazo.brillo = 51.2;
    mazo.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
    glPopMatrix();
    //mano
    glPushMatrix();
    glRotatef(-moviCodo,1,0,0);
    glTranslatef(0,-0.595,-0.075);
    glScalef(0.9,0.9,0.5);
    glRotatef(90,1,0,0);
    mazo.ambiente_difusa = _vertex4f(0.25f, 0.25f, 0.25f, 1.0f); //coeficientes ambiente y difuso
    mazo.especular = _vertex4f(0.774597f, 0.774597f, 0.774597f, 1.0f );       //coeficiente especular
    mazo.brillo = 76.8; 
    mazo.draw(modo, r3, g3, b3, r4, g4, b4, grosor);
    glPopMatrix();
    //mazo
    glPushMatrix();
    glRotatef(-moviCodo,1,0,0);
    glTranslatef(0,-0.75,0.4);
    mazo.ambiente_difusa = _vertex4f( 0.714f, 0.4284f, 0.18144f, 1.0f); //coeficientes ambiente y difuso
    mazo.especular = _vertex4f(0.393548f, 0.271906f, 0.166721f, 1.0f);       //coeficiente especular
    mazo.brillo = 25.6;
    mazo.draw(modo, r5, g5, b5, r6, g6, b6, grosor);
    glPopMatrix();
    //mango
    glPushMatrix();
    glRotatef(-moviCodo,1,0,0);
    glTranslatef(0,-0.6,-0.15);
    glRotatef(90,1,0,0);
    glScalef(0.2,1.7,0.2);
    mazo.ambiente_difusa = _vertex4f( 0.714f, 0.4284f, 0.18144f, 1.0f); //coeficientes ambiente y difuso
    mazo.especular = _vertex4f(0.393548f, 0.271906f, 0.166721f, 1.0f);       //coeficiente especular
    mazo.brillo = 25.6;
    mazo.draw(modo, r5, g5, b5, r6, g6, b6, grosor);
    glPopMatrix();
  glPopMatrix();

};

//************************************************************************

_brazoVacio::_brazoVacio(){

  //Brazo
  vector<_vertex3f> perfil2;
  _vertex3f aux1;
  aux1.x=0.12;aux1.y=0;aux1.z=0.0;
  perfil2.push_back(aux1);
  aux1.x=0.12;aux1.y=0.3;aux1.z=0.0;
  perfil2.push_back(aux1);
  vacio.parametros(perfil2,12,0);

  //Codo
  vector<_vertex3f> perfil1;
  _vertex3f aux;
  int i;
  for(i=1;i<16;i++){
    aux.x=0.15*cos(M_PI*i/16-M_PI/2.0);
    aux.y=0.15*sin(M_PI*i/16-M_PI/2.0);
    aux.z=0.0;
    perfil1.push_back(aux);
  }

  codo.parametros(perfil1,16,2);

  moviCodo=0;

  codo.ambiente_difusa = _vertex4f(0.25f, 0.25f, 0.25f, 1.0f); //coeficientes ambiente y difuso
  codo.especular = _vertex4f(0.774597f, 0.774597f, 0.774597f, 1.0f );       //coeficiente especular
  codo.brillo = 76.8; 

};

void _brazoVacio::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float r3, float g3, float b3, float r4, float g4, float b4, float grosor)
{
  //Antebrazo
  glPushMatrix();
  glTranslatef(0,-0.2,0);
  glRotatef(45,0,0,1);
  vacio.ambiente_difusa = _vertex4f(0.75164f, 0.60648f, 0.22648f, 1.0f); //coeficientes ambiente y difuso
  vacio.especular = _vertex4f(0.628281f, 0.555802f, 0.366065f, 1.0f);       //coeficiente especular
  vacio.brillo = 51.2;
  vacio.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();
  //brazo
  glPushMatrix();
  glRotatef(-moviCodo,1,0,0);
  glTranslatef(0,-0.5,0);
  vacio.ambiente_difusa = _vertex4f(0.75164f, 0.60648f, 0.22648f, 1.0f); //coeficientes ambiente y difuso
  vacio.especular = _vertex4f(0.628281f, 0.555802f, 0.366065f, 1.0f);       //coeficiente especular
  vacio.brillo = 51.2;
  vacio.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();
  //mano
  glPushMatrix();
  glRotatef(-moviCodo,1,0,0);
  glTranslatef(0,-0.595,-0.075);
  glScalef(0.9,0.9,0.5);
  glRotatef(90,1,0,0);
  vacio.ambiente_difusa = _vertex4f(0.25f, 0.25f, 0.25f, 1.0f); //coeficientes ambiente y difuso
  vacio.especular = _vertex4f(0.774597f, 0.774597f, 0.774597f, 1.0f );       //coeficiente especular
  vacio.brillo = 76.8; 
  vacio.draw(modo, r3, g3, b3, r4, g4, b4, grosor);
  glPopMatrix();
  //Codo
  glPushMatrix();
  glRotatef(-moviCodo,1,0,0);
  glTranslatef(0,-0.2,0);
  glRotatef(45,0,0,1);
  codo.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();

};

//************************************************************************

_detalles::_detalles(){
  
  vector<_vertex3f> perfil1;
  _vertex3f aux;
  int i;
  for(i=1;i<16;i++){
    aux.x=0.15*cos(M_PI*i/16-M_PI/2.0);
    aux.y=0.15*sin(M_PI*i/16-M_PI/2.0);
    aux.z=0.0;
    perfil1.push_back(aux);
  }

  detalle.parametros(perfil1,16,2);

  detalle.ambiente_difusa = _vertex4f(0.25f, 0.25f, 0.25f, 1.0f); //coeficientes ambiente y difuso
  detalle.especular = _vertex4f(0.774597f, 0.774597f, 0.774597f, 1.0f );       //coeficiente especular
  detalle.brillo = 76.8; 

  //Chrome
/* float[] mat_ambient ={0.25f, 0.25f, 0.25f, 1.0f  };
float[] mat_diffuse ={0.4f, 0.4f, 0.4f, 1.0f };
float[] mat_specular ={0.774597f, 0.774597f, 0.774597f, 1.0f };
float shine = 76.8f;
 */
};

void _detalles::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float r3, float g3, float b3, float r4, float g4, float b4, float grosor)
{

  //hombroIzqd
  glPushMatrix();
  glTranslatef(1.04,0.96,0);
  detalle.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();
  //hombroDcha
  glPushMatrix();
  glTranslatef(-0.04,0.96,0);
  detalle.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();
  //Cambio los valores de los colores para las distintas piezas:
  
  //Casco
  glPushMatrix();
  glTranslatef(0.52,1.15,0);
  glScalef(1.3,1,1);
  detalle.draw(modo, r3, g3, b3, r4, g4, b4, grosor);
  glPopMatrix();
  //tapacubos1
  glPushMatrix();
  glTranslatef(0.52,0.15,0.18);
  glScalef(1,1,1.3);
  detalle.draw(modo, r3, g3, b3, r4, g4, b4, grosor);
  glPopMatrix();
  //tapacubos2
  glPushMatrix();
  glTranslatef(0,0.15,0);
  glScalef(1,1,1.3);
  detalle.draw(modo, r3, g3, b3, r4, g4, b4, grosor);
  glPopMatrix();
  //tapacubos3
  glPushMatrix();
  glTranslatef(1,0.15,0);
  glScalef(1,1,1.3);
  detalle.draw(modo, r3, g3, b3, r4, g4, b4, grosor);
  glPopMatrix();
  //antena1
  glPushMatrix();
  glTranslatef(0.15,1.5,0);
  glScalef(0.2,0.2,0.2);
  detalle.draw(modo, r3, g3, b3, r4, g4, b4, grosor);
  glPopMatrix();
  //antena2
  glPushMatrix();
  glTranslatef(0.9,1.5,0);
  glScalef(0.2,0.2,0.2);
  detalle.draw(modo, r3, g3, b3, r4, g4, b4, grosor);
  glPopMatrix();
};


//************************************************************************
_robotHomer::_robotHomer(){
  giroHombroM=0.0;
  giroHombroM_min=-15;
  giroHombroM_max=130;
  
  giroHombroV=0.0;
  giroHombroV_min=-15;
  giroHombroV_max=130;

  abrirHombroM=0.0;
  abrirHombroM_min=-10.0;
  abrirHombroM_max=8;

  abrirHombroV=0.0;
  abrirHombroV_min=-8.0;
  abrirHombroV_max=10;

  movCodo=0;
  movCodo_max=25;
  movCodo_min=-10;

//para la seleccion de color:
int c=100;
piezas=3;
color_pick[0]=0.0;
color_pick[1]=1.0;
color_pick[2]=0.0; 

for (int i=0;i<piezas;i++){
  activo[i]=0;
  color_selec[0][i]=color_selec[1][i]=color_selec[2][i]=c;
  c=c+20;
}
  
};

void _robotHomer::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{

  float r_p,g_p,b_p;

  r_p=color_pick[0];
  g_p=color_pick[1];
  b_p=color_pick[2];

  glPushMatrix();
  if(activo[0]==1)
    chasis.draw(modo, r_p,g_p,b_p, r_p,g_p,b_p, r_p,g_p,b_p, r_p,g_p,b_p, grosor);
  else
    chasis.draw(modo, r1, g1, b1, r2, g2, b2, 0, 0, 0, 0.2, 0.2, 0.2, grosor);
  glPopMatrix();

  brazoM.moviCodo=movCodo;
  glPushMatrix();
  glTranslatef(-0.3,0.9,0);
  glRotatef(-giroHombroM,1,0,0);
  glRotatef(-abrirHombroM,0,0,1);
  if(activo[1]==1)
    brazoM.draw(modo, r_p,g_p,b_p, r_p,g_p,b_p, r_p,g_p,b_p, r_p,g_p,b_p, r_p,g_p,b_p, r_p,g_p,b_p, grosor);
  else
    brazoM.draw(modo, r1, g1, b1, r2, g2, b2, 0.7353, 0.7078, 0.7039, 0.7653, 0.7378, 0.7339, 0.545, 0.2705, 0.0745, 0.595, 0.3105, 0.145, grosor);
  glPopMatrix();

  brazoV.moviCodo=movCodo;
  glPushMatrix();
  glTranslatef(1.3,0.9,0);
  glRotatef(-giroHombroV,1,0,0);
  glRotatef(-abrirHombroV,0,0,1);
  if(activo[2]==1)
    brazoV.draw(modo, r_p,g_p,b_p, r_p,g_p,b_p, r_p,g_p,b_p, r_p,g_p,b_p, grosor);
  else
    brazoV.draw(modo, r1, g1, b1, r2, g2, b2, 0.7353, 0.7078, 0.7039, 0.7653, 0.7378, 0.7339, grosor);
  glPopMatrix();

  //Pintare los objetos de esta clase junto a los del chasis
  glPushMatrix();
  if(activo[0]==1)
    detalles.draw(modo, r_p,g_p,b_p, r_p,g_p,b_p, r_p,g_p,b_p, r_p,g_p,b_p, grosor);
  else
    detalles.draw(modo, r1, g1, b1, r2, g2, b2, 0.7353, 0.7078, 0.7039, 0.7653, 0.7378, 0.7339,  grosor);
  glPopMatrix();
};

void _robotHomer::seleccion(){  //Funcion para seleccionar color
  int c;
  c=color_selec[0][0];
  glPushMatrix();
  chasis.draw(SELECT, c, c, c, c, c, c, c, c, c, c, c, c, 1);
  detalles.draw(SELECT, c, c, c, c, c, c, c, c, c, c, c, c, 1);
  glPopMatrix();

  c=color_selec[0][1];
  brazoM.moviCodo=movCodo;
  glPushMatrix();
  glTranslatef(-0.3,0.9,0);
  glRotatef(-giroHombroM,1,0,0);
  glRotatef(-abrirHombroM,0,0,1);
  brazoM.draw(SELECT, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, 1);
  glPopMatrix();

  c=color_selec[0][2];
  brazoV.moviCodo=movCodo;
  glPushMatrix();
  glTranslatef(1.3, 0.9, 0);
  glRotatef(-giroHombroV, 1, 0, 0);
  glRotatef(-abrirHombroV, 0, 0, 1);
  brazoV.draw(SELECT, c, c, c, c, c, c, c, c, c, c, c, c, 1);
  glPopMatrix();
  glPopMatrix();
}

