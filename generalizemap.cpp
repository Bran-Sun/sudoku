#include "generalizemap.h"

GeneralizeMap::GeneralizeMap()
{
    std::vector<int> vec(9, 0);
    m_map.assign(9, vec);
}


void GeneralizeMap::_choose_seed()
{
    static const int seed_map[4][9][9] = { {
        { 1, 2, 3, 4, 5, 6, 7, 8, 9 },
       { 4, 5, 6, 7, 8, 9, 1, 2, 3 },
       { 7, 8, 9, 1, 2, 3, 4, 5, 6 },
       { 2, 1, 4, 3, 6, 5, 8, 9, 7 },
       { 3, 6, 5, 8, 9, 7, 2, 1, 4 },
       { 8, 9, 7, 2, 1, 4, 3, 6, 5 },
       { 5, 3, 1, 6, 4, 2, 9, 7, 8 },
       { 6, 4, 2, 9, 7, 8, 5, 3, 1 },
       { 9, 7, 8, 5, 3, 1, 6, 4, 2 } },
      { { 3, 9, 4, 5, 1, 7, 6, 2, 8 },
       { 5, 1, 7, 6, 2, 8, 3, 9, 4 },
       { 6, 2, 8, 3, 9, 4, 5, 1, 7 },
       { 9, 3, 5, 4, 7, 1, 2, 8, 6 },
       { 4, 7, 1, 2, 8, 6, 9, 3, 5 },
       { 2, 8, 6, 9, 3, 5, 4, 7, 1 },
       { 1, 4, 3, 7, 5, 9, 8, 6, 2 },
       { 7, 5, 9, 8, 6, 2, 1, 4, 3 },
       { 8, 6, 2, 1, 4, 3, 7, 5, 9 } },
      { { 7, 6, 1, 9, 8, 4, 2, 3, 5 },
        { 9, 8, 4, 2, 3, 5, 7, 6, 1 },
       { 2, 3, 5, 7, 6, 1, 9, 8, 4 },
       { 6, 7, 9, 1, 4, 8, 3, 5, 2 },
       { 1, 4, 8, 3, 5, 2, 6, 7, 9 },
       { 3, 5, 2, 6, 7, 9, 1, 4, 8 },
       { 8, 1, 7, 4, 9, 6, 5, 2, 3 },
       { 4, 9, 6, 5, 2, 3, 8, 1, 7 },
       { 5, 2, 3, 8, 1, 7, 4, 9, 6 } },
        { { 7, 1, 5, 4, 3, 6, 2, 9, 8 },
        { 4, 3, 6, 2, 9, 8, 7, 1, 5 },
       { 2, 9, 8, 7, 1, 5, 4, 3, 6 },
       { 1, 7, 4, 5, 6, 3, 9, 8, 2 },
       { 5, 6, 3, 9, 8, 2, 1, 7, 4 },
       { 9, 8, 2, 1, 7, 4, 5, 6, 3 },
       { 3, 5, 7, 6, 4, 1, 8, 2, 9 },
       { 6, 4, 1, 8, 2, 9, 3, 5, 7 },
       { 8, 2, 9, 3, 5, 7, 6, 4, 1 } } };

    srand((unsigned)time(NULL));
    int r = rand() % 4;
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
        {
            m_map[i][j] = seed_map[r][i][j];
        }
}

void GeneralizeMap::_change_column(int c1, int c2)
{
    for (int i = 0; i < 9; i++)
    {
        int t = m_map[c1][i];
        m_map[c1][i] = m_map[c2][i];
        m_map[c2][i] = t;
    }
}

void GeneralizeMap::_change_row(int r1, int r2)
{
    for (int i = 0; i < 9; i++)
    {
        int t = m_map[i][r1];
        m_map[i][r1] = m_map[i][r2];
        m_map[i][r2] = t;
    }
}

void GeneralizeMap::print()
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            std::cout << m_map[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

std::vector<std::vector<int>> GeneralizeMap::_genenralize(int difficulty)
{
    _choose_seed();

    int count = 0;
    srand((unsigned)time(NULL));
    while (count < 20)
    {
        int r0 = rand() % 3;
        int r1 = rand() % 3;
        int r2 = (r1 + 1) % 3;
        _change_row(r0 * 3 + r1, r0 * 3 + r2);

        int c0 = rand() % 3;
        int c1 = rand() % 3;
        int c2 = (c1 + 1) % 3;
        _change_column(c0 * 3 + c1, c0 * 3 + c2);
        count++;
    }

    m_ans = m_map;

    _delete(difficulty);

    return m_map;
}

void GeneralizeMap::_delete(int difficulty)
{
    static const int diff[10] = {10, 15, 20, 25, 30, 35, 40, 45, 50, 55};

    srand((unsigned)time(NULL));
    int cnt = 0;
    while (cnt < diff[difficulty])
    {
        int x = rand() % 9;
        int y = rand() % 9;
        if (m_map[x][y] != 0)
        {
            m_map[x][y] = 0;
            cnt++;
        }
    }
}

std::vector<std::vector<int>> GeneralizeMap::_get_ans(const std::vector<std::vector<int>> &map)
{
    m_map = map;
    _solve();
    print();
    return m_map;
}

void GeneralizeMap::_solve()
{
    m_ans_cnt = 0;
    _step(0, 0);
}

void GeneralizeMap::_step(int x, int y)
{
    if (m_ans_cnt > 0 ) return;
    if (x == 9)
    {
        m_ans_cnt++;
        return;
    }
    int newx, newy;
    if (y == 8)
    {
        newy = 0;
        newx = x + 1;
    }
    else
    {
        newy = y + 1;
        newx = x;
    }
    if (m_map[x][y] == 0)
    {
        for (int i = 1; i < 10; i++)
            if (_check(x, y, i))
            {
                m_map[x][y] = i;
                _step(newx, newy);
                if (m_ans_cnt > 0 ) return;
                m_map[x][y] = 0;
            }
    }
    else
    {
        _step(newx, newy);
    }
  //  print();
}

bool GeneralizeMap::_check(int x, int y, int num)
{
    bool sati = true;
    for (int i = 0; i < 9; i++)
    {
        if (m_map[x][i] == num && i != y)
            sati = false;
        if (m_map[i][y] == num && i != x)
            sati = false;
    }
    int xi = x / 3, yi = y / 3;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
        {
            int newx, newy;
            newx = xi * 3 + i;
            newy = yi * 3 + j;
            if ((newx != x || newy != y) && m_map[newx][newy] == num)
                sati = false;
        }
    return sati;
}
