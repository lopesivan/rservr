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

extern "C" {
#include "open-file.h"
}

#include <string>

#include <string.h> //'strlen', 'strsep'
#include <stdlib.h> //'free', 'setenv'
#include <stdio.h> //'FILE', 'sscanf'
#include <unistd.h> //'fork', 'pipe', 'dup2', '_exit', 'access', 'setuid', 'setgid'
#include <signal.h> //'raise', 'kill'
#include <fcntl.h> //standard files
#include <sys/wait.h> //'waitpid'
#include <sys/select.h> //'select'
#include <sys/types.h> //'uid_t', 'gid_t'
#include <errno.h> //'errno'

extern "C" {
#include "param.h"
}


#define STRINGIFY(value) #value
#define STRING_FORMAT(format, size) "%" STRINGIFY(size) format


static std::string last_protocol;
static std::string last_file;


int open_file(const char *sSpec, pid_t *pProcess)
{
	if (!sSpec) return RSERVR_BAD_PROTOCOL;

	static char protocol[PARAM_MAX_INPUT_SECTION], file[PARAM_MAX_INPUT_SECTION];

	if (sscanf(sSpec,
	           STRING_FORMAT("[^:/]", PARAM_MAX_INPUT_SECTION) "://"
	           STRING_FORMAT("s", PARAM_MAX_INPUT_SECTION),
	           protocol, file) == 2)
	{
	//reuse the last protocol if the protocol specified was "*"
	if (strcmp(protocol, "*") == 0) strncpy(protocol, last_protocol.c_str(), PARAM_MAX_INPUT_SECTION);

	//unset the last file if the protocol isn't being reused
	else last_file.clear();

	std::string protocol_file = std::string(PROTOCOL_PATH "/") + protocol;

	int pipes[2] = { -1, -1 };
	if (pipe(pipes) != 0) return RSERVR_PROTOCOL_ERROR;

	pid_t new_process = fork();


	if (new_process < 0) return RSERVR_PROTOCOL_ERROR;

	//TODO: add logging of errors


	else if (new_process == 0)
	 {
	if (access(protocol_file.c_str(), X_OK | R_OK) == -1) _exit(1);

	char *command[] = { &protocol_file[0], file, last_file.size()? &last_file[0] : NULL, NULL };

	close(pipes[0]);
	if (dup2(pipes[1], STDOUT_FILENO) < 0) _exit(1);
	raise(SIGSTOP);
	_exit(execvp(command[0], command));
	 }


	else
	 {
	close(pipes[1]);
	int status = 0, error;

	while ( ((error = waitpid(new_process, &status, WUNTRACED)) < 0 && errno == EINTR) ||
	        (!WIFSTOPPED(status) && !WIFEXITED(status)) );

	if (error < 0 || WIFEXITED(status))
	  {
	kill(new_process, SIGKILL);
	close(pipes[0]);
	return RSERVR_BAD_PROTOCOL;
	  }

	kill(new_process, SIGCONT);

	fd_set read_ready;
	FD_ZERO(&read_ready);
	FD_SET(pipes[0], &read_ready);

	while ((error = select(pipes[0] + 1, &read_ready, NULL, NULL, NULL)) < 0 && errno == EINTR);

	if (error < 0)
	  {
	close(pipes[0]);
	kill(new_process, SIGTERM);
	waitpid(new_process, &status, 0x00);
	return RSERVR_PROTOCOL_ERROR;
	  }

	else if (waitpid(new_process, &status, WNOHANG | WEXITED) == new_process)
	  {
	close(pipes[0]);
	return RSERVR_PROTOCOL_ERROR;
	  }

	last_protocol = protocol;
	last_file     = file;

	if (pProcess) *pProcess = new_process;
	return pipes[0];
	 }
	}


	else
	{
	int new_file = open(sSpec, O_RDONLY);
	if (new_file == -1) return RSERVR_PROTOCOL_ERROR;
	return new_file;
	}
}


void close_file(int fFile, pid_t pProcess)
{
	if (fFile < 0) return;
	close(fFile);
	close_process(pProcess);
}


void close_process(pid_t pProcess)
{
	if (pProcess < 0) return;
	kill(pProcess, SIGTERM);
	waitpid(pProcess, NULL, 0x00);
}
