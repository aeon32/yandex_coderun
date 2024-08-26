#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <sstream>

int choose_stop(const std::vector<int> & stops, int req)
{
    //auto lower_it = std::lower_bound(stops.begin(), stops.end(), req);
    auto lower_it = std::lower_bound(stops.cbegin(), stops.cend(), req);

    int right_distance_min = std::numeric_limits<int>::max();
    int left_distance_min = std::numeric_limits<int>::max();
    if (lower_it != stops.end())
    {
        if(*lower_it == req)
        {
            //found equal coord, return it
            return lower_it - stops.cbegin() + 1;
        }
        right_distance_min = *lower_it - req;
    }
    if (lower_it != stops.cbegin())
    {
        left_distance_min =  req - *(lower_it - 1);
    }

    if (right_distance_min < left_distance_min)
    {
        return lower_it - stops.cbegin() + 1;
    } else
    {
        if (lower_it != stops.cbegin())
            --lower_it ;
        return lower_it - stops.cbegin() + 1;

    }

}

void closest_stop(std::istream & in, std::ostream & out)
{
    int n, k;
    in >> n >> k;

    std::vector<int> stops;
    stops.reserve(n);

    for (int i = 0; i < n; i++)
    {
        int a;
        in >> a;
        stops.push_back(a);
    }

    for (int i = 0; i < k; i++)
    {
        int req;
        in >> req;
        int stop = choose_stop(stops, req);
        out<<stop<<std::endl;

    }

}

void test1()
{
    std::string data =
R"(3 3
1 3 5
4 1 5)";

    std::istringstream in(std::move(data));
    closest_stop(in, std::cout);
}

void test2()
{
    std::string data =
        R"(3 3
1 3 5
4 1 5)";

    std::istringstream in(std::move(data));
    closest_stop(in, std::cout);
}


int main()
{


    test2();

    return 0;
}