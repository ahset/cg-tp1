typedef struct {
    GLfloat l, a, x, y; // l = largura, a = altura
    bool opt;
} dimpos; // tudo que tem dimensões e/ou posição e estado true/false

dimpos mundo, barraE, barraD, bola, buraco1, buraco2, acrescebola,
acresceburaco1, acresceburaco2, bpause, optyes, optno, foguete, 
borda_mundo, menuplay, auxFundo, intro, /*menu, */restartbut, exitbut,
placarE, placarD, pw, coroa, replay;
// barraE é a barra da esquerda e barraD é a barra da direita
// acrescebola é a variável que muda de sinal quando a bola atinge
// as bordas do mundo
bool keyStates[256]; // vetor de estados de teclas (também usado
//pra algumas telas específicas)
GLfloat ssstop=1.9; // tempo da splash screen
GLint tempo_colisao = 0; // gambiarra pra evitar bug
GLint placarE_atual = 0, placarD_atual = 0;
GLint vmaxbola = 15; // velocidade máxima da bola
GLfloat acrescevelocidade = 1.05; // incremento na velocidade
GLint velocidadebarra = 10; // incremento na posição da barra
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
//GLuint idTexturaMenu;
GLuint idTexturaPlay;
GLuint idTextura1; GLuint idTextura5; GLuint idTextura10;
GLuint idTextura2; GLuint idTextura7; GLuint idTextura11;
GLuint idTextura3; GLuint idTextura8; GLuint idTextura0;
GLuint idTextura4; GLuint idTextura9; GLuint idTextura6;
GLuint idTexturaCoroa;
GLuint idTexturaReplay;
GLuint idTexturaBack;