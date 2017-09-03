#ifndef GENERALIZEMAP_H
#define GENERALIZEMAP_H

#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>


class GeneralizeMap
{
public:
    GeneralizeMap();

    std::vector<std::vector<int>> _genenralize(int difficulty);

    void print();
    void _solve();
    void _step(int x, int y);
    bool _check(int x, int y,int num);

    std::vector<std::vector<int>> _get_ans(const std::vector<std::vector<int>> &map);

private:
    void _choose_seed();
    void _change_column(int c1, int c2);
    void _change_row(int r1, int r2);
    void _delete(int difficulty);

private:
    std::vector<std::vector<int>> m_map;
    std::vector<std::vector<int>> m_ans;
    int m_ans_cnt;
};

#endif // GENERALIZEMAP_H
