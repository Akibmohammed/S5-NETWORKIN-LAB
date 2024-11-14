#include <stdio.h>
#include <stdbool.h>

#define INF 999
#define MAX_NODES 10

void linkstate(int graph[MAX_NODES][MAX_NODES], int numnodes, int src) {
    int distance[MAX_NODES];  // Distance from src to each node
    bool visited[MAX_NODES];  // Track visited nodes
    int nexthop[MAX_NODES];   // Track the next hop for each destination

    // Initialize distances and visited array
    for (int i = 0; i < numnodes; i++) {
        distance[i] = INF;    // Start with all distances as infinity
        visited[i] = false;   // No nodes are visited at the start
        nexthop[i] = -1;      // No next hop at the beginning
    }
    
    distance[src] = 0;  // Distance to itself is 0

    // Find shortest path for each node
    for (int count = 0; count < numnodes - 1; count++) {
        int min = INF, u;

        // Select the minimum distance node that hasn't been visited
        for (int v = 0; v < numnodes; v++) {
            if (!visited[v] && distance[v] <= min) {
                min = distance[v];
                u = v;
            }
        }

        visited[u] = true;

        // Update distances for the neighboring nodes
        for (int v = 0; v < numnodes; v++) {
            if (!visited[v] && graph[u][v] && distance[u] != INF 
                && distance[u] + graph[u][v] < distance[v]) {
                distance[v] = distance[u] + graph[u][v];
                nexthop[v] = u;  // Set the next hop as u for node v
            }
        }
    }

    // Display the shortest distances and next hops
    printf("Routing Table for Node %d:\n", src);
    printf("Destination\tCost\tNext Hop\n");
    for (int i = 0; i < numnodes; i++) {
        if (i != src) {
            printf("%d\t\t%d\t\t%d\n", i, distance[i], nexthop[i]);
        }
    }
}

int main() {
    int numnodes;
    int graph[MAX_NODES][MAX_NODES];

    // Input number of nodes and cost adjacency matrix
    printf("Enter the number of nodes: ");
    scanf("%d", &numnodes);

    printf("Enter the cost matrix (use %d for infinity):\n", INF);
    for (int i = 0; i < numnodes; i++) {
        for (int j = 0; j < numnodes; j++) {
            scanf("%d", &graph[i][j]);
            if (i == j) graph[i][j] = 0;  // Distance to itself is zero
        }
    }

    // Run Dijkstra's algorithm for each node as the source
    for (int src = 0; src < numnodes; src++) {
        linkstate(graph, numnodes, src);
        printf("\n");
    }

    return 0;
}
