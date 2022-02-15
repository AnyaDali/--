#include "graph_visual.h"

using namespace std;

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC *, HGLRC *);
void DisableOpenGL(HWND, HDC, HGLRC);

GLuint indVertex = 0;
GLuint indLine = 0;

enum typeButton
{
    _BUTTON_VERTEX,
    _BUTTON_LINE,
    _BUTTON_BACK,
    _BUTTON_CLEAR,
    _BUTTON_DELETE_GRAPH,
    _BUTTON_QUIT
};

int width = 800;
int length = 800;

GLint vecBackgroundMenu[] = {
    0, 0, 80, 0, 80, length, 0, length};

GLfloat vecBackgroundColorMenu[] = {
    0.0f, 0.0f, 0.7f, //
    0.0f, 0.0f, 0.7f, //
    0.0f, 0.0f, 0.1f, //
    0.0f, 0.0f, 0.1f  //
};

GLuint vecIndexMenu[] = {
    0, 1, 2, 2, 3, 0};

struct Button
{
    int type;
    int vecCoord2d[8];
    bool flagPressing;
    bool flagAiming;
    GLuint texture;
    GLint coordTexture[8];
};

vector<Button> vecBtns = {
    {_BUTTON_VERTEX, {5, 5, 70, 5, 70, 35, 5, 35}, false, false, 0, {0, 0, 1, 0, 1, 1, 0, 1}},
    {_BUTTON_LINE, {5, 45, 70, 45, 70, 75, 5, 75}, false, false, 0, {0, 0, 1, 0, 1, 1, 0, 1}},
    {_BUTTON_BACK, {5, 85, 70, 85, 70, 115, 5, 115}, false, false, 0, {0, 0, 1, 0, 1, 1, 0, 1}},
    {_BUTTON_CLEAR, {5, 125, 70, 125, 70, 155, 5, 155}, false, false, 0, {0, 0, 1, 0, 1, 1, 0, 1}},
    {_BUTTON_DELETE_GRAPH, {5, 165, 70, 165, 70, 195, 5, 195}, false, false, 0, {0, 0, 1, 0, 1, 1, 0, 1}},
    {_BUTTON_QUIT, {5, 205, 70, 205, 70, 235, 5, 235}, false, false, 0, {0, 0, 1, 0, 1, 1, 0, 1}}};

int vecIndexBottom[] = {0, 1, 2, 2, 3, 0};

GLuint backgroundTexture;
GLint vecVertexBackGround[8];
GLuint vecIndexBackground[] = {0, 1, 2, 2, 3, 0};
GLint coordTexBackground[] = {0, 0, 1, 0, 1, 1, 0, 1};
GLfloat vecColorsBackground[] = {0.5f, 0.5f, 0.5f,
                                 0.5f, 0.5f, 0.5f,
                                 0.5f, 0.5f, 0.5f,
                                 0.5f, 0.5f, 0.5f};

enum stateMouseLButtonDown
{
    _STATE_NOT_CHOSEN,
    _STATE_DRAW_THE_VERTEX,
    _STATE_DRAW_THE_LINE,
    _STATE_DELETE
};

int stateMoseL = _STATE_NOT_CHOSEN;

int countClick = 0;

graph_visual<40> obj(length, width);

void loadTex(const char *imgName, GLuint &texture)
{
    int width, height, cnt;
    unsigned char *data = SOIL_load_image(imgName, &width, &height, &cnt, 0);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                 cnt == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);

    glBindTexture(GL_TEXTURE_2D, 0);

    SOIL_free_image_data(data);
}

void loadBackgroundTex(const char *fName)
{
    loadTex(fName, backgroundTexture);
}

void loadImg(const vector<string> &imgNames)
{
    for (int i = 0; i < vecBtns.size(); ++i)
        loadTex(imgNames[i].c_str(), vecBtns[i].texture);
}

bool checkButtonArea(int x, int y, const Button &obj)
{
    return obj.vecCoord2d[0] <= x && x <= obj.vecCoord2d[2] && obj.vecCoord2d[1] <= y && y <= obj.vecCoord2d[7];
}

void ShowButtons()
{
    glEnable(GL_TEXTURE_2D);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    for (int i = 0; i < vecBtns.size(); ++i)
    {
        glBindTexture(GL_TEXTURE_2D, vecBtns[i].texture);
        glVertexPointer(2, GL_INT, 0, &vecBtns[i].vecCoord2d);

        glTexCoordPointer(2, GL_INT, 0, vecBtns[i].coordTexture);

        if (vecBtns[i].flagPressing)
        {
            glColor3f(1.0f, 0.0f, 1.0f);
        }
        else
        {
            if (vecBtns[i].flagAiming)
                glColor3f(1.0f, 0.0f, 0.5f);
            else
                glColor3f(0.5f, 0.0f, 0.5f);
        }
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, &vecIndexBottom);
    }
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
}

void ShowMenu()
{

    glLoadIdentity();
    glPushMatrix();
    glOrtho(0, width, length, 0, -1, 1);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_INT, 0, &vecBackgroundMenu);

    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(3, GL_FLOAT, 0, &vecBackgroundColorMenu);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, &vecIndexMenu);

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    ShowButtons();
    glPopMatrix();
}

void ShowTexBackground()
{
    glEnable(GL_TEXTURE_2D);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_INT, 0, vecVertexBackGround);

    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(3, GL_FLOAT, 0, vecColorsBackground);

    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glBindTexture(GL_TEXTURE_2D, backgroundTexture);
    glTexCoordPointer(2, GL_INT, 0, coordTexBackground);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, vecIndexBackground);

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisable(GL_TEXTURE_2D);
}

void setCoordForBackground()
{
    vecVertexBackGround[0] = 0;
    vecVertexBackGround[1] = 0;

    vecVertexBackGround[2] = width;
    vecVertexBackGround[3] = 0;

    vecVertexBackGround[4] = width;
    vecVertexBackGround[5] = length;

    vecVertexBackGround[6] = 0;
    vecVertexBackGround[7] = length;
}

void resizeTheWindow(LPARAM &lParam)
{
    width = LOWORD(lParam);
    length = HIWORD(lParam);
    glViewport(0, 0, width, length);
    float k = width / (float)length;
    glLoadIdentity();
    glOrtho(-k, k, -1, 1, -1, 1);
    setCoordForBackground();
}

// drawing: vertex
void DrawVertex()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, obj.data_vertex());
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(4, GL_FLOAT, 0, obj.data_color());
    glDrawElements(GL_TRIANGLES, obj.size_vec_index(), GL_UNSIGNED_INT, obj.data_index());
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

// drawing lines
void DrawLines()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, obj.vertex_line_data());
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(3, GL_FLOAT, 0, obj.color_line_data());
    glDrawElements(GL_LINES, obj.index_line_size(), GL_UNSIGNED_INT, obj.index_line_data());
    glDisableClientState(GL_COLOR_ARRAY);

    glDisableClientState(GL_VERTEX_ARRAY);
}

void emplaceVertexInVector(LPARAM lParam, float radius)
{
    std::pair<int, int> p = {LOWORD(lParam), HIWORD(lParam)};
    obj.emplace(p);
}

void setTheLines(LPARAM lParam)
{
    std::pair<int, int> p1 = {LOWORD(lParam), HIWORD(lParam)};
    obj.find(p1);
}

void setCoordSystem()
{
    // glLoadIdentity();
    glOrtho(0, width, length, 0, -1, 1);
    // glPopMatrix();
}

void delete_clck(LPARAM &lParam)
{
    std::pair<int, int> p = {LOWORD(lParam), HIWORD(lParam)};
    obj.delete_for_clck(p);
}

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;
    float theta = 0.0f;

    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "GLSample";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    ;

    if (!RegisterClassEx(&wcex))
        return 0;

    /* create main window */
    hwnd = CreateWindowEx(0,
                          "GLSample",
                          "OpenGL Sample",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          width,
                          length,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);

    std::cout << "size container graph = " << sizeof(obj) << std::endl;

    loadImg({"1.jpg", "2.jpg", "3.jpg", "5.png", "6.jpg", "4.jpg"});
    // loadBackgroundTex("11.jpg");

    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            /* OpenGL animation code goes here */

            glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
            glClear(GL_COLOR_BUFFER_BIT);
            setCoordSystem();
            // ShowTexBackground();

            DrawLines();
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            DrawVertex();
            glDisable(GL_BLEND);

            ShowMenu();

            glPopMatrix();

            SwapBuffers(hDC);

            Sleep(1);
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

    switch (uMsg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        break;
    case WM_LBUTTONDOWN:
        if (LOWORD(lParam) > 80)
        {
            if (stateMoseL == _STATE_DRAW_THE_VERTEX)
            {
                emplaceVertexInVector(lParam, point_vertex::radius);
            }
            else if (stateMoseL == _STATE_DRAW_THE_LINE)
            {
                setTheLines(lParam);
            }
            else if (stateMoseL == _STATE_DELETE)
            {
                delete_clck(lParam);
            }
        }
        else
        {
            if (checkButtonArea(LOWORD(lParam), HIWORD(lParam), vecBtns[_BUTTON_VERTEX]))
            {
                stateMoseL = _STATE_DRAW_THE_VERTEX;
                for (int i = 0; i < vecBtns.size(); ++i)
                    vecBtns[i].flagPressing = false;

                vecBtns[_BUTTON_VERTEX].flagPressing = true;
            }
            else if (checkButtonArea(LOWORD(lParam), HIWORD(lParam), vecBtns[_BUTTON_LINE]))
            {
                stateMoseL = _STATE_DRAW_THE_LINE;
                for (int i = 0; i < vecBtns.size(); ++i)
                    vecBtns[i].flagPressing = false;

                vecBtns[_BUTTON_LINE].flagPressing = true;
            }
            else if (checkButtonArea(LOWORD(lParam), HIWORD(lParam), vecBtns[_BUTTON_QUIT]))
            {
                PostQuitMessage(1);
            }
            else if (checkButtonArea(LOWORD(lParam), HIWORD(lParam), vecBtns[_BUTTON_BACK]))
            {
                for (int i = 0; i < vecBtns.size(); ++i)
                    vecBtns[i].flagPressing = false;

                obj.repay_the_vertex();
                obj.undo_action();

                stateMoseL = _STATE_NOT_CHOSEN;
            }
            else if (checkButtonArea(LOWORD(lParam), HIWORD(lParam), vecBtns[_BUTTON_DELETE_GRAPH]))
            {
                for (int i = 0; i < vecBtns.size(); ++i)
                    vecBtns[i].flagPressing = false;
                obj.delteGraph();
                stateMoseL = _STATE_NOT_CHOSEN;
            }
            else if (checkButtonArea(LOWORD(lParam), HIWORD(lParam), vecBtns[_BUTTON_CLEAR]))
            {
                for (int i = 0; i < vecBtns.size(); ++i)
                    vecBtns[i].flagPressing = false;
                stateMoseL = _STATE_DELETE;
                obj.repay_the_vertex();
                vecBtns[_BUTTON_CLEAR].flagPressing = true;
            }
        }

        break;

    case WM_MOUSEMOVE:
        for (int i = 0; i < vecBtns.size(); ++i)
            vecBtns[i].flagAiming = checkButtonArea(LOWORD(lParam), HIWORD(lParam), vecBtns[i]);
        break;

    case WM_DESTROY:
        return 0;

    case WM_SIZE:
        resizeTheWindow(lParam);
        vecBackgroundMenu[5] = vecBackgroundMenu[7] = length;
        break;

    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case VK_ESCAPE:
            PostQuitMessage(0);
            break;
        case VK_SPACE:
            obj.set_the_line();
            break;
        }
    }
    break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void EnableOpenGL(HWND hwnd, HDC *hDC, HGLRC *hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL(HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}