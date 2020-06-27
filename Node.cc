#include "Node.h"

Define_Module(Node);

void Node::initialize()
{
    id = par("id");
    sizeX = int(getParentModule()->par("nodeSizeX"));
    sizeY = int(getParentModule()->par("nodeSizeY"));
    bool oneTokenMode = bool(getParentModule()->par("oneTokenMode"));
    numberOfGates = gateSize("inoutGateVector");
    myDispStr = this->getDisplayString();

    //if current node is the initiator - wants to be leader
    if(id == int(getParentModule()->par("initiator")))
    {
        myDispStr.setTagArg("b",0,sizeX);
        myDispStr.setTagArg("b",1,sizeY);
        myDispStr.setTagArg("b",3,"red");
        this->setDisplayString(myDispStr.str()); //make node red

        scheduleAt(simTime()+1,new cMessage("I want to be the leader!")); //send a self message
        iWantToBeLeader = true;
    }
    else
    {
        double probabilityToBeLeader = double(getParentModule()->par("probabilityToBeLeader"));

        if(probabilityToBeLeader == 0)
        {
            //no one wants to become leader - except initiator
            iWantToBeLeader = false;
        }
        else if(probabilityToBeLeader == 0.5)
        {
            //there is a 50% probability of the nodes to want the leadership
            if(intuniform(0,1) == 1)
            {
                myDispStr.setTagArg("b",0,sizeX);
                myDispStr.setTagArg("b",1,sizeY);
                myDispStr.setTagArg("b",3,"red");
                this->setDisplayString(myDispStr.str()); //make node red

                if(!oneTokenMode)
                    scheduleAt(simTime()+1,new cMessage("I want to be the leader!")); //send a self message
                iWantToBeLeader = true;
            }
        }
        else if(probabilityToBeLeader == 1)
        {
            //everyone wants to become leaders
            myDispStr.setTagArg("b",0,sizeX);
            myDispStr.setTagArg("b",1,sizeY);
            myDispStr.setTagArg("b",3,"red");
            this->setDisplayString(myDispStr.str()); //make node red

            if(!oneTokenMode)
                scheduleAt(simTime()+1,new cMessage("I want to be the leader!")); //send a self message
            iWantToBeLeader = true;
        }
        else if(probabilityToBeLeader == 2)
        {
            //the node before initiator wants to become leader
            if(this->id == int(getParentModule()->par("initiator")) - 1)
            {
                myDispStr.setTagArg("b",0,sizeX);
                myDispStr.setTagArg("b",1,sizeY);
                myDispStr.setTagArg("b",3,"red");
                this->setDisplayString(myDispStr.str()); //make node red

                if(!oneTokenMode)
                    scheduleAt(simTime()+1,new cMessage("I want to be the leader!")); //send a self message
                iWantToBeLeader = true;
            }
        }
        else if(probabilityToBeLeader == 3)
        {
            //the node after initiator wants to become leader
            if(this->id == int(getParentModule()->par("initiator")) + 1)
            {
                myDispStr.setTagArg("b",0,sizeX);
                myDispStr.setTagArg("b",1,sizeY);
                myDispStr.setTagArg("b",3,"red");
                this->setDisplayString(myDispStr.str()); //make node red

                if(!oneTokenMode)
                    scheduleAt(simTime()+1,new cMessage("I want to be the leader!")); //send a self message
                iWantToBeLeader = true;
            }
        }
    }
}

void Node::handleMessage(cMessage *msg)
{
    int senderId; //the node who send me the msg
    int msgcontent; //the content of the msg converted to int
    int currentNeighborId; //the neighbor node that i am checking in each loop
    int i;

    bubble(msg->getName());

    if(msg->isSelfMessage()) //if msg is a self message from scheduleAt
    {
        std::string newMsgString = std::to_string(this->id); //convert int id to string

        //convert string to char array and create cMessage
        int n = newMsgString.length();
        char newMsgCharArray[n + 1];
        strcpy(newMsgCharArray, newMsgString.c_str());
        cMessage *message = new cMessage(newMsgCharArray);

        if(bool(getParentModule()->par("clockwiseDirection"))) //send your id to your clockwise neighbor
        {
            if(this->id == 1000)
                send(message->dup(),gate("inoutGateVector$o",0));
            else
                send(message->dup(),gate("inoutGateVector$o",1));
        }
        else
        {
            if(this->id == 1000)
                send(message->dup(),gate("inoutGateVector$o",1));
            else
                send(message->dup(),gate("inoutGateVector$o",0));
        }

        this->iHaveSendMyToken = true;
        delete message;
    }
    else //if msg is not a self message
    {
        senderId = ((Node*)msg->getSenderModule())->id; //get the id of the sender

        if(iWantToBeLeader) //if i want to become leader
        {
            msgcontent = std::stoi(msg->getName()); //convert msg to int

            if(msgcontent == this->id) //the msg contains my id - i received my id back - i have the smallest id
            {
                bubble("I am the leader");

                myDispStr.setTagArg("b",3,"green");
                myDispStr.setTagArg("b",0,sizeX);
                myDispStr.setTagArg("b",1,sizeY);
                this->setDisplayString(myDispStr.str()); //make node green
            }
            else if (msgcontent < this->id) //there is a node with smaller id
            {
                for(i=0;i<numberOfGates;i++)
                {
                    //get the id of the node at the current gate
                    currentNeighborId = ((Node*)gate("inoutGateVector$o",i)->getPathEndGate()->getOwnerModule())->id;
                    if(currentNeighborId != senderId)
                    {
                        bubble("I lost :(");

                        myDispStr.setTagArg("b",3,"black");
                        myDispStr.setTagArg("b",0,sizeX);
                        myDispStr.setTagArg("b",1,sizeY);
                        this->setDisplayString(myDispStr.str()); //make node black

                        send(msg->dup(),gate("inoutGateVector$o",i)); //just forward the msg
                    }
                }
            }
            else //my id is smaller than the one in the token
            {
                std::string newMsgString = std::to_string(this->id); //convert int id to string

                //convert string to char array and create cMessage
                int n = newMsgString.length();
                char newMsgCharArray[n + 1];
                strcpy(newMsgCharArray, newMsgString.c_str());
                cMessage *message = new cMessage(newMsgCharArray);

                for(i=0;i<numberOfGates;i++)
                {
                    //get the id of the node at the current gate
                    currentNeighborId = ((Node*)gate("inoutGateVector$o",i)->getPathEndGate()->getOwnerModule())->id;
                    if(currentNeighborId != senderId)
                    {
                        if(!iHaveSendMyToken) //if i already sent my token
                        {
                            //send my id
                            send(message->dup(),gate("inoutGateVector$o",i));
                            delete message;
                        }
                    }
                }
            }
        }
        else //if i don't want to become leader
        {
            for(i=0;i<numberOfGates;i++)
            {
                //get the id of the node at the current gate
                currentNeighborId = ((Node*)gate("inoutGateVector$o",i)->getPathEndGate()->getOwnerModule())->id;

                if(currentNeighborId != senderId)
                    send(msg->dup(),gate("inoutGateVector$o",i)); //just forward the msg
            }
        }
    }
    delete msg;
}
