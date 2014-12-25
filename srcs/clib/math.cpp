double sin(double angle) {
	double answer;
	asm("fsin" : "=t"(answer) : "0"(angle));
	return answer;
}

double cos(double angle) {
	double answer;
	asm("fcos" : "=t"(answer) : "0"(angle));
	return answer;
}

double sqrt(double angle) {
	double answer;
	asm("fsqrt" : "=t"(answer) : "0"(angle));
	return answer;
}