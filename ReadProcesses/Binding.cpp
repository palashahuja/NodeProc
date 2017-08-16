#include <node.h>
#include <v8.h>
#include "ReadProcesses.h"


void MethodCall(const v8::FunctionCallbackInfo<v8::Value> &args){ 
	v8::Isolate *current_isolate = args.GetIsolate();
	if (args[0]->IsString()) {

	}
	

}



