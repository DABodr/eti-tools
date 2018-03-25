/*
 * eti_ni2http.h
 *
 *  Created on: 17.09.2013
 *      Author: tipok
 */

#ifndef ETI_NI2HTTP_H_
#define ETI_NI2HTTP_H_

#include <stdio.h>
#include <inttypes.h>
#include <endian.h>
#include <netinet/in.h>

// Use by gethostname()
#ifndef HOST_NAME_MAX
#ifdef _POSIX_HOST_NAME_MAX
#define HOST_NAME_MAX	_POSIX_HOST_NAME_MAX
#else
#define HOST_NAME_MAX	(256)
#endif
#endif

#ifndef DOMAIN_NAME_MAX
#define DOMAIN_NAME_MAX	(1024)
#endif

#define MAX_CHANNEL_COUNT		128

// Standard string buffer size
#define STR_BUF_SIZE			1025


#define MAX_CU_COUNT			864


/* Structure containing single channel */
typedef struct ni2http_channel_s {

	int num;				// channel number
	int sid;				// Packet Identifier of audio stream

	// Metadata about the channel
	char name[STR_BUF_SIZE];	// channel name
	char genre[STR_BUF_SIZE];	// genre
	char description[STR_BUF_SIZE];	// description
	char url[STR_BUF_SIZE];		// Informational URL

	char file_name[STR_BUF_SIZE];		// Informational URL
        FILE *file;
	int shout_state;
	char is_public;				// announce existance?

	uint8_t *buf;		//  MPEG Audio Buffer
	uint32_t buf_size;		// Usable size of MPEG Audio Buffer
	uint32_t buf_used;		// Amount of buffer used

	int bitrate;

	int payload_size;				// Size of the payload

	int is_dabplus;
	uint8_t *dabplus_data;
	void *dabplus_rs;
	int dabplus_frame;

	int extract_dabplus;
	int extract_pad;
	uint8_t pad_data[STR_BUF_SIZE];
	int pad_fillness;
	int pad_bytes_left;
	int title_switcher;
	int pad_last_fl;
	void *zmq_sock;   /* zmq socket */
} ni2http_channel_t;

#define ZMQ_ENCODER_FDK 1
#define ZMQ_ENCODER_TOOLAME 2
struct zmq_frame_header
{
    uint16_t version; // we support version=1 now
    uint16_t encoder; // see ZMQ_ENCODER_XYZ

    /* length of the 'data' field */
    uint32_t datasize;

    /* Audio level, peak, linear PCM */
    int16_t audiolevel_left;
    int16_t audiolevel_right;

    /* Data follows this header */
} __attribute__ ((packed));


/* Structure server settings */
typedef struct ni2http_server_s {
	char host[STR_BUF_SIZE];
	int port;
	char user[STR_BUF_SIZE];
	char password[STR_BUF_SIZE];
	int protocol;

} ni2http_server_t;


typedef struct {
	union {
#if __BYTE_ORDER == __LITTLE_ENDIAN
		/* reverse order for little-endian */
		struct {
			uint32_t		fl:11;
			uint32_t		mid:2;
			uint32_t		fp:3;
			uint32_t		nst:7;
			uint32_t		ficf:1;
			uint32_t		fct:8;
		};
#elif __BYTE_ORDER == __BIG_ENDIAN
		/* reverse order for little-endian */
		struct {
			uint32_t		fct:8;
			uint32_t		ficf:1;
			uint32_t		nst:7;
			uint32_t		fp:3;
			uint32_t		mid:2;
			uint32_t		fl:11;
		};
#else
#error "Unknown system endian"
#endif
		uint32_t		val;
	};
} fc_t;

typedef struct {
	union {
#if __BYTE_ORDER == __LITTLE_ENDIAN
		/* reverse order for little-endian */
		struct {
			uint32_t		stl:10;
			uint32_t		tpl:6;
			uint32_t		sad:10;
			uint32_t		scid:6;
		};
#elif __BYTE_ORDER == __BIG_ENDIAN
		/* reverse order for little-endian */
		struct {
			uint32_t		scid:6;
			uint32_t		sad:10;
			uint32_t		tpl:6;
			uint32_t		stl:10;
		};
#else
#error "Unknown system endian"
#endif
		uint32_t		val;
	};
} sstc_t;

/* In eti_ni2http.c */
extern int channel_count;
extern ni2http_channel_t *channel_map[MAX_CU_COUNT];
extern ni2http_channel_t *channels[MAX_CHANNEL_COUNT];
extern ni2http_server_t ni2http_server;

extern void *zmq_context;


/* In parse_config.c */
int parse_config( char *filepath );

/* In wfcrc.c */
int crccheck(unsigned char* buf, int len);
int crc16check(unsigned char* buf, int len);

/* In wffirecrc.c */
int firecrccheck(unsigned char* buf);

#define DABPLUS


#endif /* ETI_NI2HTTP_H_ */
