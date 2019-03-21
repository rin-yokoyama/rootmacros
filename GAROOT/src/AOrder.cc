#include"GAROOT/src/AOrder.h"

AOrder::AOrder(int Anum)
{
  n = Anum;
  Order = new int[n];
}

AOrder::~AOrder(void)
{
}

void AOrder::Ascend(double* Array)
{
  int* Picked = new int[n];
  double* fArray = new double[n];
  for(int i=0; i<n; i++){
    Picked[i] = 0;
    fArray[i] = Array[i];
    Order[i] = 0;
  }
  for(int i=0; i<n; i++){
    double min=0;
    int i_min=0;
    for(int j=0; j<n; j++){
      if(!Picked[j]){
        min = fArray[j];
        i_min = j;
      }
    }
    for(int j=0; j<n; j++){
      if(!Picked[j] && (fArray[j]<min)){
	min = fArray[j];
	i_min = j;
      }
    }
    Order[i] = i_min;
    Picked[i_min] = 1;
    Array[i] = min;
  }
  return ;
}

void AOrder::Descend(double* Array)
{
  int* Picked = new int[n];
  double* fArray = new double[n];
  for(int i=0; i<n; i++){
    Picked[i] = 0;
    fArray[i] = Array[i];
    Order[i] = 0;
  }
  for(int i=0; i<n; i++){
    double max=0;
    int i_max=0;
    for(int j=0; j<n; j++){
      if(!Picked[j]){
	max = fArray[0];
	i_max = 0;
      }
    }
    for(int j=0; j<n; j++){
      if(!Picked[j] && (fArray[j]>max)){
	max = fArray[j];
	i_max = j;
      }
    }
    Order[i] = i_max;
    Picked[i_max] = 1;
    Array[i] = max;
  }
  return ;
}

void AOrder::OrderSame(double* Array)
{
  double* fArray = new double[n];
  for(int i=0; i<n; i++)
    fArray[i] = Array[i];
  for(int i=0; i<n; i++)
    Array[i] = fArray[Order[i]];
  return;
}
