#include "painlessMesh.h"

/*
    WiFi Credentials (Should be same for all the nodes)
*/
#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

/*
    object for scheduler and painlessmesh
*/
Scheduler userScheduler;                                                                      // to control your personal task
painlessMesh  mesh;


void sendMessage() ;

/*
    Creating task to send the message
*/
Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );


/*
    Function for sending message
*/
void sendMessage()                                                                            //this function is used for sending messages to other nodes
{
  String msg_send;                                                                            //Sending message should be stored in this variable
  msg_send = "HELLO";
  mesh.sendBroadcast(msg_send);                                                               //Broadcasting to all nodes
  //  mesh.sendSingle(uint32_t <node ID>, String msg_send);                                   //Broadcasting to a particular node

  taskSendMessage.setInterval((TASK_SECOND * 1));
}


/*
    Function for receiving message
*/
void receivedCallback( uint32_t from, String &msg )                                           //this function is used for receiving messages form other node
{
  String msg_recv;                                                                            //Received message should be stored in this variable
  msg_recv = msg.c_str();
  Serial.println(msg_recv);
}

/*
    New connection callback function
*/
void newConnectionCallback(uint32_t nodeId)
{
  Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

/*
   Changed Connection callback
*/
void changedConnectionCallback()
{
  Serial.printf("Changed connections\n");
}

/*
   node time adjustment callback
*/
void nodeTimeAdjustedCallback(int32_t offset)
{
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}


void setup()
{
  Serial.begin(115200);

  //do whatever you want to setup the controller

  //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );                                                      // set before init() so that you can see startup messages
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );                            // This will initialize the mesh network
  mesh.onReceive(&receivedCallback);                                                             // Set a callback routine for any messages that are addressed to this node
  mesh.onNewConnection(&newConnectionCallback);                                                  // This fires every time the local node makes a new connection.
  mesh.onChangedConnections(&changedConnectionCallback);                                         // This fires every time there is a change in mesh topology
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);                                            // This fires every time local time is adjusted to synchronize it with mesh time

  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();


}

void loop() 
{
   mesh.update();
}
