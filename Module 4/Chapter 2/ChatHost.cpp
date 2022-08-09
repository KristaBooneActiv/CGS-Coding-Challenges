#include <iostream>

#include "ChatHost.h"
#include "ChatMessage.h"

using namespace chat;

// ========================================================================
//                       Base Host Class
// ========================================================================
Host::Host(std::string aHostname, int aPort)
	: mHostname(aHostname)
	, mPort(aPort)
{ /* no-op */ }

Host::~Host()
{
	if (mHostPtr)
	{
		enet_host_destroy(mHostPtr);
	}
}

void Host::SetSubscriber(IMessageSubscriber* aSubscriberPtr)
{
    mSubscriberPtr = aSubscriberPtr;
}

void Host::Publish(const Message& aMessage) const
{
    if (mSubscriberPtr)
    {
        mSubscriberPtr->OnMessageReceived(aMessage);
    }
}

// ========================================================================
//                       Client Host Class
// ========================================================================
Client::Client()
    : Host()
{
    InitHost();
}

Client::Client(std::string aHostname, int aPort)
    : Host(aHostname, aPort)
{
    InitHost();
}

void Client::InitHost()
{
    mHostPtr = enet_host_create(NULL /* create a client host */,
        1 /* only allow 1 outgoing connection */,
        2 /* allow up 2 channels to be used, 0 and 1 */,
        0 /* assume any amount of incoming bandwidth */,
        0 /* assume any amount of outgoing bandwidth */);

    ENetAddress address;
    enet_address_set_host(&address, mHostname.c_str());
    address.port = mPort;

    /* Initiate the connection, allocating the two channels 0 and 1. */
    ENetPeer* peer = enet_host_connect(mHostPtr, &address, 2, 0);
    if (!peer)
    {
        std::cerr << "No available peers for initiating an ENet connection." << std::endl;
        throw std::exception("Unable to create client");
    }

    /* Wait up to 5 seconds for the connection attempt to succeed. */
    ENetEvent event;
    if (enet_host_service(mHostPtr, &event, 5000) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT)
    {
        // no-op
    }
    else
    {
        /* Either the 5 seconds are up or a disconnect event was */
        /* received. Reset the peer in the event the 5 seconds   */
        /* had run out without any significant event.            */
        enet_peer_reset(peer);
        std::cout << "Connection to the server failed." << std::endl;
        throw std::exception("Unable to create client");
    }
}

bool Client::ProcessesEventType(ENetEventType aEventType) const
{
    switch (aEventType)
    {
    case ENET_EVENT_TYPE_RECEIVE:
        return true;
    default:
        return false;
    }
}

void Client::ProcessEvent(const ENetEvent& aEvent)
{
    if (aEvent.type != ENET_EVENT_TYPE_RECEIVE)
    {
        return;
    }

    Message message;
    Message::deserialize((char*)aEvent.packet->data, aEvent.packet->dataLength, message);
    Publish(message);

    /* Clean up the packet now that we're done using it. */
    enet_packet_destroy(aEvent.packet);
}

// ========================================================================
//                       Server Host Class
// ========================================================================
Server::Server()
	: Host()
{
    InitHost();
}

Server::Server(std::string aHostname, int aPort)
    : Host(aHostname, aPort)
{
    InitHost();
}

void Server::InitHost()
{
    ENetAddress address;
    /* Bind the server to the default localhost.     */
    /* A specific host address can be specified by   */
    /* enet_address_set_host (& address, "x.x.x.x"); */
    address.host = ENET_HOST_ANY;
    /* Bind the server to port 1234. */
    address.port = mPort;
    mHostPtr = enet_host_create(&address /* the address to bind the server host to */,
        32      /* allow up to 32 clients and/or outgoing connections */,
        2      /* allow up to 2 channels to be used, 0 and 1 */,
        0      /* assume any amount of incoming bandwidth */,
        0      /* assume any amount of outgoing bandwidth */);
}

bool Server::ProcessesEventType(ENetEventType aEventType) const
{
    switch (aEventType)
    {
    case ENET_EVENT_TYPE_RECEIVE:
    case ENET_EVENT_TYPE_CONNECT:
    case ENET_EVENT_TYPE_DISCONNECT:
        return true;
    default:
        return false;
    }
}

void Server::ProcessEvent(const ENetEvent& aEvent)
{
    switch (aEvent.type)
    {
    case ENET_EVENT_TYPE_RECEIVE:
        ProcessReceiveEvent(aEvent);
        break;
    case ENET_EVENT_TYPE_CONNECT:
        ProcessConnectionEvent(aEvent);
        return;
    case ENET_EVENT_TYPE_DISCONNECT:
        ProcessDisconnectEvent(aEvent);
        break;
    default:
        return;
    }
}

void Server::ProcessConnectionEvent(const ENetEvent& aEvent)
{
    std::cout << "A new user has entered the chat." << std::endl;
}

void Server::ProcessDisconnectEvent(const ENetEvent& aEvent)
{
    std::cout << "User disconnected." << std::endl;
}

void Server::ProcessReceiveEvent(const ENetEvent& aEvent)
{
    Message message;
    Message::deserialize((char*)aEvent.packet->data, aEvent.packet->dataLength, message);
    Publish(message);
    
    /* Clean up the packet now that we're done using it. */
    enet_packet_destroy(aEvent.packet);
}