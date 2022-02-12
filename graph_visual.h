#ifndef GRAPH_VISUAL
#define GRAPH_VISUAL

#include "vertex_visual.h"
#include "matrix_bool.h"

template <std::size_t size_sq, typename _Alloc = std::allocator<std::list<point_vertex>>>
class graph_visual : public vertexVisual<size_sq, _Alloc>
{
protected:
    std::vector<GLfloat> vecLines;              // массив вершин для отрисовки рёбер графа
    std::vector<GLuint> indexLines;             // массив индексов для отрисовки рёбер графа
    std::vector<GLfloat> vecColorLines;         // массив цветов для отрисовки рёбер
    std::list<point_vertex *> lightingVertexLi; //список указателей на подсвечивающиеся вершины
    GLuint curIndexLine;
    std::stack<void (graph_visual<size_sq, _Alloc>::*)()> stack_back; // стек для отмены действия

    bool_matrix bool_mx;                   // булева матрица для предсталения графа
    std::list<int> index_vertex_back;      // список индексов рёбер для стека
    std::list<point_vertex *> li_ptr_back; // список указателей, для возвращения удалённого ребра (для стека)

    void set_color(const GLfloat &R, const GLfloat &G, const GLfloat &B)
    {
        vecColorLines.emplace_back(R);
        vecColorLines.emplace_back(G);
        vecColorLines.emplace_back(B);
    }

    void __stack_pop_back_line()
    {
        int i1 = index_vertex_back.back();
        index_vertex_back.pop_back();
        int i2 = index_vertex_back.back();
        index_vertex_back.pop_back();
        bool_mx.set(i1, i2, false);

        this->repay_the_vertex();

        for (size_t i = 0; i < 4; ++i)
            vecLines.pop_back();

        indexLines.pop_back();
        indexLines.pop_back();

        for (std::size_t i = 0; i < 6; ++i)
            vecColorLines.pop_back();
        curIndexLine -= 2;
    }

    void __stack_pop_back_vertex()
    {
        bool_mx.erase(hash_matrix<size_sq, _Alloc>::_vecPtr[hash_matrix<size_sq, _Alloc>::liInd.back()]->back().indexArray);
        hash_matrix<size_sq, _Alloc>::pop_back_vertex();
        vertexVisual<size_sq, _Alloc>::__popBackVertex();
        graph_visual<size_sq, _Alloc>::repay_the_vertex();
    }

    void __stack_draw_line()
    {
        if (!lightingVertexLi.empty())
        {
            throw "error";
            return;
        }

        lightingVertexLi.push_back(li_ptr_back.back());
        li_ptr_back.pop_back();
        lightingVertexLi.push_back(li_ptr_back.back());
        li_ptr_back.pop_back();

        if (lightingVertexLi.size() != 2)
            return;

        bool_mx.set(lightingVertexLi.front()->indexArray, lightingVertexLi.back()->indexArray);
        index_vertex_back.push_back(lightingVertexLi.front()->indexArray);
        index_vertex_back.push_back(lightingVertexLi.back()->indexArray);

        while (!lightingVertexLi.empty())
        {
            indexLines.emplace_back(curIndexLine);
            curIndexLine += 1;
            vecLines.emplace_back(lightingVertexLi.back()->point.first);
            vecLines.emplace_back(lightingVertexLi.back()->point.second);
            vecColorLines.emplace_back(0.0f);
            vecColorLines.emplace_back(0.0f);
            vecColorLines.emplace_back(0.0f);
            vertexVisual<size_sq, _Alloc>::change_the_color_of_the_vertex(lightingVertexLi.back()->indexArray, 1.0f, 0.0f, 0.4f);
            lightingVertexLi.pop_back();
        }
        std::cout << bool_mx << std::endl;
    }

    bool __check_line(const glm::vec2 &a, const glm::vec2 &b, const glm::vec2 &p)
    {
        const glm::vec2 a2b = b - a;
        const glm::vec2 a2p = p - a;
        double area = a2b.x * a2p.y - a2p.x * a2b.y;

        if ((a2b.x * a2p.x < 0) || (a2b.y * a2p.y < 0))
        {
            return false;
        }
        if (glm::length(a2b) < glm::length(a2p))
        {
            return false;
        }

        if (glm::abs(area) < 400.0f)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

public:
    graph_visual(std::size_t height, std::size_t width) : vertexVisual<size_sq, _Alloc>::vertexVisual(height, width) { curIndexLine = 0; }

    void repay_the_vertex()
    {
        while (!lightingVertexLi.empty())
        {
            vertexVisual<size_sq, _Alloc>::change_the_color_of_the_vertex(lightingVertexLi.back()->indexArray, 1.0f, 0.0f, 0.4f);
            lightingVertexLi.pop_back();
        }
    }

    void set_the_line()
    {
        if (lightingVertexLi.size() != 2)
            return;

        bool_mx.set(lightingVertexLi.front()->indexArray, lightingVertexLi.back()->indexArray);
        index_vertex_back.push_back(lightingVertexLi.front()->indexArray);
        index_vertex_back.push_back(lightingVertexLi.back()->indexArray);

        while (!lightingVertexLi.empty())
        {
            indexLines.emplace_back(curIndexLine);
            curIndexLine += 1;
            vecLines.emplace_back(lightingVertexLi.back()->point.first);
            vecLines.emplace_back(lightingVertexLi.back()->point.second);
            vecColorLines.emplace_back(0.0f);
            vecColorLines.emplace_back(0.0f);
            vecColorLines.emplace_back(0.0f);
            vertexVisual<size_sq, _Alloc>::change_the_color_of_the_vertex(lightingVertexLi.back()->indexArray, 1.0f, 0.0f, 0.4f);
            lightingVertexLi.pop_back();
        }
        stack_back.emplace(graph_visual<size_sq, _Alloc>::__stack_pop_back_line);
        std::cout << bool_mx << std::endl;
    }

    point_vertex *find(const std::pair<int, int> &obj)
    {
        point_vertex *ptr = hash_matrix<size_sq, _Alloc>::find(obj);
        if (ptr == nullptr)
        {
            while (!lightingVertexLi.empty())
            {
                vertexVisual<size_sq, _Alloc>::change_the_color_of_the_vertex(lightingVertexLi.back()->indexArray, 1.0f, 0.0f, 0.4f);
                lightingVertexLi.pop_back();
            }
        }
        else
        {

            vertexVisual<size_sq, _Alloc>::change_the_color_of_the_vertex(ptr->indexArray, 1.0f, 0.0f, 1.0f);
            lightingVertexLi.emplace_back(ptr);

            if (lightingVertexLi.size() > 2)
            {
                vertexVisual<size_sq, _Alloc>::change_the_color_of_the_vertex(lightingVertexLi.front()->indexArray, 1.0f, 0.0f, 0.4f);
                lightingVertexLi.pop_front();
            }
        }

        return ptr;
    }

    void emplace(const std::pair<int, int> &obj)
    {
        vertexVisual<size_sq, _Alloc>::emplace(obj);
        stack_back.emplace(graph_visual<size_sq, _Alloc>::__stack_pop_back_vertex);
        bool_mx.resize(obj);
    }

    void undo_action()
    {
        if (!stack_back.empty())
        {
            (this->*stack_back.top())();
            stack_back.pop();
        }
        else
        {
            std::cout << "stack empty\n";
        }
        std::cout << "vec_lines = " << vecLines.size() << '\n'
                  << "vec_index = " << indexLines.size() << '\n'
                  << "vec_color = " << vecColorLines.size() << '\n';
    }

    void delte_line(const std::pair<int, int> &point)
    {
        int ind = -1;
        for (std::size_t i = 0; i < vecLines.size(); i += 4)
        {
            glm::vec2 a = {vecLines[i], vecLines[i + 1]};
            glm::vec2 b = {vecLines[i + 2], vecLines[i + 3]};
            glm::vec2 p = {point.first, point.second};
            if (__check_line(a, b, p))
            {
                point_vertex *pv1 = hash_matrix<size_sq, _Alloc>::find({vecLines[i], vecLines[i + 1]});
                point_vertex *pv2 = hash_matrix<size_sq, _Alloc>::find({vecLines[i + 2], vecLines[i + 3]});
                bool_mx.set(pv1->indexArray, pv2->indexArray, false);
                li_ptr_back.push_back(pv1);
                li_ptr_back.push_back(pv2);
                stack_back.emplace(graph_visual<size_sq, _Alloc>::__stack_draw_line);
                ind = i;
                break;
            }
        }

        if (ind == -1)
        {
            return;
        }

        vecLines.erase(vecLines.begin() + ind, vecLines.begin() + ind + 4);

        for (std::size_t i = 0; i < 6; ++i)
            vecColorLines.pop_back();

        indexLines.pop_back();
        indexLines.pop_back();

        curIndexLine -= 2;
    }

    void delete_line_vertex(const std::pair<int, int> &point1, const std::pair<int, int> &point2)
    {
      //  std::cout << point2.first << " " << point2.second << std::endl;
        for (int i = 0; i < vecLines.size(); i += 4)
        {
            if ((vecLines[i] == point1.first && vecLines[i + 1] == point1.second &&
                 vecLines[i + 2] == point2.first && vecLines[i + 3] == point2.second) ||
                (vecLines[i] == point2.first && vecLines[i + 1] == point2.second &&
                 vecLines[i + 2] == point1.first && vecLines[i + 3] == point1.second))
            {
                vecLines.erase(vecLines.begin() + i, vecLines.begin() + i + 4);
                indexLines.pop_back();
                indexLines.pop_back();
                for (std::size_t i = 0; i < 6; ++i)
                    vecColorLines.pop_back();
                curIndexLine -= 2;
                return;
            }
        }
    }

    void delte_vertex(point_vertex *ptr)
    {

        std::list<std::pair<int, int>> li_point = bool_mx.comp_erase(ptr->indexArray);
        for (const auto &el : li_point)
        {
            // точное нахождение вершины
            point_vertex *p = hash_matrix<size_sq, _Alloc>::exact_location(el);
            // удаление
            delete_line_vertex(p->point, ptr->point);
        }
    }

    void delete_for_clck(const std::pair<int, int> &point)
    {
        point_vertex *ptr = vertexVisual<size_sq, _Alloc>::__clear_vertex(point);
        if (ptr == nullptr)
        {
            delte_line(point);
        }
        else
        {
            delte_vertex(ptr);
        }

        // delete_line
        /*
        int ind = -1;
        for (std::size_t i = 0; i < vecLines.size(); i += 4)
        {
            glm::vec2 a = {vecLines[i], vecLines[i + 1]};
            glm::vec2 b = {vecLines[i + 2], vecLines[i + 3]};
            glm::vec2 p = {point.first, point.second};
            if (__check_line(a, b, p))
            {
                point_vertex *pv1 = hash_matrix<size_sq, _Alloc>::find({vecLines[i], vecLines[i + 1]});
                point_vertex *pv2 = hash_matrix<size_sq, _Alloc>::find({vecLines[i + 2], vecLines[i + 3]});
                bool_mx.set(pv1->indexArray, pv2->indexArray, false);
                li_ptr_back.push_back(pv1);
                li_ptr_back.push_back(pv2);
                stack_back.emplace(graph_visual<size_sq, _Alloc>::__stack_draw_line);
                ind = i;
                break;
            }
        }

        if (ind == -1)
        {
            return;
        }

        vecLines.erase(vecLines.begin() + ind, vecLines.begin() + ind + 4);

        for (std::size_t i = 0; i < 6; ++i)
            vecColorLines.pop_back();

        indexLines.pop_back();
        indexLines.pop_back();

        curIndexLine -= 2;
        */
    }

    GLuint *index_line_data() { return indexLines.data(); }
    GLfloat *vertex_line_data() { return vecLines.data(); }
    GLfloat *color_line_data() { return vecColorLines.data(); }

    std::size_t index_line_size() { return indexLines.size(); }
    std::size_t vertex_line_size() { return vecLines.size(); }
    std::size_t color_line_size() { return vecColorLines.size(); }

    void clear()
    {
        if (!vecLines.empty())
        {
            vecLines.clear();
        }
        if (!indexLines.empty())
        {
            indexLines.clear();
        }
        if (!vecColorLines.empty())
        {
            vecColorLines.clear();
        }
        if (!lightingVertexLi.empty())
        {
            lightingVertexLi.clear();
        }
        while (!stack_back.empty())
        {
            stack_back.pop();
        }
    }

    void delteGraph()
    {
        for (auto &ptr : hash_matrix<size_sq, _Alloc>::_vecPtr)
        {
            if (ptr != nullptr)
            {
                ptr->clear();
                delete ptr;
            }
        }

        for (auto &ptr : hash_matrix<size_sq, _Alloc>::_vecPtr)
        {
            ptr = nullptr;
        }

        if (!hash_matrix<size_sq, _Alloc>::liInd.empty())
        {
            hash_matrix<size_sq, _Alloc>::liInd.clear();
        }

        vertexVisual<size_sq, _Alloc>::clear();

        graph_visual<size_sq, _Alloc>::clear();

        bool_mx.delete_matrix();

        index_vertex_back.clear();

        li_ptr_back.clear();

        point_vertex::index = 0;

        graph_visual<size_sq, _Alloc>::curIndexLine = 0;
    }

    ~graph_visual()
    {
        std::cout << "~Graph\n";
        clear();
    }
};

#endif