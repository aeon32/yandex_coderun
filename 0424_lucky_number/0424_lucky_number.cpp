#include <iostream>
#include <queue>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stack>
#include <sstream>
#include <iomanip>


void get_lucky_number(std::string & number)
{
    int left_sum = 0;
    int N = number.size();

    for (int i = 0; i < N / 2; i++)
    {
        left_sum += number[i] - '0';
    }

    int right_sum = 0;
    for (int i = N / 2; i < N; i++)
    {
        right_sum += number[i] - '0';
    }

    bool carry_flag = false;
    for ( int i = N -1 ; i >= 0  && right_sum >= left_sum;)
    {
        int * sum = i < N /2 ? &left_sum : &right_sum;

        int digit = number[i] - '0';
        *sum -= digit;
        number[i] = '0';

        i--;
        carry_flag = true;
        while (i >=0 && carry_flag)
        {
            int digit = number[i] - '0';
            int * sum = i < N /2 ? &left_sum : &right_sum;

            *sum -= digit;
            if (digit == 9)
            {
                digit = 0;
            } else
            {
                digit++;
                carry_flag = false;
            }
            *sum += digit;
            number[i] = digit + '0';
            if (carry_flag)
                i--;
        }
        if (right_sum == left_sum)
            break;

    }

    if (left_sum == 0)
    {
        number = std::string(N, '0');
        number[N / 2 - 1] = '1';
        number[N - 1] = '1';
        return;
    }

    int diff = left_sum - right_sum;
    for (int i = N - 1; diff != 0 && i >= N/2; i-- )
    {
        int digit = number[i] - '0';
        int new_digit = std::min(diff + digit, 9);
        diff -= (new_digit - digit);
        number[i] = new_digit + '0';
    }
}


void lucky_number(std::istream & in, std::ostream & out)
{
    std::string number;
    in >> number;
    get_lucky_number(number);

    out<<number<<std::endl;

}

void test()
{
    //std::string data = "4512390121";
    std::string data = "2298";
    std::cout << data<<" - ";
    std::istringstream in(std::move(data));
    lucky_number(in, std::cout);
}

void test_n(const char * data)
{
    std::string res = data;
    get_lucky_number(res);
    std::cout<<data<<" "<<res<<std::endl;

}

int lucky_number_backtrack(int number, unsigned  int N, unsigned int MAX_NUMBER)
{
    int expected = number;
    bool equal_sum = false;
    do {
        expected++;
        if (expected > MAX_NUMBER)
            expected = 0;

        int left_sum = 0;
        int right_sum = 0;
        int aux = expected;
        for (int i = N - 1; i>= 0; i--)
        {
            int digit = aux % 10;
            if (i < N / 2)
                left_sum += digit;
            else
                right_sum += digit;
            aux = aux / 10;
        }
        if (left_sum == right_sum && left_sum != 0)
            return  expected;

    } while (true);
    return 0;
}

void backtracking_test(unsigned int N)
{
    unsigned int MAX_NUMBER = 1;
    for (int i = 0; i< N ; MAX_NUMBER *=10, i++)
    {

    }
    MAX_NUMBER -=1;

    for (int i = 0; i <= MAX_NUMBER; i++)
    {
        std::ostringstream out;

        int res = lucky_number_backtrack(i, N, MAX_NUMBER);

        out << std::setfill('0') << std::setw(N) << i;
        std::string in_number = out.str();

        out.str("");
        out.clear();
        out<<std::setfill('0') << std::setw(N) << res;
        std::string expected = out.str();

        std::string aux = in_number;
        get_lucky_number(aux);

        if (expected != aux)
            std::cout<<in_number<<" "<<expected<<" "<<aux<<std::endl;
    }
    return;
}

int main(int argc, char** argv)
{
    //lucky_number(std::cin, std::cout);
   // test_n("0000");
    backtracking_test(8);
   return 0;
}

