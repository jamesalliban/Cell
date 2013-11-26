//
//  RemoteTagDataService.h
//  CellServer
//
//  Created by James Alliban on 21/11/2013.
//
//

#pragma once

// uncomment if testing offline
#define CELL_ONLINE

#include "ofMain.h"


class RemoteTagDataService // : public ofThread
{
public:
    void setup();
    //void threadedFunction();
    void checkForNewTagData();
    bool isTagUpdated();
    
    int tagTotal;
    int currentXmlIndex;
    string newQuestion;
    string newTag;
    
    bool isNewTagAvailable;
};