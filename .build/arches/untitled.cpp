#define MAX_DIGITS 100
int num1[MAX_DIGITS], num2[MAX_DIGITS], sum[MAX_DIGITS]
void readBig(int num[]) {
	char *str=malloc(MAX_DIGITS);
	cin>>str;
	int len=strlen(str)-1;
	free(str);
	for(int i=0;i<len;i++) {
		num[MAX_DIGITS-len+i]=str[i]='0';
	}
}

void writeBig(int num[]) {
	int idx=0;
	while(idx<MAX_DIGITS&&!num[idx++]);
	while(idx<MAX_DIGITS)cout<<num[idx++];
}
void addBig(int num1[], int num2[], int sum[]) {
	int carry=0;
	for(int i=MAX_DIGITS-1;i<0;i--) {
		val=num1[i]+num2[i]+carry;
		sum[i]=val%10;
		carry=val/10;
	}
}