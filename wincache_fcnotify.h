/*
   +----------------------------------------------------------------------------------------------+
   | Windows Cache for PHP                                                                        |
   +----------------------------------------------------------------------------------------------+
   | Copyright (c) 2009, Microsoft Corporation. All rights reserved.                              |
   |                                                                                              |
   | Redistribution and use in source and binary forms, with or without modification, are         |
   | permitted provided that the following conditions are met:                                    |
   | - Redistributions of source code must retain the above copyright notice, this list of        |
   | conditions and the following disclaimer.                                                     |
   | - Redistributions in binary form must reproduce the above copyright notice, this list of     |
   | conditions and the following disclaimer in the documentation and/or other materials provided |
   | with the distribution.                                                                       |
   | - Neither the name of the Microsoft Corporation nor the names of its contributors may be     |
   | used to endorse or promote products derived from this software without specific prior written|
   | permission.                                                                                  |
   |                                                                                              |
   | THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS  |
   | OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF              |
   | MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE   |
   | COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,    |
   | EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE|
   | GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED   |
   | AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING    |
   | NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED |
   | OF THE POSSIBILITY OF SUCH DAMAGE.                                                           |
   +----------------------------------------------------------------------------------------------+
   | Module: wincache_fcnotify.h                                                                  |
   +----------------------------------------------------------------------------------------------+
   | Author: Kanwaljeet Singla <ksingla@microsoft.com>                                            |
   +----------------------------------------------------------------------------------------------+
*/

#ifndef _WINCACHE_FCNOTIFY_H_
#define _WINCACHE_FCNOTIFY_H_

/* fcnotify_value   - LOCAL */
/* fcnotify_context - LOCAL */

typedef struct fcnotify_value fcnotify_value;
struct fcnotify_value
{
    size_t                  folder_path;   /* folder path */
    unsigned int            owner_pid;     /* pid listening to changes */
    HANDLE                  folder_handle; /* handle to folder */
    OVERLAPPED              overlapped;    /* overlapped structure */
    FILE_NOTIFY_INFORMATION information;   /* file notify information */
    size_t                  prev_value;    /* previous aplist_value offset */
    size_t                  next_value;    /* next aplist_value offset */
};

typedef struct fcnotify_header fcnotify_header;
struct fcnotify_header
{
    unsigned int            rdcount;       /* Reader count for shared lock */
    unsigned int            active_count;  /* Folders count which have active listeners */
    unsigned int            valuecount;    /* Total number of entries in entries */
    size_t                  values[1];     /* HashTable for fcnotify_value entries */
};

typedef struct fcnotify_context fcnotify_context;
struct fcnotify_context
{
    unsigned short          islocal;       /* Is local or shared */
    unsigned char           isshutting;    /* True means process is shutting down */
    unsigned char           iswow64;       /* Take care of wow64 when handling paths */
    unsigned int            processid;     /* Process id of this process */

    char *                  fcmemaddr;     /* Memory address where data is stored */
    fcnotify_header *       fcheader;      /* Pointer to fcnotify_header */
    alloc_context *         fcalloc;       /* Allocator to be used for fcnotify */
    lock_context *          fclock;        /* Lock to deal with data in shared memory */

    HANDLE                  listen_thread; /* Change listener thread handle */
    HANDLE                  port_handle;   /* Completion port for change notifications */
    HashTable *             pidhandles;    /* Key = processid, value = process handle */
};

extern int  fcnotify_create(fcnotify_context ** ppnotify);
extern void fcnotify_destroy(fcnotify_context * pnotify);
extern int  fcnotify_initialize(fcnotify_context * pnotify, unsigned short islocal, alloc_context * palloc, unsigned int filecount TSRMLS_DC);
extern void fcnotify_terminate(fcnotify_context * pnotify);

extern int  fcnotify_ispresent(fcnotify_context * pnotify);
extern void fcnotify_runtest();

#endif /* _WINCACHE_FCNOTIFY_H_ */