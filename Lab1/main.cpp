#include "read.cpp"
#include "graph.cpp"
#include <sstream>
int main()
{
    read file = read("input.txt");
    std::vector<std::string> words = file.processedLines;
    Graph graph(words);
    graph.todo();
}
