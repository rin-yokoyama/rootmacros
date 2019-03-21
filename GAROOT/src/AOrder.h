#ifndef GAROOT_AORDER_H
#define GAROOT_AORDER_H

#include<iostream>

//class for ordering an array
class AOrder
{
 protected:
  int n;
  int* Order;
 public:
  AOrder(int Anum);
  ~AOrder(void);
  void Ascend(double* Array);
  void Descend(double* Array);
  void OrderSame(double* Array);
};

#endif
