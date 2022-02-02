#include "fileInclude.h"

const double radius = 10.0;

template <std::size_t size_sq, typename _Alloc>
class hash_matrix;

template <std::size_t size_sq, typename _Alloc>
std::ostream &operator<<(std::ostream &os, hash_matrix<size_sq, _Alloc> &obj);

template <std::size_t size_sq, typename _Alloc = std::allocator<std::list<std::pair<int, int>>>>
class hash_matrix
{
public:
    using value_type = std::list<std::pair<int, int>>;
    using allocator_type = _Alloc;
    using reference = value_type &;
    using const_reference = const value_type &;
    using size_type = std::size_t;
    using pointer = value_type *;
    using const_pointer = const value_type *;

protected:
    std::vector<value_type, allocator_type> _vecPtr;
    size_type height, width;

    std::pair<int, int> __tranform_coord(const std::pair<int, int> &point) const { return {point.first / size_sq, point.second / size_sq}; }

public:
    class iterator : public std::vector<value_type, allocator_type>::iterator
    {
    };
    class const_iterator : public std::vector<value_type, allocator_type>::const_iterator
    {
    };

    auto begin() { return _vecPtr.begin(); }

    auto end() { return _vecPtr.end(); }

    hash_matrix(size_type height, size_type width, const_reference val) : height(height), width(width)
    {
        _vecPtr.resize(height * width, val);
    }

    std::pair<size_type, size_type> get_size() const { return {height, width}; }

    auto find(const std::pair<int, int> &point) const
    {
        auto [x, y] = __tranform_coord(point);
        return std::find_if(_vecPtr[x * width + y].begin(), _vecPtr[x * width + y].end(), [&point](const auto &p1)
                            {
                         double deltaX = fabs(point.first - p1.first);
                         double deltaY = fabs(point.second - p1.second);
                         return (sqrt(deltaX * deltaX + deltaY * deltaY) < radius) ? true : false; });
    }

    auto find(int x, int y) const { return find({x, y}); }

    void emplace(const std::pair<int, int> &point)
    {
        auto [x, y] = __tranform_coord(point);
        _vecPtr[x * width + y].push_back(point);
    }

    ~hash_matrix()
    {
        if (!_vecPtr.empty())
        {
            _vecPtr.clear();
        }
    }

    friend std::ostream &operator<<<size_sq, _Alloc>(std::ostream &os, hash_matrix<size_sq, _Alloc> &obj);
};

template <std::size_t size_sq, typename _Alloc>
std::ostream &operator<<(std::ostream &os, hash_matrix<size_sq, _Alloc> &obj)
{
    for (const auto el : obj)
    {
        os << "{";
        for (const auto el1 : el)
        {
            os << "(" << el1.first << ", " << el1.second << ")";
        }
        os << "}";
    }
    return os;
}

int main()
{
    hash_matrix<10> obj(3, 10, {});
    obj.emplace({12, 34});
    obj.emplace({13, 35});
    obj.emplace({8, 70});
    std::cout << obj << std::endl;
    auto it = obj.find({14, 38});
    std::cout << it->first << "   " << it->second << std::endl;

    return 0;
}