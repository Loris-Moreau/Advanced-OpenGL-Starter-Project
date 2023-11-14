#include <SDL.h>
#include <glew.h>

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

string LoadShader(string fileName);

int main(int argc = 0, char** argv = nullptr) {

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        cout << "SDL initialized successfully \n";
    }

    int flags = SDL_WINDOW_OPENGL;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        cout << "SDL failed to initialize" << endl;
        return 1;
    }

    ///////////SETTING UP SDL/////////////
    //Create a simple window
    int width = 900;
    int height = 600;
    unsigned int center = SDL_WINDOWPOS_CENTERED;
    SDL_Window* Window = SDL_CreateWindow("Pong OpenGLShader.exe", center, center, width, height, SDL_WINDOW_OPENGL);
    //SDL_WINDOW_OPENGL is a u32 flag !


    ////Create an OpenGL compatible context to let glew draw on it
    SDL_GLContext Context = SDL_GL_CreateContext(Window);

    /////////SETTING UP OPENGL WITH GLEW///
    //Initialize glew
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        cout << "Glew failed to initialize\n";
    }

    // Get info
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    cout << "Renderer: " << renderer << endl;
    cout << "OpenGL version supported: " << version << endl;

    // Tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

    // Set viewport and clear color
    glViewport(0, 0, width, height);
    glClearColor(0.0f, 0.0f, 0.2f, 1.0f);

    //Create an ID to be given at object generation
    unsigned int vbo = 0;
    unsigned int vbo1 = 1;

    //Pass how many buffers should be created and the reference of the ID to get the value set
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &vbo1);

    //Binds the buffer linked to this ID to the vertex array buffer to be rendered. Put 0 instead of vbo to reset the value.
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo1);

    //Finally send the vertices array in the array buffer (linked to vbo)
   /* glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); */

    string vs = LoadShader("RectangleVertex.shader");
    const char* vertexShaderSource = vs.c_str();
    string fs = LoadShader("BallFragment.shader");
    const char* fragmentShaderSource = fs.c_str();
    string bvs = LoadShader("BallVertex.shader");
    const char* ballVertexShaderSource = bvs.c_str();

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    unsigned int ballVertexShader;
    ballVertexShader = glCreateShader(GL_VERTEX_SHADER);

    //now that we have a vertex shader, let’s put the code text inside
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

    //aaaaand… Compile !
    glCompileShader(vertexShader);

    //Do the same with the fragment shader !
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    //now attach shaders to use to the program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glAttachShader(shaderProgram, ballVertexShader);

    //and link it 
    glLinkProgram(shaderProgram);

    //now that the program is complete, we can use it 
    glUseProgram(shaderProgram);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // ..:: Initialization code (done once (unless your object frequently changes)) :: ..
    // 1. bind Vertex Array Object
    glBindVertexArray(VAO);
    // 2. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    /*glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);*/

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Déclarez des variables pour les paddles
    float leftPaddleY = 0.0f;
    float rightPaddleY = 0.0f;
    float paddleSpeed = 0.005f;

    // Déclarez des variables pour la balle
    float ballX = 0.0f;
    float ballY = 0.0f;
    float ballSpeedX = 0.0005f;
    float ballSpeedY = 0.0005f;
    float ballRadius = 0.02f;

    // Game loop
    bool isRunning = true;
    while (isRunning) {
        // Inputs
        float timeValue = (float)SDL_GetTicks() / 1000;
        float xPos = (sin(timeValue));

        int vertexOffsetLocation = glGetUniformLocation(shaderProgram, "offset");
        glUseProgram(shaderProgram);
        glUniform1f(vertexOffsetLocation, xPos);

        // Mise à jour des positions des paddles en fonction des entrées utilisateur
        const Uint8* keys = SDL_GetKeyboardState(NULL);
        if (keys[SDL_SCANCODE_UP] && rightPaddleY < 0.8f) {
            rightPaddleY += paddleSpeed;
        }
        if (keys[SDL_SCANCODE_DOWN] && rightPaddleY > -0.8f) {
            rightPaddleY -= paddleSpeed;
        }
        if (keys[SDL_SCANCODE_Z] && leftPaddleY < 0.8f) {
            leftPaddleY += paddleSpeed;
        }
        if (keys[SDL_SCANCODE_S] && leftPaddleY > -0.8f) {
            leftPaddleY -= paddleSpeed;
        }

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
                break;

            default:
                break;
            }
        }
        // Update
        // Mettez à jour les coordonnées des vertices des paddles
        float paddleVertices[] =
        {
            // Gauche
            -0.9f, 0.2f + leftPaddleY, 0.0f, 1.0f, 0.0f, 0.0f,
            -0.9f, -0.2f + leftPaddleY, 0.0f, 0.0f, 0.0f, 1.0f,
            -0.8f, -0.2f + leftPaddleY, 0.0f, 0.0f, 0.0f, 1.0f,
            -0.8f, 0.2f + leftPaddleY, 0.0f, 0.0f, 0.0f, 1.0f,

            // Droite
            0.8f, 0.2f + rightPaddleY, 0.0f, 0.0f, 0.0f, 1.0f,
            0.8, -0.2f + rightPaddleY, 0.0f, 0.0f, 0.0f, 1.0f,
            0.9f, -0.2f + rightPaddleY, 0.0f, 0.0f, 0.0f, 1.0f,
            0.9f, 0.2f + rightPaddleY, 0.0f, 0.0f, 0.0f, 1.0f,
        };

        // Mise à jour de la position de la balle
        ballX += ballSpeedX;
        ballY += ballSpeedY;

        // Gestion des collisions avec les murs verticaux
        if (ballY + ballRadius > 1.0f || ballY - ballRadius < -1.0f) {
            ballSpeedY = -ballSpeedY; // Inverser la direction en cas de collision
        }

        // Gestion des collisions avec les paddles
        if ((ballX - ballRadius < -0.8f && ballX + ballRadius > -0.9f && ballY < leftPaddleY + 0.2f && ballY > leftPaddleY - 0.2f) ||
            (ballX + ballRadius > 0.8f && ballX - ballRadius < 0.9f && ballY < rightPaddleY + 0.2f && ballY > rightPaddleY - 0.2f)) {
            ballSpeedX = -ballSpeedX; // Inverser la direction en cas de collision avec les paddles
        }

        // Gestion des scores (exemple : remise à zéro si la balle sort de l'écran)
        if (ballX + ballRadius < -1.0f || ballX - ballRadius > 1.0f) {
            ballX = 0.0f;
            ballY = 0.0f;
            // Réinitialiser la vitesse de la balle si nécessaire
            // ballSpeedX = 0.005f;
            // ballSpeedY = 0.005f;
        }

        // Mettez à jour les coordonnées des vertices de la balle
        float ballVertices[] = {
            ballX - ballRadius, ballY, 0.0f, 1.0f, 0.0f, 0.0f,
            ballX, ballY - ballRadius, 0.0f, 0.0f, 0.0f, 1.0f,
            ballX + ballRadius, ballY, 0.0f, 1.0f, 0.0f, 0.0f,
            ballX, ballY + ballRadius, 0.0f, 0.0f, 0.0f, 1.0f,
        };

        // Draw
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen

        // Draw here
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(paddleVertices), paddleVertices, GL_STATIC_DRAW);
        glBufferData(GL_ARRAY_BUFFER, sizeof(ballVertices), ballVertices, GL_STATIC_DRAW);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
        glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
        glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
        SDL_GL_SwapWindow(Window); // Swapbuffer
    }

    // Quit
    SDL_DestroyWindow(Window);
    SDL_GL_DeleteContext(Context);

    return 0;
}

string LoadShader(string fileName) {
    ifstream myFile;
    myFile.open(fileName);
    if (myFile.fail()) {
        cerr << "Error - failed to open " << fileName << endl;
    }

    string fileText = "";
    string line = "";

    while (getline(myFile, line)) {
        fileText += line + '\n';
    }

    return fileText;
}
