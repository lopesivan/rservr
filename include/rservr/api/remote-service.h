/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*\

______]|]]]]|]__]|]]]]|]__]|]]]]]|]__]|]]]]|]__]|]__]|]__]|]]]]|]_______,_______
_____]|]__]|]__]|]_______]|]___]|]__]|]__]|]___]|]_]|]__]|]__]|]_______, ,______
____]|]__]|]__]|]]]]|]__]|]]]]]|]__]|]__]|]____]|]]|]__]|]__]|]_______,   ,_____
___]|]____________]|]__]|]________]|]__________]|]|]__]|]____________, -R- ,____
__]|]____________]|]__]|]________]|]___________]||]__]|]____________,   |   ,___
_]|]_______]|]]]]|]__]|]]]]]|]__]|]____________]|]__]|]____________, , , , , ,__
                                                                      ||  |
\*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/* This software is released under the BSD License.
 |
 | Copyright (c) 2009, Kevin P. Barry [the resourcerver project]
 | All rights reserved.
 |
 | Redistribution  and  use  in  source  and   binary  forms,  with  or  without
 | modification, are permitted provided that the following conditions are met:
 |
 | - Redistributions of source code must retain the above copyright notice, this
 |   list of conditions and the following disclaimer.
 |
 | - Redistributions in binary  form must reproduce the  above copyright notice,
 |   this list  of conditions and the following disclaimer in  the documentation
 |   and/or other materials provided with the distribution.
 |
 | - Neither the  name  of  the  Resourcerver  Project  nor  the  names  of  its
 |   contributors may be  used to endorse or promote products  derived from this
 |   software without specific prior written permission.
 |
 | THIS SOFTWARE IS  PROVIDED BY THE COPYRIGHT HOLDERS AND  CONTRIBUTORS "AS IS"
 | AND ANY  EXPRESS OR IMPLIED  WARRANTIES,  INCLUDING, BUT  NOT LIMITED TO, THE
 | IMPLIED WARRANTIES OF  MERCHANTABILITY  AND FITNESS FOR A  PARTICULAR PURPOSE
 | ARE DISCLAIMED.  IN  NO EVENT SHALL  THE COPYRIGHT  OWNER  OR CONTRIBUTORS BE
 | LIABLE  FOR  ANY  DIRECT,   INDIRECT,  INCIDENTAL,   SPECIAL,  EXEMPLARY,  OR
 | CONSEQUENTIAL   DAMAGES  (INCLUDING,  BUT  NOT  LIMITED  TO,  PROCUREMENT  OF
 | SUBSTITUTE GOODS OR SERVICES;  LOSS  OF USE,  DATA,  OR PROFITS;  OR BUSINESS
 | INTERRUPTION)  HOWEVER  CAUSED  AND ON  ANY  THEORY OF LIABILITY,  WHETHER IN
 | CONTRACT,  STRICT  LIABILITY, OR  TORT (INCLUDING  NEGLIGENCE  OR  OTHERWISE)
 | ARISING IN ANY  WAY OUT OF  THE USE OF THIS SOFTWARE, EVEN  IF ADVISED OF THE
 | POSSIBILITY OF SUCH DAMAGE.
 +~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*! \file rservr/api/remote-service.h
 *  \author Kevin P. Barry
 *  \brief Remote service actions.
 *
 * This header contains the interface used by the standard IPC forwarders to
 * monitor connections that remote services rely on. This interface isn't
 * currently supported for direct client program use.
 */

#ifndef rservr_remote_service_h
#define rservr_remote_service_h

#ifdef __cplusplus
extern "C" {
#endif

#include "command.h"


struct remote_service_data
{
	uint8_t originator:1;
	uint8_t no_check:1;
	uint8_t direction:2;
	uint8_t action:2;

	text_info current_target;
	text_info service_name;
	text_info monitor_connection;
	text_info notify_entity;
	text_info notify_address;
	text_info complete_address;
} __attribute__ ((packed));


#define RSERVR_REMOTE_TOWARD_SERVER ((uint8_t) 0x01 << 0) /*only 2 bits wide*/
#define RSERVR_REMOTE_TOWARD_CLIENT ((uint8_t) 0x01 << 1) /*only 2 bits wide*/

#define RSERVR_REMOTE_MONITOR   ((uint8_t) 0x01 << 0) /*only 2 bits wide*/
#define RSERVR_REMOTE_UNMONITOR ((uint8_t) 0x01 << 1) /*only 2 bits wide*/


extern void __remote_service_action_hook(const struct remote_service_data*);
extern void __resource_exit_hook(text_info);

extern result remote_service_broken_connection(const struct remote_service_data*);

extern result compare_original_servers(text_info, text_info, text_info, text_info);

#ifdef __cplusplus
}
#endif

#endif /*rservr_remote_service_h*/
