#include <limits.h>
#include <algorithm>
#include "graph.h"
#include "gtest/gtest.h"


TEST(graph, threecomponents) {
    Graph graph;
    VPtr v0(new Vertex("0"));
    VPtr v1(new Vertex("1"));
    VPtr v2(new Vertex("2"));
    VPtr v3(new Vertex("3"));
    VPtr v4(new Vertex("4"));
    VPtr v5(new Vertex("5"));
    VPtr v6(new Vertex("6"));
    VPtr v7(new Vertex("7"));
    VPtr v8(new Vertex("8"));
    VPtr v9(new Vertex("9"));
    VPtr v10(new Vertex("10"));
    VPtr v11(new Vertex("11"));
    VPtr v12(new Vertex("12"));

    // Component 1
    graph.insert(Edge(v0, v1));
    graph.insert(Edge(v0, v9));
    graph.insert(Edge(v1, v9));
    graph.insert(Edge(v1, v4));

    // Component 2
    graph.insert(Edge(v8, v11));

    // Component 3
    graph.insert(Edge(v2, v7));
    graph.insert(Edge(v2, v10));
    graph.insert(Edge(v2, v12));
    graph.insert(Edge(v3, v12));
    graph.insert(Edge(v5, v12));
    graph.insert(Edge(v6, v10));
    graph.insert(Edge(v6, v12));
    graph.insert(Edge(v7, v10));

    graph.dfs();
    STree tree = graph.searchTree();
    std::for_each(tree.begin(), tree.end(), [&](VPtr vptr) { std::cout << *vptr << " "; });
    std::cout << std::endl;
    std::cout << "Number of components = " << graph.components() << std::endl;
    std::cout << "Predecessor of " << *v2 << " is " <<
        (v2->pred() == 0 ? Vertex("None") : *v2->pred())
        << std::endl;
}

TEST(Graph, bfsTest)
{
    Graph graph;
    VPtr a(new Vertex("A"));
    VPtr b(new Vertex("B"));
    VPtr c(new Vertex("C"));
    VPtr d(new Vertex("D"));
    VPtr e(new Vertex("E"));
    VPtr f(new Vertex("F"));
    VPtr g(new Vertex("G"));
    VPtr h(new Vertex("H"));
    VPtr i(new Vertex("I"));
    VPtr j(new Vertex("J"));
    VPtr k(new Vertex("K"));
    VPtr l(new Vertex("L"));
    VPtr m(new Vertex("M"));

    graph.insert(Edge(a, c));
    graph.insert(Edge(a, b));
    graph.insert(Edge(b, e));
    graph.insert(Edge(b, d));
    graph.insert(Edge(e, g));
    graph.insert(Edge(e, f));
    graph.insert(Edge(g, i));
    graph.insert(Edge(g, h));
    graph.insert(Edge(h, k));
    graph.insert(Edge(h, j));
    graph.insert(Edge(j, m));
    graph.insert(Edge(j, l));

    std::cout << graph << std::endl;
    std::cout << "BFS" << std::endl;
    graph.bfs(a);
    std::cout << std::endl;

    std::cout << "DFS" << std::endl;
    // graph.dfsWithStack();
    // std::cout << std::endl;
    graph.dfs();
    std::cout << std::endl;
    std::cout << "Number of components = " << graph.components() << std::endl;

    bool connected = graph.isConnected(a, m, true);
    std::cout << std::boolalpha << connected << std::endl;
}

bool biPartiteHelper(Graph& graph, const VPtr& u, VertexColour colour)
{
    // Colour the vertices in alternate BLACK / WHITE. 
    colour ==  BLACK ? u->colour(WHITE) : u->colour(BLACK);

    AdjList adjList = graph.adjList();
    AdjList::iterator it = adjList.find(u);
    for (VList::iterator vit = it->second.begin(); vit != it->second.end(); ++vit) {
        if ((*vit)->colour() == GRAY) {
            if (!biPartiteHelper(graph, *vit, u->colour()))
                return false;
        } else if ((*vit)->colour() != colour) { // check the back edge colour
            return false;
        }
    }
    return true;
}

bool isBiPartitie(Graph& graph)
{
    // Colour all vertices GRAY at first.
    AdjList::iterator it;
    AdjList adjList = graph.adjList();
    for (it = adjList.begin(); it != adjList.end(); ++it) {
        it->first->colour(GRAY);
    }
    bool bipartite = false;
    for (it = adjList.begin(); it != adjList.end(); ++it) {
        if (it->first->colour() == GRAY) {
            if (biPartiteHelper(graph, it->first, BLACK)) {
                bipartite = true;
            }
        }
    }
    return bipartite;
}

TEST(Graph, isBiPartite)
{
    Graph graph;
    // This graph is not bipartite
    VPtr v0(new Vertex("0"));
    VPtr v1(new Vertex("1"));
    VPtr v2(new Vertex("2"));
    VPtr v3(new Vertex("3"));
    VPtr v4(new Vertex("4"));
    VPtr v5(new Vertex("5"));
    VPtr v6(new Vertex("6"));
    VPtr v7(new Vertex("7"));
    graph.insert(Edge(v0, v2));
    graph.insert(Edge(v0, v7));
    graph.insert(Edge(v0, v5));
    graph.insert(Edge(v1, v7));
    graph.insert(Edge(v2, v6));
    graph.insert(Edge(v3, v4));
    graph.insert(Edge(v3, v5));
    graph.insert(Edge(v4, v5));
    graph.insert(Edge(v4, v6));
    graph.insert(Edge(v4, v7));
    bool b = isBiPartitie(graph);
    std::cout << "is bipartite  = " << std::endl;
    EXPECT_EQ(b,false);
}

TEST(Graph, isBiPartitie_2)
{
    Graph graph;
    // This graph is bipartite
    VPtr v0(new Vertex("0"));
    VPtr v1(new Vertex("1"));
    VPtr v2(new Vertex("2"));
    VPtr v3(new Vertex("3"));
    VPtr v4(new Vertex("4"));
    VPtr v5(new Vertex("5"));
    VPtr v6(new Vertex("6"));
    VPtr v7(new Vertex("7"));
    VPtr v8(new Vertex("8"));
    VPtr v9(new Vertex("9"));
    VPtr v10(new Vertex("10"));
    VPtr v11(new Vertex("11"));
    VPtr v12(new Vertex("12"));
    graph.insert(Edge(v0, v1));
    graph.insert(Edge(v0, v3));
    graph.insert(Edge(v0, v5));
    graph.insert(Edge(v2, v1));
    graph.insert(Edge(v2, v9));
    graph.insert(Edge(v4, v3));
    graph.insert(Edge(v4, v5));
    graph.insert(Edge(v4, v11));
    graph.insert(Edge(v6, v7));
    graph.insert(Edge(v6, v9));
    graph.insert(Edge(v8, v7));
    graph.insert(Edge(v8, v9));
    graph.insert(Edge(v10, v9));
    graph.insert(Edge(v12, v9));
    graph.insert(Edge(v12, v11));
    bool b = isBiPartitie(graph);
    std::cout << "is bipartite  = " << std::endl;
    EXPECT_EQ(b ,true);
}

typedef std::map<VPtr, int> VisitOrderMap;

void edgeConnectivityHelper(Graph& graph, 
    VisitOrderMap& ordMap, VisitOrderMap& lowMap, int visitCount, int& bridgeCount, const Edge& edge)
{
    VPtr w = edge.v();
    ordMap[w] = visitCount++;
    lowMap[w] = ordMap[w];

    AdjList adjList = graph.adjList();
    AdjList::iterator it = adjList.find(w);
    for (VList::iterator vit = it->second.begin(); vit != it->second.end(); ++vit) {
        VPtr t = *vit;
        if (ordMap[t] == -1) {
            edgeConnectivityHelper(graph, ordMap, lowMap, visitCount, bridgeCount, Edge(w, t));
            if (lowMap[w] > lowMap[t]) {
                lowMap[w] = lowMap[t];
            }
            if (lowMap[t] == ordMap[t]) {
                bridgeCount++; // w-t is a bridge;
                std::cout << "Bridge found at " << *w << "-" << *t << std::endl;
            }
        } else if (t != edge.u()) {
            if (lowMap[w] > ordMap[t]) {
                lowMap[w] = ordMap[t];
            }
        }
    }
}

int edgeConnectivity(Graph& graph)
{
    int visitCount = 0;
    int bridgeCount = 0;
    VisitOrderMap ordMap;
    VisitOrderMap lowMap;

    AdjList::iterator it;
    AdjList adjList = graph.adjList();
    for (it = adjList.begin(); it != adjList.end(); ++it) {
        ordMap.insert(VisitOrderMap::value_type(it->first, -1));
        lowMap.insert(VisitOrderMap::value_type(it->first, -1));
    }

    for (it = adjList.begin(); it != adjList.end(); ++it) {
        VisitOrderMap::iterator vit = ordMap.find(it->first);
        if (vit != ordMap.end()) {
            if (vit->second == -1) {
                edgeConnectivityHelper(graph, ordMap, lowMap, visitCount,
                    bridgeCount, Edge(vit->first, vit->first));
            }
        }
    }

    return bridgeCount;
}

TEST(Graph, edgeConnectivity)
{
    Graph graph;

    // This graph has 3 edge connected components. The bridges are 
    // 0-5, 6-7 and 11-12.
    // Refer to "Algorithms in Java, Part 5" by Robert Sedgewick, p 113, Figure 18.16.

    VPtr v0(new Vertex("0"));
    VPtr v1(new Vertex("1"));
    VPtr v2(new Vertex("2"));
    VPtr v3(new Vertex("3"));
    VPtr v4(new Vertex("4"));
    VPtr v5(new Vertex("5"));
    VPtr v6(new Vertex("6"));
    VPtr v7(new Vertex("7"));
    VPtr v8(new Vertex("8"));
    VPtr v9(new Vertex("9"));
    VPtr v10(new Vertex("10"));
    VPtr v11(new Vertex("11"));
    VPtr v12(new Vertex("12"));

    graph.insert(Edge(v0, v5));
    graph.insert(Edge(v0, v1));
    graph.insert(Edge(v0, v6));
    graph.insert(Edge(v1, v2));
    graph.insert(Edge(v2, v6));
    graph.insert(Edge(v6, v7));
    graph.insert(Edge(v7, v8));
    graph.insert(Edge(v7, v10));
    graph.insert(Edge(v8, v10));

    graph.insert(Edge(v5, v3));
    graph.insert(Edge(v5, v4));
    graph.insert(Edge(v3, v4));
    graph.insert(Edge(v4, v9));
    graph.insert(Edge(v4, v11));
    graph.insert(Edge(v9, v11));

    graph.insert(Edge(v11, v12));

    int count = edgeConnectivity(graph);
    std::cout << "Edge connectivity = " << count << std::endl;
}
// Step 3. Call RUN_ALL_TESTS() in main().
//
// We do this by linking in src/gtest_main.cc file, which consists of
// a main() function which calls RUN_ALL_TESTS() for us.
//
// This runs all the tests you've defined, prints the result, and
// returns 0 if successful, or 1 otherwise.
//
// Did you notice that we didn't register the tests?  The
// RUN_ALL_TESTS() macro magically knows about all the tests we
// defined.  Isn't this convenient?
