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

#ifndef proto_external_command_hpp
#define proto_external_command_hpp

#include "plugin-dev/external-command.hpp"

#include <hparser/formats/tagged-input.hpp>


class null_command :
	public external_command,
	public tagged_input
{
public:
	null_command(const text_data&);

	//from 'storage_section'------------------------------------------------
	section_releaser copy() const;
	//----------------------------------------------------------------------

	//from 'element_interface'----------------------------------------------
	command_event evaluate_server(const command_info&, server_interface*) const;
	command_event evaluate_client(const command_info&, client_interface*) const;
	permission_mask execute_permissions() const;
	//----------------------------------------------------------------------

	//from 'connection_control'---------------------------------------------
	bool allow_next(const storage_section*) const;
	//----------------------------------------------------------------------

private:
	//from 'tagged_input'---------------------------------------------------
	bool is_close_tag(data_input*) const;
	//----------------------------------------------------------------------

	//from 'managed_input'--------------------------------------------------
	bool makes_sense(data_input*) const;
	bool is_subsection(data_input*) const;
	input_receiver *new_subsection(data_input*);
	input_receiver *apply_input(data_input*);
	input_receiver *get_in_parent() const;
	//----------------------------------------------------------------------

	//from 'managed_ouput'--------------------------------------------------
	bool send_content(data_output*) const;
	//----------------------------------------------------------------------

	text_data inert_content;
};

#endif //proto_external_command_hpp