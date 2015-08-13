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
#include <sys/synchronization.h>
#include <sys/syscall.h>
namespace kernel {
	semaphore::semaphore(int fc) {
		this->free_count=fc;
		this->s=new spinlock();
	}
	semaphore::~semaphore() {
		delete s;
	}
	void semaphore::signal() {
		uintptr_t flag=s->acquire();
		free_count++;
		s->release(flag);
		//yield(SCHEDULE_TYPE::RELEASE,this)
		if(free_count==1) {
			syscall_2_0(0xC0,1,reinterpret_cast<uintreg_t>(this));
		}
	}
	void semaphore::wait() {
		uintptr_t flag=s->acquire();
		if(free_count) {
			free_count--;
			s->release(flag);
			return;
		}
		s->release(flag);
		//yield(SCHEDULE_TYPE::BLOCK,this)
		syscall_2_0(0xC0,0,reinterpret_cast<uintreg_t>(this));
	}
	int semaphore::get_free() {
		return free_count;
	}
}
