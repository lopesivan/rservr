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
#include <sys/stat.h> //'stat'
#include <errno.h> //'errno'

extern "C" {
#include "param.h"
}


#define STRINGIFY(value) #value
#define STRING_FORMAT(format, size) "%" STRINGIFY(size) format


static bool split_protocol(const char *sSpec, std::string &pProto, std::string &fFile)
{
	//TODO: this should really be 'assert' or something
	if (!sSpec) return false;

	char protocol[PARAM_MAX_INPUT_SECTION], file[PARAM_MAX_INPUT_SECTION];
	pProto.clear();
	fFile = sSpec;

	bool use_last = false;

	if (sscanf(sSpec,
	//NOTE: excluding "/" is very important for security!
	           STRING_FORMAT("[^:/]", PARAM_MAX_INPUT_SECTION) "://"
	           STRING_FORMAT("s", PARAM_MAX_INPUT_SECTION),
	           protocol, file) == 2)
	{
	fFile = file;

	//reuse the last protocol if the protocol specified was "-"
	//TODO: use a parameter for "-"
	if (strcmp(protocol, "-") == 0)
	 {
	if (getenv("_RSERVR_LAST_PROTOCOL"))
	  {
	strncpy(protocol, getenv("_RSERVR_LAST_PROTOCOL"), PARAM_MAX_INPUT_SECTION);
	use_last = true;
	  }
	else protocol[0] = 0x00;
	 }

	pProto = protocol;
	}

	return use_last;
}


int open_file(const char *sSpec, pid_t *pProcess)
{
	if (!sSpec) return RSERVR_BAD_PROTOCOL;

	std::string protocol, filename;
	bool use_last = split_protocol(sSpec, protocol, filename);


	if (protocol.size())
	{
	int pipes[2] = { -1, -1 };
	if (pipe(pipes) != 0) return RSERVR_PROTOCOL_ERROR;

	pid_t new_process = fork();


	if (new_process < 0) return RSERVR_PROTOCOL_ERROR;

	//TODO: add logging of errors


	else if (new_process == 0)
	 {
	struct stat protocol_stat;

	if (stat(PROTOCOL_PATH, &protocol_stat) == -1) _exit(1);
	if (!S_ISDIR(protocol_stat.st_mode)) _exit(1);

	if (getuid() == 0)
	  {
	if ((protocol_stat.st_mode & (S_IWGRP | S_IWOTH))) _exit(1);
	if (protocol_stat.st_uid != 0) _exit(1);
	if (protocol_stat.st_gid != 0) _exit(1);
	  }

	std::string protocol_file = std::string(PROTOCOL_PATH "/") + protocol;
	if (access(protocol_file.c_str(), X_OK | R_OK) == -1) _exit(1);

	if (stat(protocol_file.c_str(), &protocol_stat) == -1) _exit(1);
	if (!S_ISREG(protocol_stat.st_mode)) _exit(1);

	if (getuid() == 0)
	  {
	if ((protocol_stat.st_mode & (S_IWGRP | S_IWOTH))) _exit(1);
	if (protocol_stat.st_uid != 0) _exit(1);
	if (protocol_stat.st_gid != 0) _exit(1);
	  }

	//TODO: add a function that just returns this command, and another that
	//calls it and waits, etc.; that way, this can be used for files that
	//need to be reread (e.g., in rsvx-tls)
	char *command[] = { &protocol_file[0], &filename[0],
	  (use_last && getenv("_RSERVR_LAST_FILENAME"))? getenv("_RSERVR_LAST_FILENAME") : NULL, NULL };

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

	//TODO: this will result in awkward behavior if the process stops in
	//the background waiting for terminal access; therefore, maybe a timeout
	//should be used, followed by a 'WIFSTOPPED' check

	while ((error = select(pipes[0] + 1, &read_ready, NULL, NULL, NULL)) < 0 && errno == EINTR);

	if (error < 0)
	  {
	close(pipes[0]);
	kill(new_process, SIGTERM);
	waitpid(new_process, &status, 0x00);
	return RSERVR_PROTOCOL_ERROR;
	  }

	//TODO: this is a harmless race condition, but maybe add a 'nanosleep' or something
	else if (waitpid(new_process, &status, WNOHANG | WEXITED) == new_process && WEXITSTATUS(status) != 0)
	  {
	close(pipes[0]);
	return RSERVR_PROTOCOL_ERROR;
	  }

	if (!use_last)
	  {
	setenv("_RSERVR_LAST_PROTOCOL", protocol.c_str(), 1);
	setenv("_RSERVR_LAST_FILENAME", filename.c_str(), 1);
	  }

	if (pProcess) *pProcess = new_process;
	return pipes[0];
	 }
	}


	else
	{
	int new_file = open(filename.c_str(), O_RDONLY);
	if (new_file == -1) return RSERVR_FILE_ERROR;
	return new_file;
	}
}


char *try_filename(const char *sSpec)
{
	if (!sSpec) return NULL;

	std::string protocol, filename;
	split_protocol(sSpec, protocol, filename);

	if (protocol.size()) return NULL;
	else return strdup(filename.c_str());
}


int close_file(int fFile, pid_t pProcess)
{
	if (fFile >= 0) close(fFile);
	return close_process(pProcess);
}


int close_process(pid_t pProcess)
{
	if (pProcess < 0) return 0;
	int status = 0;
	int error = waitpid(pProcess, &status, WEXITED);
	if (error != pProcess) return (error < 0 && errno == ECHILD)? 0 : RSERVR_PROTOCOL_ERROR;
	return (!WIFEXITED(status) || WEXITSTATUS(status) != 0)? RSERVR_PROTOCOL_ERROR : 0;
}
