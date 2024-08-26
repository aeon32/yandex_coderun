#include <iostream>
#include <queue>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stack>
#include <sstream>

struct Room
{
  bool is_visited;
  bool has_robot;
};

struct Tunnel
{
    int start;
    int end;
};

struct Robot
{
    int room;
};

using Rooms = std::vector<Room>;
using Tunnels = std::vector<Tunnel>;
using Robots = std::vector<Robot>;

struct BFSQueueItem
{
    int room;
    int distance;
};

using BFSQueue = std::queue<BFSQueueItem>;


int robots(std::istream & in)
{
    int N, K;
    in >> N >> K;

    Rooms rooms(N + 1, Room{.is_visited=false, .has_robot = false});

    Tunnels tunnels;
    tunnels.reserve(K);
    for (int i = 0; i < K; i++)
    {
        Tunnel tunnel;
        in>>tunnel.start>>tunnel.end;
        tunnels.push_back(tunnel);
        std::swap(tunnel.start, tunnel.end);
        tunnels.push_back(tunnel);
    }

    auto tunnel_sorter = [] (const Tunnel & tunnel1, const Tunnel & tunnel2)
    {
        return tunnel1.start < tunnel2.start;
    };

    std::sort(tunnels.begin(), tunnels.end(), tunnel_sorter);

    int M;
    in>>M;
    int rooms_with_robots = 0;
    for (int i = 0; i < M; i++)
    {
        int room;
        in >> room;
        if (!rooms[room].has_robot)
        {
            rooms[room].has_robot = true;
            rooms_with_robots++;
        }

    }

    int total_time  = -1;
    for (int i = 1; i <= N; i++ )
    {
        BFSQueue queue;
        for (Room & room : rooms)
        {
            room.is_visited = false;
        }

        queue.push(BFSQueueItem{.room = i, .distance = 0});
        rooms[i].is_visited = true;

        int robots_got = 0;
        int current_distance = 0;


        std::vector<int> paths;
        paths.reserve(rooms_with_robots);

        if (rooms[i].has_robot)
        {
            robots_got++;
            paths.push_back(0);
        }
        bool has_cycle = false;
        while (!queue.empty() && robots_got != rooms_with_robots)
        {
            BFSQueueItem  bfs_item = queue.front();
            queue.pop();

            Tunnels::const_iterator outgoing = std::lower_bound(tunnels.cbegin(), tunnels.cend(), Tunnel{.start = bfs_item.room}, tunnel_sorter);
            while (outgoing != tunnels.cend() && outgoing->start == bfs_item.room)
            {
                Room & tunnel_end = rooms[outgoing->end];
                //if (i == 3)
                //   std::cout<<" edge "<<outgoing->start<<" "<<outgoing->end<<" "<<tunnel_end.is_visited<<std::endl;
                if (outgoing->end == outgoing->start)
                {
                    has_cycle = true;
                }
                if (!tunnel_end.is_visited)
                {
                    int new_distance = bfs_item.distance + 1;
                    queue.push(BFSQueueItem{.room = outgoing->end, .distance = new_distance});
                    tunnel_end.is_visited = true;
                    if (tunnel_end.has_robot)
                    {
                        robots_got++;
                        if (new_distance > current_distance)
                            current_distance = new_distance;
                        paths.push_back(new_distance);
                        //if (i == 3)
                        //std::cout<<new_distance<<"   "<<all_is_odd<<" "<<all_is_even<<std::endl;
                    }
                }
                ++outgoing;
            }
        }
        if (robots_got == rooms_with_robots)
        {
             std::cout<<"robots got at  "<<i<<" "<<current_distance<<std::endl;
             std::cout<<"point "<<i<<" paths: ";
            bool equal_parity = true;
            for(int distance: paths)
            {
                std::cout<<distance<<" ";
                equal_parity = equal_parity && ((distance & 1) == (paths.front() & 1));
            }
            std::cout<<"equalparity: "<< equal_parity<<std::endl;

            if (!equal_parity && !has_cycle)
            {
               // current_distance++;
            }



            if (total_time == -1)
                total_time = current_distance;
            if (current_distance < total_time)
                total_time = current_distance;

        }

    }

    return total_time;
}

void test0()
{
    std::string data =
R"(4 5
1 2
2 3
3 4
1 4
1 3
3
1 2 4)";

    std::istringstream in(std::move(data));
    std::cout<<robots(in)<<std::endl;
}


void test1()
{
    std::string data =
R"(6 5
1 2
2 3
3 4
2 5
5 6
3
1 4 6)";

    std::istringstream in(std::move(data));
    std::cout<<robots(in)<<std::endl;
}

void test2()
{
    std::string data =
R"(3 3
1 2
2 3
2 2
3
1 2 3)";

    std::istringstream in(std::move(data));
    std::cout<<robots(in)<<std::endl;
}

void test3()
{
    std::string data =
R"(7 7
1 2
2 3
3 4
4 1
1 3
2 6
6 7
3
7 2 4)";

    std::istringstream in(std::move(data));
    std::cout<<robots(in)<<std::endl;
}

void test4()
{
    std::string data =
        R"(4 3
1 2
2 3
3 4
2
1 4)";

    std::istringstream in(std::move(data));
    std::cout<<robots(in)<<std::endl;
}

int main(int argc, char** argv)
{
    test4();
}

