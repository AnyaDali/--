#ifndef MATRIX_BOOL
#define MATRIX_BOOL

#include "file_include.h"
#include "point_vertex.h"

class bool_matrix
{
protected:
    std::vector<bool> ptr;
    std::vector<std::pair<int, int>> vec_ptr_vertex;
    std::size_t n;

public:
    bool_matrix(std::size_t n = 0) : n(n)
    {
        ptr.resize(n * n, false);
    }

    bool get_value(std::size_t y, std::size_t x) const
    {
        return ptr[y * n + x];
    }

    std::size_t get_index(std::size_t y, std::size_t x) const
    {
        return y * n + x;
    }

    bool set(std::size_t y, std::size_t x, bool val = true)
    {
        ptr[y * n + x] = val;
        ptr[x * n + y] = val;
    }

    void resize(const std::pair<int, int> &point)
    {
        n += 1;
        int count_move = n - 1;
        ptr.resize(n * n, false);
        for (int i = ptr.size() - n - 1; i > 0; --i)
        {
            for (int j = 0; j < n - 1; ++j)
            {
                std::swap(ptr[i], ptr[i + count_move]);
                --i;
            }
            ++i;
            count_move -= 1;
        }
        set(n - 1, n - 1);

        vec_ptr_vertex.push_back(point);
    }

    void erase(std::size_t ind)
    {
        std::size_t count_move = 0;
        for (std::size_t i = 0; i < ind; ++i)
        {
            for (std::size_t j = 0; j < n; ++j)
            {
                if (j == ind)
                {
                    count_move += 1;
                }
                std::swap(ptr[get_index(i, j)], ptr[get_index(i, j) + count_move]);
            }
        }
        count_move += n;

        for (std::size_t i = ind; i < n; ++i)
        {
            for (std::size_t j = 0; j < n; ++j)
            {
                if (j == ind)
                {
                    count_move += 1;
                }
                std::swap(ptr[get_index(i, j)], ptr[get_index(i, j) + count_move]);
            }
        }

        n -= 1;
        ptr.resize(n * n);
    }

    std::list<std::pair<int, int>> comp_erase(const std::size_t &ind)
    {
       // std::cout << ind << std::endl;
        set(ind, ind, false);
        std::list<std::pair<int, int>> li_ind;
        for (std::size_t j = 0; j < n; ++j)
        {
            if (get_value(ind, j) == true)
            {
                set(ind, j, false);
                li_ind.push_back(vec_ptr_vertex[j]);
            }
        }
        for (const auto & el : li_ind)
        {
            std::cout << "el =  "<< el.first << " " << el.second << std::endl;
        }
        return li_ind;
    }

    void delete_matrix()
    {
        n = 0;
        if (!ptr.empty())
        {
            ptr.clear();
        }
    }

    ~bool_matrix()
    {
        std::cout << "~bool_matrix\n";
        delete_matrix();
    }

    std::size_t size_n() const { return n; }

    friend std::ostream &operator<<(std::ostream &os, bool_matrix &obj);
};

std::ostream &operator<<(std::ostream &os, bool_matrix &obj)
{
    for (std::size_t i = 0; i < obj.size_n(); ++i)
    {
        for (std::size_t j = 0; j < obj.size_n(); ++j)
        {
            os << obj.get_value(i, j) << ' ';
        }
        os << '\n';
    }

    return os;
}

#endif