#include "binary_tree.hpp"
#include "utils.hpp"
#include <algorithm>
#include <climits>
#include <math.h>     

Tree::Tree(vector<cluster_Frechet::centroid_item> OGcurves)
{
    Tree::n = OGcurves.size();
    Tree::height = floor(log2(n));
    Tree::curveSize = OGcurves[0].p.size();
    for(int i=0;i<n;i++)
        Tree::curves.push_back(OGcurves.p);
    cout << "Constructing new Tree" << endl;
    placeChildren();
}

vector<vector<float>> Tree::postOrderTraversal(Node* node)
{
    vector<vector<float>> leftCurve;
    vector<vector<float>> rightCurve;
    if(isLeaf(node))
        return node->curve;
    else
        leftCurve = postOrderTraversal(node->leftChild);
        if (node->rightChild != NULL)
            rightCurve = postOrderTraversal(node->rightChild);
        vector<vector<float>> meancurve=meanCurve(leftCurve, rightCurve);
        double e=0.1;
        while(meancurve.size()>curveSize)
        {
            TWO_DIM::filter(meancurve,e);
            e*=2;
        }
        return meancurve;
}

bool Tree::isLeaf(Node* node)  
{
    if(node->leftChild == NULL && node->rightChild == NULL)
        return true; 
    else
        return false;
}

void Tree::placeChildren()
{
    int nChildren = n;
    int nParents = ceil(nChildren/2.0);
    for(int i=0;i<n;i++)
        children[i] = new Node(curves[i]);
    Node** parents;
    Node** children = Tree::children;
    Node* childx = NULL;
    Node* childy = NULL;
    int counter = 0;
    while(1){
        counter = 0;
        for(int i=0;i<nParents;i++)
        {
            if (counter<nChildren)
                childx = children[counter++];
            else
                childx = NULL;
            if (counter<nChildren)
                childy = children[counter++];
            else
                childy = NULL;
            parents[i] = new Node(childx, childy);
        }
        nChildren = nParents;
        children = parents;
        counter=0;
        if(nChildren<=2){
            if (counter<nChildren)
                childx = children[counter++];
            else
                childx = NULL;
            if (counter<nChildren)
                childy = children[counter++];
            else
                childy = NULL;
            Tree::root = new Node(childx, childy);
        }
        parents = NULL;
    }
}

