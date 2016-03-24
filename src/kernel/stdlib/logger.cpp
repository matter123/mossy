#include <logger.h>
#include <hal/multiboot.h>
#include <string.h>
#include <numconv.h>
#include <stdarg.h>
#include <stdio.h>
#include <vga_text.h>
static int curLogLevel=1;
const char *levels[] {
	"[DEBUG]",
	"[INFO ]",
	"[WARN ]",
	"[ERROR]",
};
void logger_init() {
	hal::multiboot_command *cmd=nullptr;
	for(int i=0; i<hal::get_tag_count(); i++) {
		hal::multiboot_tag *tag=hal::get_tag(i);
		if(tag->type!=hal::t_cmd_line) {
			continue;
		}
		cmd=reinterpret_cast<hal::multiboot_command *>(tag);
		if(strlen(cmd->string)) {
			int level=std::strtonum(cmd->string,1);
			setLogLevel((LogLevel)level);
		}
	}
	Log(LOG_INFO,"[LOGGER]","Log level is %s",levels[curLogLevel]);
}
void setLogLevel(LogLevel level) {
	curLogLevel=(int)level;
}

void Log(LogLevel level,const char *service, const char *fmt,...) {
	if(level<curLogLevel)return;
	va_list list,l2;
	va_start(list,fmt);
	va_copy(l2,list);
	int len=vsnprintf(nullptr,0,fmt,l2);
	va_end(l2);
	char* buf=(char *)__alloca(len+1);
	vsnprintf(buf,len+1,fmt,list);
	va_end(list);
	printf("%s %s %s\n",levels[level],service,buf);
	if(level==LOG_ERROR) {
		asm("hlt");
	}
}
