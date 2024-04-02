/*
 * File:	basicgraphs.h
 * Author:	Rachel Bood
 * Date:	Dec 31, 2015 (?)
 * Version:	1.6
 *
 * Purpose:	Declare the basicgraphs class.
 *
 * Modification history:
 * Dec 9, 2019 (JD V1.1)
 *  (a) Reorder the function declarations.
 *  (b) Rename some variables.
 *  (c) Remove width and height from almost all functions.
 * Dec 10, 2019 (JD V1.2):
 *  (a) Change decls of Graph_Type_Name and getGraphName() to static so
 *	that I can access getGraphName() from other classes.
 * Dec 12, 2019 (JD V1.3):
 *  (b) Add "None" to enum Graph_Type, to match the index when no
 *	graph type is selected.
 * Dec 14, 2019 (JD V1.4):
 *  (a) Remove "#include defuns.h" from here.
 * Aug 24, 2020 (IC V1.5):
 *  (a) Added a new basicGraphs category, circulant graph which creates
 *      a cycle along with edges based on a list of offsets.
 * Aug 25, 2020 (JD V1.6):
 *  (a) Fix V1.5 comment.  Duh.
 */


#ifndef BASICGRAPHS_H
#define BASICGRAPHS_H

#include <graph.h>

class BasicGraphs
{
  public:
    BasicGraphs();
    QList<Node *> create_cycle(Graph * g, qreal width, qreal height,
			       int numOfNodes, qreal radians = 0);

    void generate_antiprism(Graph * g, int numOfNodes, bool drawEdges);
    void recursive_binary_tree(Graph * g, int depth, int index, int treeDepth);
    void generate_balanced_binary_tree(Graph * g, int numOfNodes,
				       bool drawEdges);
    void generate_bipartite(Graph * g, int topNodes,int bottomNodes,
			    bool drawEdges);
    void generate_circulant(Graph * g, int numOfNodes, QString offsets,
                            bool drawEdges);
    void generate_complete(Graph * g, int numOfNodes, bool drawEdges);
    void generate_crown(Graph * g, int numOfNodes, bool drawEdges);
    void generate_cycle(Graph * g, int numOfNodes, bool drawEdges);
    void generate_dutch_windmill(Graph * g, int blade, int bladeSize,
				 bool drawEdges);
    void generate_gear(Graph * g, int numOfNodes, bool drawEdges);
    void generate_grid(Graph * g, int columns, int rows, bool drawEdges);
    void generate_helm(Graph * g, int numOfNodes, bool drawEdges);
    void generate_path(Graph * g, int numOfNodes, bool drawEdges);
    void generate_petersen(Graph * g, int numOfNodes, int starSkip,
			   bool drawEdges);
    void generate_prism(Graph * g, int numOfNodes, bool drawEdges);
    void generate_star(Graph * g, int numOfNodes, bool drawEdges);
    void generate_wheel(Graph * g, int numOfNodes, bool drawEdges);

    // This must agree with Graph_Type_Name set in the BG constructor.
    enum Graph_Type {Nothing = 0, Antiprism, BBTree, Bipartite, Circulant,
		     Complete, Crown, Cycle, Dutch_Windmill, Gear, Grid,
		     Helm, Path, Petersen, Prism, Star, Wheel, Count};
    static QString getGraphName(int enumValue);

  private:
    static QVector<QString> Graph_Type_Name;
};

#endif // BASICGRAPHS_H
