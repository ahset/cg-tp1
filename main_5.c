#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

typedef struct {
    GLfloat l, a, x, y; // l = largura, a = altura
    bool opt;
} dimpos; // tudo que tem dimensões e/ou posição

dimpos mundo, barraE, barraD, bola, buraco1, buraco2, acrescebola,
acresceburaco1, acresceburaco2, bpause, optyes, optno, foguete, 
borda_mundo, menuplay, auxFundo, intro, menu, restartbut, exitbut,
placarE, placarD, pw, coroa;
// barraE é a barra da esquerda e barraD é a barra da direita
// acrescebola é a variável que muda de sinal quando a bola atinge
// as bordas do mundo
bool keyStates[256];
GLfloat ssstop=1.9;
GLint tempo_colisao = 0;
GLint placarE_atual = 0, placarD_atual = 0;
// vetor em que cada posição corresponde a uma tecla
// inicializado com valor falso para todas
// será usado para verificar se alguma tecla está pressionada
GLuint idTexturaFundo; // imagem do fundo
GLuint idTexturaBarra; // imagem da barra
GLuint idTexturaBarra2; // imagem da outra barra
GLuint idTexturaBola; // imagem da bola
GLuint idTexturaP1W; // imagem "player 1 wins"
GLuint idTexturaP2W; // imagem "player 2 wins"
GLuint idTexturaBuraco1; // imagem do buraco negro
GLuint idTexturaBuraco2;
GLuint idTexturaPause;
GLuint idTexturaRestart; // imagem "restart"
GLuint idTexturaExit; // imagem "exit"
GLuint idTexturaYes; // imagem do botão "yes"
GLuint idTexturaNo; // imagem do botão "no"
GLuint idTexturaYesOn; // imagem do botão "yes" quando o mouse passa nele
GLuint idTexturaNoOn; // imagem do botão "no" quando o mouse passa nele
GLuint idTexturaFoguete;
GLuint idTexturaIntro;
GLuint idTexturaMenu;
GLuint idTexturaPlay;
GLuint idTextura1; GLuint idTextura5; GLuint idTextura10;
GLuint idTextura2; GLuint idTextura7; GLuint idTextura11;
GLuint idTextura3; GLuint idTextura8; GLuint idTextura0;
GLuint idTextura4; GLuint idTextura9; GLuint idTextura6;
GLuint idTexturaCoroa;

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

void music(){
    if(keyStates['p'])
        Mix_PauseMusic();
    else
        Mix_ResumeMusic();
}

// detecta colisão entre dois objetos
bool colisao(dimpos um, dimpos dois){
    if(abs(um.x - dois.x) > (um.l/2+dois.l/2) ||
        abs(um.y - dois.y) > (um.a/2+dois.a/2)
        || tempo_colisao>5) 
        return false;
    tempo_colisao=10;
    return true;    
}

//detecta se o mouse está em cima de um objeto
bool hovering(float x, float y, dimpos dois){
    if(abs(x - dois.x) > (dois.l/2)) 
        return false;
    if(abs(y - dois.y) > (dois.a/2)) 
        return false;
    return true;    
}

//void displayText( float x, float y, int r, int g, int b, const char *string) {
//    int j = strlen(string);
//
//    glColor3f(r, g, b);
//    glRasterPos2f( x, y );
//    for( int i = 0; i < j; i++ ) {
//        glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, string[i] );
//    }
//}

// reseta as posições iniciais dos objetos
void restart(){
    bola.x = mundo.l/2;
    bola.y = mundo.a/2;

    buraco1.x = mundo.l/2+bola.l*2;
    buraco1.y = mundo.a/2+bola.a*2;

    buraco2.x = mundo.l/2-bola.l*2;
    buraco2.y = mundo.a/2-bola.a*2;
    
    barraE.l = barraD.l = mundo.a/20;
    barraE.a = barraD.a = mundo.l/8;
    barraD.x = (mundo.l/15)*14;
    barraD.y = barraE.y = mundo.a/2;
    barraE.x = mundo.l/15;

    acrescebola.x = 10;
    acrescebola.y = 2;
    acresceburaco1.x = (rand()%10+1);
    acresceburaco1.y = (rand()%10+1);
    acresceburaco2.x = -(rand()%10+1);
    acresceburaco2.y = -(rand()%10+1);
}

void desenhaObjeto(GLuint id, dimpos objeto){
    glBindTexture(GL_TEXTURE_2D, id);
    glPushMatrix();
        glTranslatef(objeto.x, objeto.y, 0);
        glBegin(GL_TRIANGLE_FAN);
            glTexCoord2f(0, 0); glVertex3f(-objeto.l/2, -objeto.a/2, 0);
            glTexCoord2f(1, 0); glVertex3f(objeto.l/2, -objeto.a/2, 0);
            glTexCoord2f(1, 1); glVertex3f(objeto.l/2, objeto.a/2, 0);
            glTexCoord2f(0, 1); glVertex3f(-objeto.l/2, objeto.a/2, 0);
        glEnd();
    glPopMatrix();   
}


void desenhaPlacar(int placar_atual, dimpos placar){
    switch(placar_atual){
        case 0: desenhaObjeto(idTextura0, placar);
            break;
        case 1: desenhaObjeto(idTextura1, placar);
            break;
        case 2: desenhaObjeto(idTextura2, placar);
            break;
        case 3: desenhaObjeto(idTextura3, placar);
            break;
        case 4: desenhaObjeto(idTextura4, placar);
            break;
        case 5: desenhaObjeto(idTextura5, placar);
            break;
        case 6: desenhaObjeto(idTextura6, placar);
            break;
        case 7: desenhaObjeto(idTextura7, placar);
            break;
        case 8: desenhaObjeto(idTextura8, placar);
            break;
        case 9: desenhaObjeto(idTextura9, placar);
            break;
        case 10: desenhaObjeto(idTextura10, placar);
            break;
        case 11: desenhaObjeto(idTextura11, placar);
            break;
    }
}

// desenha texturas do tamanho do mundo
void desenhaMundo(GLuint id){
    glBindTexture(GL_TEXTURE_2D, id);
    glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
        glTexCoord2f(1, 0); glVertex3f(mundo.l, 0, 0);
        glTexCoord2f(1, 1); glVertex3f(mundo.l, mundo.a, 0);
        glTexCoord2f(0, 1); glVertex3f(0, mundo.a, 0);
    glEnd();
}

void desenhaFundo(){
    glBindTexture(GL_TEXTURE_2D, idTexturaFundo);    //binda e desenha o fundo
    /*
    glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
        glTexCoord2f(1, 0); glVertex3f(auxFundo.l, 0, 0);
        glTexCoord2f(1, 1); glVertex3f(auxFundo.l, auxFundo.a, 0);
        glTexCoord2f(0, 1); glVertex3f(0, auxFundo.a, 0);
    glEnd();
    */
    if(!keyStates['p']){
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glMatrixMode(GL_TEXTURE);
        glLoadIdentity();
        glTranslatef(auxFundo.l/30000, 0, 0);
    }
    glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
        glTexCoord2f(1, 0); glVertex3f(mundo.l, 0, 0);
        glTexCoord2f(1, 1); glVertex3f(mundo.l, mundo.a, 0);
        glTexCoord2f(0, 1); glVertex3f(0, mundo.a, 0);
    glEnd();
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    
}

void desenhaPoligono(int numLados, dimpos pos){
    
    float desloc, pi=atan(1)*4;

    if(numLados%2!=0)
    desloc=(pi/2)-2*pi/numLados;
     else
    desloc=(pi/2)-2*pi/numLados+2*pi/(2*numLados);

    glBegin(GL_TRIANGLE_FAN);
      glVertex3f(pos.x,pos.y,0);
      for(int i=0; i<numLados; i++)
        glVertex3f(pos.x+(pos.l/2)*1.1*cos(desloc+i*2*pi/numLados),pos.y+(pos.l/2)*1.1*sin(desloc+i*2*pi/numLados),0);
      glVertex3f(pos.x+(pos.l/2)*1.1*cos(desloc),pos.y+(pos.l/2)*1.1*sin(desloc),0);
    glEnd();     
}

void inicializa() {
    glClearColor(1, 1, 1, 1);

    srand(time(0));

    // habilita mesclagem de cores, para termos suporte a texturas
    // com transparência
    glEnable(GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    idTexturaFundo = carregaTextura("background.png");
    idTexturaBarra = carregaTextura("barra.png");
    idTexturaBarra2 = carregaTextura("barra2.png");
    idTexturaBola = carregaTextura("ufo.png");
    idTexturaP1W = carregaTextura("p1w.png");
    idTexturaP2W = carregaTextura("p2w.png");
    idTexturaBuraco1 = carregaTextura("black-hole.png");
    idTexturaBuraco2 = carregaTextura("white-hole.png");
    idTexturaPause = carregaTextura("pause-button.png");
    idTexturaRestart = carregaTextura("restart.png");
    idTexturaExit = carregaTextura("exit.png");
    idTexturaYes = carregaTextura("yes.png");
    idTexturaNo = carregaTextura("no.png");
    idTexturaYesOn = carregaTextura("yeson.png");
    idTexturaNoOn = carregaTextura("noon.png");
    idTexturaFoguete = carregaTextura("rocket.png");
    idTexturaIntro = carregaTextura("intro.png");
    idTexturaMenu = carregaTextura("menu.png");
    idTexturaPlay = carregaTextura("menu-conflict.png");
    idTextura1 = carregaTextura("number-one.png");
    idTextura2 = carregaTextura("number-two.png");
    idTextura3 = carregaTextura("number-three.png");
    idTextura4 = carregaTextura("number-four.png");
    idTextura5 = carregaTextura("number-five.png");
    idTextura6 = carregaTextura("number-six.png");
    idTextura7 = carregaTextura("number-seven.png");
    idTextura8 = carregaTextura("number-eight.png");
    idTextura9 = carregaTextura("number-nine.png");
    idTextura10 = carregaTextura("number-ten.png");
    idTextura11 = carregaTextura("number-eleven.png");
    idTextura0 = carregaTextura("number-zero.png");
    idTexturaCoroa = carregaTextura("crown.png");

    // valores iniciais das dimensões dos objetos
    // bem como a posição dos que não mudam de lugar

    for(int i=0; i<256; i++)
        keyStates[i] = false;

    keyStates[1]=true;

    mundo.l = 1000;
    mundo.a = 615;

    auxFundo.l = mundo.l;
    auxFundo.a = mundo.a;

    pw.x = exitbut.x = restartbut.x = menu.x = mundo.l/2;
    intro.y = bola.y = mundo.a/2;
    intro.x = mundo.l/2;
    menu.y = mundo.a - 100;
    pw.y = exitbut.y = restartbut.y = mundo.a - 200;
    coroa.y = placarE.y = placarD.y = mundo.a - 70;
    placarE.x = mundo.l/8;
    placarD.x = (mundo.l/8)*7;

    placarE.l = placarE.a = placarD.l = placarD.a = 50;
    coroa.l = coroa.a = 30;

    intro.l = menu.l = 900;
    intro.a = menu.a = 100;

    pw.l = restartbut.l = 800;
    exitbut.a = restartbut.a = 60;
    exitbut.l = 600;
    pw.a = 200;


    bola.l = bola.a = 50;

    buraco1.l = buraco1.a = buraco2.l = buraco2.a = 75;

    bpause.l = bpause.a = 100;

    bpause.x = mundo.l/2;
    bpause.y = mundo.a/2;

    optyes.l = optno.l = optyes.a = optno.a = 100;
    optyes.y = optno.y = mundo.a/3;
    optyes.x = (mundo.l/20)*8;
    optno.x = (mundo.l/20)*12;


    foguete.x = mundo.l/2-40;
    foguete.y = 0;
    foguete.l = 100;
    foguete.a = 120;

    borda_mundo.a = 1;
    borda_mundo.l = mundo.l;
    borda_mundo.x = 0;
    borda_mundo.y = mundo.a;

    menuplay.l = 200;
    menuplay.a = 200;
    menuplay.x = mundo.l/2;
    menuplay.y = mundo.a/2;

    restart();
}

void desenha() {
    
    
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f (1, 1, 1);

    // habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);

    desenhaFundo();

    music();

    // se alguma das teclas de mudança de posição das barras
    // estiver pressionada (true) e a barra não tiver atingido
    // a borda do mundo, continua mudando a posição
    if(keyStates[112] == false){
        if(keyStates['l'] && barraD.y>barraD.a/2)
            barraD.y-=10;
        if(keyStates['o'] && barraD.y<mundo.a-barraD.a/2)
            barraD.y+=10;
        if(keyStates[115] && barraE.y>barraE.a/2)
            barraE.y-=10;
        if(keyStates[119] && barraE.y<mundo.a-barraE.a/2)
            barraE.y+=10;
    }

    if(!keyStates[0]){
        keyStates['p'] = true;
        keyStates['r'] = false;
        keyStates[27] = false;
        desenhaObjeto(idTexturaIntro, intro);
        desenhaObjeto(idTexturaFoguete, foguete);
    }
    else if(keyStates[1]){
        keyStates['p'] = true;
        keyStates['r'] = false;
        keyStates[27] = false;
        desenhaObjeto(idTexturaMenu, menu);
        desenhaObjeto(idTexturaPlay, menuplay);
    }
    else if(placarE_atual>10 || placarD_atual>10){
        keyStates['p'] = true;
        //se o mouse estiver em cima do botão, muda de cor
        if(optyes.opt)
            desenhaObjeto(idTexturaYesOn, optyes);
        //se não, fica com a cor original
        else 
            desenhaObjeto(idTexturaYes, optyes);
        //se o mouse estiver em cima do botão, muda de cor
        if(optno.opt)
            desenhaObjeto(idTexturaNoOn, optno);
        //se não, fica com a cor original
        else
            desenhaObjeto(idTexturaNo, optno);
        keyStates[3]=true;
        if(placarE_atual>10)
            desenhaObjeto(idTexturaP1W,pw);
        else if(placarD_atual>10)
            desenhaObjeto(idTexturaP2W,pw);
    }
    else if(!keyStates['r'] && !keyStates[27]){
        desenhaObjeto(idTexturaBarra, barraD);
        desenhaObjeto(idTexturaBarra2, barraE);
        desenhaObjeto(idTexturaBola, bola);
        desenhaObjeto(idTexturaBuraco1, buraco1);
        desenhaObjeto(idTexturaBuraco2, buraco2);
        desenhaPlacar(placarE_atual, placarE);
        desenhaPlacar(placarD_atual, placarD);    
        if(placarD_atual==10 && placarE_atual<10){
            coroa.x = (mundo.l/8)*7 + 50;
            desenhaObjeto(idTexturaCoroa, coroa);
        }
        else if(placarE_atual==10 && placarD_atual<10){
            coroa.x = (mundo.l/8) - 50;
            desenhaObjeto(idTexturaCoroa, coroa);
        }
    }

    // se a tecla 'p' for apertada uma vez,
    //desenha o símbolo de pause
    if(keyStates[112] && !keyStates[1]
        && !keyStates['r'] && !keyStates[27]
        && !keyStates[3]){
        desenhaObjeto(idTexturaPause, bpause);
    }

    if(keyStates[1] && glutGet(GLUT_ELAPSED_TIME)/1000>ssstop){
        keyStates[112]=true;
        desenhaObjeto(idTexturaMenu, menu);
        //se o mouse estiver em cima do botão, muda de cor
        if(menuplay.opt){
            glColor3f(1, 1, 1);
            glDisable(GL_TEXTURE_2D);
            desenhaPoligono(50, menuplay);
            glEnable(GL_TEXTURE_2D);
            desenhaObjeto(idTexturaPlay, menuplay);
        }
        //se não, fica com a cor original
        else 
            desenhaObjeto(idTexturaPlay, menuplay);
    }

    //se a tecla 'r' for apertada, exibe a mensagem de confirmação
    if(keyStates['r']){
        keyStates['p']=true;
        keyStates[27] = false;
        desenhaObjeto(idTexturaRestart, restartbut);
        //se o mouse estiver em cima do botão, muda de cor
        if(optyes.opt)
            desenhaObjeto(idTexturaYesOn, optyes);
        //se não, fica com a cor original
        else 
            desenhaObjeto(idTexturaYes, optyes);
        //se o mouse estiver em cima do botão, muda de cor
        if(optno.opt)
            desenhaObjeto(idTexturaNoOn, optno);
        //se não, fica com a cor original
        else
            desenhaObjeto(idTexturaNo, optno);
    }

    //mesma coisa da tecla 'r', mas com 'esc'
    if(keyStates[27]){
        keyStates['p']=true;
        keyStates['r'] = false;
        desenhaObjeto(idTexturaExit, exitbut);
        if(optyes.opt)
            desenhaObjeto(idTexturaYesOn, optyes);
        else 
            desenhaObjeto(idTexturaYes, optyes);
        if(optno.opt)
            desenhaObjeto(idTexturaNoOn, optno);
        else
            desenhaObjeto(idTexturaNo, optno);
    }

    // diz que o player 1 ganhou quando a bola passa pela direita
    // e reinicia
    if((bola.x-bola.l/2) >= mundo.l){
//        auxFundo.l = mundo.l;
//        auxFundo.a = mundo.a;
        placarE_atual++;
        restart();
    }

    // diz que o player 2 ganhou quando a bola passa pela esquerda
    // e reinicia
    if((bola.x+bola.l/2) <= 0){
//        auxFundo.l = mundo.l;
//        auxFundo.a = mundo.a;
        placarD_atual++;
        restart();
    }

    //displayText(mundo.l/2, mundo.a/2, 1, 1, 1, "teste");

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

void mouseClick(int button, int state, int x, int y)
{
    // início da conversão de coordenadas da tela para do mundo
    // retirado de https://community.khronos.org/t/converting-
    //window-coordinates-to-world-coordinates/16029/8

    GLint viewport[4]; //var to hold the viewport info
    GLdouble modelview[16]; //var to hold the modelview info
    GLdouble projection[16]; //var to hold the projection matrix info
    GLfloat winX, winY, winZ; //variables to hold screen x,y,z coordinates
    GLdouble worldX, worldY, worldZ; //variables to hold world x,y,z coordinates

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview); //get the modelview info
    glGetDoublev(GL_PROJECTION_MATRIX, projection); //get the projection matrix info
    glGetIntegerv(GL_VIEWPORT, viewport); //get the viewport info

    winX = (float)x;
        winY = (float)viewport[3] - (float)y;
    winZ = 0;
    
    gluUnProject(winX, winY, winZ, modelview, projection, viewport, &worldX, &worldY, &worldZ);

    // fim da conversão de coordenadas da tela para do mundo


    // se tiver na tela do 'menu'
    if(keyStates[1])
        // se o mouse for clicado
        if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
            // em cima do botão 'play'
            if(hovering(worldX, worldY, menuplay)){
                // fecha o jogo
                keyStates[1]=false;
                keyStates['p']=false;
            }
            // em cima do botão 'não'
            else if(hovering(worldX, worldY, optno)){
                // despausa e sai do 'exit'
                keyStates[27] = false;
                keyStates[112] = false;
            }
        }
    // se tiver na tela do 'exit'
    if(keyStates[27])
        // se o mouse for clicado
        if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
            // em cima do botão 'sim'
            if(hovering(worldX, worldY, optyes)){
                // fecha o jogo
                exit(0);
            }
            // em cima do botão 'não'
            else if(hovering(worldX, worldY, optno)){
                // despausa e sai do 'exit'
                keyStates[27] = false;
                keyStates[112] = false;
            }
        }
    // se tiver na tela do 'restart'
    if(keyStates[114])
        // se o mouse for clicado
        if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
            // em cima do botão 'sim'
            if(hovering(worldX, worldY, optyes)){
                // despausa, sai do 'restart' e reinicia
                keyStates[114] = false;
                keyStates[112] = false;
                restart();
            }
            // despausa e sai do 'restart'
            else if(hovering(worldX, worldY, optno)){
                keyStates[114] = false;
                keyStates[112] = false;
            }
        }

    if(keyStates[3])
        // se o mouse for clicado
        if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
            // em cima do botão 'sim'
            if(hovering(worldX, worldY, optyes)){
                // despausa, sai do 'restart' e reinicia
                keyStates[114] = false;
                keyStates[112] = false;
                keyStates[27] = false;
                keyStates[3] = false;
                restart();
                placarE_atual=0;
                placarD_atual=0;
            }
            // despausa e sai do 'restart'
            else if(hovering(worldX, worldY, optno)){
                keyStates[114] = false;
                keyStates[27] = false;
                keyStates[3] = false;
                keyStates[1] = true;
                placarE_atual=0;
                placarD_atual=0;
            }
        }    



    // se tiver no 'menu'
    if(keyStates[1])
        // se o mouse for clicado
        if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
            // em cima do botão 'sim'
            if(hovering(worldX, worldY, menuplay)){
                // fecha o jogo
            }
        }
}

void mouseHover(int x, int y)
{
    // início da conversão de coordenadas da tela para do mundo
    // retirado de https://community.khronos.org/t/converting-
    //window-coordinates-to-world-coordinates/16029/8

    GLint viewport[4]; //var to hold the viewport info
    GLdouble modelview[16]; //var to hold the modelview info
    GLdouble projection[16]; //var to hold the projection matrix info
    GLfloat winX, winY, winZ; //variables to hold screen x,y,z coordinates
    GLdouble worldX, worldY, worldZ; //variables to hold world x,y,z coordinates

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview); //get the modelview info
    glGetDoublev(GL_PROJECTION_MATRIX, projection); //get the projection matrix info
    glGetIntegerv(GL_VIEWPORT, viewport); //get the viewport info

    winX = (float)x;
        winY = (float)viewport[3] - (float)y;
    winZ = 0;
    
    //get the world coordinates from the screen coordinates
    gluUnProject(winX, winY, winZ, modelview, projection, viewport, &worldX, &worldY, &worldZ);

    // fim da conversão de coordenadas da tela para do mundo

    if(keyStates[27] || keyStates[114] || keyStates[3]){
        if(hovering(worldX, worldY, optyes))
            optyes.opt=true;
        else
            optyes.opt=false;
        if(hovering(worldX, worldY, optno))
            optno.opt=true;
        else
            optno.opt=false;
    }
    //tela menu
    if(keyStates[1]){
        if(hovering(worldX, worldY, menuplay))
            menuplay.opt=true;
        else
            menuplay.opt=false;
    }

    if(keyStates[3]){
        if(hovering(worldX, worldY, menuplay))
            menuplay.opt=true;
        else
            menuplay.opt=false;
    }

}

void teclado(unsigned char key, int x, int y) {
    switch (key) {
        case 'q': exit(0); break;
        case 27: // tecla 'esc'
            // se a tecla é pressionada, altera o valor
            // para true
            keyStates[key] = true;
            keyStates[112] = true; // pausa
            break;
        case 'l': // tecla '0'
            keyStates[key] = true;
            break;
        case 'o': // tecla '1'
            keyStates[key] = true;
            break;
        case 115: // tecla 's'
            keyStates[key] = true;
            break;
        case 119: // tecla 'w'
            keyStates[key] = true;
            break;            
        case 112: // tecla 'p'
        // se ela tá true, vira false e vice-versa
            if(keyStates[key]){
                keyStates[key]=false; // para despausar quando tá pausado
            }
            else if(!keyStates[key]){
                keyStates[key]=true; // para pausar quando não tá pausado
            }
            break;
        case 114: // tecla 'r'
            keyStates[key] = true;
            keyStates[112] = true; // pausa
            break;
    }
}

void soltaTecla (unsigned char key, int x, int y){
    switch (key) {
        // se a tecla é solta, volta o valor
        // para false
        case 'l': // tecla '0'
            keyStates[key] = false;
            break;
        case 'o': // tecla '1'
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

    auxFundo.l += 100;
    if(glutGet(GLUT_ELAPSED_TIME)/1000>ssstop)
        keyStates[0]=true;

    if(foguete.y<=mundo.a+foguete.a)
        foguete.y += 15;

    // muda a direção da bola quando ela atinge a borda do mundo
    if(bola.y + bola.a/2 >= mundo.a || bola.y - bola.a/2 <= 0)
        acrescebola.y *= -1;
    //else if(bola.y + bola.a/2 >= mundo.a + 10){
        //bola.y = mundo.a - bola.a/2;
        //acrescebola.y *= -1;
    //}
    // muda a direção da bola quando ela atinge a barra da esquerda
    if(colisao(bola, barraE)){
        acrescebola.x *= -1;
        Mix_PlayChannel(-1, Mix_LoadWAV("laser-shoot.wav"), 0);
    }
    // muda a direção da bola quando ela atinge a barra da direita
    if(colisao(bola, barraD)){
        acrescebola.x *= -1;
        Mix_PlayChannel(-1, Mix_LoadWAV("laser-shoot.wav"), 0);
    }

    // se o jogo não está pausado
    if(!keyStates[112]){
        // faz a bola mudar de posição ao longo do tempo (animação)
        // também aumenta a velocidade da bola com as batidas nas barras
        bola.x += acrescebola.x*1.1;
        bola.y += acrescebola.y*1.1;
        // faz os buracos mudarem de posição ao longo do tempo (animação)
        buraco1.x += acresceburaco1.x;
        buraco1.y += acresceburaco1.y;
        buraco2.x += acresceburaco2.x;
        buraco2.y += acresceburaco2.y;
    }

    // muda a direção dos buracos se atingem a borda de um
    // espaço no meio do mundo
    if(buraco1.y >= (mundo.a/3)*2 || buraco1.y <= mundo.a/3)
        acresceburaco1.y *= -1;
    if(buraco1.x >= (mundo.l/5)*4 || buraco1.x <= mundo.l/5)
        acresceburaco1.x *= -1;
    if(buraco2.y >= (mundo.a/3)*2 || buraco2.y <= mundo.a/3)
        acresceburaco2.y *= -1;
    if(buraco2.x >= (mundo.l/5)*4 || buraco2.x <= mundo.l/5)
        acresceburaco2.x *= -1;


    // muda a bola do buraco negro para o buraco branco
    if(colisao(bola, buraco1)){
        bola.x = buraco2.x;
        bola.y = buraco2.y;
        acrescebola.x *= -1;
        acrescebola.y *= -1;
        Mix_PlayChannel(-1, Mix_LoadWAV("black-hole.wav"), 0);
    } 

    tempo_colisao--;

    // pede ao GLUT para redesenhar a tela, assim que possível
    glutPostRedisplay();

    // se registra novamente, para que fique sempre sendo chamada
    glutTimerFunc(periodo, atualizaCena, periodo);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    SDL_Init(SDL_INIT_AUDIO);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    Mix_PlayChannel(-1, Mix_LoadWAV("rocket.wav"), 0);
    Mix_Volume(-1,MIX_MAX_VOLUME/15);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1000, 615);
    glutInitWindowPosition(100, 100);

    glutCreateWindow("Space Ping Pong");
    inicializa();

    glutDisplayFunc(desenha);    
    glutReshapeFunc(redimensiona);


    glutKeyboardFunc(teclado);
    glutKeyboardUpFunc(soltaTecla);
    glutMouseFunc(mouseClick);
    glutPassiveMotionFunc(mouseHover);

    glutTimerFunc(0, atualizaCena, 33);

    Mix_PlayMusic(Mix_LoadMUS("tentacle-wedding.mp3"), -1);
    glutMainLoop();

    return 0;
}