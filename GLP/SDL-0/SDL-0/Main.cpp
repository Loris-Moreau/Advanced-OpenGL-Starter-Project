#include <SDL.h>
#include <glew.h>

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

string LoadShader(const string& fileName);

int main(int argc = 0, char** argv = nullptr)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        cout << "SDL initialized successfully \n";
    }

    int flags = SDL_WINDOW_OPENGL;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        cout << "SDL failed to initialize" << '\n';
        cout << "(づ｡◕‿‿◕｡)づ  (⎻▵ ⎻)" << '\n';
        return 1;
    }

    ///////////SETTING UP SDL/////////////
    //Create a simple window
    constexpr int width = 900;
    constexpr int height = 600;
    constexpr unsigned int center = SDL_WINDOWPOS_CENTERED;
    SDL_Window* Window = SDL_CreateWindow("Pong of Doom (O//w//O).exe", center, center, width, height, SDL_WINDOW_OPENGL);
    //SDL_WINDOW_OPENGL is a u32 flag !

    ////Create an OpenGL compatible context to let glew draw on it
    const SDL_GLContext Context = SDL_GL_CreateContext(Window);

    /////////SETTING UP OPENGL WITH GLEW///
    //Initialize glew
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        cout << "Glew failed to initialize\n";
    }

    ////Object Info////
    //ball
    float ballX = 0.0f;
    float ballY = 0.0f;
    float ballSpeedX = 0.007f;
    float ballSpeedY = 0.007f;
    constexpr float ballRadius = 0.025f;
    //Paddles
    float leftPaddleY = 0.0f;
    float rightPaddleY = 0.0f;
    ////

    // Get info
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    cout << "Renderer : " << renderer << '\n';
    cout << "OpenGL version supported : " << version << '\n';

    // Tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

    // Set viewport and clear color
    glViewport(0, 0, width, height);
    glClearColor(0.0f, 0.0f, 0.2f, 1.0f);

    /*
    The Points are as Follows :
    --> (X, Y, Z, Colors(R, G, B))
    */

    constexpr float paddleVertice[] =
    {
        //Left
        -0.95f,  0.2f, 0.0f,     1.0f, 0.0f, 0.0f,
        -0.95f, -0.2f, 0.0f,     0.0f, 0.0f, 1.0f,
         -0.9f, -0.2f, 0.0f,     0.0f, 0.0f, 1.0f,
         -0.9f,  0.2f, 0.0f,     0.0f, 0.0f, 1.0f,

         //Right
          0.9f,  0.2f, 0.0f,      0.0f, 0.0f, 1.0f,
          0.9f, -0.2f, 0.0f,      0.0f, 0.0f, 1.0f,
         0.95f, -0.2f, 0.0f,      0.0f, 0.0f, 1.0f,
         0.95f,  0.2f, 0.0f,      0.0f, 0.0f, 1.0f,
    };

    const float ballVertice[] =
    {
        ballX - ballRadius, ballY, 0.0f, 1.0f, 0.0f, 0.0f,
        ballX, ballY - ballRadius, 0.0f, 0.0f, 0.0f, 1.0f,
        ballX + ballRadius, ballY, 0.0f, 1.0f, 0.0f, 0.0f,
        ballX, ballY + ballRadius, 0.0f, 0.0f, 0.0f, 1.0f,
    };

    //Create an ID to be given at object generation
    //Vertex Buffer Objects (VBO)
    unsigned int VBO;
    unsigned int VBO1;

    //Pass how many buffers should be created and the reference of the ID to get the value set
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VBO1);

    //Binds the buffer linked to this ID to the vertex array buffer to be rendered. Put 0 instead of vbo to reset the value.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);

    //Finally send the vertices array in the array buffer (linked to vbo)
    const string vs = LoadShader("Vertex.shader");
    const char* vertexShaderSource = vs.c_str();
    const string fs = LoadShader("Fragment.shader");
    const char* fragmentShaderSource = fs.c_str();
    const string bvs = LoadShader("BallVertex.shader");
    const char* ballVertexShaderSource = bvs.c_str();

    const unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

    const unsigned int ballVertexShader = glCreateShader(GL_VERTEX_SHADER);

    //now that we have a vertex shader, let’s put the code text inside
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

    glShaderSource(ballVertexShader, 1, &ballVertexShaderSource, NULL);

    //aaaaand… Compile !
    glCompileShader(vertexShader);
    glCompileShader(ballVertexShader);

    //Do the same with the fragment shader !
    const unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);


    const unsigned int paddleShaderProgram = glCreateProgram();

    //now attach shaders to use to the program
    glAttachShader(paddleShaderProgram, vertexShader);
    glAttachShader(paddleShaderProgram, fragmentShader);

    //and link it 
    glLinkProgram(paddleShaderProgram);
    const unsigned int ballShaderProgram = glCreateProgram();

    //now attach shaders to use to the program
    glAttachShader(ballShaderProgram, ballVertexShader);
    glAttachShader(ballShaderProgram, fragmentShader);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // ..:: Initialization code (done once (unless your object frequently changes)) :: ..
    // 1. bind Vertex Array Object
    glBindVertexArray(VAO);

    // 2. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(paddleVertice), paddleVertice, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int VAOBall;
    glGenVertexArrays(1, &VAOBall);

    // ..:: Initialization code (done once (unless your object frequently changes)) :: ..
    // 1. bind Vertex Array Object (VAO)
    glBindVertexArray(VAOBall);
    // 2. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ballVertice), paddleVertice, GL_STATIC_DRAW);


    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ballVertice), ballVertice, GL_STATIC_DRAW);

    glLinkProgram(ballShaderProgram);

    glUseProgram(ballShaderProgram);
    unsigned int movementLocation = glGetUniformLocation(ballShaderProgram, "movement");

    // Game loop
    bool isRunning = true;
    while (isRunning)
    {
        constexpr float paddleSpeed = 0.025f;
        // Inputs
        const float timeValue = (float)SDL_GetTicks() / 1000;
        const float xPos = (sin(timeValue));

        const int vertexOffsetLocation = glGetUniformLocation(paddleShaderProgram, "offset");
        glUseProgram(paddleShaderProgram);
        glUniform1f(vertexOffsetLocation, xPos);

        const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);
        if (keyboardState[SDL_SCANCODE_ESCAPE])
        {
            isRunning = false;
        }

        if (keyboardState[SDL_SCANCODE_UP] && rightPaddleY < 0.8f)
        {
            rightPaddleY += paddleSpeed;
        }
        if (keyboardState[SDL_SCANCODE_DOWN] && rightPaddleY > -0.8f)
        {
            rightPaddleY -= paddleSpeed;
        }
        if (keyboardState[SDL_SCANCODE_W] && leftPaddleY < 0.8f)
        {
            leftPaddleY += paddleSpeed;
        }
        if (keyboardState[SDL_SCANCODE_S] && leftPaddleY > -0.8f)
        {
            leftPaddleY -= paddleSpeed;
        }
        
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                isRunning = false;
                break;

            default:
                break;
            }
        }
        
        // Mise à jour de la position de la balle
        ballX += ballSpeedX;
        ballY += ballSpeedY;

        if (ballY + ballRadius > 1.0f || ballY - ballRadius < -1.0f)
        {
            ballSpeedY = -ballSpeedY;
        }

        if ((ballX - ballRadius < -0.9f && ballX + ballRadius > -0.95f && ballY < leftPaddleY + 0.25f && ballY > leftPaddleY - 0.25f) ||
            (ballX + ballRadius > 0.9f && ballX - ballRadius < 0.95f && ballY < rightPaddleY + 0.25f && ballY > rightPaddleY - 0.25f))
        {
            ballSpeedX = -ballSpeedX;
        }

        if (ballX + ballRadius < -1.0f || ballX - ballRadius > 1.0f)
        {
            ballX = 0.0f;
            ballY = 0.0f;
            // Réinitialiser la vitesse de la balle 
            // ballSpeedX = 0.005f;
            // ballSpeedY = 0.005f;
        }

        // Mettez à jour les coordonnées des vertices de la balle
        float ballVertices[] = 
        {
            ballX - ballRadius, ballY, 0.0f, 1.0f, 0.0f, 0.0f,
            ballX, ballY - ballRadius, 0.0f, 0.0f, 0.0f, 1.0f,
            ballX + ballRadius, ballY, 0.0f, 1.0f, 0.0f, 0.0f,
            ballX, ballY + ballRadius, 0.0f, 0.0f, 0.0f, 1.0f,
        };

        // Draw
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen

        // Draw here    
        glBindVertexArray(VAO);
        glUseProgram(paddleShaderProgram);
        const unsigned int offsetLocation = glGetUniformLocation(paddleShaderProgram, "offset");
        glUniform1f(offsetLocation, leftPaddleY);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glUniform1f(offsetLocation, rightPaddleY);
        glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
        glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
        
        glBindVertexArray(VAOBall);
        glUseProgram(ballShaderProgram);
        const unsigned int movementLocation = glGetUniformLocation(ballShaderProgram, "movement");
        glUniform2f(movementLocation, ballX, ballY);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        SDL_GL_SwapWindow(Window); //Swapbuffer
    }

    // Quit
    SDL_DestroyWindow(Window);
    SDL_GL_DeleteContext(Context);

    return 0;
}

string LoadShader(const string& fileName) {
    ifstream myFile;
    myFile.open(fileName);
    if (myFile.fail())
    {
        cerr << "Error - failed to open " << fileName << '\n';
    }

    string fileText = "";
    string line = "";

    while (getline(myFile, line))
    {
        fileText += line + '\n';
    }

    return fileText;
}
