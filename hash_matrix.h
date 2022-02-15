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

    point_vertex *__find_list(const std::pair<int, int> &point, const std::size_t ind) const
    {
        if (_vecPtr[ind] == nullptr)
        {
            return nullptr;
        }

        for (auto it = _vecPtr[ind]->begin(); it != _vecPtr[ind]->end(); ++it)
        {
            if (it->flag_aiming != false)
            {

                double deltaX = fabs(it->point.first - point.first);
                double deltaY = fabs(it->point.second - point.second);
                if (sqrt(deltaX * deltaX + deltaY * deltaY) < point_vertex::radius)
                    return &(*it);
            }
        }

        return nullptr;
    }
    /*
        auto __find_list_iter(const std::pair<int, int> &point, const std::size_t &ind) const
        {
            auto it = _vecPtr[ind]->begin();
            for (; it != _vecPtr[ind]->end(); ++it)
            {
                std::cout << "+" << std::endl;
                //if (it->flag_aiming != false)
               // {
                    double deltaX = fabs(it->point.first - point.first);
                    double deltaY = fabs(it->point.second - point.second);
                    if (sqrt(deltaX * deltaX + deltaY * deltaY) < point_vertex::radius)
                        return it;
                //}
            }
            if (it == _vecPtr[ind]->end())
                std::cout << "not_found\n";
            else
                std::cout << "check = " << *it << std::endl;

            return it;
        }
        */

public:
    auto begin() { return _vecPtr.begin(); }

    auto end() { return _vecPtr.end(); }

    hash_matrix(size_type height, size_type width) : height(height / size_sq), width(width / size_sq)
    {
        _vecPtr.resize(this->height * this->width, nullptr);
    }

    std::pair<size_type, size_type> get_size() const { return {height, width}; }

    // О(1)
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

    point_vertex *exact_location(const std::pair<int, int> &point)
    {
        auto p = __tranform_coord(point);
        std::size_t ind = __get_index_vector(p.first, p.second);

        for (std::list<point_vertex>::iterator it = _vecPtr[ind]->begin(); it != _vecPtr[ind]->end(); ++it)
        {
            if (it->point.first == point.first && it->point.second == point.second)
                return &(*it);
        }
        return nullptr;
    }

    ~hash_matrix()
    {
        std::cout << "~hash_matrix\n";
        clear();
    }

    point_vertex *find(const std::pair<int, int> &point)
    {
        auto p = __tranform_coord(point);
        std::size_t ind = __get_index_vector(p.first, p.second);
        point_vertex *ptr = hash_matrix<size_sq, _Alloc>::__find_list(point, ind);
        auto copy_point = point;
        bool flag1 = false, flag2 = false;
        if (ptr == nullptr)
        {
            if (point.first % size_sq < point_vertex::radius)
            {
                auto x1 = __tranform_coord({point.first - 10, point.second});
                ind = __get_index_vector(x1.first, x1.second);
                ptr = hash_matrix<size_sq, _Alloc>::__find_list(point, ind);
                copy_point.first -= 10;
                flag1 = true;
            }
            else if (point.first % size_sq > size_sq - point_vertex::radius)
            {
                auto x1 = __tranform_coord({point.first + 10, point.second});
                ind = __get_index_vector(x1.first, x1.second);
                ptr = hash_matrix<size_sq, _Alloc>::__find_list(point, ind);
                copy_point.first += 10;
                flag1 = true;
            }
        }
        if (ptr == nullptr)
        {
            if (point.second % size_sq < point_vertex::radius)
            {
                auto x1 = __tranform_coord({point.first, point.second - 10});
                ind = __get_index_vector(x1.first, x1.second);
                ptr = hash_matrix<size_sq, _Alloc>::__find_list(point, ind);
                copy_point.second -= 10;
                flag2 = true;
            }
            else if (point.second % size_sq > size_sq - point_vertex::radius)
            {
                auto x1 = __tranform_coord({point.first, point.second + 10});
                ind = __get_index_vector(x1.first, x1.second);
                ptr = hash_matrix<size_sq, _Alloc>::__find_list(point, ind);
                copy_point.second += 10;
                flag2 = true;
            }
        }
        if (ptr == nullptr)
        {
            if (flag1 && flag2)
            {
                auto x1 = __tranform_coord(copy_point);
                ind = __get_index_vector(x1.first, x1.second);
                ptr = hash_matrix<size_sq, _Alloc>::__find_list(point, ind);
                if (ptr != nullptr)
                    std::cout << "transform _ XY = " << *ptr << std::endl;
            }
        }

        return ptr;
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

    point_vertex *erase(const std::pair<int, int> &point)
    {
        auto p = hash_matrix<size_sq, _Alloc>::__tranform_coord(point);
        std::size_t ind = hash_matrix<size_sq, _Alloc>::__get_index_vector(p.first, p.second);
        point_vertex *ptr = hash_matrix<size_sq, _Alloc>::__find_list(point, ind);
        auto copy_point = point;
        bool flag1 = false, flag2 = false;
        if (ptr == nullptr)
        {
            if (point.first % size_sq < point_vertex::radius)
            {
                auto x1 = __tranform_coord({point.first - 10, point.second});
                ind = __get_index_vector(x1.first, x1.second);
                ptr = hash_matrix<size_sq, _Alloc>::__find_list(point, ind);
                copy_point.first -= 10;
                flag1 = true;
            }
            else if (point.first % size_sq > size_sq - point_vertex::radius)
            {
                auto x1 = __tranform_coord({point.first + 10, point.second});
                ind = __get_index_vector(x1.first, x1.second);
                ptr = hash_matrix<size_sq, _Alloc>::__find_list(point, ind);
                copy_point.first += 10;
                flag1 = true;
            }
        }
        if (ptr == nullptr)
        {
            if (point.second % size_sq < point_vertex::radius)
            {
                auto x1 = __tranform_coord({point.first, point.second - 10});
                ind = __get_index_vector(x1.first, x1.second);
                ptr = hash_matrix<size_sq, _Alloc>::__find_list(point, ind);
                copy_point.second -= 10;
                flag2 = true;
            }
            else if (point.second % size_sq > size_sq - point_vertex::radius)
            {
                auto x1 = __tranform_coord({point.first, point.second + 10});
                ind = __get_index_vector(x1.first, x1.second);
                ptr = hash_matrix<size_sq, _Alloc>::__find_list(point, ind);
                copy_point.second += 10;
                flag2 = true;
            }
        }
        if (ptr == nullptr)
        {
            if (flag1 && flag2)
            {
                auto x1 = __tranform_coord(copy_point);
                ind = __get_index_vector(x1.first, x1.second);
                ptr = hash_matrix<size_sq, _Alloc>::__find_list(point, ind);
                if (ptr != nullptr)
                    std::cout << "transform _ XY = " << *ptr << std::endl;
            }
        }

        if (ptr != nullptr)
        {
            ptr->flag_aiming = false;
        }

        return ptr;
    }

    void pop_back_vertex()
    {
        _vecPtr[liInd.back()]->pop_back();
        liInd.pop_back();
        // point_vertex::delete_point();
    }

    friend std::ostream &operator<<<size_sq, _Alloc>(std::ostream &os, hash_matrix<size_sq, _Alloc> &obj);
};

template <std::size_t size_sq, typename _Alloc>
std::ostream &operator<<(std::ostream &os, hash_matrix<size_sq, _Alloc> &obj)
{
    return os;
}

#endif