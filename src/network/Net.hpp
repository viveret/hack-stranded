#ifndef SURVIVOR_ENGINE_NETWORK
#define SURVIVOR_ENGINE_NETWORK

#include <string>
#include <vector>
//#include <queue>


#include "../Core.hpp"
#include "../utility/Array.hpp"
#include "../content/Manager.hpp"
#include "../module/Events.hpp"

#define SE_NET_PACKET_SIZE 1024
#define SE_NET_PING_LOST 30

#define SE_DEFAULT_IP "127.0.0.1"
#define SE_DEFAULT_PORT "7777" // default input is as string


// event parameters
#define SE_NET_FILE_DWN 512 // max 512 bytes download
#define SE_NET_FS_AVG 105 // a minimum for filesystem items per packet

#ifdef _WIN32
typedef char NET_BUF_TYPE;
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <cstdlib>
#include <strings.h>

typedef void NET_BUF_TYPE;
#endif

#include <queue>
namespace SE_Network
{
#ifdef _WIN32
	typedef SOCKET socket_t;
#else
	typedef int socket_t;
#endif

	struct SE_Socket_Packet // data to be sent
	{
		uint Signature;
		uchar Data[ SE_NET_PACKET_SIZE - sizeof(uint) ];

		uint Buf_Index;



		SE_Socket_Packet();
		uint Size();
		uchar& operator[] ( const uint nIndex );
		void Clear();

		// add data to buffer
		bool Buf_Add( const std::string& What );
		bool Buf_Add( void* What, uchar Size );

		bool Buf_Get( std::string* Where );
		bool Buf_Get( void* Where, uchar Size );

		void Buf_Revert();
		void Buf_Index_Push();
		void Buf_Index_Pop();

		std::vector< uint > Buf_Index_Stack;
	};

	struct SE_Socket // needs a message queue for special events (net message, disconnect)
	{
		SE_Socket();

		void Initialize();
		void Cleanup();

		void Recieve();
		void Send();

		void Send_Flush();// send all
		void Recieve_Process();

		void Update( double dt );
		void (*Update_Send)( SE_Socket* );
		u_char m_State;

		E_Threads::T_Thread m_Thread;

		std::string Get_IP();

		SE_Socket_Packet Buffer;
		ssize_t Buf_Amount;


		float Ping;// millisecond ping
		bool Alive;

		socket_t Sock_h;
		sockaddr_in Address;

		// attributes
		std::string Username, Password, Display_Name;


#ifdef _WIN32
		HANDLE Thread_h;
#else

#endif
		// event handler
		void Add_Event( SE_Events::Event_Base* nEv );
	private:
		SE_Thread::Mutex m_Mutex;
		std::queue<SE_Events::Event_Base*> Event_Out_List; // send out
	};
	namespace Socket_States
	{enum{

		STATE_NONE = 0,
		STATE_INIT = STATE_NONE,
		STATE_VERIFY,
		STATE_SYNC,
		STATE_DOWNLOAD,
		STATE_UPLOAD,
		STATE_NORMAL
	};}


	struct SE_Net_Manager_Base
	{
		SE_Net_Manager_Base();
#ifdef _WIN32
		void Initialize_WSA();// the network library
		void Cleanup_WSA();

		WSADATA WSA_Data;
		SOCKADDR_IN Target;//Socket address information
#else
#endif
		bool Assert( std::string Message );

		virtual void Initialize() = 0;

		virtual void Update( double dt ) = 0;
		virtual void Cleanup() = 0;

		bool Is_Host;// is the application a host?

		SE_Socket Main_Socket;
		virtual void Event_Process( SE_Socket* This, SE_Events::Event_Base* nEv ) = 0;

		ushort Con_Port;
	};

	struct SE_Net_Manager_HOST:public SE_Net_Manager_Base//, public SE_Manager::SE_Man_Manager
	{
		SE_Net_Manager_HOST();

		void Initialize();
		void Cleanup();

		void Update( double dt );
		void Event_Process( SE_Socket* This, SE_Events::Event_Base* nEv );

		void Start_Listening_For_Clients();

		static void Listen_For_Clients();
		static void Update_Client( void* This_Void );

		void Global_Event( SE_Events::Event_Base* nEv );

		std::vector< SE_Socket* > Clients;
		SE_Thread::Mutex m_Mutex;
	};
	void Update_Host_Send( SE_Socket* This );

	struct SE_Net_Manager_CLIENT:public SE_Net_Manager_Base
	{
		SE_Net_Manager_CLIENT();
		void Initialize();
		void Cleanup();

		void Update( double dt );
		void Event_Process( SE_Socket* This, SE_Events::Event_Base* nEv );

		void Connect(); // strings have been set
		static void Listen_To_Host(); // start listening thread

		std::string Con_IP;// connected to ip
		hostent* Server_Info;
	};
	void Update_Client_Send( SE_Socket* This );



	void Input_Start_Host( sshort* Data );
	void Input_Start_Client( sshort* Data );


#ifdef _WIN32
	DWORD WINAPI HOST_Listen_For_New(void*);
	DWORD WINAPI HOST_Handle_Client(void*);
	DWORD WINAPI CLIENT_Connect_To_Host(void*);

	bool WSA_Assert( std::string, int );
#endif
}

namespace SE_Net_Message
{
enum {

	NET_N_A = 0,// nothing / anything

// network events
	NET_INFO, // send new network info
	NET_DISCON, // disconnect with message
	NET_CHAT, // send / recieve c-string message

// file server
	FILE_VERIFY,
	FILE_CHECKSUM, // compare checksums
	FILE_DOWNLOAD, // downloading file
	FILE_CACHE_COMP, // compare cache

// content
	CONT_MESH,

// physics
	PHYS_TRANS, // position and rotation

// user message
	NET_OWN,

// other / less used
	ETC_VERSION, // send engine version

// external
	NET_EXT
};}
namespace SE_Net_Events // data containers for event_var
{
	struct Event_VFS // right now it just checks if it exists
	{
		std::string Name, Name_FS;
//		uint Verify_Code; // md5 for now

		std::vector< Event_VFS > Children;

		void Recursive_Add( uint* Amount, SE_Network::SE_Socket_Packet* Where );
		void Recursive_Get( SE_Network::SE_Socket_Packet* Where );

		static void Create_From_VFS( Event_VFS* Where, SE_Virt_Filesys::F_Folder* Folder );
		static SE_Virt_Filesys::F_Folder* Create_VFS();
	};
	struct Event_Download_File // downloading file
	{
		std::string Path, FS_Path;
		uint Length;
		uchar* Data;

		Event_Download_File();
	};
}

namespace Net_Info_Msg
{enum {
	INFO_N_A = 0,
	INFO_DISPLAY_NAME,

};}


#endif
