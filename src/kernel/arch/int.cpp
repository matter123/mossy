/*
	Copyright 2015 Matthew Fosdick

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
#include <arch/int.h>
#include <string.h>
extern "C" {
	extern void *exc0; extern void *exc1; extern void *exc2; extern void *exc3;
	extern void *exc4; extern void *exc5; extern void *exc6; extern void *exc7;
	extern void *exc8; extern void *exc9; extern void *exc10; extern void *exc11;
	extern void *exc12; extern void *exc13; extern void *exc14; extern void *exc15;
	extern void *exc16; extern void *exc17; extern void *exc18; extern void *exc19;
	extern void *exc20; extern void *exc21; extern void *exc22; extern void *exc23;
	extern void *exc24; extern void *exc25; extern void *exc26; extern void *exc27;
	extern void *exc28; extern void *exc29; extern void *exc30; extern void *exc31;
	extern void *exc32; extern void *exc33; extern void *exc34; extern void *exc35;
	extern void *exc36; extern void *exc37; extern void *exc38; extern void *exc39;
	extern void *exc40; extern void *exc41; extern void *exc42; extern void *exc43;
	extern void *exc44; extern void *exc45; extern void *exc46; extern void *exc47;
	extern void *exc48; extern void *exc49; extern void *exc50; extern void *exc51;
	extern void *exc52; extern void *exc53; extern void *exc54; extern void *exc55;
	extern void *exc56; extern void *exc57; extern void *exc58; extern void *exc59;
	extern void *exc60; extern void *exc61; extern void *exc62; extern void *exc63;
	extern void *exc64; extern void *exc65; extern void *exc66; extern void *exc67;
	extern void *exc68; extern void *exc69; extern void *exc70; extern void *exc71;
	extern void *exc72; extern void *exc73; extern void *exc74; extern void *exc75;
	extern void *exc76; extern void *exc77; extern void *exc78; extern void *exc79;
	extern void *exc80; extern void *exc81; extern void *exc82; extern void *exc83;
	extern void *exc84; extern void *exc85; extern void *exc86; extern void *exc87;
	extern void *exc88; extern void *exc89; extern void *exc90; extern void *exc91;
	extern void *exc92; extern void *exc93; extern void *exc94; extern void *exc95;
	extern void *exc96; extern void *exc97; extern void *exc98; extern void *exc99;
	extern void *exc100; extern void *exc101; extern void *exc102; extern void *exc103;
	extern void *exc104; extern void *exc105; extern void *exc106; extern void *exc107;
	extern void *exc108; extern void *exc109; extern void *exc110; extern void *exc111;
	extern void *exc112; extern void *exc113; extern void *exc114; extern void *exc115;
	extern void *exc116; extern void *exc117; extern void *exc118; extern void *exc119;
	extern void *exc120; extern void *exc121; extern void *exc122; extern void *exc123;
	extern void *exc124; extern void *exc125; extern void *exc126; extern void *exc127;
	extern void *exc128; extern void *exc129; extern void *exc130; extern void *exc131;
	extern void *exc132; extern void *exc133; extern void *exc134; extern void *exc135;
	extern void *exc136; extern void *exc137; extern void *exc138; extern void *exc139;
	extern void *exc140; extern void *exc141; extern void *exc142; extern void *exc143;
	extern void *exc144; extern void *exc145; extern void *exc146; extern void *exc147;
	extern void *exc148; extern void *exc149; extern void *exc150; extern void *exc151;
	extern void *exc152; extern void *exc153; extern void *exc154; extern void *exc155;
	extern void *exc156; extern void *exc157; extern void *exc158; extern void *exc159;
	extern void *exc160; extern void *exc161; extern void *exc162; extern void *exc163;
	extern void *exc164; extern void *exc165; extern void *exc166; extern void *exc167;
	extern void *exc168; extern void *exc169; extern void *exc170; extern void *exc171;
	extern void *exc172; extern void *exc173; extern void *exc174; extern void *exc175;
	extern void *exc176; extern void *exc177; extern void *exc178; extern void *exc179;
	extern void *exc180; extern void *exc181; extern void *exc182; extern void *exc183;
	extern void *exc184; extern void *exc185; extern void *exc186; extern void *exc187;
	extern void *exc188; extern void *exc189; extern void *exc190; extern void *exc191;
	extern void *exc192; extern void *exc193; extern void *exc194; extern void *exc195;
	extern void *exc196; extern void *exc197; extern void *exc198; extern void *exc199;
	extern void *exc200; extern void *exc201; extern void *exc202; extern void *exc203;
	extern void *exc204; extern void *exc205; extern void *exc206; extern void *exc207;
	extern void *exc208; extern void *exc209; extern void *exc210; extern void *exc211;
	extern void *exc212; extern void *exc213; extern void *exc214; extern void *exc215;
	extern void *exc216; extern void *exc217; extern void *exc218; extern void *exc219;
	extern void *exc220; extern void *exc221; extern void *exc222; extern void *exc223;
	extern void *exc224; extern void *exc225; extern void *exc226; extern void *exc227;
	extern void *exc228; extern void *exc229; extern void *exc230; extern void *exc231;
	extern void *exc232; extern void *exc233; extern void *exc234; extern void *exc235;
	extern void *exc236; extern void *exc237; extern void *exc238; extern void *exc239;
	extern void *exc240; extern void *exc241; extern void *exc242; extern void *exc243;
	extern void *exc244; extern void *exc245; extern void *exc246; extern void *exc247;
	extern void *exc248; extern void *exc249; extern void *exc250; extern void *exc251;
	extern void *exc252; extern void *exc253; extern void *exc254; extern void *exc255;
}
void *exc_arr[256] {
	&exc0, &exc1, &exc2, &exc3, &exc4, &exc5, &exc6, &exc7, &exc8, &exc9, &exc10, &exc11,
	&exc12, &exc13, &exc14, &exc15, &exc16, &exc17, &exc18, &exc19, &exc20, &exc21, &exc22, &exc23,
	&exc24, &exc25, &exc26, &exc27, &exc28, &exc29, &exc30, &exc31, &exc32, &exc33, &exc34, &exc35,
	&exc36, &exc37, &exc38, &exc39, &exc40, &exc41, &exc42, &exc43, &exc44, &exc45, &exc46, &exc47,
	&exc48, &exc49, &exc50, &exc51, &exc52, &exc53, &exc54, &exc55, &exc56, &exc57, &exc58, &exc59,
	&exc60, &exc61, &exc62, &exc63, &exc64, &exc65, &exc66, &exc67, &exc68, &exc69, &exc70, &exc71,
	&exc72, &exc73, &exc74, &exc75, &exc76, &exc77, &exc78, &exc79, &exc80, &exc81, &exc82, &exc83,
	&exc84, &exc85, &exc86, &exc87, &exc88, &exc89, &exc90, &exc91, &exc92, &exc93, &exc94, &exc95,
	&exc96, &exc97, &exc98, &exc99, &exc100, &exc101, &exc102, &exc103, &exc104, &exc105, &exc106,
	&exc107, &exc108, &exc109, &exc110, &exc111, &exc112, &exc113, &exc114, &exc115, &exc116,
	&exc117, &exc118, &exc119, &exc120, &exc121, &exc122, &exc123, &exc124, &exc125, &exc126,
	&exc127, &exc128, &exc129, &exc130, &exc131, &exc132, &exc133, &exc134, &exc135, &exc136,
	&exc137, &exc138, &exc139, &exc140, &exc141, &exc142, &exc143, &exc144, &exc145, &exc146,
	&exc147, &exc148, &exc149, &exc150, &exc151, &exc152, &exc153, &exc154, &exc155, &exc156,
	&exc157, &exc158, &exc159, &exc160, &exc161, &exc162, &exc163, &exc164, &exc165, &exc166,
	&exc167, &exc168, &exc169, &exc170, &exc171, &exc172, &exc173, &exc174, &exc175, &exc176,
	&exc177, &exc178, &exc179, &exc180, &exc181, &exc182, &exc183, &exc184, &exc185, &exc186,
	&exc187, &exc188, &exc189, &exc190, &exc191, &exc192, &exc193, &exc194, &exc195, &exc196,
	&exc197, &exc198, &exc199, &exc200, &exc201, &exc202, &exc203, &exc204, &exc205, &exc206,
	&exc207, &exc208, &exc209, &exc210, &exc211, &exc212, &exc213, &exc214, &exc215, &exc216,
	&exc217, &exc218, &exc219, &exc220, &exc221, &exc222, &exc223, &exc224, &exc225, &exc226,
	&exc227, &exc228, &exc229, &exc230, &exc231, &exc232, &exc233, &exc234, &exc235, &exc236,
	&exc237, &exc238, &exc239, &exc240, &exc241, &exc242, &exc243, &exc244, &exc245, &exc246,
	&exc247, &exc248, &exc249, &exc250, &exc251, &exc252, &exc253, &exc254, &exc255,
};
extern "C" idt_entry IDT[256];
void set_entry(int int_num,idt_entry *entry) {
	memcpy(&IDT[int_num],entry,sizeof(idt_entry));
}
void set_entry(int int_num, void *target,int type,bool userspace) {
	idt_entry entry;
	memset(&entry,0,sizeof(idt_entry));
	entry.type=type;
	entry.present=true;
	entry.dpl=userspace?0x3:0x0;
	entry.target_cs=0x8;
	entry.target_low=(uintptr_t)target&0xFFFF;
	entry.target_mid=((uintptr_t)target>>16)&0xFFFF;
	entry.target_high=((uintptr_t)target>>32)&0xFFFFFFFF;
	set_entry(int_num, &entry);
}
extern "C" uintptr_t jump_table[256];
extern "C" uintptr_t jump_table2[256];
void install_JT1(int int_num,void *target) {
	jump_table[int_num]=(uintptr_t)target;
}
void install_JT2(int int_num,void *target) {
	jump_table2[int_num]=(uintptr_t)target;
}
extern "C" int lidtr();
void install_interrupts() {
	for(int i=0;i<256;i++) {
		set_entry(i,exc_arr[i],INT_INT_GATE,false);
	}
	lidtr();
}
def_interrupt *handles[256];
void default_handler(cpu_state * cpu,void *sse,bool *use) {
	if(handles[cpu->int_num]!=nullptr) {
		def_interrupt *handle=handles[cpu->int_num];
		bool resolved = handle->default_interrupt(cpu,sse,use);
		if(resolved)return;
		while(handle->next!=nullptr) {
			handle=handle->next;
			resolved = handle->default_interrupt(cpu,sse,use);
			if(resolved)return;
		}
	}
}
extern "C" void def_handler();
void install_single_interrupt(int int_num,void (*handler)(cpu_state *,void *sse_save,bool *in_use)) {
	install_JT1(int_num,(void *)def_handler);
	install_JT2(int_num,(void *)handler);
}
void install_interrupt(int int_num,def_interrupt *new_handle) {
	install_JT1(int_num,(void *)def_handler);
	install_JT2(int_num,(void *)default_handler);
	def_interrupt *handle = handles[int_num];
	if(handle==nullptr) {
		handles[int_num]=new_handle;
		return;
	}
	while(handle->next!=nullptr)handle=handle->next;
	handle->next=new_handle;
}
