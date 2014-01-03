#define BULDING_NODE_EXTENSION
#include <node.h>

using namespace v8;

int f(int n) {
  if (n <= 1) {
    return n;
  } else {
    return f(n - 1) + f(n - 2);
  }
}

int get_random_number(int maxValue) {
  return rand()%maxValue+1;
}

Handle<Value> ArrToFib(const Arguments& args) {
  HandleScope scope;

  Local<Array> arrIn = Local<Array>::Cast(args[0]);
  int length = arrIn->Length();
  Local<Array> array = Array::New(length);
  int i=0;
  for(i=0;i<length;i++) {
    array->Set(i, Integer::New(f(arrIn->Get(i)->Int32Value())));  
  }

  scope.Close(array);
}

Handle<Value> Fib(const Arguments& args) {
  HandleScope scope;

  Local<Number> num = Number::New(args[0]->NumberValue());
  int res = f(num->Int32Value());

  return scope.Close(Number::New(res));
}

Handle<Value> RndArr(const Arguments& args) {
  HandleScope scope;

  Handle<Array> array = Array::New(10);
  int j=0;
  for(j=0;j<10;j++){
    array->Set(j, Integer::New(get_random_number(40)));
  }

  return scope.Close(array);
}

void Init(Handle<Object> exports) {
  exports->Set(String::NewSymbol("Fib"),
      FunctionTemplate::New(Fib)->GetFunction());
  exports->Set(String::NewSymbol("RndArr"),
      FunctionTemplate::New(RndArr)->GetFunction());
  exports->Set(String::NewSymbol("ArrToFib"),
      FunctionTemplate::New(ArrToFib)->GetFunction());
}

NODE_MODULE(fib, Init)
