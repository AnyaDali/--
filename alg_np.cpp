#include "graph_model.h"

struct info
{
    std::map<int, bool> val;

    void emplace(int &_val, bool f) { val.emplace(_val, f); }

    void used(size_t ind) { val[ind] = false; }

    bool empty() const noexcept { return val.empty(); }

    ~info() {}
};

class adj_colors
{
protected:
    std::vector<std::list<int *>> adj_list;
    std::vector<int *> _vec_ptr;

    void __init_ptr(size_t n)
    {
        _vec_ptr.resize(n);
        for (size_t i = 0; i < n; ++i)
        {
            _vec_ptr[i] = new int(-1);
        }
    }

public:
    friend std::ostream &operator<<(std::ostream &os, adj_colors &obj);

    adj_colors(graph_model &obj)
    {
        __init_ptr(obj.count_of_vertex());
        adj_list.resize(obj.count_of_vertex());
        for (size_t i = 0; i < obj.count_of_vertex(); ++i)
        {
            for (auto &el : obj[i])
            {
                adj_list[i].emplace_back(_vec_ptr[*el]);
            }
        }
    }

    void paint_the_vertex(size_t i, int val)
    {
        *(_vec_ptr[i]) = val;
    }

    int get_free_index_color(size_t i)
    {
        for (int j = 0; true; ++j)
        {
            auto it = std::find_if(adj_list[i].begin(), adj_list[i].end(), [&](const auto &el)
                                   { return (*el == j) ? true : false; });

            if (it == adj_list[i].end())
            {
                return j;
            }
        }
    }

    const int *find(size_t i, int &__val) const
    {
        auto it = std::find_if(adj_list[i].begin(), adj_list[i].end(), [&](const auto &el)
                               { return (*el == __val) ? true : false; });

        if (it == adj_list[i].end())
            return nullptr;
        else
            return *it;
    }

    bool contained(size_t i, int &__val) const
    {
        auto it = std::find_if(adj_list[i].begin(), adj_list[i].end(), [&](const auto &el)
                               { return (*el == __val) ? true : false; });

        if (it == adj_list[i].end())
            return false;
        else
            return true;
    }

    const std::list<int *> &operator[](size_t ind) const noexcept { return adj_list[ind]; }

    const std::vector<int *> &get_li_color() const noexcept { return _vec_ptr; }

    int get_min_index(size_t i)
    {
    }

    ~adj_colors()
    {
        for (size_t i = 0; i < _vec_ptr.size(); ++i)
        {
            delete _vec_ptr[i];
        }
    }
};

std::ostream &operator<<(std::ostream &os, adj_colors &obj)
{
    size_t i = 0;
    for (auto &li : obj.adj_list)
    {
        os << i << ": { ";
        for (auto &el : li)
        {
            os << "(" << *el << ") ";
        }
        os << "}\n";
        ++i;
    }
    return os;
}

bool not_found(const adj_colors &adj_color_vertex, size_t index_U, int val) { return !(adj_color_vertex.contained(index_U, val)); }

void calc_Ui(const adj_colors &adj_color_vertex, info &U, size_t index_U, size_t q)
{
    for (int i = 0; i < q + 1; ++i)
    {
        if (not_found(adj_color_vertex, index_U, i)) // занести в множество U[i] вершины
        {
            U.emplace(i, true);
        }
        else
        {
            U.emplace(i, false);
        }
    }
}

int find_min_color_Ui_and_erase(info Ui)
{
    for (auto &el : Ui.val)
    {
        if (el.second == true)
        {
            el.second = false;
            return el.first;
        }
    }
    return -1;
}

void init(size_t n,
          std::vector<info> &U, /* множество цветов для вершин */
          adj_colors &ad_cl,    /* смежные цвета */
          std::vector<int> &l,
          size_t &k, /* верхняя граница */
          size_t &i, /* итерация */
          size_t &q, /* нижняя граница */
          bool &updateU,
          std::vector<int> &c /*вектор цветов*/)
{
    l.resize(n);
    U.resize(n);
    c.resize(n, -1);
    i = 1;
    k = n;
    q = 1;
    c[0] = 0;
    U[0];
    l[0] = 0;
    ad_cl.paint_the_vertex(0, 0);
    updateU = true;
}

void alg_brown(graph_model &g,       /* предсталение вершин графа в виде списков смежности */
               std::vector<info> &U, /* множество цветов для вершин */
               adj_colors &ad_cl,    /* смежные цвета */
               std::vector<int> &l,
               size_t &k, /* верхняя граница */
               size_t &i, /* итерация */
               size_t &q, /* нижняя граница */
               bool &updateU,
               std::vector<int> &c /*вектор цветов*/
)
{
    size_t n = g.count_of_vertex();
    while (i > 0)
    {
        if (updateU == true)
        {
            calc_Ui(ad_cl, U[i], i, q);
            std::cout << "set = {";
            for (auto el : U[i].val)
            {
                std::cout << "(" << el.first << " " << el.second << ") ";
            }
            std::cout << '\n';
            /* вычислить множество U[i] имеющее цвета {0,1,...,q}
             минус те, котоорые (не) используются вершиной v[i] */
        }

        if (U[i].empty())
        {
            --i;
            q = l[i];
            updateU = false;
            return;
        }
        else
        {
            int j = find_min_color_Ui_and_erase(U[i]);
            // std::cout << j << " " << ad_cl.get_free_index_color(i) << std::endl;
            ad_cl.paint_the_vertex(i, ad_cl.get_free_index_color(i));
            // выбрать j, принаждлежащее U[i],
            // такое, что это минимальное возможное значение
            // удалить цвет j из множества U[i]
            c[i] = j;
            // и задаем цвет j вершине v[i]
            if (j <= k)
            {
                if (j >= q)
                {
                    ++q;
                }

                if (i == n - 1)
                {
                    k = q;
                    /* запомнить текущее решение и установить k = q */

                    /* найти наименьший номер j такой, что цвет v[i] = k */
                    i = j - 1;
                    q = k - 1;
                    updateU = false;
                    return;
                }
                else
                {
                    l[i] = q;
                    ++i; // новая вершиная, выбранная для раскраски
                    updateU = true;
                }
            }
            else
            {
                --i;
                q = l[i];
                updateU = false;
                return;
            }
        }
    }
}

std::ostream &operator<<(std::ostream &os, const std::vector<int> &vec)
{
    for (auto el : vec)
    {
        os << el << " ";
    }
    return os;
}

int main()
{
    graph_model obj;
    for (size_t i = 0; i < 8; ++i)
    {
        obj.create_vertex();
    }

    obj.add(0, 1);
    obj.add(0, 2);
    obj.add(0, 6);

    obj.add(1, 5);
    obj.add(1, 7);

    obj.add(2, 3);
    obj.add(2, 4);

    obj.add(3, 4);
    obj.add(3, 5);

    obj.add(4, 5);

    std::vector<info> U;   /* множество цветов для вершин */
    adj_colors ad_cl(obj); /* смежные цвета */
    std::vector<int> l;
    size_t k; /* верхняя граница */
    size_t i; /* итерация */
    size_t q; /* нижняя граница */
    bool updateU;
    std::vector<int> c; /*вектор цветов*/
    init(8, U, ad_cl, l, k, i, q, updateU, c);
    alg_brown(obj, U, ad_cl, l, k, i, q, updateU, c);
    std::cout << "+\n";
    std::cout << c << std::endl;
    return 0;
}