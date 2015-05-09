
#include <cstdlib>
#include <getopt.h>
#include <cstdio>
#include <sstream>
#include "Logger.h"
#include "Graph.h"
#include "Solution.h"
#include "Algorithm.h"
#include "Algorithm2.h"
#include "Constants.h"
#include "time.h"

using namespace graphcoloring;
using namespace std;

int visitedNodes = 0;
int triedColors = 0;

int mac = 0;
int mrv = 1;
int lcv = 1;
int alg = 0;

clock_t begin;
double time_limit = -1;

void checkSolution(Solution* sol, const Graph& graph)
{
	LOG << "Checking solution";
	for(int i = 0; i < graph.getNum_Nodes(); i++)
	{
		std::vector<int> neighbors = graph.getNeighbours(i);
		for(std::vector<int>::iterator it = neighbors.begin(); it != neighbors.end(); it++)
		{
			if(sol->getColor(i) == sol->getColor(*it))
			{
				LOG << "Solution not feasible!!";
				return;
			}
		}
	}
	LOG << "Solution is feasible";
}

int main(int argc, char* argv[])
{
	srand(time(NULL));
	if (argc < 3 || argc > 15)
	{
		cout << "Usage: " << argv[0] << " " << "--instanceFile <file> "
				<< "[--k <number>] " << "[--graphVizOutFile <file>] [--mac <0/1>] [--mrv <0/1>] [--lcv <0/1>] [--timelimit <msecs>] [--alg <0/1>]" << endl;
		return -1;
	}

	// get_opt start
	int c;
	istringstream arg;
	string instanceFile;

	string graphVizOutFile;

	int k = -1;

	while (true)
	{
		int option_index = 0;
		static struct option long_options[] =
				{
						{ "instanceFile", required_argument, 0, 'i' },
						{ "k", required_argument, 0, 'k' },
						{ "graphVizOutFile", required_argument, 0, 'v' },
						{ "mac", required_argument, 0, 'm' },
						{ "mrv", required_argument, 0, 'r' },
						{ "lcv", required_argument, 0, 'l' },
						{ "timelimit", required_argument, 0, 't' },
						{ "alg", required_argument, 0, 'a' },
						{ 0, 0, 0, 0 }
				};

		c = getopt_long(argc, argv, "", long_options, &option_index);
		if (c == -1)
			break;
		arg.clear();

		switch (c)
		{
		case 'a':
			arg.str(optarg);
			arg >> alg;
			break;
		case 'i':
			instanceFile = optarg;
			break;
		case 'k':

			arg >> k;
			if (k < 0)
			{
				cerr << "k must be >= 0" << endl;
				return 1;
			}
			break;
		case 'v':
			graphVizOutFile = optarg;
			break;
		case 'm':
			arg.str(optarg);
			arg >> mac;
			break;
		case 'r':
			arg.str(optarg);
			arg >> mrv;
			break;
		case 'l':
			arg.str(optarg);
			arg >> lcv;
			break;
		case 't':
			arg.str(optarg);
			arg >> time_limit;
			time_limit = time_limit/1000;
			break;
		default:
			cerr << "?? getopt returned character code " << oct << showbase << c << " ??" << endl;
			return 1;
		}
	}


	if (instanceFile.empty())
	{
		cerr << "Argument --instanceFile <file> missing" << endl;
		return 1;
	}


	if (optind < argc)
	{
		printf("non-option ARGV-elements: ");
		while (optind < argc)
			printf("%s ", argv[optind++]);
		printf("\n");
	}
	// getopt end


	begin = clock();

	Graph graph = Graph(instanceFile);
	Solution* finalSolution;
	if(alg == 0)
	{
		Algorithm algorithm;
		if (k == -1) {
			finalSolution = algorithm.findOptimalSolution(graph);
		}
		else {
			Solution* initialSolution = new Solution(graph.getNum_Nodes(), k);
			finalSolution = algorithm.backtrack(initialSolution, graph);
		}
	} else
	{
		Algorithm2 algorithm(graph);
		finalSolution = new Solution(algorithm.findOptimalSolution());
		checkSolution(finalSolution, graph);
	}

	clock_t end = clock();



	double ms = double(end - begin) / CLOCKS_PER_SEC;
	LOG << "Took " << ms << " seconds";
	LOG << "Visited " << visitedNodes << " nodes";
	LOG << "Tried " << triedColors << " colors for the nodes";

	if (time_limit != -1 && time_limit < ms) {
		cout << "Time limit reached without finding a solution" << endl;
	}
	else if (finalSolution != NULL) {
		finalSolution->printSolution();

		if (!graphVizOutFile.empty()) {

			if (k > graphviz_color_count) {
				cout << "Too many colors, cannot write graphVizFile" << endl;
			}
			finalSolution->writeGraphVizFile(graphVizOutFile, graph);
		}
	}
	else {
		cout << "No valid Solution found!" << endl;
	}

	delete finalSolution;

	return 0;
}


