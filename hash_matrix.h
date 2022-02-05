#ifndef HASH_TABLE
#define HASH_TABLE

#include "point_vertex.h"

template <std::size_t size_sq, typename _Alloc>
class hash_matrix;

template <std::size_t size_sq, typename _Alloc>
std::ostream &operator<<(std::ostream &os, hash_matrix<size_sq, _Alloc> &obj);

template <std::size_t size_sq, typename _Alloc = std::allocator<std::list<point_vertex>>>
class hash_matrix
{
public:
    using value_type = std::list<point_vertex>;
    using allocator_type = _Alloc;
    using reference = value_type &;
    using const_reference = const value_type &;
    using size_type = std::size_t;
    using pointer = value_type *;
    using const_pointer = const value_type *;

protected:
    std::vector<pointer> _vecPtr;
    size_type height, width;
    std::list<int> liInd;

    std::pair<int, int> __tranform_coord(const std::pair<int, int> &point) const { return {point.first / size_sq, point.second / size_sq}; }

    std::size_t __get_index_vector(int y, int x) const { return static_cast<std::size_t>(y * width + x); }

        // ИЗМЕНИТЬ
    point_vertex *__find(const std::pair<int, int> &point) const
    {

        auto p = __tranform_coord(point);
        std::size_t ind = __get_index_vector(p.first, p.second);
        if (_vecPtr[ind] == nullptr)
        {
            return nullptr;
        }
        for (auto it = _vecPtr[ind]->begin(); it != _vecPtr[ind]->end(); ++it)
        {
            double deltaX = fabs(it->point.first - point.first);
            double deltaY = fabs(it->point.second - point.second);
            if (sqrt(deltaX * deltaX + deltaY * deltaY) < point_vertex::radius)
                return &(*it);
        }

        return nullptr;
    }

public:
    class iterator : public std::vector<value_type, allocator_type>::iterator
    {
    };
    class const_iterator : public std::vector<value_type, allocator_type>::const_iterator
    {
    };

    auto begin() { return _vecPtr.begin(); }

    auto end() { return _vecPtr.end(); }

    hash_matrix(size_type height, size_type width) : height(height/size_sq), width(width/size_sq)
    {
        _vecPtr.resize(height * width, nullptr);
    }

    std::pair<size_type, size_type> get_size() const { return {height, width}; }



    void emplace(const std::pair<int, int> &point)
    {
        auto p = __tranform_coord(point);
        std::size_t ind = __get_index_vector(p.first, p.second);
        if (_vecPtr[ind] == nullptr)
        {
            _vecPtr[ind] = new value_type();
        }
        _vecPtr[ind]->push_back(point);
        liInd.push_back(ind);
    }

    void clear()
    {
        for (auto &ptr : _vecPtr)
        {
            if (ptr != nullptr)
            {
                ptr->clear();
                delete ptr;
            }
        }

        if (!_vecPtr.empty())
        {
            _vecPtr.clear();
        }

        if (!liInd.empty())
        {
            liInd.clear();
        }
        point_vertex::index = 0;
    }

    ~hash_matrix()
    {
        std::cout << "~hash_matrix\n";
        clear();
    }

    void print() const
    {
        for (std::size_t i = 0; i < _vecPtr.size(); ++i)
        {
            if (_vecPtr[i] == nullptr)
            {
                std::cout << "{}";
            }
            else
            {
                std::cout << "{";

                for (auto it = _vecPtr[i]->begin(); it != _vecPtr[i]->end(); ++it)
                {
                    std::cout << *it << " ";
                }
                std::cout << "}";
            }
        }
        std::cout << '\n';
    }

    void pop_back_vertex()
    {
        _vecPtr[liInd.back()]->pop_back();
        liInd.pop_back();
        point_vertex::delete_point();
    }

    friend std::ostream &operator<<<size_sq, _Alloc>(std::ostream &os, hash_matrix<size_sq, _Alloc> &obj);
};

template <std::size_t size_sq, typename _Alloc>
std::ostream &operator<<(std::ostream &os, hash_matrix<size_sq, _Alloc> &obj)
{
    return os;
}

#endif