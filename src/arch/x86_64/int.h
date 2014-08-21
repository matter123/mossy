/*
    Copyright 2013-2014 Matthew Fosdick

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
extern "C" void *exc128; extern "C" void *exc129; extern "C" void *exc130; extern "C" void *exc131;
extern "C" void *exc132; extern "C" void *exc133; extern "C" void *exc134; extern "C" void *exc135;
extern "C" void *exc136; extern "C" void *exc137; extern "C" void *exc138; extern "C" void *exc139;
extern "C" void *exc140; extern "C" void *exc141; extern "C" void *exc142; extern "C" void *exc143;
extern "C" void *exc144; extern "C" void *exc145; extern "C" void *exc146; extern "C" void *exc147;
extern "C" void *exc148; extern "C" void *exc149; extern "C" void *exc150; extern "C" void *exc151;
extern "C" void *exc152; extern "C" void *exc153; extern "C" void *exc154; extern "C" void *exc155;
extern "C" void *exc156; extern "C" void *exc157; extern "C" void *exc158; extern "C" void *exc159;
extern "C" void *exc160; extern "C" void *exc161; extern "C" void *exc162; extern "C" void *exc163;
extern "C" void *exc164; extern "C" void *exc165; extern "C" void *exc166; extern "C" void *exc167;
extern "C" void *exc168; extern "C" void *exc169; extern "C" void *exc170; extern "C" void *exc171;
extern "C" void *exc172; extern "C" void *exc173; extern "C" void *exc174; extern "C" void *exc175;
extern "C" void *exc176; extern "C" void *exc177; extern "C" void *exc178; extern "C" void *exc179;
extern "C" void *exc180; extern "C" void *exc181; extern "C" void *exc182; extern "C" void *exc183;
extern "C" void *exc184; extern "C" void *exc185; extern "C" void *exc186; extern "C" void *exc187;
extern "C" void *exc188; extern "C" void *exc189; extern "C" void *exc190; extern "C" void *exc191;
extern "C" void *exc192; extern "C" void *exc193; extern "C" void *exc194; extern "C" void *exc195;
extern "C" void *exc196; extern "C" void *exc197; extern "C" void *exc198; extern "C" void *exc199;
extern "C" void *exc200; extern "C" void *exc201; extern "C" void *exc202; extern "C" void *exc203;
extern "C" void *exc204; extern "C" void *exc205; extern "C" void *exc206; extern "C" void *exc207;
extern "C" void *exc208; extern "C" void *exc209; extern "C" void *exc210; extern "C" void *exc211;
extern "C" void *exc212; extern "C" void *exc213; extern "C" void *exc214; extern "C" void *exc215;
extern "C" void *exc216; extern "C" void *exc217; extern "C" void *exc218; extern "C" void *exc219;
extern "C" void *exc220; extern "C" void *exc221; extern "C" void *exc222; extern "C" void *exc223;
extern "C" void *exc224; extern "C" void *exc225; extern "C" void *exc226; extern "C" void *exc227;
extern "C" void *exc228; extern "C" void *exc229; extern "C" void *exc230; extern "C" void *exc231;
extern "C" void *exc232; extern "C" void *exc233; extern "C" void *exc234; extern "C" void *exc235;
extern "C" void *exc236; extern "C" void *exc237; extern "C" void *exc238; extern "C" void *exc239;
extern "C" void *exc240; extern "C" void *exc241; extern "C" void *exc242; extern "C" void *exc243;
extern "C" void *exc244; extern "C" void *exc245; extern "C" void *exc246; extern "C" void *exc247;
extern "C" void *exc248; extern "C" void *exc249; extern "C" void *exc250; extern "C" void *exc251;
extern "C" void *exc252; extern "C" void *exc253; extern "C" void *exc254; extern "C" void *exc255;

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
#endif
