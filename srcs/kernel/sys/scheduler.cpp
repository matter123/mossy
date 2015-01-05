/*
    Copyright 2014 Matthew Fosdick

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/
#include <arch.h>
#include <sys/tasks.h>
#include <hal/int.h>
#include <hal/hal.h>
#include <hal/int.h>
#include <hal/console.h>
#include <text_color.h>
namespace kernel {
	//static std::vector<thread_info *> *blocked;
	//static std::vector<thread_info *> *active;
	static void schedule_int(cpu_state *s);
	//static uint64_t current=0;
	static bool init=false;
	//static void *wait_on;
	cpu_state *schedule(cpu_state *s) {
		//if(!init) {
		return s;
		/*}
		thread_info *cur_task=active->at(current);
		cur_task->running=false;
		cur_task->cpu_state = s;

		if(active->at(current)->waiting) {
		    active->erase(current);
		    current--;
		}
		if(wait_on) {
		    thread_info *t=NULL;
		    for(int i=blocked->size(); i>=0; i--) {
		        if(blocked->at(i)->wait_on==wait_on) {
		            blocked->at(i)->wait_on=NULL;
		            active->push_back(blocked->at(i));
		            blocked->at(i)->active=true;
		            if(!t) {
		                t=blocked->at(i);
		                t->waiting=false;
		                t->running=true;
		            }
		            blocked->erase(i);
		        }
		    }
		    wait_on=NULL;
		    if(t!=NULL) {
		        hal::magic_break();
		        return t->cpu_state;
		    } else {
		        thread_info *t2=active->at(current);
		        t2->running=true;
		        return t2->cpu_state;
		    }
		}
		if((++current)%active->size()==0) {
		    current=0;
		}
		thread_info *new_task=active->at(current);
		new_task->running=true;
		return new_task->cpu_state;*/
	}

	/*thread_info *get_self() {
	    return active->at(current);
	}*/

	void add_task(thread_info *t) {
		//  active->push_back(t);
	}

	bool init_scheduler(thread_info *cur_task) {
		//  active=new std::vector<thread_info *>(256);
		cur_task->running=true;
		add_task(cur_task);
		//  blocked=new std::vector<thread_info *>(16);
		hal::register_int(0xC0,&schedule_int, NON_REENTRANT,true);
		init=true;
		return true;
	}

	void schedule_int(cpu_state *s) {
		/*
		if(get_reg(s,0)==0) {
		    active->at(current)->waiting=true;
		    active->at(current)->wait_on=(void *)get_reg(s,1);
		    blocked->push_back(active->at(current));
		}
		if(get_reg(s,0)==1) {
		    hal::magic_break();
		    hal::cout<<std::TC::WHITE<<hal::address<<(void *)get_reg(s,1)<<" ";
		    wait_on=(void *)get_reg(s,1);
		}
		*/
	}
}
