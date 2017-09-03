#ifndef MODEL_H
#define MODEL_H

#include "generalizemap.h"
#include <vector>


class model
{
public:
    model();

    std::vector<int> _get_data(int x, int y) const;
    bool _fixed(int x, int y) const;
    bool _get_if_chosen() const;
    int _get_chosen_index() const;
    bool _get_if_sign(int x, int y) const;
    bool _get_if_highlight(int x, int y) const;
    bool _check_solved() const;

    void _set_chosen(int x, int y);
    void _set_highlight(int x, int y);
    void _set_difficulty(int diff);

    void _back();
    void _forward();


    void _restart();
    void _initialize(); //start a new game
    void _clean();  //clean all the data
    void _solve();

    void _set_number(int num);
    void _sign();

private:
    void test();
    bool _in(int x, int y);
    bool _satisfy(int num) const;

private:
    std::vector<std::vector<std::vector<int>>> m_map;
    int m_difficulty;
    int m_chosen_x, m_chosen_y;

    std::vector<std::vector<bool>> m_fixed;
    std::vector<std::vector<bool>> m_signed;
    std::vector<std::vector<bool>> m_highlight;

    bool m_chosen;

    std::vector<std::vector<int>> m_record, m_pre, m_aft;
    int m_pos;

    GeneralizeMap m_generator;
};

#endif // MODEL_H
