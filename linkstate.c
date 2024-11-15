#include <stdio.h>
#include <stdbool.h>

#define INF 999
#define MAX_NODES 10

void linkstate(int graph[MAX_NODES][MAX_NODES], int numnodes, int src) {
    int distance[MAX_NODES];  
    bool visited[MAX_NODES]; 
    int nexthop[MAX_NODES];   
    
    for (int i = 0; i < numnodes; i++) {
        distance[i] = INF;    
        visited[i] = false;  
        nexthop[i] = -1;      
    }    
    distance[src] = 0;

    for (int count = 0; count < numnodes - 1; count++) {
        int min = INF, u;

        for (int v = 0; v < numnodes; v++) {
            if (!visited[v] && distance[v] <= min) {
                min = distance[v];
                u = v;
            }
        }

        visited[u] = true;

        for (int v = 0; v < numnodes; v++) {
            if (!visited[v] && graph[u][v] && distance[u] != INF 
                && distance[u] + graph[u][v] < distance[v]) {
                distance[v] = distance[u] + graph[u][v];
                nexthop[v] = u;
        }
    }
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

    printf("Enter the number of nodes: ");
    scanf("%d", &numnodes);

    printf("Enter the cost matrix (use %d for infinity):\n", INF);
    for (int i = 0; i < numnodes; i++) {
        for (int j = 0; j < numnodes; j++) {
            scanf("%d", &graph[i][j]);
            if (i == j) graph[i][j] = 0;  
        }
    }

    for (int src = 0; src < numnodes; src++) {
        linkstate(graph, numnodes, src);
        printf("\n");
    }

    return 0;
}
