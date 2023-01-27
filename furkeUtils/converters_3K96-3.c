#include "converters_3K96-3.h"
#include "unistd.h"
#include "lconv.h"
#include "lRLI.h"
#include "l3K96.h"




void conv_write_ip2_3K96(
		        const RlsTarget * rlsTarget,// источник данных
		        m3K96_Ip2Tgt_t *ip2) // указатель на ip2 в map3k96
{
	unsigned int tp;	
	if (rlsTarget->nSource ==0) return;
	if (rlsTarget->bPassive ==1) return;
		ip2->nRlk = rlsTarget->nSource;
		switch(rlsTarget->own)
		{
			case owOur:		ip2->GO = 1;		break; // svoi
			case owEnemy:	ip2->GO = 2;		break; // chugoi
			default:
			case owUnknown:	ip2->GO = 0;		break; // ?
		}
		ip2->ED  = rlsTarget->bLoss;
		ip2->PNS  = rlsTarget->bLoss;
		ip2->PSC = rlsTarget->bAutoEscort; // 
		ip2->OPr = (rlsTarget->b3k==0); // 
		ip2->PP  = 0;//rlsTarget->bPassive;
		ip2->PMPC = rlsTarget->bSlow;
		switch(rlsTarget->objType)
		{
			case otLft: 
			{	
					if (rlsTarget->z <=150.0) ip2->typeTarget = 3; // nlc
					else  ip2->typeTarget = 2;	// vc
			}		
			break; 
			case otSurface: 	ip2->typeTarget = 1;	break; // nc
			case otShip: 		ip2->typeTarget = 1;	break; // nc
			case otHelicopter: 	ip2->typeTarget = 7;	break; // vt
			case otPn: 			ip2->typeTarget = 2;	break; // pn не будет vc
			case otFalseTarget: ip2->typeTarget = 2;	break; // lc не будет vc
			case otPkr:			ip2->typeTarget = 5;	break; // pkr
			case otPrr:			ip2->typeTarget = 6;		break; // prr
			default:
				if (rlsTarget->objType == 0 ){ip2->typeTarget = 2; break;}
				if(isSurface(rlsTarget->objType)){ip2->typeTarget = 1;	break;} // nc
				if(isAir(rlsTarget->objType))	{ip2->typeTarget = 2;	break;} // vc
				ip2->typeTarget = 2;
				break;
		}
		if (isSurface(rlsTarget->objType))ip2->EPR = 3; // >20m2
			else if (isRocket(rlsTarget->objType))ip2->EPR = 1; // <1m2
				else ip2->EPR = 2; // <1m2

		if(rlsTarget->bPassive)
		{
			//ip2->X  = rlsTarget->x*2.0*M_PI/pow(2,16);
			ip2->Y  = 0;
			//ip2->Z  = rlsTarget->z*M_PI/pow(2,15);
			ip2->X  = use2data(rlsTarget->x,2*M_PI/pow(2,16), 0);
			ip2->Y  = 0;
			ip2->Z  = use2data(rlsTarget->z,M_PI/pow(2,16), 15);
			ip2->Vx = use2data(rlsTarget->vx,5,9);
			ip2->Vz = use2data(rlsTarget->vz,5,9);
//			ip2->Vx = rlsTarget->vx*1000.0/ 5.0;
			ip2->Vy = 0;
//			ip2->Vz = rlsTarget->vz*1000.0/ 5.0;
		}
		else
		{

			ip2->X = (signed short)(rlsTarget->x/5.0);
			ip2->Z = (unsigned short)rlsTarget->z;
			ip2->Y = (signed short)(rlsTarget->y/5.0);

			ip2->Vx = use2data(rlsTarget->vx,5,9);
			ip2->Vz = use2data(rlsTarget->vz,5,9);
			ip2->Vy = use2data(rlsTarget->vy,5,9);
			
			ip2->tVx = use2data(rlsTarget->skoVx,5,0);
			ip2->tVy = use2data(rlsTarget->skoVy,5,0);
			ip2->tVz = 0;
			ip2->tX  = use2data(rlsTarget->skoX ,  1.0,  0);
			ip2->tY  = use2data(rlsTarget->skoY ,  1.0,  0);
			ip2->tZ  = use2data(rlsTarget->skoZ ,  1.0,  0);
		}
	//	printf("RLI Tsev=%d %d %d\n",ip1[i].TSev_1,ip1[i].TSev_2,data1[i]->TSev);
		// Здесь СКО нужны
		tp = (unsigned int)((rlsTarget->t3k96_ms)  % 86400000);
		ip2->TizmE_hi  = (unsigned short)(0xFFFF&(tp>>16));
		ip2->TizmE_low = (unsigned short)(tp&0xFFFF);
}// write

void conv_write_ip3_3K96(
//		        const Plan_CR * plan,// источник данных
		        unsigned int stateTarget, // из плана
		        m3K96_Ip3Tgt_t *cu, // указатель на ip3 в map3k96
		        int nFormular,
		        int nEco,
		        RlsTarget * rlsTarget,
		        TargetX_f * targetsX,
		        fSfera_Ip5Tgt * ip5Tgt_Sfera
		        )  // номер канала
{
	unsigned int tp;	
	float workX,workY,workZ,worktZ;
//	if ((plan->nRlsSent ==0) & (plan->nRlsSent))return;
	if (targetsX != NULL) if (targetsX->nn !=0) cu->TIP = 5; else;
	else if (rlsTarget !=0) if (rlsTarget->nSource!=0) cu->TIP = 4; else;
	else if (ip5Tgt_Sfera !=0) if (ip5Tgt_Sfera->nSfera!=0) cu->TIP = 2; else
			  return;
		//4-рлк 2-сф 5-х
	if (rlsTarget != NULL) cu->nRlk = rlsTarget->nSource; else  cu->nRlk = 0;
	if (targetsX != NULL) cu->nX = targetsX->nn; else  cu->nX = 0;
	if (ip5Tgt_Sfera != NULL) cu->nOpt = ip5Tgt_Sfera->nSfera; else  cu->nOpt = 0;
	cu->nEco = nEco;
	cu->nF = nFormular;
	
	cu->POC= (stateTarget == 1)+ 2*(stateTarget == 2);
	cu->OTK_CP= 0;
	if (cu->TIP ==4)
	{	
		if (rlsTarget == NULL) return;
		if (rlsTarget->bPassive ==1) return;
		cu->PMPC=rlsTarget->bSlow;
		cu->DoubleAcc=rlsTarget->bAutoEscort;
		cu->OPr=(rlsTarget->b3k==0);
		cu->ED=rlsTarget->bLoss;
		cu->PNS=rlsTarget->bLoss;
		
		if (isSurface(rlsTarget->objType))cu->EPR = 3; // >20m2
			else if (isRocket(rlsTarget->objType)) cu->EPR = 1; // <1m2
				else cu->EPR = 2; // <1m2
		switch(rlsTarget->objType)
		{
			case otLft: 
			{	
					if (rlsTarget->z <=150.0) cu->typeTarget = 3; // nlc
					else  cu->typeTarget = 2;	// vc
			}		
			break; 
			case otSurface: 	cu->typeTarget = 1;	break; // nc
			case otShip: 	cu->typeTarget = 1;	break; // nc
			case otHelicopter: 	cu->typeTarget = 7;	break; // vt
			case otPn: 			cu->typeTarget = 2;	break; // pn не будет, ВЦ
			case otFalseTarget: cu->typeTarget = 2;	break; // lc не будет, ВЦ
			case otPkr:			cu->typeTarget = 5;	break; // pkr
			case otPrr:			cu->typeTarget = 6;		break; // prr
			default:
				if (rlsTarget->objType == 0 ){cu->typeTarget = 2; break;}
				if(isSurface(rlsTarget->objType)){cu->typeTarget = 1;	break;} // nc
				if(isAir(rlsTarget->objType))	{cu->typeTarget = 2;	break;} // vc
				cu->typeTarget = 2;
				break;
		}
			if(rlsTarget->bPassive)
			{
				cu->X  = rlsTarget->x*2.0*M_PI/pow(2,16);
				cu->Y  = 0;
				cu->Z  = rlsTarget->z*M_PI/pow(2,15);
				cu->Vx = rlsTarget->vx*1000.0/ 5.0;
				cu->Vy = 0;
				cu->Vz = rlsTarget->vz*1000.0/ 5.0;
			}
			else
			{
				cu->X  = (signed short)(rlsTarget->x/ 5.0);
				cu->Y  = (signed short)(rlsTarget->y / 5.0);
				if (rlsTarget->z < 0) rlsTarget->z =0.0; 
				cu->Z  = (unsigned short)(rlsTarget->z / 1.0);
				cu->Vx = use2data(rlsTarget->vx,5,9);
				cu->Vy = use2data(rlsTarget->vy,5,9);
				cu->Vz = use2data(rlsTarget->vz,5,9);

				if (rlsTarget->skoX> 1000.0) rlsTarget->skoX = 1000.0;
				if (rlsTarget->skoY> 1000.0) rlsTarget->skoY = 1000.0;
				if (rlsTarget->skoZ> 1000.0) rlsTarget->skoZ = 1000.0;
				if (rlsTarget->skoVx> 1000.0) rlsTarget->skoVx = 1000.0;
				if (rlsTarget->skoVy> 1000.0) rlsTarget->skoVy = 1000.0;

				cu->tVx = use2data(rlsTarget->skoVx,5,0);
				cu->tVy = use2data(rlsTarget->skoVy,5,0);
				cu->tVz = 0;
				cu->tX  = use2data(rlsTarget->skoX ,  1.0,  0);
				cu->tY  = use2data(rlsTarget->skoY ,  1.0,  0);
				cu->tZ  = use2data(rlsTarget->skoZ ,  1.0,  0);
			}
			// Здесь СКО нужны
			tp = (unsigned int)((rlsTarget->t3k96_ms)  % 86400000);
			cu->TizmE_hi  = (unsigned short)(0xFFFF&(tp>>16));
			cu->TizmE_low = (unsigned short)(tp&0xFFFF);
	} // tip ==4
	if (cu->TIP ==5)
	{	// цель Х
		if (targetsX == NULL) return;
/* 	cu->PMPC=targetsX->bSlow;
//		cu->DoubleAcc=rlsTarget->bAutoEscort;
//		cu->OPr=(rlsTarget->b3k==0);
//		cu->ED=rlsTarget->bLoss;
//		cu->PNS=rlsTarget->bLoss;
*/		
		workX = targetsX->distance *cosf(targetsX->peleng) *cosf (targetsX->um);
		workY = targetsX->distance *sinf(targetsX->peleng) *cosf (targetsX->um);
		workZ = targetsX->distance *sinf(targetsX->um); // TODO высота отн плоскости - переделать
		if (workZ <=0) workZ = 0.0;
		cu->X  = (signed short)(workX/ 5.0);
		cu->Y  = (signed short)(workY / 5.0);
		cu->Z  = (unsigned short)(workZ / 1.0);
		cu->Vx = use2data(targetsX->Vx,5,9);
		cu->Vy = use2data(targetsX->Vy,5,9);
		cu->Vz = use2data(targetsX->Vz,5,9);

		if (targetsX->skoDistance> 1000.0) targetsX->skoDistance = 1000.0;
		worktZ = targetsX->skoDistance *sinf(targetsX->skoUm);
		if (worktZ> 1000.0) worktZ = 1000.0;

		if (targetsX->skoVx> 1000.0) targetsX->skoVx = 1000.0;
		if (targetsX->skoVy> 1000.0) targetsX->skoVy = 1000.0;
		if (targetsX->skoVz> 1000.0) targetsX->skoVz = 1000.0;

		cu->tVx = use2data(targetsX->skoVx,5,0);
		cu->tVy = use2data(targetsX->skoVy,5,0);
		cu->tVz = use2data(targetsX->skoVz,5,0);
		cu->tX  = use2data(targetsX->skoDistance ,  1.0,  0);
		cu->tY  = use2data(targetsX->skoDistance ,  1.0,  0);
		cu->tZ  = use2data(worktZ ,  1.0,  0);
		
		if (isSurface(targetsX->typeTarget))cu->EPR = 3; // >20m2
			else if (isRocket(targetsX->typeTarget)) cu->EPR = 1; // <1m2
				else cu->EPR = 2; // <1m2
		switch(targetsX->typeTarget)
		{
			case otLft: 
			{	
					if (workZ <=150.0) cu->typeTarget = 3; // nlc
					else  cu->typeTarget = 2;	// vc
			}		
			break; 
			case otSurface: 	cu->typeTarget = 1;	break; // nc
			case otShip: 		cu->typeTarget = 1;	break; // nc
			case otHelicopter: 	cu->typeTarget = 7;	break; // vt
			case otPn: 			cu->typeTarget = 2;	break; // pn не будет, ВЦ
			case otFalseTarget: cu->typeTarget = 2;	break; // pn не будет, ВЦ
			case otPkr:			cu->typeTarget = 5;	break; // pkr
			case otPrr:			cu->typeTarget = 6;		break; // prr
			default:
				if (targetsX->typeTarget == 0 ){cu->typeTarget = 2; break;}
				if(isSurface(targetsX->typeTarget)){cu->typeTarget = 3;	break;} // nc
				if(isAir(targetsX->typeTarget))	{cu->typeTarget = 2;	break;} // vc
				cu->typeTarget = 2;
				break;
		}
		tp = (unsigned int)((targetsX->t3k96_ms)  % 86400000);
		cu->TizmE_hi  = (unsigned short)(0xFFFF&(tp>>16));
		cu->TizmE_low = (unsigned short)(tp&0xFFFF);
	} // tip ==5

}// write

