#include <hal/commandline.h>
#include <hal/multiboot.h>
#include <hal/workspace.h>
#include <string.h>
#include <ctype.h>

static void parse(char *cmd) RUN_ONCE;
static char **args=nullptr;
static int arg_count=0;

void command_line_init() {
	hal::multiboot_command *cmd=nullptr;
	for(int i=0; i<hal::get_tag_count(); i++) {
		hal::multiboot_tag *tag=hal::get_tag(i);
		if(tag->type!=hal::t_cmd_line) {
			continue;
		}
		cmd=reinterpret_cast<hal::multiboot_command *>(tag);
		break;
	}
	parse(cmd->string);
}

void parse(char *cmd) {
	int count=0;
	char *s=cmd;
	while(*s) {
		while(isspace(*s))s++;
		if(*s=='\0') {
			break;
		}
		count++;
		while(*s&&!isspace(*s))s++;
	}
	s=cmd;
	args=(char **)hal::w_malloc(sizeof(char *)*count);
	count=0;
	while(*s) {
		while(isspace(*s)) {
			*s='\0';
			s++;
		}
		if(*s) {
			args[count++]=s;
			while(*s&&!isspace(*s))s++;
		}
	}
	arg_count=count;
}

char *get_arg(const char *arg) {
	for(int i=0;i<arg_count;i++) {
		if(!strncmp(arg, args[i],strlen(arg))) {
			char *split=strpbrk(args[i],"=");
			if(split) {
				return ++split;
			}
			return args[i];
		}
	}
	return nullptr;
}
