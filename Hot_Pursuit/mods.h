template <typename T>
__ALWAYS_INLINE T (*call(unsigned int address, unsigned int toc = TOC))(...) {
	volatile opd_s opd = {address,toc};
	T(*func)(...) = (T(*)(...))&opd;
	return func;
}

struct SingleArgument {//size of 0x10
	//unsigned int argumentValue;
	//unsigned int unk1;
	double argumentValue;
	unsigned int shouldBe3;
	unsigned int unk3;
	

	
	void setArgument(int argument) {
		argumentValue = (double)argument;
	}

	unsigned int getArgument() {
		return (unsigned int)argumentValue;
	}

};

#define argsPoolSize 20
SingleArgument argsPool[argsPoolSize];//last one is return value

struct ArgumentStructure {
	unsigned int unk1;//0x0
	unsigned int unk2;//0x4
	unsigned int returnLocation;//0x8
	SingleArgument *startArgumentLocation;//0xC
	
	
	int getArgCount() {
		return ((unsigned int)startArgumentLocation-returnLocation) >> 4;  //divide by 0x10
	}
	
	unsigned int getReturnValue() {
		SingleArgument *ret = (SingleArgument *)(returnLocation-0x10);
		return ret->getArgument();
	}
	
	
};

unsigned int callNative(int func, int argCount = 0, unsigned int a = 0,unsigned int b = 0,unsigned int c = 0,unsigned int d = 0,unsigned int e = 0,unsigned int f = 0) {
	ArgumentStructure args;
	args.unk1 = 0;//this is a pointer to the next call but idk if it's necessary
	args.unk2 = 0;//0 should work fine
	args.startArgumentLocation = (SingleArgument *)(&(argsPool[0]));
	args.returnLocation = (unsigned int)(&(argsPool[argCount]));//so if argCount was 1, this would be + 0x10 off
	printf("Here...\n");
	for (int i = 0; i < argsPoolSize; i++) {
		argsPool[i].argumentValue = 0;
		argsPool[i].shouldBe3 = 3;//has to be 3 or getArg will not work correctly
		argsPool[i].unk3 = 0;//idk
	}
	
	argsPool[0].argumentValue = a;
	argsPool[1].argumentValue = b;
	argsPool[2].argumentValue = c;
	argsPool[3].argumentValue = d;
	argsPool[4].argumentValue = e;
	argsPool[5].argumentValue = f;
	
	printf("About to call, args at 0x%X\n",&args);
	call<int>(func)(&args);
	printf("Called!\n");
	
	return args.getReturnValue();
}



void dumpNatives(unsigned int table) {
	unsigned int currentSize = *(int*)(table+0x22D0);
	for (int i = 0; i < currentSize; i++) {
		unsigned int r9 = i << 3;
		r9 = r9 + 0x1290;
		unsigned int addr = table + r9;

		char *name = (char*)*(int*)(addr);
		unsigned int addressFound = *(int*)*(int*)(addr+4);
		sleep(10);
		printf("int %s(int a, int b, int c) { return callNative(0x%X,3,a,b,c); }\n",name,addressFound);

	}
}

void dumpNativeTables() {
	printf("Dumping table:\n\n\n");
	dumpNatives(*(int*)0xD77E60 + 0x46B10);
	printf("\n\n\nDump done..\n");
}

bool option;
//*(bool*)0x10020000 = 0x01;



/*void monitorbuttons() {
	uint32_t button1, button2;

	PadRead(&button1, &button2);//use in thread

	if (button2 & CELL_PAD_CTRL_TRIANGLE)
	{
		int val = 0x01000000;
		//*(int*)0x10020000 = 0x01;
		memcpy((void*)0x10020000, &val, 4);// crashed thread..
		printf("Button pressed: %s\n", CELL_PAD_CTRL_TRIANGLE);

	}

}*/