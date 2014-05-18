/*
    Copyright 2013 Matthew Fosdick

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
#ifdef X86_64

extern "C" void *exc0; extern "C" void *exc1; extern "C" void *exc2; extern "C" void *exc3;
extern "C" void *exc4; extern "C" void *exc5; extern "C" void *exc6; extern "C" void *exc7;
extern "C" void *exc8; extern "C" void *exc9; extern "C" void *exc10; extern "C" void *exc11;
extern "C" void *exc12; extern "C" void *exc13; extern "C" void *exc14; extern "C" void *exc15;
extern "C" void *exc16; extern "C" void *exc17; extern "C" void *exc18; extern "C" void *exc19;
extern "C" void *exc20; extern "C" void *exc21; extern "C" void *exc22; extern "C" void *exc23;
extern "C" void *exc24; extern "C" void *exc25; extern "C" void *exc26; extern "C" void *exc27;
extern "C" void *exc28; extern "C" void *exc29; extern "C" void *exc30; extern "C" void *exc31;
extern "C" void *exc32; extern "C" void *exc33; extern "C" void *exc34; extern "C" void *exc35;
extern "C" void *exc36; extern "C" void *exc37; extern "C" void *exc38; extern "C" void *exc39;
extern "C" void *exc40; extern "C" void *exc41; extern "C" void *exc42; extern "C" void *exc43;
extern "C" void *exc44; extern "C" void *exc45; extern "C" void *exc46; extern "C" void *exc47;
extern "C" void *exc48; extern "C" void *exc49; extern "C" void *exc50; extern "C" void *exc51;
extern "C" void *exc52; extern "C" void *exc53; extern "C" void *exc54; extern "C" void *exc55;
extern "C" void *exc56; extern "C" void *exc57; extern "C" void *exc58; extern "C" void *exc59;
extern "C" void *exc60; extern "C" void *exc61; extern "C" void *exc62; extern "C" void *exc63;
extern "C" void *exc64; extern "C" void *exc65; extern "C" void *exc66; extern "C" void *exc67;
extern "C" void *exc68; extern "C" void *exc69; extern "C" void *exc70; extern "C" void *exc71;
extern "C" void *exc72; extern "C" void *exc73; extern "C" void *exc74; extern "C" void *exc75;
extern "C" void *exc76; extern "C" void *exc77; extern "C" void *exc78; extern "C" void *exc79;
extern "C" void *exc80; extern "C" void *exc81; extern "C" void *exc82; extern "C" void *exc83;
extern "C" void *exc84; extern "C" void *exc85; extern "C" void *exc86; extern "C" void *exc87;
extern "C" void *exc88; extern "C" void *exc89; extern "C" void *exc90; extern "C" void *exc91;
extern "C" void *exc92; extern "C" void *exc93; extern "C" void *exc94; extern "C" void *exc95;
extern "C" void *exc96; extern "C" void *exc97; extern "C" void *exc98; extern "C" void *exc99;
extern "C" void *exc100; extern "C" void *exc101; extern "C" void *exc102; extern "C" void *exc103;
extern "C" void *exc104; extern "C" void *exc105; extern "C" void *exc106; extern "C" void *exc107;
extern "C" void *exc108; extern "C" void *exc109; extern "C" void *exc110; extern "C" void *exc111;
extern "C" void *exc112; extern "C" void *exc113; extern "C" void *exc114; extern "C" void *exc115;
extern "C" void *exc116; extern "C" void *exc117; extern "C" void *exc118; extern "C" void *exc119;
extern "C" void *exc120; extern "C" void *exc121; extern "C" void *exc122; extern "C" void *exc123;
extern "C" void *exc124; extern "C" void *exc125; extern "C" void *exc126; extern "C" void *exc127;

void *exc_arr[256] {
	&exc0, &exc1, &exc2, &exc3, &exc4, &exc5, &exc6, &exc7, &exc8, &exc1, &exc9, &exc10, &exc11,
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
	&exc127,
};
#endif
