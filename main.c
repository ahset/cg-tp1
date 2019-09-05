#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

//TESTE2

typedef struct {
    float l, a, x, y; // l = largura, a = altura
} dimpos; // tudo que tem dimensões e/ou posição

dimpos mundo, barra, barraE, barraD, bola, acresce;
// barra é qualquer barra (criei pra dizer as dimensões delas
// e como são iguais, não faz diferença se é a da esquerda ou da direita)
// barraE é a barra da esquerda e barraD é a barra da direita
// acresce é a variável que muda de sinal quando a bola atinge
// as bordas do mundo

// vetor em que cada posição corresponde a uma tecla
// inicializado com valor falso para todas
// será usado para verificar se alguma tecla está pressionada
bool keyStates[256] = {false};

GLuint idTexturaFundo; // imagem do fundo
GLuint idTexturaBarra; // imagem da barra
GLuint idTexturaBola; // imagem da bola

GLuint carregaTextura(const char* arquivo) {
    GLuint idTextura = SOIL_load_OGL_texture(
                           arquivo,
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );

    if (idTextura == 0) {
        printf("Erro do SOIL: '%s'\n", SOIL_last_result());
    }

    return idTextura;
}

void inicializa() {
    glClearColor(1, 1, 1, 1);

    // habilita mesclagem de cores, para termos suporte a texturas
    // com transparência
    glEnable(GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    idTexturaFundo = carregaTextura("background.png");
    idTexturaBarra = carregaTextura("barra.png");
    idTexturaBola = carregaTextura("ufo.png");

    // valores iniciais das dimensões e posições dos objetos da cena
    // as dimensões não mudam, as posições vão mudar em outro lugar

    mundo.l = 1000;
    mundo.a = 615;

    bola.l = 50;
    bola.a = 50;
    bola.x = mundo.l/2;
    bola.y = mundo.a/2;
    
    barra.l = mundo.l/4;
    barra.a = mundo.a/20;
    barraE.x = (mundo.l/15)*14;
    barraE.y = mundo.a/2;
    barraD.x = mundo.l/15;
    barraD.y = mundo.a/2;

    // eu tinha pensado em fazer a posição da bolinha mudar
    // com um valor inicial aleatório, pra não ser sempre a mesma coisa
    // srand(time(0));

    acresce.x = 1; // rand()%10+1;
    acresce.y = 1; // rand()%10+1;
}

void desenha() {
    
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f (1, 1, 1);

    // habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);

    // começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaFundo);
    glBegin(GL_TRIANGLE_FAN);
        // associamos um canto da textura para cada vértice
        glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
        glTexCoord2f(1, 0); glVertex3f(mundo.l, 0, 0);
        glTexCoord2f(1, 1); glVertex3f(mundo.l, mundo.a, 0);
        glTexCoord2f(0, 1); glVertex3f(0, mundo.a, 0);
    glEnd();

    // se alguma das teclas de mudança de posição das barras
    // estiver pressionada (true) e a barra não tiver atingido
    // a borda da "mesa" de ping pong, continua mudando a posição
    if(keyStates[48]==true && barraD.y>50+barra.l/2)
        barraD.y-=5;
    if(keyStates[49]==true && barraD.y<mundo.a-50-barra.l/2)
        barraD.y+=5;
    if(keyStates[115]==true && barraE.y>50+barra.l/2)
        barraE.y-=5;
    if(keyStates[119]==true && barraE.y<mundo.a-50-barra.l/2)
        barraE.y+=5;

    glBindTexture(GL_TEXTURE_2D, idTexturaBarra);
    glPushMatrix();
        glTranslatef((mundo.l/15)*14, barraD.y, 0);
        glBegin(GL_TRIANGLE_FAN);
            glTexCoord2f(0, 0); glVertex3f(-barra.a/2, -barra.l/2, 0);
            glTexCoord2f(1, 0); glVertex3f(barra.a/2, -barra.l/2, 0);
            glTexCoord2f(1, 1); glVertex3f(barra.a/2, barra.l/2, 0);
            glTexCoord2f(0, 1); glVertex3f(-barra.a/2, barra.l/2, 0);
        glEnd();
    glPopMatrix();
    
    glBindTexture(GL_TEXTURE_2D, idTexturaBarra);
    glPushMatrix();
        glTranslatef(mundo.l/15, barraE.y, 0);
        glBegin(GL_TRIANGLE_FAN);
            glTexCoord2f(0, 0); glVertex3f(-barra.a/2, -barra.l/2, 0);
            glTexCoord2f(1, 0); glVertex3f(barra.a/2, -barra.l/2, 0);
            glTexCoord2f(1, 1); glVertex3f(barra.a/2, barra.l/2, 0);
            glTexCoord2f(0, 1); glVertex3f(-barra.a/2, barra.l/2, 0);
        glEnd();
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, idTexturaBola);
    glPushMatrix();
        glTranslatef(bola.x, bola.y, 0);
        glBegin(GL_TRIANGLE_FAN);
            glTexCoord2f(0, 0); glVertex3f(-bola.a/2, -bola.l/2, 0);
            glTexCoord2f(1, 0); glVertex3f(bola.a/2, -bola.l/2, 0);
            glTexCoord2f(1, 1); glVertex3f(bola.a/2, bola.l/2, 0);
            glTexCoord2f(0, 1); glVertex3f(-bola.a/2, bola.l/2, 0);
        glEnd();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    glutSwapBuffers();
}

void redimensiona(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, mundo.l, 0, mundo.a, -1, 1);

    float razaoAspectoJanela = ((float)width)/height;
    float razaoAspectoMundo = ((float) mundo.l)/ mundo.a;
    // se a janela está menos larga do que o mundo (16:9)...
    if (razaoAspectoJanela < razaoAspectoMundo) {
        // vamos colocar barras verticais (acima e abaixo)
        float hViewport = width / razaoAspectoMundo;
        float yViewport = (height - hViewport)/2;
        glViewport(0, yViewport, width, hViewport);
    }
    // se a janela está mais larga (achatada) do que o mundo (16:9)...
    else if (razaoAspectoJanela > razaoAspectoMundo) {
        // vamos colocar barras horizontais (esquerda e direita)
        float wViewport = ((float)height) * razaoAspectoMundo;
        float xViewport = (width - wViewport)/2;
        glViewport(xViewport, 0, wViewport, height);
    } else {
        glViewport(0, 0, width, height);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void teclado(unsigned char key, int x, int y) {
    switch (key) {
        case 27:
            exit(0);
        // se a tecla é pressionada, altera o valor
        // para true
        case 48: // tecla '0'
            keyStates[key] = true;
            break;
        case 49: // tecla '1'
            keyStates[key] = true;
            break;
        case 115: // tecla 's'
            keyStates[key] = true;
            break;
        case 119: // tecla 'w'
            keyStates[key] = true;
            break;            
    }
}

void soltaTecla (unsigned char key, int x, int y){
    switch (key) {
        // se a tecla é solta, volta o valor
        // para false
        case 48: // tecla '0'
            keyStates[key] = false;
            break;
        case 49: // tecla '1'
            keyStates[key] = false;
            break;
        case 115: // tecla 's'
            keyStates[key] = false;
            break;
        case 119: // tecla 'w'
            keyStates[key] = false;
            break;            
    }
}

void atualizaCena(int periodo) {

    // muda a direção da bola quando ela atinge alguma borda
    if(bola.x >= mundo.l || bola.x <= 0)
        acresce.x *= -1;
    if(bola.y >= mundo.a || bola.y <= 0)
        acresce.y *= -1;
    
    // faz a bola mudar de posição
    bola.x += acresce.x;
    bola.y += acresce.y;
 
    // pede ao GLUT para redesenhar a tela, assim que possível
    glutPostRedisplay();

    // se registra novamente, para que fique sempre sendo chamada
    glutTimerFunc(periodo, atualizaCena, periodo);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1000, 615);
    glutInitWindowPosition(100, 100);

    glutCreateWindow("Space Ping Pong");
    inicializa();

    glutDisplayFunc(desenha);    
    glutReshapeFunc(redimensiona);

    glutKeyboardFunc(teclado);
    glutKeyboardUpFunc(soltaTecla);

    // O prof. tinha colocado 33 (pra ser 30 FPS)
    // em vez de 1 mas achei que tava devagar
    glutTimerFunc(0, atualizaCena, 1);

    glutMainLoop();

    return 0;
}