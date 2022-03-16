# Vertex Orderings to List Triangles faster
Volt is a C++ program to quickly list triangles in a network or graph.
The [associated paper](https://arxiv.org/abs/2203.04774) presents the methods, the available orderings and algorithms, and the experimental results.

## Installation & compilation
`$ git clone https://github.com/lecfab/volt.git`

`$ cd volt`

`$ make`

## Running

`$ ./volt DATASET -o ORDER -a ALGO` (more information with `$ ./volt --help`)

- `$ ./volt toygraph.txt` to count triangles
- `$ ./volt toygraph.txt -t` to list triangles
- `$ ./volt toygraph.txt -o neigh -a PM` to use specific ordering and algorithm



#### Parameters
-   `DATASET`: an _undirected_ graph representation for nodes [0 to N-1]. It must be a text
file where each line corresponds to an edge of the form `a b`
(i.e. a SPACE b, with a and b long unsigned integers). Warning: if DATASET contains loops (`a a`)
or double edges (`a b`, `b a`), remove them with `$ ./undirect DATASET NEWDATASET`.


-   `-o ORDER`: the following orderings are available:
    -   `split` (default): nodes are split between the beginning and the end of the ordering and sorted by degree
    -   `check`: each nodes checks the beginning and end of the ordering and chooses its side
    -   `neigh` (or `neighDpm`): neighbourhood heuristic to reduce sum d+d-
    -   `neighDpp`): neighbourhood heuristic to reduce sum d+²
    -   `original`:   the original order provided by the dataset (warning: it is usually not random)
    -   `rand`: random reordering of nodes
    -   `deg`:  sorted by decreasing total degree
    -   `core`: sorted by degeneracy ordering (k-core pealing algorithm)


-   `-a ALGO`: the following algorithms are available to list triangles:
    -   `PM` (default): also called _A+-_ or _L+n_, it has a complexity sum d+d-
    -   `PP`: also called _A++_ or _S+n_, it has complexity sum d+². Note that the equivalent algorithm MM with complexity sum d-²
    will be applied instead of PP if it takes less time on a given ordered graph

#### Options
-   `-t`: write triangles in standard output so that they can be saved in a FILE (`> FILE`) or piped to another PROGRAM (`| PROGRAM`)
-   `-e FILE`: create a FILE containing the edges of DATASET with the new ordering
-   `-p P` (default 1): use P parallel threads to list triangles. Note that parallelisation may slow down writing if `-t` is used
-   `-n N` (default 0.01): numeric parameter N used in `neigh` heuristics. If N=0, the process runs until local minimum is reached; if N>1, the process runs for at most N loops; if 0<N<1, the process runs until improvement threshold N is reached.


## Contributors

Fabrice Lécuyer (fabrice.lecuyer@lip6.fr)

2022
