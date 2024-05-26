#ifndef _GRAPH_
#define _GRAPH_
#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <queue>
#include <conio.h>

// #include <bits\stdc++.h>

class Graph
{
public:
    Graph(const std::vector<std::string> &words)
    {
        // 初始化节点
        int location = 0;
        for (int i = 0; i < words.size(); i++)
        {
            // std::string word = words[i];
            if (nodes.find(words[i]) == nodes.end())
            {
                nodes[words[i]] = location;
                location++;
            }
        }

        // 初始化邻接表
        adj.resize(nodes.size());

        // 建立边和权重
        for (int i = 0; i < words.size() - 1; i++)
        {
            std::string word1 = words[i];
            int j = i + 1;
            std::string word2 = words[j];
            int w = 1; // 默认权重为1
            for (int k = j; k < words.size() - 1; k++)
            {
                if (words[k] == word1 && words[k + 1] == word2)
                    w++;
            }
            adj[nodes[word1]].push_back({nodes[word2], w});
        }
    }

    // 凑合用的ui
    void todo()
    {
        std::cout << "-----------------------" << std::endl;
        std::cout << "chose one func to run" << std::endl;
        std::cout << "0.showDirectedGraph" << std::endl;
        std::cout << "1.queryBridgeWords" << std::endl;
        std::cout << "2.generateNewText" << std::endl;
        std::cout << "3.calcShortestPath" << std::endl;
        std::cout << "4.randomWalk" << std::endl;
        std::cout << "-----------------------" << std::endl;

        while (1)
        { // 无限循环，除非用户选择退出
            char ch;
            std::cout << "press '0'~'4'" << std::endl;
            std::cin >> ch; // 等待用户输入
            switch (ch)
            {
            case '0':
                try_showDirectedGraph();
                break;
            case '1':
                try_queryBridgeWords();
                break;
            case '2':
                try_generateNewText();
                break;
            case '3':
                try_calcShortestPath();
                break;
            case '4':
                try_randomWalk();
                break;
            default:
                return;
            }
        }
    }

    // 执行绘制有向图
    void try_showDirectedGraph()
    {
        showDirectedGraph();
    }

    // 执行桥接词查询
    void try_queryBridgeWords()
    {
        std::cout << "plz input word1 and word2 toget BridgeWords" << std::endl;
        std::string word1, word2;
        std::cin >> word1 >> word2;
        queryBridgeWords(word1, word2);
    }

    // 执行桥接文本生成
    void try_generateNewText()
    {
        getchar();
        std::cout << "plz input a text toget generateNewText" << std::endl;
        std::string text;
        getline(std::cin, text);
        std::string newtext = generateNewText(text);
    }

    // 执行计算最短路径
    void try_calcShortestPath()
    {
        getchar();
        std::cout << "plz input 1 or 2 words toget shortestPath " << std::endl;
        std::string input;
        getline(std::cin, input);
        std::string token = "";
        std::vector<std::string> tokens = {};
        for (char c : input)
        {
            if (c == ' ')
            {
                tokens.push_back(token);
                token = "";
            }
            else
                token += c;
        }
        tokens.push_back(token);

        if (tokens.size() - 1)
            if (tokens.size() - 2)
                std::cout << "Invalid input." << std::endl;
            else
                calcShortestPath(tokens[0], tokens[1]);
        else
        {
            std::cout << "Shortest paths from " << token << ":" << std::endl;
            for (const auto &node : nodes)
            {
                if (node.second != nodes[token])
                {
                    calcShortestPath(token, node.first);
                }
            }
        }
    }

    // 执行随机游走
    void try_randomWalk()
    {
        std::vector<std::string> randomwalk = randomWalk();
        std::cout << "randomWalk finished" << std::endl;
    }

private:
    std::unordered_map<std::string, int> nodes;        // 节点及对应编号
    std::vector<std::vector<std::pair<int, int>>> adj; // 邻接表
    // 打印图
    void print()
    {
        std::cout << "Nodes:" << std::endl;
        for (auto &p : nodes)
        {
            std::cout << p.first << " ";
        }
        std::cout << std::endl;

        std::cout << "Adjacency List:" << std::endl;
        for (int i = 0; i < adj.size(); ++i)
        {
            std::cout << i << " -> ";
            for (auto &p : adj[i])
            {
                std::cout << "(" << p.first << ", " << p.second << ") ";
            }
            std::cout << std::endl;
        }
    }

    // 绘制有向图
    void showDirectedGraph()
    {

        // 创建DOT文件
        std::ofstream dot_file("graph.dot");
        if (dot_file.is_open())
        {
            // 写入DOT文件的头部
            dot_file << "digraph G {" << std::endl;

            // 写入节点
            for (const auto &pair : nodes)
            {
                dot_file << pair.second << " [label=\"" << pair.first << "\"]" << std::endl;
            }
            dot_file << std::endl;
            // 写入边
            for (int i = 0; i < adj.size(); ++i)
            {
                for (auto &p : adj[i])
                {
                    dot_file << i << " -> " << p.first << " [label=\"" << p.second << "\"]" << std::endl;
                }
            }

            // 写入DOT文件的尾部
            dot_file << "}" << std::endl;
            dot_file.close();

            // 使用Graphviz自动生成图像
            std::system("dot -Tpng graph.dot -o graph.png");
            std::cout << "graph.png Saved" << std::endl;
        }
        else
        {
            std::cerr << "failed to write in DOT" << std::endl;
            exit(1);
        }
    }

    // 查询桥接词
    std::vector<std::string> queryBridgeWords(std::string word1, std::string word2, int if_print = 1)
    {
        std::vector<std::string> bridgewords = {};
        if (if_print)
            if ((nodes.find(word1) == nodes.end()) &&
                nodes.find(word2) == nodes.end())
            {
                std::cerr << "No \"" << word1 << "\" and \"" << word2 << "\" in the graph!" << std::endl;
                return bridgewords;
            }
            else if (nodes.find(word1) == nodes.end())
            {
                std::cerr << "No \"" << word1 << "\" in the graph!" << std::endl;
                return bridgewords;
            }
            else if (nodes.find(word2) == nodes.end())
            {
                std::cerr << "No \"" << word2 << "\" in the graph!" << std::endl;
                return bridgewords;
            }
        for (auto &w1 : nodes)
        {
            if (word1 == w1.first)
            {
                for (auto &w2 : adj[w1.second])
                    for (auto &w3 : adj[w2.first])
                        if (nodes[word2] == w3.first)
                            for (auto &bridge : nodes)
                                if (bridge.second == w2.first)
                                    bridgewords.push_back(bridge.first);
            }
        }
        if (print)
            if (bridgewords.size() == 0)
            {
                std::cerr << "No bridge words from \"" << word1 << "\" to \"" << word2 << "\"!" << std::endl;
                return bridgewords;
            }
            else if (bridgewords.size() == 1)
            {
                std::cout << "The bridge words from \"" << word1 << "\" to \"" << word2 << "\" is:" << bridgewords[0] << std::endl;
            }
            else
            {
                std::cout << "The bridge words from \"" << word1 << "\" to \"" << word2 << "\" are:" << bridgewords[0] << std::endl;
                for (auto first = bridgewords.begin() + 1; first != bridgewords.end() - 1; first++)
                {
                    std::cout << " , " << *first;
                }
                std::cout << " and " << bridgewords[bridgewords.size() - 1] << std::endl;
            }
        return bridgewords;
    }

    // 根据bridge word生成新文本
    std::string generateNewText(std::string inputText)
    {
        std::string text2word = "";
        std::vector<std::string> text2words = {};
        for (char c : inputText)
        {
            if (c == ' ')
            {
                text2words.push_back(text2word);
                text2word = "";
            }
            else
                text2word += c;
        }
        text2words.push_back(text2word);
        std::vector<std::string> lowers(text2words);
        for (auto it = lowers.begin(); it != lowers.end() && !it->empty(); ++it)
        {
            for (auto t2 = it->begin(); t2 != it->end(); t2++)
            {
                *t2 = tolower(*t2);
            }
        }
        int if_print = 0;
        std::string newtext;
        for (int i = 0; i < text2words.size() - 1; i++)
        {
            newtext += text2words[i] + " ";
            if (queryBridgeWords(lowers[i], lowers[i + 1], if_print).size() != 0)
                newtext += queryBridgeWords(lowers[i], lowers[i + 1], if_print)[0] + " ";
        }
        newtext += text2words[text2words.size() - 1];
        std::cout << newtext << std::endl;
        return newtext;
    }

    // 计算两个单词之间的最短路径
    std::vector<std::string> calcShortestPath(std::string start, std::string end)
    {
        if ((nodes.find(start) == nodes.end()) &&
            nodes.find(end) == nodes.end())
        {
            std::cerr << "No \"" << start << "\" and \"" << end << "\" in the graph!" << std::endl;
            return {};
        }
        else if (nodes.find(start) == nodes.end())
        {
            std::cerr << "No \"" << start << "\" in the graph!" << std::endl;
            return {};
        }
        else if (nodes.find(end) == nodes.end())
        {
            std::cerr << "No \"" << end << "\" in the graph!" << std::endl;
            return {};
        }

        // 距离数组，初始化为无穷大
        std::vector<int> dist(adj.size(), INT_MAX);
        // prev数组，用于追踪路径
        std::vector<std::string> prev(adj.size(), "");
        // 优先队列，按照距离排序
        std::priority_queue<std::pair<int, std::string>,
                            std::vector<std::pair<int, std::string>>,
                            std::greater<std::pair<int, std::string>>>
            pq;

        // 初始化起点到起点的距离为0
        for (auto &p : adj[nodes[start]])
        {
            if (p.first == nodes[start])
            {
                dist[nodes[start]] = p.second;
                break;
            }
            else
                dist[nodes[start]] = 0;
        }
        pq.push({0, start});
        while (!pq.empty())
        {
            auto [d, u] = pq.top();
            pq.pop();

            // 如果这个点的距离已经不是最短，则跳过
            if (d > dist[nodes[u]])
                continue;

            // 遍历相邻的节点
            for (auto [v, w] : adj[nodes[u]])
            {
                // 只有当通过u到v的路径比直接到v的路径更短时，才更新最短路径
                for (auto &p : nodes)
                    if (d + w < dist[v])
                    {
                        dist[v] = d + w;
                        prev[v] = u;
                        std::string word;
                        for (auto &p : nodes)
                            if (p.second == v)
                                word = p.first;
                        pq.push({dist[v], word});
                    }
            }
        }

        // 如果终点不可达，则返回空
        if (dist[nodes[end]] == INT_MAX)
        {
            std::cout << "The two nodes are not reachable." << std::endl;
            return {};
        }

        // 构建并打印路径
        std::vector<std::string> path;
        std::string current = end;
        while (current != "")
        {
            path.push_back(current);
            current = prev[nodes[current]];
        }
        reverse(path.begin(), path.end());

        // 打印路径和长度
        std::cout << "Shortest path: ";
        for (int i = 0; i < path.size() - 1; i++)
        {
            std::cout << path[i] << "->";
        }
        std::cout << end << std::endl;
        std::cout << "Length: " << dist[nodes[end]] << std::endl;
        return path;
    }

    // 随机游走
    std::vector<std::string> randomWalk()
    {
        std::vector<std::string> path = {};
        std::string start;
        int randnum = rand() % nodes.size();
        for (auto &p : nodes)
            if (p.second == randnum)
                start = p.first;
        int prve = nodes[start];
        int next;
        std::vector<std::vector<int>> mark(nodes.size(), std::vector<int>(nodes.size(), 0));
        std::cout << "Func has run \"" << path.size() + 1 << "\" times" << std::endl;
        std::cout << "press 'ESC' to break this Func or press any other key to continue" << std::endl;
        while (_getch() != 27)
        {
            if (adj[prve].size() == 0)
            {
                break;
            }
            else
            {
                next = adj[prve][rand() % adj[prve].size()].first;
                if (!mark[prve][next])
                {
                    mark[prve][next] = 1;
                    for (auto &p : nodes)
                        if (p.second == prve)
                            path.push_back(p.first);
                    prve = next;
                }
                else
                {
                    for (auto &p : nodes)
                        if (p.second == prve)
                            path.push_back(p.first);
                    prve = next;
                    break;
                }
            }
            std::cout << "Func has run \"" << path.size() + 1 << "\" times" << std::endl;
        }

        for (auto &p : nodes)
            if (p.second == prve)
                path.push_back(p.first);

        std::ofstream output("output.txt");
        if (output.is_open())
        {
            for (int i = 0; i < path.size() - 1; i++)
            {
                output << path[i] << " ";
            }
            output << path[path.size() - 1];
        }
        return path;
    }
};

#endif
