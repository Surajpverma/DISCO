/**
 * Group ID -
 * Member 1 Name -
 * Member 1 BITS ID -
 * Member 2 Name -
 * Member 2 BITS ID -
 * Member 3 Name -
 * Member 3 BITS ID -
 */

// ---------------------------DO NOT MODIFY (Begin)------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <limits.h>

typedef struct Graph
{
    int n;                // Size of the graph
    int **adj;            // Adjacency matrix
    char **station_names; // Array of station names
} Graph;

/**
 * This function has been pre-filled. It reads the input testcase
 * and creates the Graph structure from it.
 */
Graph *create_graph(char input_file_path[])
{
    FILE *f = fopen(input_file_path, "r");
    Graph *g = (Graph *)malloc(sizeof(Graph));
    // Size of graph
    fscanf(f, "%d", &(g->n));
    printf("Done0\n");
    // Names of stations
    g->station_names = (char **)malloc(g->n * sizeof(char *));
    for (int i = 0; i < g->n; i++)
    {
        g->station_names[i] = (char *)malloc(100 * sizeof(char));
        fscanf(f, "%s", g->station_names[i]);
    }
    printf("Done1\n");
    // Adjacency matrix
    g->adj = (int **)malloc(g->n * sizeof(int *));
    for (int i = 0; i < g->n; i++)
    {
        g->adj[i] = (int *)calloc(g->n, sizeof(int));
    }
    int edges;
    fscanf(f, "%d", &edges);
    for (int i = 0; i < edges; i++)
    {
        int x, y;
        fscanf(f, "%d%d", &x, &y);
        g->adj[x][y] = 1;
        g->adj[y][x] = 1;
    }
    fclose(f);
    for (int i = 0; i < g->n; i++)
    {
        for (int j = 0; j < g->n; j++)
            printf("%d ", g->adj[i][j]);
        printf("\n");
    }
    return g;
}
// ---------------------------DO NOT MODIFY (End)------------------------------

/**
 * Q.1
 * Return the number of junctions.
 */
int find_junctions(Graph *g)
{
    int junctions = 0;
    int counter = 0;

    for (int i = 0; i < g->n; i++)
    {
        for (int j = 0; j < g->n; j++)
        {
            if (g->adj[i][j] == 1)
                counter++;
            if (counter >= 4)
            {
                junctions++;
                break;
            }
        }
        counter = 0;
    }

    return junctions;
}

/**
 * Q.2
 * Return true if the tour specified in the question is possible for this
 * graph, else return false.
 * Parameter SAME_STATION - If it is true, then question 2(a) must be solved.
 * If false, then question 2(b) must be solved.
 */
bool sheldons_tour(Graph *g, bool SAME_STATION)
{
    int num_odd_degree = 0;

    for (int i = 0; i < g->n; i++)
    {
        int degree = 0;

        for (int j = 0; j < g->n; j++)
            if (g->adj[i][j] == 1)
                if (i == j)
                    degree += 2;
                else
                    degree++;

        if (degree % 2 != 0)
            num_odd_degree++;
    }

    if (SAME_STATION)
        return num_odd_degree == 0;
    else
        return num_odd_degree == 2;
}

/**
 * Q.3
 * Return closure, an n * n matrix filled with 0s and 1s as required.
 */
int **warshall(Graph *g)
{
    // Do not modify or delete pre-filled code!
    int **closure = (int **)malloc(g->n * sizeof(int *));
    for (int i = 0; i < g->n; i++)
        closure[i] = (int *)calloc(g->n, sizeof(int));

    // Code goes here
    for (int i = 0; i < g->n; i++)
        for (int j = 0; j < g->n; j++)
            closure[i][j] = g->adj[i][j];

    printf("Closure Matrix:\n");
    for (int s = 0; s < g->n; s++)
        for (int p = 0; p < g->n; p++)
            for (int v = 0; v < g->n; v++)
                closure[p][v] = closure[p][v] || (closure[p][s] && closure[s][v]);

    return closure; // Do not modify
}

/**
 * Q.3
 * Return the number of impossible pairs.
 */
int find_impossible_pairs(Graph *g)
{
    int **closure = warshall(g); // Do not modify

    int Required_Pairs = 0;

    for (int i = 0; i < g->n; i++)
    {
        for (int j = 0; j < g->n; j++)
            printf("%d ", closure[i][j]);
        printf("\n");
    }

    for (int s = 0; s < g->n; s++)
        for (int p = 0; p < g->n; p++)
            if (!closure[s][p] && !closure[p][s])
                Required_Pairs++;

    return Required_Pairs / 2;
}

/**
 * Q.4
 * Return the number of vital train tracks.
 */
int find_vital_train_tracks(Graph *g)
{
    int *visited = (int *)malloc(g->n * sizeof(int));
    int *disc = (int *)malloc(g->n * sizeof(int));
    int *low = (int *)malloc(g->n * sizeof(int));
    int time = 0;
    int bridgeCount = 0;

    for (int i = 0; i < g->n; i++)
        visited[i] = 0, disc[i] = -1, low[i] = -1;

    for (int i = 0; i < g->n; i++)
        if (!visited[i])
            for (int v = 0; v < g->n; v++)
                if (g->adj[i][v] && !visited[v])
                {
                    visited[v] = 1;
                    disc[v] = low[v] = ++time;
                    for (int w = 0; w < g->n; w++)
                        if (g->adj[v][w])
                            if (!visited[w])
                            {
                                visited[w] = 1;
                                disc[w] = low[w] = ++time;
                                bridgeCount++;
                            }
                            else if (w != i)
                                low[v] = (low[v] < disc[w]) ? low[v] : disc[w];
                }
    return bridgeCount;
}

/**
 * Q.5
 * Return upgrades, an array of size n.
 * upgrades[i] should be 0 if railway station i gets a restaurant
 * upgrades[i] should be 1 if railway station i gets a maintenance depot
 * If it is not possible to upgrade, then return "-1" in the entire array
 */
int *upgrade_railway_stations(Graph *g)
{
    int *upgrades = (int *)calloc(g->n, sizeof(int)); // Do not modify
    // Code goes here
    for (int i = 0; i < g->n; i++)
        upgrades[i] = -1;

    for (int i = 0; i < g->n; i++)
    {
        int changed = 0;
        upgrades[i] = 0;

        for (int j = 0; j < g->n; j++)
        {
            if (i == j)
                continue;

            if (g->adj[i][j] == 1 && upgrades[j] == 0)
            {
                if (changed == 0)
                {
                    upgrades[i] = 1;
                    changed = 1;
                }
                else
                {
                    for (int j = 0; j < g->n; j++)
                        if (g->adj[i][j] == 1 && upgrades[j] == 1 && changed == 1)
                        {
                            for (int k = 0; k < g->n; k++)
                                upgrades[k] = -1;
                            return upgrades;
                        }
                }
            }
        }

        for (int j = 0; j < g->n; j++)
            if (g->adj[i][j] == 1 && upgrades[j] == 1 && changed == 1)
            {
                for (int k = 0; k < g->n; k++)
                    upgrades[k] = -1;
                return upgrades;
            }
    }

    return upgrades; //  Do not modify
}

/**
 * Q.6
 * Return distance between X and Y
 * city_x is the index of X, city_y is the index of Y
 */
int distance(Graph *g, int city_x, int city_y)
{
    int *visited = (int *)malloc(g->n * sizeof(int));
    int *distance = (int *)malloc(g->n * sizeof(int));

    for (int i = 0; i < g->n; i += 1)
    {
        visited[i] = 0;
        distance[i] = INT_MAX;
    }

    visited[city_x] = 1;
    distance[city_x] = 0;

    int queue[g->n];
    int Front_point = 0;
    int Rear_point = 0;

    queue[Rear_point] = city_x;
    Rear_point += 1;

    while (Front_point < Rear_point)
    {

        int curr = queue[Front_point];
        Front_point += 1;

        for (int i = 0; i < g->n; i += 1)
        {
            if (g->adj[curr][i] == 1 && visited[i] == false)
            {
                visited[i] = 1;
                distance[i] = distance[curr] + 1;
                queue[Rear_point] = i;
                Rear_point += 1;
            }
        }
    }

    int Final_ans = distance[city_y];
    free(visited);
    free(distance);
    if (Final_ans == INT_MAX)
    {
        return -1;
    }
    return Final_ans;
}

/**
 * Q.7
 * Return the index of any one possible railway capital in the network
 */
int railway_capital(Graph *g)
{
    int Capital_city_index = -1;
    int min_sum = INT_MAX;
    for (int i = 0; i < g->n; i++)
    {
        int sum = 0;
        for (int j = 0; j < g->n; j++)
        {
            if (i != j)
            {
                int Distance_btw_i_and_j = distance(g, i, j);
                if (Distance_btw_i_and_j != -1)
                {
                    sum += Distance_btw_i_and_j;
                }
            }
        }
        if (sum < min_sum)
        {
            min_sum = sum;
            Capital_city_index = i;
        }
    }

    return Capital_city_index;
}

/**
 * Helper function for Q.8
 */
bool maharaja_express_tour(Graph *g, int source, int current_city, int previous_city, int *visited)
{
    if (current_city == source && visited[source])
        return true;

    visited[current_city] = 1;

    for (int i = 0; i < g->n; i++)
        if (g->adj[current_city][i] && !g->adj[current_city][previous_city] && (g->adj[current_city][source] || !visited[i]))
            maharaja_express_tour(g, source, i, current_city, visited);
        else
            return false;
}

/**
 * Q.8
 * Return true if it is possible to have a Maharaja Express tour
 * starting from source city, else return false.
 */
bool maharaja_express(Graph *g, int source)
{
    int *visited = (int *)malloc(g->n * sizeof(int));
    for (int i = 0; i < g->n; i++)
    {
        visited[i] = 0;
    }
    // Hint: Call the helper function and pass the visited array created here.
    for (int i = 0; i < g->n; i++)
        maharaja_express_tour(g, source, source, -1, visited);
}

int main()
{
    char input_file_path[100] = "testcase_1.txt"; // Can be modified
    Graph *g = create_graph(input_file_path);     // Do not modify

    // Code goes here
    // Q1
    printf("\nSOLUTION OF Q1:\n");
    printf("Number of junctions = %d\n", find_junctions(g));
    // Q2
    printf("\nSOLUTION OF Q2:\n");
    if (sheldons_tour(g, true))
    {
        printf("Sheldon's tour (ending in same city as start) = POSSIBLE\n");
    }
    else
    {
        printf("Sheldon's tour (ending in same city as start) = IMPOSSIBLE\n");
    }
    if (sheldons_tour(g, false))
    {
        printf("Sheldon's tour (ending in different city as start) = POSSIBLE\n");
    }
    else
    {
        printf("Sheldon's tour (ending in different city as start) = IMPOSSIBLE\n");
    }
    // Q3
    printf("\nSOLUTION OF Q3:\n");
    printf("Number of impossible pairs = %d\n", find_impossible_pairs(g));
    // Q4
    printf("\nSOLUTION OF Q4:\n");
    printf("Number of vital tracks = %d\n", find_vital_train_tracks(g));
    // Q5
    printf("\nSOLUTION OF Q5:\n");
    int *upgrade = upgrade_railway_stations(g);
    if (upgrade[0] == -1)
        printf("Not possible to upgarde\n");
    else
        for (int i = 0; i < g->n; i++)
            printf("Upgrade to Station %s = %s\n", g->station_names[i], upgrade[i] == 1 ? "Restaurant" : "Maintainance Depot");
    // Q6
    printf("\nSOLUTION OF Q6:\n");
    for (int i = 0; i < g->n - 1; i++)
        for (int j = i + 1; j < g->n; j++)
            printf("Distance between %s and %s = %d\n", g->station_names[i], g->station_names[j], distance(g, i, j));
    // Q7
    printf("\nSOLUTION OF Q7:\n");
    printf("Railway Capital = %s\n", g->station_names[railway_capital(g)]);
    // Q8
    printf("\nSOLUTION OF Q8:\n");
    for (int i = 0; i < g->n; i++)
    {
        printf("%d", maharaja_express(g, i));
        if (maharaja_express(g, i))
            printf("Maharaja Express tour starting from %s = POSSIBLE\n", g->station_names[i]);
        else
            printf("Maharaja Express tour starting from %s = IMPOSSIBLE\n", g->station_names[i]);
    }

    return 0;
}