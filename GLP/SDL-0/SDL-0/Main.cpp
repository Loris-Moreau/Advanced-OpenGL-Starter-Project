#include <SDL.h>
#include <glew.h>

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

string LoadShader(string fileName);

int main(int argc = 0, char** argv = nullptr) 
{
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) 
    {
        cout << "SDL initialized successfully\n";
    }

    int flags = SDL_WINDOW_OPENGL;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) 
    {
        cout << "SDL failed to initialize" << endl;
        return 1;
    }
    ///////////SETTING UP SDL/////////////
    
    //Create a simple window
    int width = 1280;
    int height = 720;
    unsigned int center = SDL_WINDOWPOS_CENTERED;
    SDL_Window* Window = SDL_CreateWindow("SDL Pong  ///0w0///", center, center, width, height, SDL_WINDOW_OPENGL);
    //SDL_WINDOW_OPENGL is a u32 flag !

    ////Create an OpenGL compatible context to let glew draw on it
    SDL_GLContext Context = SDL_GL_CreateContext(Window);

    /////////SETTING UP OPENGL WITH GLEW///
    
    //Initialize glew
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) 
    {
        cout << "Glew failed to initialize\n";
    }

    // Get info
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    cout << "Renderer : " << renderer << endl;
    cout << "OpenGL version supported : " << version << endl;

    // Tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

    // Set viewport and clear color
    glViewport(0, 0, width, height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);


    /// Load
 
    //Describe the shape by its vertices
    
    /* 
    The Points are as Follows :
    --> Top Left      (X, Y, Z, Colors)
    --> Bottom Left   (X, Y, Z, Colors)
    --> Top Right     (X, Y, Z, Colors)
    --> Bottom Right  (X, Y, Z, Colors)
    --> Bottom Left   (X, Y, Z, Colors) -> can be removed if you use GL_TRIANGLE_STRIP, but needed if use the GL_TRIANGLE_FAN 
    */

    float Vertices[] =
    {
        //Center Line (Rectangle)
        -0.01f,  1.0f, 0.0f,     1.0f, 0.0f, 0.0f,//A
        -0.01f, -1.0f, 0.0f,     0.0f, 1.0f, 0.0f,//B
         0.01f,  1.0f, 0.0f,     0.0f, 0.0f, 1.0f,//D
        0.01f,  -1.0f, 0.0f,     1.0f, 0.0f, 0.0f,//C
        //-0.01f, -1.0f, 0.0f,     0.0f, 1.0f, 0.0f, //B

        //Left Rectangle
        -0.95f,  0.15f, 0.2,  1.0f, 0.0f, 0.0f,//I
        -0.95f, -0.15f, 0.2,  0.0f, 1.0f, 0.0f,//L
         -0.9f,  0.15f, 0.2,  0.0f, 0.0f, 1.0f,//J
         -0.9f, -0.15f, 0.2,  1.0f, 0.0f, 0.0f,//K
        //-0.95f, -0.15f, 0.2,  1.0f, 1.0f, 0.0f, //L

        //Right Rectangle
        0.9f,  0.15, 0.2,  1.0f, 0.0f, 0.0f,//M
         0.9f, -0.15, 0.2,  0.0f, 1.0f, 0.0f,//N
        0.95f,  0.15, 0.2,  0.0f, 0.0f, 1.0f,//P
        0.95f, -0.15, 0.2,  1.0f, 0.0f, 0.0f,//O
        // 0.9f, -0.15, 0.2,  0.0f, 1.0f, 0.0f, //N

        //Ball
        -0.015,  0.015, 0.5,  1.0f, 0.0f, 0.0f,//Q
        -0.015, -0.015, 0.5,  0.0f, 1.0f, 0.0f,//T
         0.015,  0.015, 0.5,  0.0f, 0.0f, 1.0f,//R
         0.015, -0.015, 0.5,  1.0f, 0.0f, 0.0f,//S
        //-0.015, -0.015, 0.5,  0.0f, 1.0f, 0.0f//T
    };

    /*//Ball
    float LeftRect[] =
    {
        -0.015,  0.015, 0.5,  1.0f, 0.0f, 0.0f,//Q
        -0.015, -0.015, 0.5,  0.0f, 1.0f, 0.0f,//T
         0.015,  0.015, 0.5,  0.0f, 0.0f, 1.0f,//R
         0.015, -0.015, 0.5,  1.0f, 0.0f, 0.0f,//S
        -0.015, -0.015, 0.5,  0.0f, 1.0f, 0.0f//T
    };*/

    //Create an ID to be given at object generation
    unsigned int vbo = 0;

    //Pass how many buffers should be created and the reference of the ID to get the value set
    glGenBuffers(1, &vbo);

    //Binds the buffer linked to this ID to the vertex array buffer to be rendered. Put 0 instead of vbo to reset the value.
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    //Finally send the vertices array in the array buffer (linked to vbo)
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Shaders Initialize (Get)

    string vs = LoadShader("BallVertex.shader");
    const char* vertexShaderSource = vs.c_str();
    string fs = LoadShader("BallFragment.shader");
    const char* fragmentShaderSource = fs.c_str();

    string rvs = LoadShader("RectangleVertex.shader");
    const char* RectangleVertexShaderSource = rvs.c_str();
    string rfs = LoadShader("RectangleFragment.shader");
    const char* RectangleFragmentShaderSource = rfs.c_str();

    ///for the ball///
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    //now that we have a vertex shader, let’s put the code text inside
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

    //and… Compile !
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

    //and link it 
    glLinkProgram(shaderProgram);

    //now that the program is complete, we can use it 
    glUseProgram(shaderProgram);

    //for the others
    unsigned int RectangleVertexShader;
    RectangleVertexShader = glCreateShader(GL_VERTEX_SHADER);

    //now that we have a vertex shader, let’s put the code text inside
    glShaderSource(RectangleVertexShader, 1, &RectangleVertexShaderSource, NULL);

    //and… Compile !
    glCompileShader(RectangleVertexShader);


    //Do the same with the fragment shader !
    unsigned int RectangleFragmentShader;
    RectangleFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(RectangleFragmentShader, 1, &RectangleFragmentShaderSource, NULL);
    glCompileShader(RectangleFragmentShader);

    unsigned int RectangleshaderProgram;
    RectangleshaderProgram = glCreateProgram();

    //now attach shaders to use to the program
    glAttachShader(RectangleshaderProgram, RectangleVertexShader);
    glAttachShader(RectangleshaderProgram, RectangleFragmentShader);

    //and link it 
    glLinkProgram(RectangleshaderProgram);

    //now that the program is complete, we can use it 
    glUseProgram(RectangleshaderProgram);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // ... Initialization code (done once (unless your object frequently changes)) ...
    // 1. bind Vertex Array Object
    glBindVertexArray(VAO);
    // 2. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    // 3. then set our vertex attributes pointers
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // Game loop
    bool isRunning = true;
    while (isRunning) 
    {
        /*if (SDL_KEYDOWN == SDL_SCANCODE_E)
        {
            isRunning = false;
        }*/

        // Get the time in seconds 
        float timeValue = (float)SDL_GetTicks() / 1000;


        /*float xPos = (sin(timeValue));
        float yPos = (sin(timeValue));

        int vertexHOffsetLoc = glGetUniformLocation(shaderProgram, "HOffset");
        int vertexVOffsetLoc = glGetUniformLocation(shaderProgram, "VOffset");
        glUseProgram(shaderProgram);
        glUniform1f(vertexHOffsetLoc, xPos);
        glUniform1f(vertexVOffsetLoc, yPos);*/

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

        // Draw
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen

        // Draw here
        glUseProgram(RectangleVertexShader);
        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); //Center Line
        glDrawArrays(GL_TRIANGLE_STRIP, 4, 4); //Left Padlle
        glDrawArrays(GL_TRIANGLE_STRIP, 8, 4); //Right Padlle

        //glUseProgram(shaderProgram);
        glDrawArrays(GL_TRIANGLE_STRIP, 12, 4); //Ball

        SDL_GL_SwapWindow(Window); // Swapbuffer
    }

    // Quit
    SDL_DestroyWindow(Window);
    SDL_GL_DeleteContext(Context);

    return 0;
}


string LoadShader(string fileName) 
{
    ifstream myFile;
    myFile.open(fileName);

    if (myFile.fail()) 
    {
        cerr << "Error - failed to open " << fileName << endl;
    }

    string fileText = "";
    string line = "";

    while (getline(myFile, line)) 
    {
        fileText += line + '\n';
    }

    return fileText;
}