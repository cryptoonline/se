//
// helper.cpp
// BlindStorage
//

void gen_random(char* s, const int len){
	
	static const char alphanumsym[] = 
		"`~!@#$%^&*()-_=+[{]}\\|;:\'\",<.>/\?"
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	for(int i = 0; i < len; ++i) {
		s[i] = alphanumsym[rand() % (sizeof(alphanumsym) - 1);
	}

//	s[len] = 0;
}
