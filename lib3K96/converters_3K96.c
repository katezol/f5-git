#include "l3K96.h"
#include "l3K96_f.h"
#include <math.h>


void conv_read_3K96_IP11_OneTarget(m3K96_Ip11Tgt_t *t_ip11, Target_IP11_3k96_tgt    *t_data11)
{
	int i;
	memset(t_data11,0,sizeof(Target_IP11_3k96_tgt));
	t_data11->nRlk        = t_ip11->nRlk;
	t_data11->nSigma        = t_ip11->nSigma;
	t_data11->prefixSigma        = t_ip11->prefixSigma;
	t_data11->nX        = t_ip11->nX;
	t_data11->nOpt        = t_ip11->nOpt;
	t_data11->nEco        = t_ip11->nEco;
	
	t_data11->nF		  = t_ip11->nF;

	t_data11->typeSource   = t_ip11->TIP;
	t_data11->bRefuse	  = t_ip11->OtkC;
	t_data11->pd			  = t_ip11->PD;
	t_data11->stateChannel = t_ip11->PSK;

	t_data11->T1   = data2use(t_ip11->Tvstr1  ,  0.1,  0);
	t_data11->x1_f = data2use(t_ip11->X1      ,    5, 15);
	t_data11->y1_f = data2use(t_ip11->Y1      ,  	 5, 15);
	t_data11->z1_f = data2use(t_ip11->Z1      ,    1,  0);

	t_data11->T2   = data2use(t_ip11->Tvstr2  ,  0.1,  0);
	t_data11->x2_f = data2use(t_ip11->X2      ,    5, 15);
	t_data11->y2_f = data2use(t_ip11->Y2      ,  	 5, 15);
	t_data11->z2_f = data2use(t_ip11->Z2      ,    1,  0);
}
