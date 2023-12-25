#pragma once
#include <vector>
#include <string>

struct MAPFPlan {
  int num_agents;         // number of agents
  std::string solver;     // solver name
  bool solved;            // success or not
  int soc;                // sum of cost
  int makespan;           // makespan
  int comp_time;          // computation time
};
