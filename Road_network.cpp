// Copyright (c) 2013 Elements of Programming Interviews. All rights reserved.

#include <cassert>
#include <iostream>
#include <deque>
#include <limits>
#include <random>
#include <vector>

using std::cout;
using std::default_random_engine;
using std::deque;
using std::endl;
using std::numeric_limits;
using std::random_device;
using std::uniform_int_distribution;
using std::uniform_real_distribution;
using std::vector;

// @include
template <typename DistanceType>
struct HighwaySection {
  int x, y;
  DistanceType distance;
};

template <typename DistanceType>
void Floyd_Warshall(vector<vector<DistanceType>>* G) {
  for (int k = 0; k < G->size(); ++k) {
    for (int i = 0; i < G->size(); ++i) {
      for (int j = 0; j < G->size(); ++j) {
        if ((*G)[i][k] != numeric_limits<DistanceType>::max() &&
            (*G)[k][j] != numeric_limits<DistanceType>::max() &&
            (*G)[i][j] > (*G)[i][k] + (*G)[k][j]) {
          (*G)[i][j] = (*G)[i][k] + (*G)[k][j];
        }
      }
    }
  }
}

template <typename DistanceType>
HighwaySection<DistanceType> find_best_proposals(
    const vector<HighwaySection<DistanceType>>& H,
    const vector<HighwaySection<DistanceType>>& P,
    int a, int b, int n) {
  // G stores the shortest path distances between all pairs of vertices.
  vector<vector<DistanceType>>
      G(n, vector<DistanceType>(n, numeric_limits<DistanceType>::max()));
  for (int i = 0; i < n; ++i) {
    G[i][i] = 0;
  }

  // Build a undirected graph G based on existing highway sections H.
  for (const HighwaySection<DistanceType> &h : H) {
    G[h.x][h.y] = G[h.y][h.x] = h.distance;
  }
  // Perform Floyd Warshall to build the shortest path between vertices.
  Floyd_Warshall(&G);

  // Examine each proposal for shorter distance between a and b.
  DistanceType min_dis_a_b = G[a][b];
  HighwaySection<DistanceType> best_proposal = {-1, -1, 0.0};  // default
  for (const HighwaySection<DistanceType> &p : P) {
    // Check the path of a => p.x => p.y => b.
    if (G[a][p.x] != numeric_limits<DistanceType>::max() &&
        G[p.y][b] != numeric_limits<DistanceType>::max() &&
        min_dis_a_b > G[a][p.x] + p.distance + G[p.y][b]) {
      min_dis_a_b = G[a][p.x] + p.distance + G[p.y][b];
      best_proposal = p;
    }
    // Check the path of a => p.y => p.x => b.
    if (G[a][p.y] != numeric_limits<DistanceType>::max() &&
        G[p.x][b] != numeric_limits<DistanceType>::max() &&
        min_dis_a_b > G[a][p.y] + p.distance + G[p.x][b]) {
      min_dis_a_b = G[a][p.y] + p.distance + G[p.x][b];
      best_proposal = p;
    }
  }
  return best_proposal;
}
// @exclude

// Try to add each proposal and use Floyd Warshall to solve, O(n^4) algorithm.
template <typename DistanceType>
HighwaySection<DistanceType> check_ans(
    const vector<HighwaySection<DistanceType>>& H,
    const vector<HighwaySection<DistanceType>>& P,
    int a, int b, int n) {
  // G stores the shortest path distances between all pairs of vertices.
  vector<vector<DistanceType>>
      G(n, vector<DistanceType>(n, numeric_limits<DistanceType>::max()));
  for (int i = 0; i < n; ++i) {
    G[i][i] = 0;
  }

  // Build a undirected graph G based on existing highway sections H.
  for (const HighwaySection<DistanceType> &h : H) {
    G[h.x][h.y] = G[h.y][h.x] = h.distance;
  }
  // Perform Floyd Warshall to build the shortest path between vertices.
  Floyd_Warshall(&G);

  DistanceType best_cost = G[a][b];
  HighwaySection<DistanceType> best_proposal = {-1, -1, 0.0};  // default
  for (const HighwaySection<DistanceType> &p : P) {
    // Create new G_test for Floyd Warshall.
    vector<vector<DistanceType>> G_test(G);
    G_test[p.x][p.y] = G_test[p.y][p.x] = p.distance;
    Floyd_Warshall(&G_test);
    if (best_cost > G_test[a][b]) {
      best_cost = G_test[a][b];
      best_proposal = p;
    }
  }
  return best_proposal;
}

int main(int argc, char *argv[]) {
  for (int times = 0; times < 1000; ++times) {
    default_random_engine gen((random_device())());
    int n, m, k;
    if (argc == 2) {
      n = atoi(argv[1]);
      uniform_int_distribution<int> dis1(1, n * ((n - 1) >> 1) - 1);
      m = dis1(gen);
      uniform_int_distribution<int> dis2(1, n * ((n - 1) >> 1) - m);
      k = dis2(gen);
    } else if (argc == 3) {
      n = atoi(argv[1]);
      m = atoi(argv[2]);
      uniform_int_distribution<int> dis(1, n * ((n - 1) >> 1) - m);
      k = dis(gen);
    } else {
      uniform_int_distribution<int> one_to_100(1, 100);
      n = one_to_100(gen);
      uniform_int_distribution<int> dis1(1, n * ((n - 1) >> 1) - 1);
      m = dis1(gen);
      uniform_int_distribution<int> dis2(1, n * ((n - 1) >> 1) - m);
      k = dis2(gen);
    }
    cout << "n = " << n << ", m = " << m << ", k = " << k << endl;

    vector<deque<bool>> have_edges(n, deque<bool>(n, false));
    vector<HighwaySection<double>> H;  // existing highway sections
    uniform_int_distribution<int> vertex_dis(0, n - 1);
    uniform_real_distribution<double> cost_dis(1.0, 10000.0);
    for (int i = 0; i < m; ++i) {
      int a, b;
      do {
        a = vertex_dis(gen), b = vertex_dis(gen);
      } while (a == b || have_edges[a][b] == true);
      have_edges[a][b] = have_edges[b][a] = true;
      H.emplace_back(HighwaySection<double>{a, b, cost_dis(gen)});
    }
    //*
    for (int i = 0; i < m; ++i) {
      cout << "H[i] = " << H[i].x << " " << H[i].y 
           << " " << H[i].distance << endl;
    }
    //*/

    vector<HighwaySection<double>> P;  // proposals
    for (int i = 0; i < k; ++i) {
      int a, b;
      do {
        a = vertex_dis(gen), b = vertex_dis(gen);
      } while (a == b || have_edges[a][b] == true);
      have_edges[a][b] = have_edges[b][a] = true;
      uniform_real_distribution<double> new_cost_dis(1.0, 50.0);
      P.emplace_back(HighwaySection<double>{a, b, new_cost_dis(gen)});
    }
    //*
    for (int i = 0; i < k; ++i) {
      cout << "P[i] = " << P[i].x << " " << P[i].y 
           << " " << P[i].distance << endl;
    }
    //*/

    int a, b;
    do {
      a = vertex_dis(gen), b = vertex_dis(gen);
    } while (a == b);
    cout << "a = " << a << ", b = " << b << endl;
    HighwaySection<double> t = find_best_proposals(H, P, a, b, n);
    cout << t.x << ' ' << t.y << ' ' << t.distance << endl;
    HighwaySection<double> ans = check_ans(H, P, a, b, n);
    cout << ans.x << ' ' << ans.y << ' ' << ans.distance << endl;
    // TODO(THL): follow assert may fail sometime due to epsilon problem.
    assert(t.x == ans.x && t.y == ans.y && t.distance == ans.distance);
  }
  return 0;
}
