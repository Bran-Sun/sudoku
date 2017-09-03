#include "model.h"

model::model()
{
    std::vector<int> vec1(1, 0);
    std::vector<std::vector<int>> vec2(9, vec1);
    m_map.assign(9, vec2);

    std::vector<bool> vecb(9, false);
    m_fixed.assign(9, vecb);

    m_signed = m_fixed;
    m_highlight = m_fixed;

    m_difficulty = 1;

    m_chosen_x = -1;
    m_chosen_y = -1;
    m_chosen = false;

    m_pos = 0;

   // test();

}

void model::test()
{
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            m_map[i][j][0] = i + 1;
    m_map[0][0][0] = 0;

    m_fixed[1][1] = true;
}

bool model::_fixed(int x, int y) const
{
    return m_fixed[x][y];
}

std::vector<int> model::_get_data(int x, int y) const
{
    return m_map[x][y];
}

void model::_set_chosen(int x, int y)
{
    if (_in(x, y))
    {
        /*if (m_fixed[x][y])
        {

        }*/
        if (!m_fixed[x][y])
        {
            m_chosen_x = x;
            m_chosen_y = y;
            m_chosen = true;
        }
        else
            m_chosen = false;
    }
    else
        m_chosen = false;
}

bool model::_in(int x, int y)
{
    if ((x >= 0 && x <= 8) && (y >= 0 && y <=8))
        return true;
    else
        return false;
}

bool model::_get_if_chosen() const
{
    return m_chosen;
}

int model::_get_chosen_index() const
{
    return m_chosen_x + m_chosen_y * 9;
}

void model::_clean()
{
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
        {
            m_map[i][j].clear();
            m_map[i][j].push_back(0);
            m_fixed[i][j] = false;
            m_signed[i][j] = false;
            m_highlight[i][j] = false;
        }
    m_chosen_x = -1;
    m_chosen_y = -1;
    m_chosen = false;
    m_record.clear();
    m_pre.clear();
    m_aft.clear();
    m_pos = 0;
}

void model::_set_number(int num)
{
    if (m_chosen)
    {
        if (m_map[m_chosen_x][m_chosen_y][0] == 0 && num == 0)
            return;
        if (!_satisfy(num))
            return;

        std::vector<int> vec;
        vec.push_back(m_chosen_x);
        vec.push_back(m_chosen_y);

        if (m_pos == m_record.size())
        {
            m_record.push_back(vec);
            m_pos++;
        }
        else
        {
           while(m_record.size() > m_pos)
           {
               m_record.pop_back();
               m_pre.pop_back();
               m_aft.pop_back();
           }
           m_record.push_back(vec);
           m_pos++;
        }

        if (num == 0)
        {
            vec = m_map[m_chosen_x][m_chosen_y];
            m_pre.push_back(vec);
            vec.clear();
            vec.push_back(0);
            m_aft.push_back(vec);
            m_map[m_chosen_x][m_chosen_y] = vec;
        }
        else if (m_map[m_chosen_x][m_chosen_y][0] != 0)
        {
            bool find = false;
            for (int k = 0; k < m_map[m_chosen_x][m_chosen_y].size(); k++)
                if (m_map[m_chosen_x][m_chosen_y][k] == num)
                {
                    find = true;
                    break;
                }
            if (!find)
            {
                m_pre.push_back(m_map[m_chosen_x][m_chosen_y]);
                m_map[m_chosen_x][m_chosen_y].push_back(num);
                m_aft.push_back(m_map[m_chosen_x][m_chosen_y]);
            }
        }
        else
        {
            m_pre.push_back(m_map[m_chosen_x][m_chosen_y]);
            m_map[m_chosen_x][m_chosen_y][0] = num;
            m_aft.push_back(m_map[m_chosen_x][m_chosen_y]);
        }

        _set_highlight(m_chosen_x, m_chosen_y);
    }
 /*   std::cout << m_pos << std::endl;
    std::cout << m_record[0][0] << " " << m_record[0][1] << std::endl;*/

}

void model::_initialize()
{
    _clean();
//    std::cout << m_difficulty << std::endl;
    std::vector<std::vector<int>> tem = m_generator._genenralize(m_difficulty);
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            m_map[i][j][0] = tem[i][j];
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
        {
            if (m_map[i][j][0] != 0)
                    m_fixed[i][j] = true;
        }

}

void model::_set_difficulty(int diff)
{
    m_difficulty = diff;
  //  std::cout << diff << std::endl;
}

void model::_back()
{
    if (m_pos > 0)
    {
        int x = m_record[m_pos - 1][0];
        int y = m_record[m_pos - 1][1];
        m_chosen_x = x;
        m_chosen_y = y;
        m_map[x][y] = m_pre[m_pos - 1];
        m_pos--;
        _set_highlight(x, y);
    }
}

void model::_forward()
{
    if (m_pos < m_record.size())
    {
        m_chosen_x = m_record[m_pos][0];
        m_chosen_y = m_record[m_pos][1];
        m_map[m_chosen_x][m_chosen_y] = m_aft[m_pos];
        m_pos++;
    }
}

void model::_sign()
{
    if (m_chosen)
    {
        m_signed[m_chosen_x][m_chosen_y] = !m_signed[m_chosen_x][m_chosen_y];
    }
}

bool model::_get_if_sign(int x, int y) const
{
    return  m_signed[x][y];
}

void model::_set_highlight(int x, int y)
{
    if (!_in(x, y)) return;
    if (m_map[x][y][0] != 0 && m_map[x][y].size() == 1)
    {
        int t = m_map[x][y][0];
        for (int i = 0; i < 9; i++)
            for (int j= 0; j < 9; j++)
            {
                if (m_map[i][j].size() == 1 && m_map[i][j][0] == t)
                    m_highlight[i][j] = true;
                else
                    m_highlight[i][j] = false;
            }

    }
    else
    {
        for (int i = 0; i < 9; i++)
            for (int j = 0; j < 9; j++)
                m_highlight[i][j] = false;
    }
}

bool model::_get_if_highlight(int x, int y) const
{
    return m_highlight[x][y];
}

void model::_solve()
{
      std::vector<std::vector<int>> map;
      std::vector<int> vec(9, 0);
      map.assign(9, vec);
      for (int i = 0; i < 9; i++)
          for (int j= 0; j < 9; j++)
              if (m_map[i][j].size() == 1)
                  map[i][j] = m_map[i][j][0];
      std::vector<std::vector<int>> answer = m_generator._get_ans(map);
      if (answer.size() == 0) return;
      for (int i = 0; i < 9; i++)
          for (int j = 0; j < 9; j++)
          {
              m_map[i][j].clear();
              m_map[i][j].push_back(answer[i][j]);
          }

}

bool model::_satisfy(int num) const
{
    if (num == 0) return true;
    bool sati = true;
    for (int i = 0; i < 9; i++)
    {
        if (m_map[m_chosen_x][i].size() == 1)
        {
            if (m_map[m_chosen_x][i][0] == num && m_chosen_y != i)
                sati = false;
            //std::cout << sati << std::endl;
        }
    }

    for (int i = 0; i < 9;  i++)
        if (m_map[i][m_chosen_y].size() == 1)
        {
            if (m_map[i][m_chosen_y][0] == num && m_chosen_x != i)
                sati = false;
            //std::cout << sati << std::endl;
        }

    int xi = m_chosen_x / 3;
    int yi = m_chosen_y / 3;

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
        {
            int x = xi * 3 + i;
            int y = yi * 3 + j;

            if ((x != m_chosen_x || y != m_chosen_y) && m_map[x][y].size() == 1)
            {
                if (num == m_map[x][y][0])
                    sati = false;
            }

        }

    return sati;
}

bool model::_check_solved() const
{
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (m_map[i][j].size() > 1 || m_map[i][j][0] == 0)
                return false;
    return true;
}
