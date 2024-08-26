#include <iostream>
#include <queue>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stack>
#include <sstream>
#include <iomanip>
#include <cstdint>

template <int BUTTON_N>
class MicrowaveOvenSolver
{
 public:
    MicrowaveOvenSolver(unsigned int ia, unsigned int ib, unsigned int ic)
      :times{ia, ib, ic}, nod(1)
    {
        std::sort(times, times+BUTTON_N, std::greater<int>());
        //calcNod();

    }

    void calcNod()
    {
        int nod2 = 1;
        while ( times[0]%2 == 0 && times[1]%2 == 0 && times[2]%2 == 0)
        {
            nod2*=2;
            for (int i=0; i<3; i++)
            {
                times[i]/=2;
            }
        }

        int nod1 = 1;
        for (int i = 3; i <= times[2]; i++)
        {
            if (times[0] % i == 0 && times[1]%i == 0 && times[2] %i == 0)
            {
                nod1 = i;
            }
        }
        for (int i=0; i<3; i++)
        {
            times[i]/=nod1;
        }
        nod = nod1 * nod2;
        std::cout<<"nod "<<nod<<std::endl;

    }

    bool solve(uint64_t N)
    {
        return true;
    }

    uint64_t solve_dynamic_partial(uint64_t N)
    {
        std::vector<bool> dp(N, 0);
        dp.front() = true;
        uint64_t res = 1;
        for (int i = 1; i < N; i++)
        {
            bool ok = false;

            for (int j = 0; j < BUTTON_N; j++)
            {
                if (i >= times[j])
                {
                    ok = ok || dp[i - times[j]];
                }
            }
            if (ok)
            {
                dp[i] = true;
                res++;
            }
        }
        return  res;
    }

    uint64_t solve_dynamic(uint64_t N)
    {
        std::vector<bool> dp0(times[0], 0);
        std::vector<bool> dp1(times[0], 0);
        dp0.front() = true;
        uint64_t res = 0;
        uint64_t previous_res = 0;
        uint64_t current_res = 0;
        int index = 0;

        int i = 0;
        bool period_found = false;
        for (i = 0; !period_found && (i < N); i++)
        {
            bool ok = false;

            for (int j = 0; !ok && j < BUTTON_N; j++)
            {
                if (index >= times[j])
                {
                    int pos = index - times[j];
                    ok = ok || dp1[index - times[j]];
                }
                else
                {
                    int pos = times[0] + index - times[j];
                    ok = ok || dp0[pos];
                }
            }
            dp1[index] = ok;
            if (ok)
            {
                current_res++;
                res++;
            }
            index++;
            if (index == times[0])
            {
                std::swap(dp0, dp1);
                if (current_res != previous_res)
                {
                } else
                {
                    period_found = true;
                }
                previous_res = current_res;
                current_res = 0;
                index = 0;
            }
        }
        if (period_found)
        {
            uint64_t period_length = times[0];
            uint64_t periods_count = (N - i) / period_length;
            res += periods_count * previous_res;
            i += periods_count * period_length;
        }

        //calculate tail
        for (; (i < N); i++)
        {
            bool ok = false;

            for (int j = 0; !ok && j < BUTTON_N; j++)
            {
                if (index >= times[j])
                {
                    int pos = index - times[j];
                    ok = ok || dp1[index - times[j]];
                }
                else
                {
                    int pos = times[0] + index - times[j];
                    ok = ok || dp0[pos];
                }
            }
            dp1[index] = ok;
            if (ok)
            {
                res++;
            }
            index++;
            if (index == times[0])
            {
                std::swap(dp0, dp1);
                index = 0;
            }
        }

        return  res;

        //return res_period;
    }

 private:
    unsigned int times[BUTTON_N];
    int nod;

};



void microwave1(std::istream & in, std::ostream & out)
{
    uint64_t N, A, B, C;
    in >> N;
    in>>A>>B>>C;


    MicrowaveOvenSolver<3> solver(A, B, C);
    for (uint64_t i = 1; i < 100; i++)
    {
        uint64_t expected = solver.solve_dynamic_partial(i);
        uint64_t real = solver.solve_dynamic(i);
        if (expected != real)
            out<<i<<" "<<expected<<" "<<real<<std::endl;
    }

}

void microwave(std::istream & in, std::ostream & out)
{
    uint64_t N, A, B, C;
    in >> N;
    in>>A>>B>>C;


    MicrowaveOvenSolver<3> solver(A, B, C);
    uint64_t real = solver.solve_dynamic(N);
    out<<real<<std::endl;
}

void test()
{
    std::string data =
        R"(100000
2 3 10)";

    std::istringstream in(std::move(data));
    microwave1(in, std::cout);
}

int main(int argc, char** argv)
{
   test();
   return 0;
}

