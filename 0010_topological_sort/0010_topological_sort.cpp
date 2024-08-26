#include <iostream>
#include <queue>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stack>
#include <sstream>

struct Vertex
{
    int number;
    int new_number;
    bool is_visited;
};

struct Edge
{
    int out;
    int in;
};


using EdgeVector = std::vector<Edge>;
using VertexVector = std::vector<Vertex>;

struct DFSStackItem
{
    int vertex;
    EdgeVector::const_iterator currentDFSAdjacentIt;
};

using DFSStack = std::stack<DFSStackItem>;

void topological_sort(std::istream & in, std::ostream & out)
{
    int N, M;
    in >> N >> M;

    VertexVector vertexes(N+1);
    for (int i = 1; i <= N; i++)
    {
        vertexes[i].number = i;
        vertexes[i].new_number = -1;
    }

    std::vector<int> out_vertexes;
    EdgeVector edges;
    edges.reserve(M);
    for (int i = 0; i < M; i++)
    {
        Edge edge;
        int vout, vin;
        in >> vout >> vin;
        edges.emplace_back(Edge { vout, vin});
        out_vertexes.push_back(vout);
    }
    std::sort(out_vertexes.begin(), out_vertexes.end());
    out_vertexes.erase(std::unique(out_vertexes.begin(), out_vertexes.end()), out_vertexes.end());

    auto edge_sorter = [](const Edge & one, const Edge & two)
    {
        return one.in < two.in;
    };
    EdgeVector aux_edges = edges;
    std::sort(edges.begin(), edges.end(), edge_sorter);


    decltype(out_vertexes)::const_iterator it = out_vertexes.cbegin();
    DFSStack stack;
    for (int i = 1; i <= N; i++)
    {
        if ( it != out_vertexes.cend() && i == *it)
        {
            ++it;
        } else
        {
            std::cout<<"Push0 vertex "<<i<<std::endl;
            EdgeVector::const_iterator adj_it =std::lower_bound(edges.cbegin(), edges.cend(), Edge{0, i},edge_sorter);
            stack.push(DFSStackItem{i, adj_it });
        }
    }

    int currentNumber = 1;
    while (!stack.empty())
    {
        DFSStackItem & currentVertex = stack.top();
        Vertex * vertex = &vertexes[currentVertex.vertex];

        if (currentVertex.currentDFSAdjacentIt != edges.cend() && currentVertex.currentDFSAdjacentIt->in == currentVertex.vertex)
        {
            int out = currentVertex.currentDFSAdjacentIt->out;
            ++currentVertex.currentDFSAdjacentIt;

            if (!vertexes[out].is_visited)
            {
                EdgeVector::const_iterator new_adj_it = std::lower_bound(edges.cbegin(), edges.cend(), Edge{0, out}, edge_sorter);
                std::cout<<"Push1 vertex "<<out<<std::endl;
                stack.push(DFSStackItem{out, new_adj_it});
                vertexes[out].is_visited = true;
            } else
            {
                if (vertexes[out].new_number == -1)
                    break;
            }

        } else
        {
            std::cout<<"Pop vertex "<<currentVertex.vertex<<std::endl;
            vertex->new_number = currentNumber++;
            stack.pop();
        }

    }

    if (currentNumber != N+1)
    {
        std::cout<<-1<<std::endl;
        return;
    }

    std::vector<int> answ(N+1);
    for (int i = 1; i<=N; i++)
    {
        answ[vertexes[i].new_number] = vertexes[i].number;
        //std::cout<<vertexes[i].new_number<<" ";
        //std::cout<<hren[i]<<" ";
    }

    for (int i = 1; i<=N; i++)
    {
        std::cout<<answ[i]<<" ";
    }


    std::cout<<std::endl;

    for (Edge & edge : aux_edges)
    {
        std::cout <<"Edge "<<edge.out<<" "<<edge.in<<" -> "<<vertexes[edge.out].new_number<<" "<<vertexes[edge.in].new_number<<std::endl;
    }

}

void test()
{
    std::string data =
R"(6 6
1 2
3 2
4 2
2 5
6 5
4 6)";

/*
    data =
R"(5 5
1 2
3 1
2 4
4 3
4 5
)";
*/
    std::istringstream in(std::move(data));
    topological_sort(in, std::cout);
}

int main(int argc, char** argv)
{
    test();
}

