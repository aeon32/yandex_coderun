#include <iostream>
#include <queue>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stack>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <numeric>
#include <chrono>


template <std::size_t BUTTON_N>
class MicrowaveOvenSolver
{
 public:

    MicrowaveOvenSolver(const std::array<unsigned int, BUTTON_N > values)
    {

        times = values;
        std::sort(times.begin(), times.end(), std::greater<int>());
        calcNod();
        for (unsigned  int & time : times)
        {
            time /= nod;
        }
    }


    void calcNod()
    {
        unsigned int nod2 = 1;
        std::array <unsigned int, BUTTON_N>  aux_times = times;

        while (  std::reduce(aux_times.begin(), aux_times.end(), true,
                           [](bool res, unsigned  int val) {
                               return res && val % 2 == 0;
                           }
                           ))
        {
            nod2*=2;
            for (int i=0; i< BUTTON_N; i++)
            {
                aux_times[i]/=2;
            }
        }

        unsigned int nod1 = 1;
        for (int i = 3; i <= aux_times.back(); i+=2)
        {
            if (std::reduce(aux_times.begin(), aux_times.end(), true, [i](bool res, unsigned  int val) {return res && val % i == 0;}))
            {
                nod1 = i;
            }
        }
        nod = nod1 * nod2;
        nok = std::reduce(times.begin(), times.end(), 1ULL, [] (uint64_t res, unsigned  int val) {return res * val;} );
        nok /= nod;
    }

    bool solve(uint64_t N)
    {
        return true;
    }

    uint64_t solve_dynamic_partial(uint64_t N)
    {
        N = (N + nod - 1) / nod;
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
        N = (N + nod - 1) / nod;
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

    uint64_t solve_advanced(uint64_t N)
    {

        MicrowaveOvenSolver<2> aux_solver({ times[1], times[2]});
        uint32_t nok_little = aux_solver.nok;

        return 0;
    }

 private:
    std::array<unsigned int, BUTTON_N> times;
    uint64_t nod;
    uint64_t nok;

};





void microwave1(std::istream & in, std::ostream & out)
{
    uint64_t N;
    unsigned  int A, B, C;
    in >> N;
    in>>A>>B>>C;


    MicrowaveOvenSolver<3> solver({A, B, C});
    for (uint64_t i = 7; i < 1000; i++)
    {
        uint64_t expected = solver.solve_dynamic_partial(i);
        uint64_t real = solver.solve_dynamic(i);

        if (expected != real)
            out<<i<<" "<<expected<<" "<<real<<std::endl;
    }

}

void microwave(std::istream & in, std::ostream & out)
{
    uint64_t N;
    unsigned  int A, B, C;
    in >> N;
    in>>A>>B>>C;


    MicrowaveOvenSolver<3> solver({A, B, C});
    uint64_t real = solver.solve_dynamic(N);
    out<<real<<std::endl;
}


void test()
{
    std::string data =
        R"(100000
6 10 14)";

    std::istringstream in(std::move(data));
    microwave1(in, std::cout);
}

void test_nod()
{
    //{99991, 99989, 99971}
    auto beg = std::chrono::high_resolution_clock::now();



    // Taking a timestamp after the code is ran


    // Subtracting the end timestamp from the beginning
    // And we choose to receive the difference in
    // microseconds


    // Displaying the elapsed time


    MicrowaveOvenSolver<2> solver({99989, 99971});
    uint64_t solv = solver.solve_dynamic(99991);

    auto end = std::chrono::high_resolution_clock::now();
    std::cout<<solv<<std::endl;
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - beg);
    std::cout << "Elapsed Time: " << duration.count();

}

int main(int argc, char** argv)
{
   //test();
   test_nod();
   return 0;
}

