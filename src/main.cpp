
#include <cstdlib>
#include <getopt.h>
#include <cstdio>
#include <sstream>
#include "Logger.h"
#include "Graph.h"
#include "Solution.h"
#include "Algorithm.h"
#include "Constants.h"

using namespace graphcoloring;
using namespace std;

int main(int argc, char* argv[])
{
	srand(time(NULL));
	if (argc < 5 || argc > 7)
	{
		cout << "Usage: " << argv[0] << " " << "--instanceFile <file> "
				<< "--k <number> " << "[--graphVizOutFile <file>]" << endl;
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
						{ 0, 0, 0, 0 }
				};

		c = getopt_long(argc, argv, "", long_options, &option_index);
		if (c == -1)
			break;
		arg.clear();

		switch (c)
		{
		case 'i':
			instanceFile = optarg;
			break;
		case 'k':
			arg.str(optarg);
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

	if (k == -1) {
		cerr << "Argument --k <number> missing" << endl;
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


	clock_t begin = clock();

	Graph graph = Graph(instanceFile);

	Solution* initialSolution = new Solution(graph.getNum_Nodes(), k);

	Algorithm algorithm;

	Solution* finalSolution = algorithm.backtrack(initialSolution, graph);


	clock_t end = clock();



	double ms = double(end - begin) / CLOCKS_PER_SEC;
	LOG << "Took " << ms << " seconds";


	if (finalSolution != NULL) {
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


