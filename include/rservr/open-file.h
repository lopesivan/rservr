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
 | Copyright (c) 2014, Kevin P. Barry [the resourcerver project]
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

/*! \file rservr/open-file.h
 *  \author Kevin P. Barry
 *  \brief Protocol-based file access.
 *
 * This header contains a function to automatically open a file, based on an
 * optionally-specified protocol, e.g., 'tar:configs.tar:config'.
 */

#ifndef rservr_open_file_h
#define rservr_open_file_h

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h> /*'uid_t', 'gid_t', */


#define RSERVR_FILE_ERROR     -1 /**< Problem opening file without a protocol.*/
#define RSERVR_PROTOCOL_ERROR -2 /**< Protocol exited with an error before sending data.*/
#define RSERVR_BAD_PROTOCOL   -3 /**< Bad protocol specified.*/


/*! \brief Open a file using a protocol.
 *
 * @see close_file
 * \note This function isn't reentrant.
 *
 * \param Spec a file specification with the format 'protocol:specification',
 * where 'protocol' names a protocol (e.g., 'tar') and 'specification' is a
 * string whose format depends on 'protocol'
 * \param Process pointer for storing the process id of the protocol
 * \return open file descriptor ready to read, or < 0 for an error
 */
extern int open_file(const char *Spec, pid_t *Process);


/*! \brief Attempt to extract the filename of the protocol specification.
 *
 * \param Spec a file specification with the format 'protocol:specification',
 * where 'protocol' names a protocol (e.g., 'tar') and 'specification' is a
 * string whose format depends on 'protocol'
 * \return filename, or NULL if a protocol is used (must be freed)
 */
extern char *try_filename(const char *Spec);


/*! \brief Close a file opened by open_file.
 * @see open_file
 *
 * \param File file descriptor returned by open_file
 * \param Process process id of the protocol returned by open_file
 * \return process exit status (nonzero for error)
 */
extern int close_file(int File, pid_t Process);


/*! \brief Close a protocol process without closing a file.
 * @see open_file
 *
 * \param Process process id of the protocol returned by open_file
 * \return process exit status (nonzero for error)
 */
extern int close_process(pid_t Process);

#ifdef __cplusplus
}
#endif

#endif /*rservr_open_file_h*/
