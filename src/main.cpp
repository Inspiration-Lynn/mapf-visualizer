#include <fstream>
#include <iostream>
#include <regex>

#include "../include/ofApp.hpp"
#include "ofMain.h"
#include "../include/mapfplan.hpp"

const std::regex r_config = std::regex(R"(^\d+:(.+))");
const std::regex r_pos = std::regex(R"(\((\d+),(\d+)\),)");


void readSetResult(const std::string& result_file, MAPFPlan* plan)
{
  std::ifstream file(result_file);
  if (!file) {
    std::cout << "error@main," << "file " << result_file << " is not found." << std::endl;
    std::exit(1);
  };

  std::regex r_agents    = std::regex(R"(agents=(.+))");
  std::regex r_solver    = std::regex(R"(solver=(.+))");
  std::regex r_solved    = std::regex(R"(solved=(\d))");
  std::regex r_soc       = std::regex(R"(soc=(\d+))");
  std::regex r_makespan  = std::regex(R"(makespan=(\d+))");
  std::regex r_comp_time = std::regex(R"(comp_time=(\d+))");

  std::string line;
  std::smatch results;
  while (getline(file, line)) {
    // set agent num
    if (std::regex_match(line, results, r_agents)) {
      plan->num_agents = std::stoi(results[1].str());
      continue;
    }
    // solver
    if (std::regex_match(line, results, r_solver)) {
      plan->solver = results[1].str();
      continue;
    }
    // solved?
    if (std::regex_match(line, results, r_solved)) {
      plan->solved = (bool)std::stoi(results[1].str());
      continue;
    }
    // soc
    if (std::regex_match(line, results, r_soc)) {
      plan->soc = std::stoi(results[1].str());
      continue;
    }
    // makespan
    if (std::regex_match(line, results, r_makespan)) {
      plan->makespan = std::stoi(results[1].str());
      continue;
    }
    // comp_time
    if (std::regex_match(line, results, r_comp_time)) {
      plan->comp_time = std::stoi(results[1].str());
      continue;
    }
  }
}



int main(int argc, char *argv[])
{
  // simple arguments check
  if (argc != 3 || !std::ifstream(argv[1]) || !std::ifstream(argv[2])) {
    std::cout << "Please check the arguments, e.g.,\n"
              << "> mapf-visualizer assets/random-32-32-20.map "
                 "assets/demo_random-32-32-20.txt"
              << std::endl;
    return 0;
  }

  MAPFPlan* mapfPlan = new MAPFPlan;  // deleted in ofApp destructor
  readSetResult(argv[2], mapfPlan);

  // load graph
  Graph G(argv[1]);

  // // load plan
  auto solution_file = std::ifstream(argv[2]);
  Solution solution;
  std::string line;
  std::smatch m, results;
  while (getline(solution_file, line)) {
    if (std::regex_match(line, results, r_config)) {
      auto s = results[1].str();
      Config c;
      auto iter = s.cbegin();
      while (std::regex_search(iter, s.cend(), m, r_pos)) {
        iter = m[0].second;
        auto x = std::stoi(m[1].str());
        auto y = std::stoi(m[2].str());
        c.push_back(G.U[G.width * y + x]);
      }
      solution.push_back(c);
    }
  }
  solution_file.close();

  // visualize
  ofSetupOpenGL(100, 100, OF_WINDOW);
  ofRunApp(new ofApp(&G, &solution, mapfPlan));
  return 0;
}
