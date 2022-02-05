#ifndef GRAPH
#define GRAPH

#include "vertex_visual.h"

template <std::size_t size_sq, typename _Alloc = std::allocator<std::list<point_vertex>>>
class Graph : public vertexVisual<size_sq, _Alloc>
{
protected:
    std::vector<GLfloat> vecLines;
    std::vector<GLuint> indexLines;
    std::vector<GLfloat> vecColorLines;
    std::list<point_vertex *> lightingVertexLi;
    GLuint curIndexLine;
    // std::stack<std::function<void(Graph<size_sq, _Alloc>)>> stackBack;
    enum function : char
    {
        POP_VERTEX,
        POP_LINES
    };

    std::stack<function> stackBack;

    void set_color(const GLfloat &R, const GLfloat &G, const GLfloat &B)
    {
        vecColorLines.emplace_back(R);
        vecColorLines.emplace_back(G);
        vecColorLines.emplace_back(B);
    }

    void popBackLine()
    {
        this->repay_the_vertex();

        for (size_t i = 0; i < 4; ++i)
            vecLines.pop_back();

        indexLines.pop_back();
        indexLines.pop_back();

        for (std::size_t i = 0; i < 6; ++i)
            vecColorLines.pop_back();
        curIndexLine -= 2;
    }

    void pop_back_vertex()
    {
        hash_matrix<size_sq, _Alloc>::pop_back_vertex();
        vertexVisual<size_sq, _Alloc>::__popBackVertex();
        Graph<size_sq, _Alloc>::repay_the_vertex();
    }

public:
    Graph(std::size_t height, std::size_t width) : vertexVisual<size_sq, _Alloc>::vertexVisual(height, width) { curIndexLine = 0; }

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
        stackBack.emplace(POP_LINES);
    }

    point_vertex *find(const std::pair<int, int> &obj)
    {
        point_vertex *ptr = hash_matrix<size_sq, _Alloc>::__find(obj);
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
        stackBack.emplace(POP_VERTEX);
    }
    
        void undo_action()
        {
            if (!stackBack.empty())
            {
                if (stackBack.top() == POP_VERTEX)
                    Graph<size_sq, _Alloc>::pop_back_vertex();
                else if (stackBack.top() == POP_LINES)
                    popBackLine();

                stackBack.pop();
            }
        }
    
    GLuint *index_line_data() { return indexLines.data(); }
    GLfloat *vertex_line_data() { return vecLines.data(); }
    GLfloat *color_line_data() { return vecColorLines.data(); }

    std::size_t index_line_size() { return this->indexLines.size(); }
    std::size_t vertex_line_size() { return this->vecLines.size(); }
    std::size_t color_line_size() { return this->vecColorLines.size(); }
};

#endif