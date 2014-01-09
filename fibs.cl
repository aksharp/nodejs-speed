int fib(int a) {
  int b = -1;
  int c = 1;
  int d;
  int i;
  for (i=0; i<=a; i++){
      d = c + b;
      b = c;
      c = d;
  }
  return c;
}

__kernel void fibs(__global const int *a,
   __global int *b) {
    int gid = get_global_id(0);
    b[gid] = fib(a[gid]);
}
