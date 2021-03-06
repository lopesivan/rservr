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
 | Copyright (c) 2011, Kevin P. Barry [the resourcerver project]
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

#ifndef rsvp_dataref_hook_h
#define rsvp_dataref_hook_h

#ifdef __cplusplus
extern "C" {
#endif

#include "rsvp-dataref.h"

#include <unistd.h> /* 'ssize_t' */

#include "../api/command.h"


struct dataref_source_info
{
	text_info origin;
	text_info target;
	text_info sender;
	text_info address;

	message_handle respond;
} __attribute__ ((packed));

extern command_event __rsvp_dataref_hook_open_reference(const struct dataref_source_info*, text_info, int, uint8_t, uint8_t);
extern command_event __rsvp_dataref_hook_change_reference(const struct dataref_source_info*, text_info, int, uint8_t, uint8_t);
extern command_event __rsvp_dataref_hook_close_reference(const struct dataref_source_info*, int);
extern command_event __rsvp_dataref_hook_transfer_data(const struct dataref_source_info*, int, uint8_t, ssize_t, ssize_t);
extern command_event __rsvp_dataref_hook_alteration(const struct dataref_source_info*, int, ssize_t, ssize_t);

#ifdef __cplusplus
}
#endif

#endif /*rsvp_dataref_hook_h*/
