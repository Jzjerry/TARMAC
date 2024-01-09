//
//  vertex.cpp
//  test
//
//  Created by Yangdi Lyu on 1/1/19.
//  Copyright © 2019 Yangdi Lyu. All rights reserved.
//

#include <algorithm>
#include "vertex.h"
#include "edge.h"

Vertex::~Vertex() {};

inline void IdtCombVertex::evaluate()
{
    outEdge->newVal = inEdges[0]->newVal;
}

inline void IdtCombVertex::setExpr()
{
    Vertex::setExprHelper(inEdges[0]->getExpr());
}

inline void NotCombVertex::evaluate()
{
    outEdge->newVal = !inEdges[0]->newVal;
}

inline void NotCombVertex::setExpr()
{
    Vertex::setExprHelper(~inEdges[0]->getExpr());
}

inline void AndCombVertex::evaluate()
{
    outEdge->newVal = true;
    // std::for_each(inEdges.begin(), inEdges.end(), [this](const std::shared_ptr<Edge> &edg) {if (edg->newVal == false) outEdge->newVal = false;});
    for(auto& e: inEdges){
        if(e->newVal == false){ outEdge->newVal = false; break;}
    }
}

inline void AndCombVertex::setExpr()
{
    z3::expr e = inEdges[0]->getExpr();
    for (auto it = inEdges.begin() + 1; it != inEdges.end(); ++it)
        e = e & (*it)->getExpr();
    Vertex::setExprHelper(e);
}

inline void NandCombVertex::evaluate()
{
    outEdge->newVal = false;
    // std::for_each(inEdges.begin(), inEdges.end(), [this](const std::shared_ptr<Edge> &edg) {if (edg->newVal == false) outEdge->newVal = false;});
    // outEdge->newVal = !outEdge->newVal;
    for(auto& e: inEdges){
        if(e->newVal == false){ outEdge->newVal = true; break;}
    }
}

inline void NandCombVertex::setExpr()
{
    z3::expr e = inEdges[0]->getExpr();
    for (auto it = inEdges.begin() + 1; it != inEdges.end(); ++it)
        e = e & (*it)->getExpr();
    Vertex::setExprHelper(~e);
}

inline void OrCombVertex::evaluate()
{
    outEdge->newVal = false;
    // std::for_each(inEdges.begin(), inEdges.end(), [this](const std::shared_ptr<Edge> &edg) {if (edg->newVal == true) outEdge->newVal = true;});
    for(auto& e: inEdges){
        if(e->newVal == true){ outEdge->newVal = true; break;}
    }
}

inline void OrCombVertex::setExpr()
{
    z3::expr e = inEdges[0]->getExpr();
    for (auto it = inEdges.begin() + 1; it != inEdges.end(); ++it)
        e = e | (*it)->getExpr();
    Vertex::setExprHelper(e);
}

inline void NorCombVertex::evaluate()
{
    outEdge->newVal = true;
    // std::for_each(inEdges.begin(), inEdges.end(), [this](const std::shared_ptr<Edge> &edg) {if (edg->newVal == true) outEdge->newVal = true;});
    // outEdge->newVal = !outEdge->newVal;
    for(auto& e: inEdges){
        if(e->newVal == true){ outEdge->newVal = false; break;}
    }
}

inline void NorCombVertex::setExpr()
{
    z3::expr e = inEdges[0]->getExpr();
    for (auto it = inEdges.begin() + 1; it != inEdges.end(); ++it)
        e = e | (*it)->getExpr();
    Vertex::setExprHelper(~e);
}

inline void XorCombVertex::evaluate()
{
    long cnt = std::count_if(inEdges.begin(), inEdges.end(), [](const std::shared_ptr<Edge> &edg) {return edg->newVal;});
    outEdge->newVal = (cnt % 2 == 1);
}

inline void XorCombVertex::setExpr()
{
    z3::expr e = inEdges[0]->getExpr();
    for (auto it = inEdges.begin() + 1; it != inEdges.end(); ++it)
        e = e ^ (*it)->getExpr();
    Vertex::setExprHelper(e);
}

inline void NxorCombVertex::evaluate()
{
    long cnt = std::count_if(inEdges.begin(), inEdges.end(), [](const std::shared_ptr<Edge> &edg) {return edg->newVal;});
    outEdge->newVal = (cnt % 2 == 0);
}

inline void NxorCombVertex::setExpr()
{
    z3::expr e = inEdges[0]->getExpr();
    for (auto it = inEdges.begin() + 1; it != inEdges.end(); ++it)
        e = e ^ (*it)->getExpr();
    Vertex::setExprHelper(~e);
}

inline void SeqVertex::evaluate()
{
    outEdge->newVal = (outPort.substr(0, 3).compare(".QN")) ? scanInEdge->newVal : !scanInEdge->newVal;
    if (extraOutEdge)
        extraOutEdge->newVal = (extraOutPort.substr(0, 3).compare(".QN")) ? scanInEdge->newVal : !scanInEdge->newVal;
}

inline void SeqVertex::setExpr()
{
    outEdge->e = (outPort.substr(0, 3).compare(".QN")) ? scanInEdge->e : ~scanInEdge->e;
    if (extraOutEdge)
        extraOutEdge->e = (extraOutPort.substr(0, 3).compare(".QN")) ? scanInEdge->e : ~scanInEdge->e;
}
