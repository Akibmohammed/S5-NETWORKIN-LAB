#include <stdio.h>
#define INF 999
#define MAX_NODE 10

int main(){
    int numnodes;
    int costmatrix[MAX_NODE][MAX_NODE];
    int distancevector[MAX_NODE][MAX_NODE];
    int nexthop[MAX_NODE][MAX_NODE];

    printf("Enter the number of nodes: ");
    scanf("%d", &numnodes);

    printf("Enter the cost matrix (use %d for infinity):\n", INF);
    for(int i = 0; i < numnodes; i++){
        for(int j = 0; j < numnodes; j++){
            scanf("%d", &costmatrix[i][j]);
            if(i == j){
                costmatrix[i][j] = 0; 
            }
            distancevector[i][j] = costmatrix[i][j];  
            if(costmatrix[i][j] != INF && i != j){
                nexthop[i][j] = j; 
            } else {
                nexthop[i][j] = -1; 
            }
        }
    }
    int updated;
    do{
        updated = 0;
        for(int i = 0; i < numnodes; i++){
            for(int j = 0; j < numnodes; j++){
                for(int k = 0; k < numnodes; k++){
                    if(distancevector[i][j] > costmatrix[i][k] + distancevector[k][j]){
                        distancevector[i][j] = costmatrix[i][k] + distancevector[k][j];
                        nexthop[i][j] = nexthop[i][k];
                        updated = 1;
                    }
                }
            }
        }
    } while(updated);
    
    for(int i = 0; i < numnodes; i++){
        printf("\nRouting table for node %d:\n", i);
        printf("Destination\tCost\tNext Hop\n");
        for(int j = 0; j < numnodes; j++){
            if(i != j){
                printf("%d\t\t%d\t\t%d\n", j, distancevector[i][j], nexthop[i][j]);
            }
        }
    }

    return 0;
}
