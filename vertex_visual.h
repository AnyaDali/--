#ifndef VERTEX_VISUAL
#define VERTEX_VISUAL

#include "hash_matrix.h"

template <std::size_t size_sq, typename _Alloc = std::allocator<std::list<point_vertex>>>
class vertexVisual : public hash_matrix<size_sq, _Alloc>
{
protected:
    std::vector<GLfloat> vecVertex;
    std::vector<GLfloat> vecColor;
    std::vector<GLuint> vecIndexVertex;
    std::size_t indVertex;

    void __setPointColor(const GLfloat &R, const GLfloat &G, const GLfloat &B)
    {
        vecColor.emplace_back(R);
        vecColor.emplace_back(G);
        vecColor.emplace_back(B);
    }

    void __popBackVertex()
    {
        for (std::size_t i = 0; i < 24; ++i)
            vecVertex.pop_back();
        for (std::size_t i = 0; i < 33; ++i)
            vecIndexVertex.pop_back();
        for (std::size_t i = 0; i < 36; ++i)
            vecColor.pop_back();
        indVertex -= 12;
    }

    void __emplaceVertexInVector(const std::pair<int, int> &p)
    {
        auto  cx = p.first, cy = p.second;
        GLfloat x, y;
        float cnt = 10;
        float a = M_PI * 2 / cnt;
        vecVertex.emplace_back(cx);
        vecVertex.emplace_back(cy);
        GLuint indCenter = indVertex;
        __setPointColor(1.0f, 0.0f, 0.4f);

        for (std::size_t i = 0; i < cnt + 1; ++i)
        {
            x = sin(a * i) * point_vertex::radius;
            y = cos(a * i) * point_vertex::radius;
            vecVertex.emplace_back(x + cx);
            vecVertex.emplace_back(y + cy);
            __setPointColor(1.0f, 0.0f, 0.4f);

            vecIndexVertex.emplace_back(indCenter);
            vecIndexVertex.emplace_back(indVertex + 1);
            vecIndexVertex.emplace_back(indVertex + 2);

            ++indVertex;
        }
        vecIndexVertex[vecIndexVertex.size() - 1] = indCenter + 1;
        ++indVertex;
    }

public:
    vertexVisual(std::size_t height, std::size_t width) : hash_matrix<size_sq, _Alloc>::hash_matrix(height, width), indVertex(0ULL) {}

    void emplace(const std::pair<int, int> &val)
    {
        vertexVisual::__emplaceVertexInVector(val);
        hash_matrix<size_sq, _Alloc>::emplace(val);
    }

    void clear()
    {
        if (!vecVertex.empty())
            vecVertex.clear();

        if (!vecColor.empty())
            vecColor.clear();

        if (!vecIndexVertex.empty())
            vecIndexVertex.clear();
    }

    void change_the_color_of_the_vertex(const GLuint &indVert, const GLfloat &R, const GLfloat &G, const GLfloat &B)
    {
        std::size_t i = indVert * 36;
        std::size_t len = i + 36;

        for (; i < len; i += 3)
        {
            vecColor[i] = R;
            vecColor[i + 1] = G;
            vecColor[i + 2] = B;
        }
    }

    ~vertexVisual()
    {
        std::cout << "~vertexVisual\n";
        vertexVisual::clear();
    }

    std::size_t size_vec_color() { return this->vecColor.size(); }
    std::size_t size_vec_vertex() { return this->vecVertex.size(); }
    std::size_t size_vec_index() { return this->vecIndexVertex.size(); }

    GLfloat *data_color() { return this->vecColor.data(); }
    GLfloat *data_vertex() { return this->vecVertex.data(); }
    GLuint *data_index() { return this->vecIndexVertex.data(); }
};

#endif
