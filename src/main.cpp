
#include <cstdlib>
#include <getopt.h>
#include <cstdio>
#include <sstream>
#include "Logger.h"
#include "Graph.h"
#include "Solution.h"
#include "Algorithm.h"

using namespace graphcoloring;

int main(int argc, char* argv[])
{
	srand(time(NULL));
	if (argc < 3 || argc > 17)
	{
		std::cout << "Usage: " << argv[0] << " " << "--instanceFile <file> "
				<< "--k <number> ]" << std::endl;
		return -1;
	}

	// get_opt start
	int c;
	std::istringstream arg;
	std::string instanceFile;

	int k;

	while (true)
	{
		int option_index = 0;
		static struct option long_options[] =
				{
						{ "instanceFile", required_argument, 0, 'i' },
						{ "k", required_argument, 0, 'k' },
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
				std::cerr << "k must be >= 0" << std::endl;
				return 1;
			}
			break;


		default:
			std::cerr << "?? getopt returned character code " << std::oct << std::showbase << c << " ??" << std::endl;
			return 1;
		}
	}


	if (instanceFile.empty())
	{
		std::cerr << "Argument --instanceFile <file> missing" << std::endl;
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
	}
	else {
		cout << "No valid Solution found!" << endl;
	}

	delete initialSolution;
	delete finalSolution;

	return 0;
}


