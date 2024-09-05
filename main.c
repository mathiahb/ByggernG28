int main()
{
  int *PORT_A = (int *)0x1B;
  int *DDR_A = (int *)0x1A;
  int *PIN_A = (int *)0x19;

  int PIN_0 = 0;

  *DDR_A = (1 << PIN_0);

  int on = 1;

  while (1)
  {
    *PORT_A = (on << PIN_0);
    int sleep = 1000000;

    while (--sleep)
    {
    }

    on = !on;
  }

  return 0;
}