=========================================

Algorithm for the n-graphcoloring problem

=========================================

Authors:
Christian Wagner e0725942@student.tuwien.ac.at
Felix Winter e0825516@student.tuwien.ac.at




To build and run the program simply run "make" and then launch "gcolor":

=========
Phase 1:
=========
Usage: gcolor --instanceFile <file> --alg 0 [--k <number>] [--graphVizOutFile <file>]  [--mac <0/1>] [--mrv <0/1>] [--lcv <0/1>]

If the --k number argument is omitted the algorithm tries to find the minimum k to color the graph, otherwise the algorithm solves the decision problem for the given k.

Setting the switches for --mac, --mrv and --lcv to 0/1 will disable/enable the Maintaining Arc Consistency approach, the Minimum Remaining Values heuristic and the Least Constraining Value heuristic respectively.
When --mac is set to 0 the algorithm will use simple forward checking. Per default --mac is set to 0 and --mrv as well as --lcv are set to 1.

=========
Phase 2:
=========
Usage: gcolor --instanceFile <file> --timeLimit <msecs> --alg 1 --tl <tlparam> --randomWalkProbability <p> [--graphVizOutFile <file>]

=================================================

To visualize the graphVizOutFile install graphviz (http://www.graphviz.org) and run e.g. the following command if the filename is "solution.dot":

sfdp -Tpng -Goverlap=prism solution.dot -o solution.png
