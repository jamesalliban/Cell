#pragma once


#include <Windows.h>
#include "MS_NUI_Def.h"
#include "ofxMSKinectSkeleton.h"
#include "ofThread.h" 
#include "ofxNetwork.h"

#define MAX_CONNECTIONS 5     //number of external laptop with kinetic devices attached ... could be a lot more

typedef struct _ofMS_NETWORK_SKELETON_DATA
{
  // unsigned char depthColorData[ (320 * 240) * 3]; // do we need this ??? leave out for moment this is always fixed at 320*240
  char clientname [30];
  ofxMSKinectSkeleton  ofxSkels[6]   ;     // currently on a Kinetic device there can only be max of 6 skels (2 tracked 4 position-only)

} ofMS_NETWORK_SKELETON_DATA;

typedef struct _ofMS_NETWORK_TRACKED_SKELETON_DATA
{
  // unsigned char depthColorData[ (320 * 240) * 3]; // do we need this ??? leave out for moment this is always fixed at 320*240
  char clientname [30];
  ofxMSKinectSkeleton  ofxSkels[2]   ;     // just tracked max of 2 

} ofMS_NETWORK_TRACKED_SKELETON_DATA;

typedef struct _ofMS_NETWORK_COMMAND
{
  char command ;
  char command_data[100];
 
} ofMS_NETWORK_COMMAND;

typedef struct _ofMS_NETWORK_CLIENT
{
	char clientname [30];
	vector<ofxMSKinectSkeleton> tracked_skeletons;
	vector<ofxMSKinectSkeleton> positiononly_skeletons;
} ofMS_NETWORK_CLIENT;




typedef struct _ofMS_NETWORK_SKELETON_DATA_CLIENTS
{
  ofMS_NETWORK_SKELETON_DATA clients[MAX_CONNECTIONS] ;
  int  client_size[MAX_CONNECTIONS] ;
  char buffer[MAX_CONNECTIONS][30] ;
} ofMS_NETWORK_SKELETON_DATA_CLIENTS;




class KineticNetWorkClient: public ofThread
{
public:
	
	KineticNetWorkClient(int port, const char *ipaddress, int tracked, const char *clientname );
	~KineticNetWorkClient(void);
	bool Send( vector<ofxMSKinectSkeleton> *tracked_skeletons,
						vector<ofxMSKinectSkeleton> *positiononly_skeletons);
	bool GetCommand(ofMS_NETWORK_COMMAND *command);
	void CloseSocket();

private:

	ofMS_NETWORK_SKELETON_DATA ofdata;
	virtual void threadedFunction();
	ofxTCPClient tcpClient;
	
	int port;
	char ipaddress[30]; 
	bool bHasdata;
	bool bTrackedOnly ;
	
};


class KineticNetWorkServer: public ofThread
{
public:
	static const int MaxPossibleConnections = MAX_CONNECTIONS;
	KineticNetWorkServer(int port );
	~KineticNetWorkServer(void);
	bool GetTrackedSkeleton( vector<ofxMSKinectSkeleton> * pdata, int clientid, char * name );
	bool GetPostionOnlySkeletons( vector<ofxMSKinectSkeleton> * pdata, int clientid, char*name );
	int NumberOfConnectedClients();
	string  ClientsIPaddresss(int clientid);
	void SetCameraElevation(int elevation );
	void Pause();
	void Resume();
	void CloseClients();
	void SetClientsSmoothingParams(char *data, int sizeofthedata );
	void NotifyClientsServerClosing();
private:
	
	_ofMS_NETWORK_CLIENT client[MAX_CONNECTIONS];

	//ofMS_NETWORK_SKELETON_DATA ofdata;
	virtual void threadedFunction();
	ofxTCPServer TCP;
	int port;


	ofMS_NETWORK_SKELETON_DATA_CLIENTS   ofxdata;
	
};

class UDPKineticNetWorkClient: public ofThread
{
public:
	
	UDPKineticNetWorkClient(int port, const char *ipaddress, int tracked, const char *clientname );
	~UDPKineticNetWorkClient(void);
	bool Send( vector<ofxMSKinectSkeleton> *tracked_skeletons,
						vector<ofxMSKinectSkeleton> *positiononly_skeletons);
	bool GetCommand(ofMS_NETWORK_COMMAND *command);
	void CloseSocket();

private:

	ofMS_NETWORK_SKELETON_DATA ofdata;
	virtual void threadedFunction();
	ofxUDPManager udpConnection;
	int port;
	char ipaddress[30]; 
	bool bHasdata;
	bool bTrackedOnly ;
	
};


class UDPKineticNetWorkServer: public ofThread
{
public:
	static const int MaxPossibleConnections = MAX_CONNECTIONS;
	UDPKineticNetWorkServer(int port );
	~UDPKineticNetWorkServer(void);
	bool GetTrackedSkeleton( vector<ofxMSKinectSkeleton> * pdata, int clientid, char * name );
	bool GetPostionOnlySkeletons( vector<ofxMSKinectSkeleton> * pdata, int clientid, char*name );
	int NumberOfConnectedClients();
	string  ClientsIPaddresss(int clientid);
	void SetCameraElevation(int elevation );
	void Pause();
	void Resume();
	void CloseClients();
	void SetClientsSmoothingParams(char *data, int sizeofthedata );
	void NotifyClientsServerClosing();
private:
	
	_ofMS_NETWORK_CLIENT client[MAX_CONNECTIONS];

	//ofMS_NETWORK_SKELETON_DATA ofdata;
	virtual void threadedFunction();
//ofxTCPServer TCP;
	int port;
	ofxUDPManager udpConnection;

	ofMS_NETWORK_SKELETON_DATA_CLIENTS   ofxdata;
	
};