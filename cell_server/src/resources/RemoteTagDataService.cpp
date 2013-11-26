//
//  RemoteTagDataService.cpp
//  CellServer
//
//  Created by James Alliban on 21/11/2013.
//
//

#include "RemoteTagDataService.h"


void RemoteTagDataService::setup()
{
    isNewTagAvailable = false;
    
    // now start the thread to check the current tag index
    tagTotal = -1;
    //startThread(true, false);
    
    
    //ofHttpResponse total = ofLoadURL("http://192.30.139.232/b10/sites/questionnaire//q-7.xml");
}


//void RemoteTagDataService::threadedFunction()
//{
//    while(isThreadRunning())
//    {
//        // lock access to the resource
//        lock();
//        
//        //checkForNewTagData();
//        //ofHttpResponse total = ofLoadURL("http://192.30.139.232/b10/sites/questionnaire//q-7.xml");
//        
//        
//        
//        
//        
//#ifdef CELL_ONLINE
//
//        
//        
//        
//        
//        
//        
//        
////        // load the xml file that holds the latest tag index.
////        ofHttpResponse total = ofLoadURL("http://192.30.139.232/b10/sites/questionnaire//q-7.xml");
////        //ofHttpResponse total = ofLoadURL("https://dl.dropboxusercontent.com/u/1619383/data.txt");
////        ofXml totalXML;
////        totalXML.loadFromBuffer(total.data);
////        int latestTagTotal = ofToInt(totalXML.getValue(""));
////
////        printf("RemoteTagDataService::checkForNewTagData() - latestTagTotal = %i \n", latestTagTotal);
////        
////        // this is called if the tag index is called on startup
////        if (tagTotal == -1)
////        {
////            printf("Initialising RemoteTagDataService - Current tag index is %i\n", tagTotal);
////            tagTotal = latestTagTotal;
////            return;
////        }
////        
////        // if loaded index value is not latestTagTotal, load the xml file using indexNumber - e.g. 342.xml
////        if (latestTagTotal != tagTotal)
////        {
////            ofHttpResponse tag = ofLoadURL("http://192.30.139.232/b10/sites/questionnaire//q-7/" + ofToString(latestTagTotal) + ".xml");
////            ofXml tagXML;
////            tagXML.loadFromBuffer(tag.data);
////            newQuestion = tagXML.getValue("question");
////            newTag = tagXML.getValue("content");
////            isNewTagAvailable = true;
////            tagTotal = latestTagTotal;
////            
////            printf("RemoteTagDataService::checkForNewTagData() - new tag = %s \n", newTag.c_str());
////        }
//        
//        
//#endif
//        
//        // send an event containing new string. If there is a problem accessing this, set a newTag flag to true and save new tag to member var
//        
//        
//        
//        
//        
//        
//        
//        
//        
//        // done with the resource
//        unlock();
//        
//    }
//    stopThread();
//}


void RemoteTagDataService::checkForNewTagData()
{
    
}


bool RemoteTagDataService::isTagUpdated()
{
//    if (!isThreadRunning())
//    {
        if (isNewTagAvailable)
            return true;
        else
            return false;
//    }
//    else
//    {
//        return false;
//    }
}
