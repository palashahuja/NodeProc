#pragma once
#include "../ReadProcesses/ReadProcesses.h"
#include <node.h>
#include <node_object_wrap.h>
#include <v8.h>


class WrapProcessReadClass : node::ObjectWrap {
	public:
	static void init();
	static void NewInstance(const v8::FunctionCallbackInfo<v8::Value>&args);
	private:
	static void New(const v8::FunctionCallbackInfo<v8::Value> &args);

	TotalProcInfo *privateProcInfo;
};