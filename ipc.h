/*! @file ipc.h
 * @brief Interprocess communication class
 */


#ifndef IPC_H_
#define IPC_H_

#include <sys/stat.h>
#include <sys/types.h>

#include "camera.h"
#include "image_processing.h"


#define BUFFER_SIZE (1024)


/*! @brief File permissions of the server socket file node. */
#define SERV_SOCKET_PERMISSIONS     \
	(S_IXUSR | S_IRUSR | S_IWUSR |  \
		S_IXGRP | S_IRGRP | S_IWGRP |  \
		S_IXOTH | S_IROTH | S_IWOTH)




#define INIT_EXPOSURE_TIME 10000

/* settings that can be changed over the web */
struct WEB_SETTINGS {
	int exposure_time; // [us]
};

enum HTML_HEADER_TYPE {
	HEADER_TEXT_PLAIN,
	HEADER_IMAGE_BMP,
        HEADER_IMAGE_JPG
};


class CIPC {
public:
	CIPC(CCamera& camera, CImageProcessor& img_process);
	~CIPC();
	
	OSC_ERR Init();
	
	
	const WEB_SETTINGS& WebSettings() const { return(m_web_settings); }
	
	/*! @brief answer cgi requests from webapp */
	OSC_ERR handleIpcRequests();
	
	int img_count;
	
private:
	void ProcessRequest(char* request);
	
	void WriteHtmlHeader(HTML_HEADER_TYPE type, int content_length=0);
	bool m_bHeader_written;
	
	/* returns begin to header or NULL
	 * request will be set to next line */
	char* ReadHeader(char ** request);
	OSC_ERR ReadArgument(char ** pBuffer, char ** pKey, char ** pValue);
	OSC_ERR WriteArgument(const char * pKey, const char * pValue);
	OSC_ERR WriteArgument(const char * pKey, int value);
	OSC_ERR WriteImage(const cv::Mat img);
	int IpcWrite(const void* buf, size_t count); /* write to socket, returns > 0 on success */
	int m_fd; //file handle
	
	/*! @brief Strips whitespace from the beginning and the end of a string and returns the new beginning of the string. Be advised, that the original string gets mangled! */
	char* strtrim(char* str);
	
	
	CCamera& m_camera;
        CImageProcessor& m_img_process;
        
	int m_socket_fd;
	
	static const int m_buffer_count=1024;
	char m_buffer[m_buffer_count];
	
	
	bool m_bInit;
	WEB_SETTINGS m_web_settings;
};



#endif // #ifndef IPC_H_
