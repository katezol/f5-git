#ifndef CONV_H_
#define CONV_H_

#ifdef __cplusplus
extern "C"{
#endif


unsigned long use2data(float value, float step, int sign);
float data2use(unsigned long data, float step, int sign);
float data2useDouble(unsigned long data, double step, int sign);
unsigned long use2dataDouble(double value, double step, int sign);

#ifdef __cplusplus
}
#endif


#endif /*CONV_H_*/
