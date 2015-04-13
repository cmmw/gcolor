==================================

Algorithm for the n-graphcoloring problem

==================================

Authors:
Christian Wagner e0725942@student.tuwien.ac.at
Felix Winter e0825516@student.tuwien.ac.at

To build and run the program simply run "make" and then launch "gcolor":

Usage: gcolor --instanceFile <file> --k <number> [--graphVizOutFile <file>]

To visualize the graphVizOutFile install graphviz (http://www.graphviz.org) and run e.g. the following command if the filename is "solution.dot":

sfdp -Tpng -Goverlap=prism solution.dot -o solution.png
