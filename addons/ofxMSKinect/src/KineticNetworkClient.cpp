
#include "KineticNetworkClient.h"

	KineticNetWorkClient::KineticNetWorkClient(int in_port, const char *in_ipaddress, int tracked, const char *clientname ):ofThread()
	{
		bTrackedOnly = (tracked==0) ? true :false ;
		bHasdata = false;
		port = in_port;
		if( in_ipaddress!=NULL)
			strcpy( ipaddress, (const char *)in_ipaddress );
		else
			strcpy( ipaddress, (const char *)"127.0.0.1");

		memset( &ofdata.clientname[0], 0 , 30 );
		if( clientname != NULL && strlen( clientname ) < 30 )
			strcpy( &ofdata.clientname[0], clientname) ;

		startThread( true, false ) ;

	}

	KineticNetWorkClient::~KineticNetWorkClient(void)
	{
		if( isThreadRunning() )
		{
			stopThread() ;

			while( isThreadRunning()  )
				::Sleep( 15 ) ;
		}

		if( tcpClient.isConnected() )
			 tcpClient.close();
	}


	void KineticNetWorkClient::CloseSocket()
	{
		if( tcpClient.isConnected() )
		{
			bHasdata = false;
		    lock();
			tcpClient.close();
			unlock();
		}
	}

	bool KineticNetWorkClient::GetCommand(ofMS_NETWORK_COMMAND *command)
	{
		int iret = 0;

		if( command != NULL &&  tcpClient.isConnected() == true )
		{
			lock();
			iret = tcpClient.receiveRawBytes( (char *) command, sizeof( ofMS_NETWORK_COMMAND ) );
			unlock();
		}
	/*	else
		{
			 if( tcpClient.isConnected() == false )
			 {
				 lock();
				 tcpClient.setup(ipaddress, port);
				 unlock();
			 }
		}*/
		return (iret == 0) ? false : true;
	}


	bool KineticNetWorkClient::Send( vector<ofxMSKinectSkeleton> *tracked_skeletons,
						vector<ofxMSKinectSkeleton> *positiononly_skeletons)
	{
		if( tcpClient.isConnected() )
		{
			int index = 0 ;
			lock() ;

			for( int i = 0; i < 6 ; i++) // clear down skels
			{
				ofdata.ofxSkels[i].eTrackingState = NUI_SKELETON_NOT_TRACKED;
				ofdata.ofxSkels[i].dwTrackingID = -1;
			}


			//memcpy( ofdata.depthColorData, depthdata, (320 * 240) * 3);
			if(tracked_skeletons != NULL)
			{
				int size = tracked_skeletons->size();
				for( int i = 0; i < size && i < 2 ; i++)
				{
					memcpy( &ofdata.ofxSkels[index], &tracked_skeletons->at(i), sizeof(ofxMSKinectSkeleton) ) ;
					index++;
				}
			}


			if( bTrackedOnly == false )
			{
				if(positiononly_skeletons != NULL)
				{
					int size = positiononly_skeletons->size();
					index = 2;
					for( int i =0; i < size  && i < 4; i++)
					{
						memcpy( &ofdata.ofxSkels[index], &positiononly_skeletons->at(i), sizeof(ofxMSKinectSkeleton) ) ;
						index++;
					}
				}
			}

			bHasdata = true;

			unlock();
			return true;
		}
		return false ;
	}


void KineticNetWorkClient::threadedFunction()
{
   while( isThreadRunning()  )
    {
		if( tcpClient.isConnected() == false )
		{
		   lock();
		   tcpClient.setup(ipaddress, port);
		   unlock();
		}
		else
		{
			if( bHasdata )
			{
				lock();
				tcpClient.sendRawBytes( (char *) &ofdata,
					(bTrackedOnly == false) ? sizeof(ofMS_NETWORK_SKELETON_DATA) : sizeof(ofMS_NETWORK_TRACKED_SKELETON_DATA));
				bHasdata = false ;
				unlock();

			}
		}
		Sleep( 10 ) ;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//The server part .. listens for connections from other laptops/pc's then handles the imput of their
// skeletal information. and provides calls to allow the servers host app to get this skeletal information
// based on the index of the connection
 KineticNetWorkServer::KineticNetWorkServer(int in_port):ofThread()
{
		TCP.setup(in_port);
		startThread( true, false ) ;
}


KineticNetWorkServer::~KineticNetWorkServer(void)
{
		if( isThreadRunning() )
		{
			stopThread() ;

			while( isThreadRunning()  )
				::Sleep( 15 ) ;
		}

		if( TCP.isConnected() )
			 TCP.close();
}

void KineticNetWorkServer::Pause()
{
	ofMS_NETWORK_COMMAND c;
	c.command = 'P';
	TCP.sendRawBytesToAll((const char*)&c, sizeof(ofMS_NETWORK_COMMAND) );
}

void KineticNetWorkServer::Resume()
{
	ofMS_NETWORK_COMMAND c;
	c.command = 'R';
	TCP.sendRawBytesToAll((const char*)&c, sizeof(ofMS_NETWORK_COMMAND) );
}

void KineticNetWorkServer::CloseClients()
{
	ofMS_NETWORK_COMMAND c;
	c.command = 'X';
	TCP.sendRawBytesToAll((const char*)&c, sizeof(ofMS_NETWORK_COMMAND) );
}

void KineticNetWorkServer::SetCameraElevation(int elevation )
{
	ofMS_NETWORK_COMMAND c;
	c.command = 'C';
	sprintf( &c.command_data[0],"%d", elevation );
	TCP.sendRawBytesToAll((const char*)&c, sizeof(ofMS_NETWORK_COMMAND) );
}


void KineticNetWorkServer::SetClientsSmoothingParams(char *data, int sizeofthedata )
{
	ofMS_NETWORK_COMMAND c;
	c.command = 'S';
	memcpy( &c.command_data[0], data, sizeofthedata );
	TCP.sendRawBytesToAll((const char*)&c, sizeof(ofMS_NETWORK_COMMAND) );
}

void KineticNetWorkServer::NotifyClientsServerClosing()
{
	ofMS_NETWORK_COMMAND c;
	c.command = 'Z';
	TCP.sendRawBytesToAll((const char*)&c, sizeof(ofMS_NETWORK_COMMAND) );
}

int  KineticNetWorkServer::NumberOfConnectedClients()
{
	return TCP.getNumClients() ;
}


string  KineticNetWorkServer::ClientsIPaddresss(int clientid)
{
	if( clientid < MAX_CONNECTIONS && TCP.isClientConnected(clientid) )
		return TCP.getClientIP(clientid);

	return "";
}


void KineticNetWorkServer::threadedFunction()
{
	memset( &ofxdata,0,sizeof(ofMS_NETWORK_SKELETON_DATA_CLIENTS) );

	//Sleep(1000 ); //we could be starting again whilst clients are already running.
	//Resume();     //so we could send a resume here to allow paused clients to start sending again ?

	while( isThreadRunning()  )
    {
		lock() ;
		for( int i=0; i <  MAX_CONNECTIONS ; i++)
		{
			if( TCP.isClientConnected(i) )
			{
				int icount = TCP.receiveRawBytes( i, (char *)  &ofxdata.clients[i], sizeof(ofMS_NETWORK_SKELETON_DATA) );
				if( icount ==  sizeof(ofMS_NETWORK_SKELETON_DATA) || icount ==  sizeof(ofMS_NETWORK_TRACKED_SKELETON_DATA) )
			//	if( icount > -1)
				{	ofxdata.client_size[i] = icount;	}
			}
			else
			{
				ofxdata.client_size[i] = 0;
			}
		}
		unlock() ;
		Sleep( 10 ) ;
    }
}


bool KineticNetWorkServer::GetTrackedSkeleton(vector<ofxMSKinectSkeleton> * pdata, int clientid, char *clientname = NULL)
{
	bool retval = false ;
	if( clientid < MAX_CONNECTIONS && pdata != NULL)
	{
		if( TCP.isClientConnected(clientid) )
		{
			lock();
			int icount = ofxdata.client_size[clientid] ;

			if( icount ==  sizeof(ofMS_NETWORK_SKELETON_DATA) ||
					icount ==  sizeof(ofMS_NETWORK_TRACKED_SKELETON_DATA) )
			{
				if( clientname != NULL )
					memcpy( clientname, &ofxdata.clients[clientid].clientname, 30 ) ;

				for( int i =0;  i < 2 ;i++)
				{
					if( ofxdata.clients[clientid].ofxSkels[i].eTrackingState == NUI_SKELETON_TRACKED )
					{
					    // Filthy hack
					    ofxdata.clients[clientid].ofxSkels[i].clientID = ofToInt(ofToString(clientname[6]));
						pdata->push_back( ofxdata.clients[clientid].ofxSkels[i] ) ;
						retval = true;
					}
				}
			}
			unlock() ;
		}
	}
	return retval;
}

bool KineticNetWorkServer::GetPostionOnlySkeletons(vector<ofxMSKinectSkeleton> * pdata, int clientid, char *clientname = NULL )
{
	bool retval = false ;
	if( clientid < 2 )
	   return retval ;

	if( clientid  < MAX_CONNECTIONS && pdata != NULL)
	{
		if( TCP.isClientConnected(clientid) )
		{
			lock();
			if(	ofxdata.client_size[clientid] ==  sizeof(ofMS_NETWORK_TRACKED_SKELETON_DATA) )
			{
				if( clientname != NULL )
					memcpy( clientname, &ofxdata.clients[clientid].clientname, 30 ) ;

				for( int i = 2;  i < 6 ;i++)
				{
					if( ofxdata.clients[clientid].ofxSkels[i].eTrackingState == NUI_SKELETON_POSITION_ONLY)
					{
						pdata->push_back( ofxdata.clients[clientid].ofxSkels[i] ) ;
						retval = true;
					}
				}
			}
			unlock();
		}
	}
	return retval;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	UDPKineticNetWorkClient::UDPKineticNetWorkClient(int in_port, const char *in_ipaddress, int tracked, const char *clientname ):ofThread()
	{
		bTrackedOnly = (tracked==0) ? true :false ;
		bHasdata = false;
		port = in_port;
		if( in_ipaddress!=NULL)
			strcpy( ipaddress, (const char *)in_ipaddress );
		else
			strcpy( ipaddress, (const char *)"127.0.0.1");





		memset( &ofdata.clientname[0], 0 , 30 );
		if( clientname != NULL && strlen( clientname ) < 30 )
			strcpy( &ofdata.clientname[0], clientname) ;

		startThread( true, false ) ;

	}

	UDPKineticNetWorkClient::~UDPKineticNetWorkClient(void)
	{
		if( isThreadRunning() )
		{
			stopThread() ;

			while( isThreadRunning()  )
				::Sleep( 15 ) ;
		}

		udpConnection.Close();
	}


	void UDPKineticNetWorkClient::CloseSocket()
	{

			bHasdata = false;
		    lock();
			udpConnection.Close();
			unlock();
	}

	bool UDPKineticNetWorkClient::GetCommand(ofMS_NETWORK_COMMAND *command)
	{
	 return false ;
	}


	bool UDPKineticNetWorkClient::Send( vector<ofxMSKinectSkeleton> *tracked_skeletons,
						vector<ofxMSKinectSkeleton> *positiononly_skeletons)
	{
		//if( tcpClient.isConnected() )
		{
			int index = 0 ;
			lock() ;

			for( int i = 0; i < 6 ; i++) // clear down skels
				ofdata.ofxSkels[i].eTrackingState = NUI_SKELETON_NOT_TRACKED;


			//memcpy( ofdata.depthColorData, depthdata, (320 * 240) * 3);
			if(tracked_skeletons != NULL)
			{
				int size = tracked_skeletons->size();
				for( int i = 0; i < size && i < 2 ; i++)
				{
					memcpy( &ofdata.ofxSkels[index], &tracked_skeletons->at(i), sizeof(ofxMSKinectSkeleton) ) ;
					index++;
				}
			}


			if( bTrackedOnly == false )
			{
				if(positiononly_skeletons != NULL)
				{
					int size = positiononly_skeletons->size();
					index = 2;
					for( int i =0; i < size  && i < 4; i++)
					{
						memcpy( &ofdata.ofxSkels[index], &positiononly_skeletons->at(i), sizeof(ofxMSKinectSkeleton) ) ;
						index++;
					}
				}
			}

			bHasdata = true;

			unlock();
			return true;
		}
		return false ;
	}


void UDPKineticNetWorkClient::threadedFunction()
{
    udpConnection.Create();
	udpConnection.Connect(ipaddress, port);
	udpConnection.SetNonBlocking(true);

	while( isThreadRunning()  )
    {
			if( bHasdata )
			{
				lock();
				udpConnection.Send( (char *) &ofdata,
					(bTrackedOnly == false) ? sizeof(ofMS_NETWORK_SKELETON_DATA) : sizeof(ofMS_NETWORK_TRACKED_SKELETON_DATA));
				bHasdata = false ;
				unlock();

			}
		Sleep( 10 ) ;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//The server part .. listens for connections from other laptops/pc's then handles the imput of their
// skeletal information. and provides calls to allow the servers host app to get this skeletal information
// based on the index of the connection
 UDPKineticNetWorkServer::UDPKineticNetWorkServer(int in_port):ofThread()
{
		udpConnection.Create() ;
		udpConnection.Bind ( in_port );
		udpConnection.SetNonBlocking(true);
		startThread( true, false ) ;
}


UDPKineticNetWorkServer::~UDPKineticNetWorkServer(void)
{
		if( isThreadRunning() )
		{
			stopThread() ;

			while( isThreadRunning()  )
				::Sleep( 15 ) ;
		}
		udpConnection.Close();
}

void UDPKineticNetWorkServer::Pause()
{
	ofMS_NETWORK_COMMAND c;
	c.command = 'P';
//	TCP.sendRawBytesToAll((const char*)&c, sizeof(ofMS_NETWORK_COMMAND) );
}

void UDPKineticNetWorkServer::Resume()
{
	ofMS_NETWORK_COMMAND c;
	c.command = 'R';
//	TCP.sendRawBytesToAll((const char*)&c, sizeof(ofMS_NETWORK_COMMAND) );
}

void UDPKineticNetWorkServer::CloseClients()
{
	ofMS_NETWORK_COMMAND c;
	c.command = 'X';
//	TCP.sendRawBytesToAll((const char*)&c, sizeof(ofMS_NETWORK_COMMAND) );
}

void UDPKineticNetWorkServer::SetCameraElevation(int elevation )
{
	ofMS_NETWORK_COMMAND c;
	c.command = 'C';
	sprintf( &c.command_data[0],"%d", elevation );
//	TCP.sendRawBytesToAll((const char*)&c, sizeof(ofMS_NETWORK_COMMAND) );
}


void UDPKineticNetWorkServer::SetClientsSmoothingParams(char *data, int sizeofthedata )
{
	ofMS_NETWORK_COMMAND c;
	c.command = 'S';
	memcpy( &c.command_data[0], data, sizeofthedata );
//	TCP.sendRawBytesToAll((const char*)&c, sizeof(ofMS_NETWORK_COMMAND) );
}

void UDPKineticNetWorkServer::NotifyClientsServerClosing()
{
	ofMS_NETWORK_COMMAND c;
	c.command = 'Z';
//	TCP.sendRawBytesToAll((const char*)&c, sizeof(ofMS_NETWORK_COMMAND) );
}

int  UDPKineticNetWorkServer::NumberOfConnectedClients()
{
	return 0;//TCP.getNumClients() ;
}


string  UDPKineticNetWorkServer::ClientsIPaddresss(int clientid)
{
	//if( clientid < MAX_CONNECTIONS && TCP.isClientConnected(clientid) )
		//return TCP.getClientIP(clientid);

	return "";
}


void UDPKineticNetWorkServer::threadedFunction()
{
	memset( &ofxdata,0,sizeof(ofMS_NETWORK_SKELETON_DATA_CLIENTS) );

	//Sleep(1000 ); //we could be starting again whilst clients are already running.
	//Resume();     //so we could send a resume here to allow paused clients to start sending again ?

	while( isThreadRunning()  )
    {
		lock() ;
		 ofMS_NETWORK_SKELETON_DATA ofx;
		int icount = udpConnection.Receive((char *)  &ofx, sizeof(ofMS_NETWORK_SKELETON_DATA) );
		if( icount ==  sizeof(ofMS_NETWORK_SKELETON_DATA) || icount ==  sizeof(ofMS_NETWORK_TRACKED_SKELETON_DATA) )
		{
			char buffer[30];
			buffer[0] =0;
			udpConnection.GetRemoteAddr(buffer);
			bool bfound = false ;
			for( int i=0; i <  MAX_CONNECTIONS && bfound == false ; i++)
			{
				if( strcmp( buffer, ofxdata.buffer[i] ) == 0 )
				{
					memcpy( &ofxdata.clients[i], &ofx, icount) ;
					ofxdata.client_size[i] = icount;
					bfound = true;
				}
			}

			if( bfound == false )
			{
				for( int i=0; i <  MAX_CONNECTIONS && bfound == false ; i++)
				{
					if( ofxdata.client_size[i]  == 0 )
					{
						memcpy( &ofxdata.clients[i], &ofx, icount) ;
						ofxdata.client_size[i] = icount;
						strcpy( ofxdata.buffer[i], buffer );
						bfound = true;
					}
				}
			}
		}

		unlock() ;
		Sleep( 2 ) ;
    }
}


bool UDPKineticNetWorkServer::GetTrackedSkeleton(vector<ofxMSKinectSkeleton> * pdata, int clientid, char *clientname = NULL)
{
	bool retval = false ;
	if( clientid < MAX_CONNECTIONS && pdata != NULL)
	{
	//	if( TCP.isClientConnected(clientid) )
		{
			lock();
			int icount = ofxdata.client_size[clientid] ;

			if( icount ==  sizeof(ofMS_NETWORK_SKELETON_DATA) ||
					icount ==  sizeof(ofMS_NETWORK_TRACKED_SKELETON_DATA) )
			{
				if( clientname != NULL )
					memcpy( clientname, &ofxdata.clients[clientid].clientname, 30 ) ;

				for( int i =0;  i < 2 ;i++)
				{
					if( ofxdata.clients[clientid].ofxSkels[i].eTrackingState == NUI_SKELETON_TRACKED )
					{
						pdata->push_back( ofxdata.clients[clientid].ofxSkels[i] ) ;
						retval = true;
					}
				}

			}
			unlock() ;
		}
	}
	return retval;
}

bool UDPKineticNetWorkServer::GetPostionOnlySkeletons(vector<ofxMSKinectSkeleton> * pdata, int clientid, char *clientname = NULL )
{
	bool retval = false ;
	if( clientid < 2 )
	   return retval ;

	if( clientid  < MAX_CONNECTIONS && pdata != NULL)
	{
//		if( TCP.isClientConnected(clientid) )
		{
			lock();
			if(	ofxdata.client_size[clientid] ==  sizeof(ofMS_NETWORK_TRACKED_SKELETON_DATA) )
			{
				if( clientname != NULL )
					memcpy( clientname, &ofxdata.clients[clientid].clientname, 30 ) ;

				for( int i = 2;  i < 6 ;i++)
				{
					if( ofxdata.clients[clientid].ofxSkels[i].eTrackingState == NUI_SKELETON_POSITION_ONLY)
					{
						pdata->push_back( ofxdata.clients[clientid].ofxSkels[i] ) ;
						retval = true;
					}
				}
			}
			unlock();
		}
	}
	return retval;
}

