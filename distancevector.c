#include <stdio.h>
#define INF 999  // Represents infinity for nodes not directly connected
#define MAX_NODES 10

int main() {
    int numNodes;
    int costMatrix[MAX_NODES][MAX_NODES];
    int distanceVector[MAX_NODES][MAX_NODES];
    int nextHop[MAX_NODES][MAX_NODES];

    // Read the number of nodes in the network
    printf("Enter the number of nodes: ");
    scanf("%d", &numNodes);

    // Initialize the cost matrix
    printf("Enter the cost matrix (use %d for infinity):\n", INF);
    for (int i = 0; i < numNodes; i++) {
        for (int j = 0; j < numNodes; j++) {
            scanf("%d", &costMatrix[i][j]);
            if (i == j)
                costMatrix[i][j] = 0;  // Distance to itself is zero
            distanceVector[i][j] = costMatrix[i][j];  // Initialize distance vector
            if (costMatrix[i][j] != INF && i != j)
                nextHop[i][j] = j;  // Initial next hop is the direct neighbor
            else
                nextHop[i][j] = -1; // No direct connection
        }
    }

    // Distance Vector Routing Algorithm
    int updated;
    do {
        updated = 0;
        for (int i = 0; i < numNodes; i++) {
            for (int j = 0; j < numNodes; j++) {
                for (int k = 0; k < numNodes; k++) {
                    if (distanceVector[i][j] > costMatrix[i][k] + distanceVector[k][j]) {
                        distanceVector[i][j] = costMatrix[i][k] + distanceVector[k][j];
                        nextHop[i][j] = k;
                        updated = 1;
                    }
                }
            }
        }
    } while (updated);

    // Display the distance and next hop for each pair of nodes
    for (int i = 0; i < numNodes; i++) {
        printf("\nRouting table for node %d:\n", i);
        printf("Destination\tCost\tNext Hop\n");
        for (int j = 0; j < numNodes; j++) {
            if (i != j) {
                printf("%d\t\t%d\t\t%d\n", j, distanceVector[i][j], nextHop[i][j]);
            }
        }
    }

    return 0;
}
