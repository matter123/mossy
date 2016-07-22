/*
	Copyright 2016 Matthew Fosdick

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
#ifdef TTY_H
#define TTY_H
#define ESC_STR "\x1b"
#define CSI_SEQ_STR ESC_STR "["
#define CSI_CUR_UP(num) CSI_SEQ_STR #num "A"
#define CSI_CUR_DOWN(num) CSI_SEQ_STR #num "B"
#define CSI_CUR_FWD(num) CSI_SEQ_STR #num "C"
#define CSI_CUR_BACK(num) CSI_SEQ_STR #num "D"
#define CSI_CUR_NEXTLN(num) CSI_SEQ_STR #num "E"
#define CSI_CUR_PREVLN(num) CSI_SEQ_STR #num "F"
#define CSI_CUR_HORZ(num) CSI_SEQ_STR #num "G"
#define CSI_CUR_POS(row, col) CSI_SEQ_STR #row ";" #col "H"
#define CSI_CLEAR(mode) CSI_SEQ_STR #mode "J"
#define CSI_CLEAR_LINE(mode) CSI_SEQ_STR #mode "K"
#define CSI_SCROLL_UP(lines) CSI_SEQ_STR #lines "S"
#define CSI_SCROLL_DOWN(lines) CSI_SEQ_STR #lines "T"
#define CSI_SGR(code) CSI_SEQ_STR code "m"
#define CSI_SAVE_CUR_POS CSI_SEQ_STR code "s"
#define CSI_RSTR_CUR_POS CSI_SEQ_STR code "u"
#define CSI_HIDE_CUR CSI_SEQ_STR code "?25l"
#define CSI_SHOW_CUR CSI_SEQ_STR code "?25h"


#define SGR_RESET "0"
#define SGR_BOLD "1"
// italic is  the same as inverse
#define SGR_ITALIC "7"
#define SGR_INVERSE "7"
#define SGR_CONCEAL "8"
#define SGR_BOLD_OFF "22"
#define SGR_NORMAL "27"
#define SGR_REVEAL "28"
#define SGR_FORGROUND "3"
#define SGR_BACKGROUND "4"

#define SGR_COLOR_BLACK "0"
#define SGR_COLOR_RED "1"
#define SGR_COLOR_GREEN "2"
#define SGR_COLOR_YELLOW "3"
#define SGR_COLOR_BLUE "4"
#define SGR_COLOR_MAGENTA "5"
#define SGR_COLOR_CYAN "6"
#define SGR_COLOR_WHITE "7"
#define SGR_COLOR_DEFAULT "9"

#define CSI_CLEAR_FOWARD 0
#define CSI_CLEAR_BACKWARD 1
#define CSI_CLEAR_ALL 2
#endif
