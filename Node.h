#ifndef __DISTRASSIGNMENT02_NODE_H_
#define __DISTRASSIGNMENT02_NODE_H_

#include <omnetpp.h>
#include <string>

using namespace omnetpp;

class Node : public cSimpleModule
{
  protected:
    int id;
    int sizeX;
    int sizeY;
    int numberOfGates;
    bool iWantToBeLeader = false;
    bool iHaveSendMyToken = false;
    cDisplayString myDispStr;
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

#endif
