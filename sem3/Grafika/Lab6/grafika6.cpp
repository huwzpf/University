#include <iostream>

#define GL3_PROTOTYPES 1
#include <glew.h>
#include <SDL.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#define WIDTH 800
#define HEIGHT 600

//------------------------------------------------------------------------------------------------------------------------------
//Globalne zmienne 

// kod zrodlowy shadera wierzcholkow
const GLchar* vertexSource =
"#version 150 core\n"
"in vec3 position;"
"in vec3 color;"
"in vec2 texcoord;"
"out vec3 Color;"
"out vec2 Texcoord;"
"uniform mat4 transformMatrix;"
"void main()"
"{"
"    gl_Position =  transformMatrix * vec4(position, 1.0);"
"    Color = color;"
"	 Texcoord = texcoord;"
"}";

// kod zrodlowy shadera fragmentow
const GLchar* fragmentSource =
"#version 150 core\n"
"in vec3 Color;"
"in vec2 Texcoord;"
"out vec4 outColor;"
"uniform sampler2D tex;"
"void main()"
"{"
"    outColor = texture(tex, Texcoord) * vec4(Color, 1.0);"
"}";


//------------------------------------------------------------------------------------------------------------------------------

GLint posAttrib, colAttrib, texAttrib;					//wskazniki atrybutow wierzcholkow
GLuint vertexShader, fragmentShader, shaderProgram;		//shadery


GLuint vao[5], vbo[14], ebo, tex[4];	// identyfikatory poszczegolnych obiektow (obiekty tablic wierzcholkow, buforow wierzcholkow, elementow, tekstury)

//------------------------------------------------------------------------------------------------------------------------------

GLfloat ver_floor[] = { //wspolrzedne wierzcholkow podlogi
	-5.0f,  0.0f, -5.0f,
	5.0f,  0.0f, -5.0f,
	5.0f,  0.0f,  5.0f,
	-5.0f,  0.0f,  5.0f,
};

GLfloat ver_wall1[] = { //wspolrzedne wierzcholkow sciany
	5.0f,  0.0f, 5.0f,
	5.0f,  2.0f, 5.0f,
	5.0f,  2.0f, -5.0f,
	5.0f,  0.0f, -5.0f,
};

GLfloat ver_wall2[] = { //wspolrzedne wierzcholkow sciany
	-5.0f,  0.0f, -3.0f,
	-5.0f,  2.0f, -3.0f,
	0.0f,  2.0f, -3.0f,
	0.0f,  0.0f, -3.0f,
};

GLfloat ver_wall3[] = { //wspolrzedne wierzcholkow sciany
	-3.0f,  0.0f, 4.0f,
	-3.0f,  2.0f, 4.0f,
	-3.0f,  2.0f, -1.0f,
	-3.0f,  0.0f, -1.0f,
};

GLfloat col_floor[] = { //kolory wierzcholkow podlogi
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
};

GLfloat tex_floor[] = {	//wspolrzedne tekstury dla podlogi, 
						//jesli wykraczaja poza przedzial <0,1> nastapi nakladanie zalezne od parametrow ustawionych za pomoca funkcji glTexParameteri, np. GL_REPEAT
	0.0f, 0.0f,	// lewy dolny rog tekstury
	5.0f, 0.0f,
	5.0f, 5.0f,
	0.0f, 5.0f, // prawy gorny rog tekstury
};

GLfloat tex_wall1[] = {

	0.0f, 0.0f,
	5.0f, 0.0f,
	5.0f, 5.0f,
	0.0f, 5.0f,
};
GLfloat tex_wall2[] = {

	0.0f, 0.0f,
	5.0f, 0.0f,
	5.0f, 5.0f,
	0.0f, 5.0f,
};
GLfloat tex_wall3[] = {

	0.0f, 0.0f,
	5.0f, 0.0f,
	5.0f, 5.0f,
	0.0f, 5.0f,
};

GLuint elements[] = { // pogrupowanie wierzcholkow w trojkaty, wykorzystane zarowno dla sciany jaki dla podlogi
	0,1,2,
	2,3,0,
};

float pixels_floor[] = { //tekstura o wymiarach 2x2; dla kazdego punktu okreslone skladowe RGB koloru 
	1.0f, 1.0f, 1.0f,   0.1f, 0.1f, 0.1f,
	0.1f, 0.1f, 0.1f,   1.0f, 1.0f, 1.0f,
};


float pixels_wall1[] = {			//tekstura o wymiarach 4x4; dla kazdego punktu okreslone skladowe RGB koloru 
	0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f, 0.0f,

};
float pixels_wall2[] = {			//tekstura o wymiarach 4x4; dla kazdego punktu okreslone skladowe RGB koloru 
	1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f, 1.0f,

};
float pixels_wall3[] = {			//tekstura o wymiarach 4x4; dla kazdego punktu okreslone skladowe RGB koloru 
	0.0f, 0.0f, 0.1f,   1.0f, 0.0f, 1.0f,   1.0f, 0.0f, 1.0f,   1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.1f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.1f,   1.0f, 0.0f, 1.0f,   1.0f, 0.0f, 1.0f,   1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.1f,   1.0f, 0.0f, 1.0f,   1.0f, 0.0f, 1.0f,   1.0f, 0.0f, 0.0f,

};

GLfloat ver_triangle[] = { //wspolrzedne wierzcholkow trojkata okreslajacego polozenie obserwatora (kamery)
	-0.2f,  0.0f, 0.0f,
	 0.2f,  0.0f, 0.0f,
	 0.0f,  0.0f, -1.0f,
};

GLfloat col_triangle[] = { //kolory wierzcholkow trojkata okreslajacego polozenie obserwatora (kamery)
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
};

GLfloat col_wall1[] = { //kolory wierzcholkow sciany
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
};
GLfloat col_wall2[] = { //kolory wierzcholkow sciany
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
};
GLfloat col_wall3[] = { //kolory wierzcholkow sciany
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
};
struct Point {
	float x;
	float y;
};


// jaki� algorytm wykrywaj�cy przeci�cie dw�ch linii z internetu
bool ccw(Point A, Point B, Point C) {
	return ((C.y - A.y) * (B.x - A.x) > (B.y - A.y) * (C.x - A.x));
}

bool intersection(Point A, Point B, Point C, Point D) {
	return (ccw(A, C, D) != ccw(B, C, D) && ccw(A, B, C) != ccw(A, B, D));
}
// kolizj� wykrywamy gdy linia ��cz�ca obecn� pozycj� z now� pozycj� przecina si� z kt�r�� z linii tworz�cych �ciany 
bool check_collision_with_wall(Point A, Point B, float* wall) {
	return intersection(A, B, { wall[0], wall[2] }, { wall[9], wall[11] });
}
// sprawdzamy kolizj� z ka�d� ze �cian
bool check_collision(Point A, Point B) {

	return check_collision_with_wall(A, B, ver_wall1)
		|| check_collision_with_wall(A, B, ver_wall2)
		|| check_collision_with_wall(A, B, ver_wall3);
}



float x_on_circ(float x, float angle, float r, float angle_1) {
	return x + r * (cos(angle + glm::radians(angle_1 - 45.0)) - sin(angle + glm::radians(angle_1 - 45.0)));
}
float y_on_circ(float y, float angle, float r, float angle_1) {
	return y + r * (sin(angle + glm::radians(angle_1 - 45.0)) + cos(angle + glm::radians(angle_1 - 45.0)));
}



//------------------------------------------------------------------------------------------------------------------------------

int init_shaders()
{
	// tworzenie i kompilacja shadera wierzcholkow
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	GLint status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		std::cout << "Kompilacja shadera wierzcholkow NIE powiodla sie!\n";
		return 0;
	}

	// tworzenie i kompilacja shadera fragmentow
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		std::cout << "Kompilacja shadera fragmentow NIE powiodla sie!\n";
		return 0;
	}

	// dolaczenie programow przetwarzajacych wierzcholki i fragmenty do programu cieniujacego
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	// wskazniki atrybutow wierzcholkow
	posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
	glEnableVertexAttribArray(texAttrib);

	return 1;

}

//------------------------------------------------------------------------------------------------------------------------------

void create_objects()
{
	// generowanie obiektow
	glGenVertexArrays(5, vao);  // obiekt tablicy wierzcholkow, dla kazdego obiektu (np. dla podlogi) mamy jedna tablice
	glGenBuffers(14, vbo);		// obiekty buforow wierzcholkow, dla kazdego typu atrubutow kazdego obiektu mamy jeden bufor (np. bufor dla kolorow podlogi, bufor dla wspolrzednych podlogi itd.)
	glGenBuffers(1, &ebo);		// obiekt bufora elementow (ten sam bufor mozna wykorzystac zarowno dla podlogi jak i sciany)

	// podloga vao[0]

	glBindVertexArray(vao[0]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);	// bufor wspolrzednych wierzcholkow podlogi
	glBufferData(GL_ARRAY_BUFFER, sizeof(ver_floor), ver_floor, GL_STATIC_DRAW);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posAttrib);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);	// bufor kolorow wierzcholkow podlogi
	glBufferData(GL_ARRAY_BUFFER, sizeof(col_floor), col_floor, GL_STATIC_DRAW);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(colAttrib);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);	// bufor wspolrzednych tekstury podlogi
	glBufferData(GL_ARRAY_BUFFER, sizeof(tex_floor), tex_floor, GL_STATIC_DRAW);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(texAttrib);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	// trojkat vao[1]

	glBindVertexArray(vao[1]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);	// bufor wspolrzednych wierzcholkow trojkata
	glBufferData(GL_ARRAY_BUFFER, sizeof(ver_triangle), ver_triangle, GL_STATIC_DRAW);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posAttrib);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);	// bufor kolorow wierzcholkow trojkata
	glBufferData(GL_ARRAY_BUFFER, sizeof(col_triangle), col_triangle, GL_STATIC_DRAW);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(colAttrib);

	// sciana vao[2]

	glBindVertexArray(vao[2]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);	// bufor wspolrzednych wierzcholkow sciany
	glBufferData(GL_ARRAY_BUFFER, sizeof(ver_wall1), ver_wall1, GL_STATIC_DRAW);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posAttrib);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);	// bufor kolorow wierzcholkow sciany
	glBufferData(GL_ARRAY_BUFFER, sizeof(col_wall1), col_wall1, GL_STATIC_DRAW);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(colAttrib);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[7]);	// bufor wspolrzednych tekstury sciany
	glBufferData(GL_ARRAY_BUFFER, sizeof(tex_wall1), tex_wall1, GL_STATIC_DRAW);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(texAttrib);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	glBindVertexArray(vao[3]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[8]);	// bufor wspolrzednych wierzcholkow sciany
	glBufferData(GL_ARRAY_BUFFER, sizeof(ver_wall2), ver_wall2, GL_STATIC_DRAW);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posAttrib);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[9]);	// bufor kolorow wierzcholkow sciany
	glBufferData(GL_ARRAY_BUFFER, sizeof(col_wall2), col_wall2, GL_STATIC_DRAW);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(colAttrib);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[10]);	// bufor wspolrzednych tekstury sciany
	glBufferData(GL_ARRAY_BUFFER, sizeof(tex_wall2), tex_wall2, GL_STATIC_DRAW);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(texAttrib);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);


	glBindVertexArray(vao[4]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[11]);	// bufor wspolrzednych wierzcholkow sciany
	glBufferData(GL_ARRAY_BUFFER, sizeof(ver_wall3), ver_wall3, GL_STATIC_DRAW);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posAttrib);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[12]);	// bufor kolorow wierzcholkow sciany
	glBufferData(GL_ARRAY_BUFFER, sizeof(col_wall3), col_wall3, GL_STATIC_DRAW);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(colAttrib);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[13]);	// bufor wspolrzednych tekstury sciany
	glBufferData(GL_ARRAY_BUFFER, sizeof(tex_wall3), tex_wall3, GL_STATIC_DRAW);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(texAttrib);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);






}

//------------------------------------------------------------------------------------------------------------------------------

void configure_texture()
{
	glGenTextures(1, &tex[0]);		// obiekt tekstury
	glBindTexture(GL_TEXTURE_2D, tex[0]);		// powiazanie tekstury z obiektem (wybor tekstury)

	// ustawienia parametrow tekstury
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// sposob nakladania tekstury
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // sposob filtrowania tekstury
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_FLOAT, pixels_floor); // ladowanie do tekstury tablicy pikseli

	glGenTextures(1, &tex[1]);
	glBindTexture(GL_TEXTURE_2D, tex[1]);		// powiazanie tekstury z obiektem (wybor tekstury)

												// ustawienia parametrow tekstury
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// sposob nakladania tekstury
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // sposob filtrowania tekstury
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 4, 4, 0, GL_RGB, GL_FLOAT, pixels_wall1); // ladowanie do tekstury tablicy pikseli

	glGenTextures(1, &tex[2]);
	glBindTexture(GL_TEXTURE_2D, tex[2]);		// powiazanie tekstury z obiektem (wybor tekstury)

												// ustawienia parametrow tekstury
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// sposob nakladania tekstury
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // sposob filtrowania tekstury
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 4, 4, 0, GL_RGB, GL_FLOAT, pixels_wall2); // ladowanie do tekstury tablicy pikseli

	glGenTextures(1, &tex[3]);
	glBindTexture(GL_TEXTURE_2D, tex[3]);		// powiazanie tekstury z obiektem (wybor tekstury)

												// ustawienia parametrow tekstury
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// sposob nakladania tekstury
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // sposob filtrowania tekstury
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 4, 4, 0, GL_RGB, GL_FLOAT, pixels_wall3); // ladowanie do tekstury tablicy pikseli

}

//------------------------------------------------------------------------------------------------------------------------------

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_Window* window = SDL_CreateWindow("OpenGL", 100, 100, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(window);
	SDL_Event windowEvent;

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "ERROR" << std::endl;
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL); // GL_ALWAYS)

	if (!init_shaders())
		return 0;

	create_objects();

	configure_texture();


	glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 1.0f, 1.0f, 20.0f);		 //macierz rzutowania perspektywicznego
	glm::mat4 viewMatrix;  //macierz widoku
	glm::mat4 transformMatrix; //macierz wynikowa

	GLint transformMatrixUniformLocation = glGetUniformLocation(shaderProgram, "transformMatrix");

	int top_view = false; //zmienna okreslajaca czy patrzymy na scene z gory

	glm::vec3 position = glm::vec3(0.0f, 1.0f, 0.0f); //poczatkowe polozenie kamery
	glm::vec3 direction;

	float angle = glm::radians(-90.0); // pocz�tkowe skierowanie kamery
	float dd = 0.1, dp = 0.1;
	glm::vec3 position_temp;
	while (true)
	{
		if (SDL_PollEvent(&windowEvent))
		{
			if (windowEvent.type == SDL_KEYUP &&
				windowEvent.key.keysym.sym == SDLK_ESCAPE) break;
			if (windowEvent.type == SDL_QUIT) break;


			if (windowEvent.type == SDL_KEYDOWN)
			{
				switch (windowEvent.key.keysym.sym)
				{
				case SDLK_SPACE:
					top_view = !top_view;
					break;

				case SDLK_DOWN:
					// position temp zmniejszamy (x o cos, z o sin)
					// // �eby wizualnie nie wchodzi� w �cian� - bez tego kolizja by�aby wykryta kiedy koordynaty �rodka
					// wejd� w �cian�, a wtedy widzimy juz to co jest za �cian�. 
					position_temp = position - direction * glm::vec3(dp);
					if (!check_collision({ position.x, position.z }, { position_temp.x - cos(angle), position_temp.z - sin(angle) }))
						position = position_temp;



					break;
				case SDLK_UP:
					// position temp zwi�kszamy (x o cos, z o sin)
					// // �eby wizualnie nie wchodzi� w �cian� - bez tego kolizja by�aby wykryta kiedy koordynaty �rodka
					// wejd� w �cian�, a wtedy widzimy juz to co jest za �cian�. 
					position_temp = position + direction * glm::vec3(dp);
					if (!check_collision({ position.x, position.z }, { position_temp.x + cos(angle), position_temp.z + sin(angle) }))
						position = position_temp;

					break;

				case SDLK_LEFT:
					angle -= dd;
					break;
				case SDLK_RIGHT:
					angle += dd;
					break;

				}

			}

		}

		// strza�k� rysujemy jako tr�jk�t wpisany w okr�g o �rodku w pozycji kamery 
		float arrow_size = 0.8;
		ver_triangle[6] = x_on_circ(position.x, angle, arrow_size, 0);
		ver_triangle[8] = y_on_circ(position.z, angle, arrow_size, 0);
		ver_triangle[0] = x_on_circ(position.x, angle, arrow_size, (360 / 5) * 2);
		ver_triangle[2] = y_on_circ(position.z, angle, arrow_size, (360 / 5) * 2);
		ver_triangle[3] = x_on_circ(position.x, angle, arrow_size, (360 / 5) * 3);
		ver_triangle[5] = y_on_circ(position.z, angle, arrow_size, (360 / 5) * 3);




		// aktualizujemy informacje dla strza�ki 
		glBindVertexArray(vao[1]);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);	// bufor wspolrzednych wierzcholkow trojkata
		glBufferData(GL_ARRAY_BUFFER, sizeof(ver_triangle), ver_triangle, GL_STATIC_DRAW);
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(posAttrib);


		direction = glm::vec3(cos(angle), 0.0f, sin(angle)); // jakas magia
		if (top_view) //patrzymy z gory
			viewMatrix = glm::lookAt(glm::vec3(0.0f, 20.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, -1.0f));

		else //patrzymy z miejsca, w ktorym jest obserwator 
			viewMatrix = glm::lookAt(position, position + direction, glm::vec3(0.0f, 1.0f, 0.0f));


		transformMatrix = projectionMatrix * viewMatrix;				// wynikowa macierz transformacji
		glUniformMatrix4fv(transformMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));	// macierz jako wej�ciowa zmienna dla shadera wierzcholkow


		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);	// szare tlo
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		glBindTexture(GL_TEXTURE_2D, tex[0]);
		glBindVertexArray(vao[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);	//rysujemy podloge

		glBindVertexArray(vao[2]);
		glBindTexture(GL_TEXTURE_2D, tex[1]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);	//rysujemy sciane



		glBindVertexArray(vao[3]);
		glBindTexture(GL_TEXTURE_2D, tex[2]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);	//rysujemy sciane

		glBindVertexArray(vao[4]);
		glBindTexture(GL_TEXTURE_2D, tex[3]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);	//rysujemy sciane


		glBindVertexArray(vao[1]);
		glDisable(GL_TEXTURE_2D);
		glDrawArrays(GL_TRIANGLES, 0, 3);	//rysujemy trojkat przedstawiajacy polozenie kamery

		SDL_GL_SwapWindow(window);
	}

	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	glDeleteBuffers(14, vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteTextures(4, tex);
	glDeleteVertexArrays(5, vao);

	SDL_GL_DeleteContext(context);
	SDL_Quit();

	return 0;
}