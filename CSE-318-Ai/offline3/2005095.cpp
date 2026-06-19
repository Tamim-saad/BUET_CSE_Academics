#include <bits/stdc++.h>
using namespace std;

#define fast                                                                   \
  ios_base::sync_with_stdio(0);                                                \
  cin.tie(0);                                                                  \
  cout.tie(0);

struct City {
  int id;
  double x, y;
};

double euclideanDistance(const City &a, const City &b) {
  return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

double tourLength(const vector<City> &cities, const vector<int> &tour) {
  double length = 0.0;
  int n = tour.size();
  for (int i = 0; i < n; i++) {
    int from = tour[i];
    int to = tour[(i + 1) % n];
    length += euclideanDistance(cities[from], cities[to]);
  }
  return length;
}

// Nearest Neighbor Heuristic (NNH)
vector<int> nearestNeighbor(const vector<City> &cities) {
  int n = cities.size();
  vector<int> tour;
  vector<bool> visited(n, false);

  // Seed random number generator for starting city
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dis(0, n - 1);

  int current = dis(gen);
  tour.push_back(current);
  visited[current] = true;

  while (tour.size() < n) {
    double minDist = numeric_limits<double>::max();
    int nextCity = -1;

    for (int i = 0; i < n; ++i) {
      if (!visited[i]) {
        double dist = euclideanDistance(cities[current], cities[i]);
        if (dist < minDist) {
          minDist = dist;
          nextCity = i;
        }
      }
    }

    tour.push_back(nextCity);
    visited[nextCity] = true;
    current = nextCity;
  }

  return tour;
}

// Greedy Heuristic (Nearest Insertion)
vector<int> kruskalGreedyTSP(const vector<City> &cities) {
  int n = cities.size();
  vector<pair<double, pair<int, int>>> edges;
  vector<int> degree(n, 0);
  vector<int> parent(n);
  vector<int> tour;

  // Initialize Union-Find structure
  iota(parent.begin(), parent.end(), 0);

  function<int(int)> find = [&](int u) {
    return (parent[u] == u) ? u : (parent[u] = find(parent[u]));
  };

  auto unionSet = [&](int u, int v) { parent[find(u)] = find(v); };

  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      double dist = euclideanDistance(cities[i], cities[j]);
      edges.push_back({dist, {i, j}});
    }
  }

  sort(edges.begin(), edges.end());

  vector<pair<int, int>> selectedEdges;
  for (const auto &edge : edges) {
    int u = edge.second.first;
    int v = edge.second.second;

    if (degree[u] < 2 && degree[v] < 2 && find(u) != find(v)) {
      selectedEdges.push_back({u, v});
      unionSet(u, v);
      degree[u]++;
      degree[v]++;
    }

    if (selectedEdges.size() == n - 1) {
      break;
    }
  }

  // Convert edges into a tour
  vector<vector<int>> adjList(n);
  for (const auto &[u, v] : selectedEdges) {
    adjList[u].push_back(v);
    adjList[v].push_back(u);
  }

  vector<bool> visited(n, false);
  function<void(int)> dfs = [&](int u) {
    visited[u] = true;
    tour.push_back(u);
    for (int v : adjList[u]) {
      if (!visited[v]) {
        dfs(v);
      }
    }
  };

  dfs(0);

  // Ensure the tour is circular
  tour.push_back(tour.front());
  return tour;
}

// MST-based Heuristic using your own MST code
vector<int> mstHeuristic(const vector<City> &cities) {
  int n = cities.size();

  vector<vector<double>> graph(n, vector<double>(n, 0));

  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      double dist = euclideanDistance(cities[i], cities[j]);
      graph[i][j] = dist;
      graph[j][i] = dist;
    }
  }

  const double INF = numeric_limits<double>::max();
  vector<double> value(n, INF);
  vector<int> parent(n, -1);
  vector<bool> visited(n, false);

  typedef pair<double, int> PDI; // Pair of (weight, vertex)
  priority_queue<PDI, vector<PDI>, greater<PDI>> pq;

  int startVertex = 0;
  value[startVertex] = 0.0;
  pq.push({0.0, startVertex});

  while (!pq.empty()) {
    int curr = pq.top().second;
    pq.pop();

    if (visited[curr])
      continue;
    visited[curr] = true;

    for (int i = 0; i < n; ++i) {
      if (!visited[i] && graph[curr][i] > 0 && graph[curr][i] < value[i]) {
        value[i] = graph[curr][i];
        parent[i] = curr;
        pq.push({value[i], i});
      }
    }
  }

  // Build adjacency list from parent array
  vector<vector<int>> adjList(n);
  for (int i = 0; i < n; ++i) {
    if (parent[i] != -1) {
      adjList[i].push_back(parent[i]);
      adjList[parent[i]].push_back(i);
    }
  }

  // Perform a preorder traversal of the MST to get the tour
  vector<int> tour;
  vector<bool> dfsVisited(n, false);

  function<void(int)> dfs = [&](int u) {
    dfsVisited[u] = true;
    tour.push_back(u);
    for (int v : adjList[u]) {
      if (!dfsVisited[v]) {
        dfs(v);
      }
    }
  };

  dfs(startVertex);
  return tour;
}

// 2-opt Heuristic
vector<int> twoOpt(const vector<City> &cities, vector<int> tour) {
  int n = tour.size();
  bool improvement = true;

  while (improvement) {
    improvement = false;
    double bestDistance = tourLength(cities, tour);

    for (int i = 0; i < n - 1; ++i) {
      for (int k = i + 1; k < n; ++k) {
        // Create a new tour by reversing the segment between i and k
        vector<int> newTour = tour;
        reverse(newTour.begin() + i, newTour.begin() + k + 1);
        double newDistance = tourLength(cities, newTour);

        if (newDistance < bestDistance) {
          tour = newTour;
          bestDistance = newDistance;
          improvement = true;
          break;
        }
      }
      if (improvement)
        break;
    }
  }

  return tour;
}

// Node Swap Heuristic
vector<int> nodeSwap(const vector<City> &cities, vector<int> tour) {
  int n = tour.size();
  bool improvement = true;

  while (improvement) {
    improvement = false;
    double bestDistance = tourLength(cities, tour);

    for (int i = 0; i < n; ++i) {
      for (int j = i + 1; j < n; ++j) {
        // Swap two nodes
        vector<int> newTour = tour;
        swap(newTour[i], newTour[j]);
        double newDistance = tourLength(cities, newTour);

        if (newDistance < bestDistance) {
          tour = newTour;
          bestDistance = newDistance;
          improvement = true;
          break;
        }
      }
      if (improvement)
        break;
    }
  }

  return tour;
}

// Node Insertion Heuristic (1-Shift)
vector<int> nodeInsertion(const vector<City> &cities, vector<int> tour) {
  int n = tour.size();
  bool improvement = true;

  while (improvement) {
    improvement = false;

    for (int i = 1; i < n; ++i) {
      int node = tour[i];
      int prevNode = tour[i - 1];
      int nextNode = tour[(i + 1) % n];

      // Remove the node from its current position
      tour.erase(tour.begin() + i);

      double bestDelta = 0.0;
      int bestPosition = i;

      for (int j = 1; j < n; ++j) { // Possible new positions
        if (j == i)
          continue;

        int prevInsertNode = tour[j - 1];
        int insertNode = tour[j % n];

        double delta =
            -euclideanDistance(cities[prevNode], cities[node]) -
            euclideanDistance(cities[node], cities[nextNode]) +
            euclideanDistance(cities[prevNode], cities[nextNode]) -
            euclideanDistance(cities[prevInsertNode], cities[insertNode]) +
            euclideanDistance(cities[prevInsertNode], cities[node]) +
            euclideanDistance(cities[node], cities[insertNode]);

        if (delta < bestDelta) {
          bestDelta = delta;
          bestPosition = j;
          improvement = true;
        }
      }

      tour.insert(tour.begin() + bestPosition, node);

      if (improvement)
        break;
      else
        tour.insert(tour.begin() + i, node);
    }
  }
  return tour;
}

vector<City> readCitiesFromFile(const string &filePath) {
  vector<City> cities;
  ifstream inputFile(filePath);

  if (!inputFile.is_open()) {
    cerr << "Error: Could not open file " << filePath << endl;
    exit(1);
  }

  string line;
  bool isDataSection = false;

  while (getline(inputFile, line)) {
    line.erase(line.find_last_not_of(" \n\r\t") + 1);
    line.erase(0, line.find_first_not_of(" \n\r\t"));

    if (line == "NODE_COORD_SECTION") {
      isDataSection = true;
      continue;
    }

    if (line == "EOF")
      break;

    if (isDataSection) {
      stringstream ss(line);
      int id;
      double x, y;

      if (ss >> id >> x >> y) {
        cities.push_back({id - 1, x, y});
      } else {
        cerr << "Error: Incorrect data format in line: " << line << endl;
        exit(1);
      }
    }
  }
  inputFile.close();
  return cities;
}

int main() {
  fast;
  string folderPath = "C:\\Users\\ASUS\\Desktop\\318-Ai-0.75\\offline3\\TSP_"
                      "assignment_task_benchmark_data\\";

  vector<string> fileNames = {"a280.tsp",    "berlin52.tsp", "bier127.tsp",
                              "ch130.tsp",   "ch150.tsp",    "eil51.tsp",
                              "eil76.tsp",   "eil101.tsp",   "kroA100.tsp",
                              "kroB100.tsp", "kroC100.tsp",  "kroD100.tsp",
                              "kroE100.tsp", "lin105.tsp",   "lin318.tsp",
                              "pr76.tsp",    "pr124.tsp",    "pr144.tsp",
                              "rat99.tsp",   "rat195.tsp",   "st70.tsp"};

  for (const auto &fileName : fileNames) {
    string filePath = folderPath + fileName;

    cout << "Processing file: " << fileName << endl;

    vector<City> cities = readCitiesFromFile(filePath);

    auto start = chrono::high_resolution_clock::now();
    vector<int> nnTour = nearestNeighbor(cities);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> nnTime = end - start;
    cout << "Nearest Neighbor Tour Length: " << tourLength(cities, nnTour)
         << ", Time: " << nnTime.count() << " s" << endl;

    start = chrono::high_resolution_clock::now();
    vector<int> greedyTour = kruskalGreedyTSP(cities);
    end = chrono::high_resolution_clock::now();
    chrono::duration<double> greedyTime = end - start;
    cout << "Greedy Heuristic Tour Length: " << tourLength(cities, greedyTour)
         << ", Time: " << greedyTime.count() << " s" << endl;

    start = chrono::high_resolution_clock::now();
    vector<int> mstTour = mstHeuristic(cities);
    end = chrono::high_resolution_clock::now();
    chrono::duration<double> mstTime = end - start;
    cout << "MST-based Tour Length: " << tourLength(cities, mstTour)
         << ", Time: " << mstTime.count() << " s" << endl;

    start = chrono::high_resolution_clock::now();
    vector<int> optTour = twoOpt(cities, nnTour);
    end = chrono::high_resolution_clock::now();
    chrono::duration<double> optTime = end - start;
    cout << "2-opt Tour Length: " << tourLength(cities, optTour)
         << ", Time: " << optTime.count() << " s" << endl;

    start = chrono::high_resolution_clock::now();
    vector<int> swapTour = nodeSwap(cities, nnTour);
    end = chrono::high_resolution_clock::now();
    chrono::duration<double> swapTime = end - start;
    cout << "Node Swap Tour Length: " << tourLength(cities, swapTour)
         << ", Time: " << swapTime.count() << " s" << endl;

    start = chrono::high_resolution_clock::now();
    vector<int> insertTour = nodeInsertion(cities, nnTour);
    end = chrono::high_resolution_clock::now();
    chrono::duration<double> insertTime = end - start;
    cout << "Node Insertion Tour Length: " << tourLength(cities, insertTour)
         << ", Time: " << insertTime.count() << " s" << endl;

    cout << "-------------------------------------------" << endl;
  }

  return 0;
}
