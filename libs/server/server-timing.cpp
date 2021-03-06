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

#include "server-timing.hpp"


bool read_server_table(const struct server_timing_table *tTable, struct server_timing_table *cCopy)
{
	if (!tTable || !cCopy) return false;
	*cCopy = *tTable;
	return true;
}


bool update_server_table(struct server_timing_table *tTable, const struct server_timing_table *uUpdate)
{
	if (!tTable || !uUpdate) return false;
	return calculate_server_table(uUpdate, tTable) >= 0;
}


bool set_server_table(struct server_timing_table *tTable, const struct server_timing_table *sSet)
{
	if (!tTable || !sSet) return false;
	*tTable = *sSet;
	return true;
}


bool calculate_server_timing_specs(const struct server_timing_table *tTable,
struct internal_server_timing_table *sSpecs)
{
	if (!tTable || !sSpecs) return false;
	return compile_server_table(tTable, sSpecs) >= 0;
}
