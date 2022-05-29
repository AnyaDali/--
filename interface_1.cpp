#include "file_include.h"
#include "interface_graph.h"

class form;

enum typeButton
{
    _BUTTON_VERTEX,
    _BUTTON_LINE,
    _BUTTON_BACK,
    _BUTTON_CLEAR,
    _BUTTON_DELETE_GRAPH,
    _BUTTON_SAVE,
    _BUTTON_LOAD,
    _BUTTON_ALGORITHM_1,
    _BUTTON_ALGORITHM_2,
    _BUTTON_QUIT
};

int width = 1000;
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

std::vector<Button> vecBtns = {
    {_BUTTON_VERTEX, {5, 5, 70, 5, 70, 35, 5, 35}, false, false, 0, {0, 0, 1, 0, 1, 1, 0, 1}},
    {_BUTTON_LINE, {5, 45, 70, 45, 70, 75, 5, 75}, false, false, 0, {0, 0, 1, 0, 1, 1, 0, 1}},
    {_BUTTON_BACK, {5, 85, 70, 85, 70, 115, 5, 115}, false, false, 0, {0, 0, 1, 0, 1, 1, 0, 1}},
    {_BUTTON_CLEAR, {5, 125, 70, 125, 70, 155, 5, 155}, false, false, 0, {0, 0, 1, 0, 1, 1, 0, 1}},
    {_BUTTON_DELETE_GRAPH, {5, 165, 70, 165, 70, 195, 5, 195}, false, false, 0, {0, 0, 1, 0, 1, 1, 0, 1}},
    {_BUTTON_SAVE, {5, 205, 70, 205, 70, 235, 5, 235}, false, false, 0, {0, 0, 1, 0, 1, 1, 0, 1}},
    {_BUTTON_LOAD, {5, 245, 70, 245, 70, 275, 5, 275}, false, false, 0, {0, 0, 1, 0, 1, 1, 0, 1}},
    {_BUTTON_ALGORITHM_1, {5, 400, 70, 400, 70, 430, 5, 430}, false, false, 0, {0, 0, 1, 0, 1, 1, 0, 1}},
    {_BUTTON_ALGORITHM_2, {5, 440, 70, 440, 70, 470, 5, 470}, false, false, 0, {0, 0, 1, 0, 1, 1, 0, 1}},
    {_BUTTON_QUIT, {5, 500, 70, 500, 70, 530, 5, 530}, false, false, 0, {0, 0, 1, 0, 1, 1, 0, 1}}};

int vecIndexBottom[] = {0, 1, 2, 2, 3, 0};

GLuint backgroundTexture;
GLint vecVertexBackGround[8];
GLuint vecIndexBackground[] = {0, 1, 2, 2, 3, 0};
GLint coordTexBackground[] = {0, 0, 1, 0, 1, 1, 0, 1};
GLfloat vecColorsBackground[] = {0.7f, 0.7f, 0.7f,
                                 0.7f, 0.7f, 0.7f,
                                 0.7f, 0.7f, 0.7f,
                                 0.7f, 0.7f, 0.7f};

enum stateMouseLButtonDown
{
    _STATE_NOT_CHOSEN,
    _STATE_DRAW_THE_VERTEX,
    _STATE_DRAW_THE_LINE,
    _STATE_DELETE
};

int stateMoseL = _STATE_NOT_CHOSEN;

GLFWcursor *hand_cursor;
GLFWcursor *cross_cursor;

interface_graph<40> obj(length, width);

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

void loadImg(const std::vector<std::string> &imgNames)
{
    for (size_t i = 0; i < vecBtns.size(); ++i)
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
    for (size_t i = 0; i < vecBtns.size(); ++i)
    {
        glBindTexture(GL_TEXTURE_2D, vecBtns[i].texture);
        glVertexPointer(2, GL_INT, 0, &vecBtns[i].vecCoord2d);

        glTexCoordPointer(2, GL_INT, 0, vecBtns[i].coordTexture);

        if (vecBtns[i].flagPressing)
        {
            glColor3f(0.5f, 0.5f, 0.5f);
        }
        else
        {
            if (vecBtns[i].flagAiming)
                glColor3f(0.6f, 0.6f, 0.6f);
            else
                glColor3f(0.8f, 0.8f, 0.8f);
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

void DrawVertex()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, obj.vertex_data());
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(4, GL_FLOAT, 0, obj.vertex_color_data());
    glDrawElements(GL_TRIANGLES, obj.vertex_index_size(), GL_UNSIGNED_INT, obj.vertex_index_data());
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

// drawing lines
void DrawLines()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, obj.lines_data());
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(3, GL_FLOAT, 0, obj.lines_color_data());
    glDrawElements(GL_LINES, obj.lines_index_size(), GL_UNSIGNED_INT, obj.lines_index_data());
    glDisableClientState(GL_COLOR_ARRAY);

    glDisableClientState(GL_VERTEX_ARRAY);
}

void draw_vertex(int x, int y)
{
    std::pair<int, int> p = std::make_pair(x, y);
    obj.add_vertex(p);
}

void chose_vertex(int x, int y)
{
    std::pair<int, int> p = std::make_pair(x, y);
    obj.find_vertex(p);
}

void setCoordSystem()
{
    glOrtho(0, width, length, 0, -1, 1);
}

void delete_clck(int x, int y)
{
    std::pair<int, int> p = std::make_pair(x, y);
    obj.erase(p);
}

static void key_callback(GLFWwindow *window, int key, int, int action, int)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        obj.add_line();
}

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (xpos < 80)
    {
        glfwSetCursor(window, NULL);
        for (size_t i = 0; i < vecBtns.size(); ++i)
        {
            vecBtns[i].flagAiming = checkButtonArea(xpos, ypos, vecBtns[i]);
            if (vecBtns[i].flagAiming)
                glfwSetCursor(window, hand_cursor);
        }
    }
    else
    {
        if (stateMoseL == _STATE_DRAW_THE_LINE ||
            stateMoseL == _STATE_DRAW_THE_VERTEX ||
            stateMoseL == _STATE_DELETE)
        {

            glfwSetCursor(window, cross_cursor);
        }
        else
        {
            glfwSetCursor(window, NULL);
        }
    }
}

void cursor_enter_callback(GLFWwindow *, int entered)
{
    if (entered)
    {
        // std::cout << "The cursor entered the content area of the window\n";
    }
    else
    {
        // std::cout << "The cursor left the content area of the window\n";
    }
}

void button_pressing(int *arr)
{
    arr[0] += 1;
    arr[1] += 1;
    arr[2] -= 1;
    arr[3] += 1;

    arr[4] -= 1;
    arr[5] -= 1;
    arr[6] += 1;
    arr[7] -= 1;
}

void button_disable(int *arr)
{

    arr[0] -= 1;
    arr[1] -= 1;
    arr[2] += 1;
    arr[3] -= 1;

    arr[4] += 1;
    arr[5] += 1;
    arr[6] -= 1;
    arr[7] += 1;
}

void disable_interface_button()
{
    for (size_t i = 0; i < vecBtns.size(); ++i)
    {
        if (vecBtns[i].flagPressing)
            button_disable(vecBtns[i].vecCoord2d);
    }
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int)
{
    double x, y;
    glfwGetCursorPos(window, &x, &y);

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        if (x > 80)
        {
            if (stateMoseL == _STATE_DRAW_THE_VERTEX)
            {
                draw_vertex(x, y);
            }
            else if (stateMoseL == _STATE_DRAW_THE_LINE)
            {
                chose_vertex(x, y);
            }
            else if (stateMoseL == _STATE_DELETE)
            {
                delete_clck(x, y);
            }
        }
        else
        {
            if (checkButtonArea(x, y, vecBtns[_BUTTON_VERTEX]))
            {
                disable_interface_button();
                glfwSetCursor(window, hand_cursor);
                stateMoseL = _STATE_DRAW_THE_VERTEX;
                for (size_t i = 0; i < vecBtns.size(); ++i)
                    vecBtns[i].flagPressing = false;

                button_pressing(vecBtns[_BUTTON_VERTEX].vecCoord2d);

                vecBtns[_BUTTON_VERTEX].flagPressing = true;
            }
            else if (checkButtonArea(x, y, vecBtns[_BUTTON_LINE]))
            {
                disable_interface_button();
                stateMoseL = _STATE_DRAW_THE_LINE;
                for (size_t i = 0; i < vecBtns.size(); ++i)
                    vecBtns[i].flagPressing = false;

                button_pressing(vecBtns[_BUTTON_LINE].vecCoord2d);
                vecBtns[_BUTTON_LINE].flagPressing = true;
            }
            else if (checkButtonArea(x, y, vecBtns[_BUTTON_QUIT]))
            {
                disable_interface_button();
                glfwSetWindowShouldClose(window, GL_TRUE);
            }
            else if (checkButtonArea(x, y, vecBtns[_BUTTON_BACK]))
            {
                disable_interface_button();
                for (size_t i = 0; i < vecBtns.size(); ++i)
                    vecBtns[i].flagPressing = false;

                obj.repay_the_vertex();
                // obj.undo_action();

                stateMoseL = _STATE_NOT_CHOSEN;
            }
            else if (checkButtonArea(x, y, vecBtns[_BUTTON_DELETE_GRAPH]))
            {
                disable_interface_button();
                for (size_t i = 0; i < vecBtns.size(); ++i)
                    vecBtns[i].flagPressing = false;

                obj.clear();
                stateMoseL = _STATE_NOT_CHOSEN;
            }
            else if (checkButtonArea(x, y, vecBtns[_BUTTON_CLEAR]))
            {
                disable_interface_button();
                for (size_t i = 0; i < vecBtns.size(); ++i)
                    vecBtns[i].flagPressing = false;
                stateMoseL = _STATE_DELETE;

                button_pressing(vecBtns[_BUTTON_CLEAR].vecCoord2d);
                vecBtns[_BUTTON_CLEAR].flagPressing = true;
            }
            else if (checkButtonArea(x, y, vecBtns[_BUTTON_ALGORITHM_1]))
            {
                disable_interface_button();
                for (size_t i = 0; i < vecBtns.size(); ++i)
                    vecBtns[i].flagPressing = false;

                obj.Dsatur_alg();
            }
            else if (checkButtonArea(x, y, vecBtns[_BUTTON_ALGORITHM_2]))
            {
                disable_interface_button();
                for (size_t i = 0; i < vecBtns.size(); ++i)
                    vecBtns[i].flagPressing = false;
                obj.Brown_alg();
            }
            else if (checkButtonArea(x, y, vecBtns[_BUTTON_SAVE]))
            {
                disable_interface_button();
                for (size_t i = 0; i < vecBtns.size(); ++i)
                    vecBtns[i].flagPressing = false;

                std::string str;
                std::cout << "enter file name: ";
                std::cin >> str;
                obj.save(std::move(str));
            }
            else if (checkButtonArea(x, y, vecBtns[_BUTTON_LOAD]))
            {
                disable_interface_button();
                for (size_t i = 0; i < vecBtns.size(); ++i)
                    vecBtns[i].flagPressing = false;
                std::string str;
                std::cout << "enter file name: ";
                std::cin >> str;
                obj.load(std::move(str));
            }
        }
    }
}

void load_icon(GLFWwindow *window, const std::string &img_name1, const std::string &img_name2)
{
    int chanels1, chanels2;
    GLFWimage data[2];
    data[0].pixels = SOIL_load_image(img_name1.c_str(), &data[0].width, &data[0].height, &chanels1, 0);
    data[1].pixels = SOIL_load_image(img_name2.c_str(), &data[1].width, &data[1].height, &chanels2, 0);
    glfwSetWindowIcon(window, 2, data);
    SOIL_free_image_data(data[0].pixels);
    SOIL_free_image_data(data[1].pixels);
}


int main()
{
    if (glfwInit() == GLFW_FALSE)
    {
        std::cout << "GLFWINIT::ERROR\n";
        return -1;
    }
    // установка ограничений окна
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);

    GLFWwindow *window = glfwCreateWindow(1000, 800, "graph", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // glfwSetWindowSizeLimits(window, 400, 400, 1000, 800);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    // cursor
    hand_cursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
    cross_cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);

    glfwSetCursorEnterCallback(window, cursor_enter_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    // glfwSetWindowIcon();

    // Define the viewport dimensions
    glfwGetFramebufferSize(window, &width, &length);
    glViewport(0, 0, width, length);



    loadImg({"1.jpg", "2.jpg", "3.jpg", "5.png", "clear_icon.png", "save_icon.png", "load_icon.png", "alg_1.jpg", "alg_2.jpg", "4.jpg"});
    load_icon(window, "lable1.png", "lable2.png");

   // form ptr;

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Render
        // Clear the colorbuffer
        glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        setCoordSystem();
        DrawLines();
        DrawVertex();
        ShowMenu();

        glfwSwapBuffers(window);
        
        glfwSwapInterval(1);
    }
    glfwDestroyCursor(hand_cursor);
    glfwDestroyCursor(cross_cursor);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
