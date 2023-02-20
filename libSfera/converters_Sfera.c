#include "lSfera.h"
#include "lSfera_f.h"
#include <math.h>

//Преобразует IP1_Sfera в fSfera_Ip1
void conv_write_Sfera_IP1(const fSfera_Ip1_t *t_data1, mSfera_Ip1_t *t_ip1)
{
	int i;
	t_ip1->OT = t_data1->OT;
	t_ip1->RU = t_data1->RU;
	t_ip1->RR = t_data1->RR;
	t_ip1->numberIP3 = t_data1->numberIP3;

	for (i = 0; i < SFERA_MODUL_NUM; ++i) 
	{
		t_ip1->tgt[i].typeTarget = t_data1->tgt[i].typeTarget;
		t_ip1->tgt[i].typeCommand = t_data1->tgt[i].typeCommand;
		t_ip1->tgt[i].typeTarget = t_data1->tgt[i].GO;
		t_ip1->tgt[i].istCU = t_data1->tgt[i].istCU;
		t_ip1->tgt[i].nIstCU = t_data1->tgt[i].nIstCU;
		t_ip1->tgt[i].UMmax = (signed short)(t_data1->tgt[i].UMmax* pow(2,14)/M_PI/2);
		t_ip1->tgt[i].UMmin = (signed short)(t_data1->tgt[i].UMmin* pow(2,14)/M_PI/2);
		t_ip1->tgt[i].pelengMax = (unsigned short)(t_data1->tgt[i].pelengMax* pow (2,15)/M_PI);
		t_ip1->tgt[i].pelengMin = (unsigned short)(t_data1->tgt[i].pelengMin* pow (2,15)/M_PI);
	}
}

//Преобразует IP2_Sfera в fSfera_Ip2
void conv_write_Sfera_IP2_OneTarget(const fSfera_Ip2Tgt *ftgt_ip2, mSfera_Ip2Target_t *mtgt_ip2)
{
	mtgt_ip2->typeTarget = ftgt_ip2->typeTarget;
	mtgt_ip2->naznCU = ftgt_ip2->naznCU;
	mtgt_ip2->GO = ftgt_ip2->GO;
	mtgt_ip2->istCU = ftgt_ip2->istCU;
	mtgt_ip2->nIstCU = ftgt_ip2->nIstCU;
	mtgt_ip2->bPeleng = 1;
	mtgt_ip2->peleng = (unsigned short)(ftgt_ip2->peleng *pow(2,15)/M_PI);
	mtgt_ip2->bUM = 1;
	mtgt_ip2->UM = (signed short)(ftgt_ip2->UM*pow(2,14)/M_PI/2);
	mtgt_ip2->bDistance = 1;
	mtgt_ip2->distance = (unsigned short)(ftgt_ip2->distance*pow(2,15)/128000.0);
}


//Преобразует IP3_Sfera в fSfera_Ip3
void conv_write_Sfera_IP3_OneTarget(const fSfera_ResGO_t *ftgt_ip3, mSfera_ResGO_t *mtgt_ip3)
{
	mtgt_ip3->process = ftgt_ip3->process;
	mtgt_ip3->GO = ftgt_ip3->GO;
	mtgt_ip3->nSfera = ftgt_ip3->nSfera;
	mtgt_ip3->P = (unsigned short)(ftgt_ip3->P *pow(2,15)/M_PI);
	mtgt_ip3->D = (unsigned short)(ftgt_ip3->D*pow(2,12)/128.0);
}


//Преобразует  mSfera_Ip5Target_t в fSfera_Ip5Target_t
void conv_read_Sfera_IP4(mSfera_Ip4_t *m_ip4, 
		fSfera_Ip4_t   * f_ip4)
{
	f_ip4->RU 	= m_ip4->RU;
	f_ip4->RR 		= m_ip4->RR;
	f_ip4->KOC 	= m_ip4->KOC;
	f_ip4->KSC 	= m_ip4->KSC;
	f_ip4->modulObn[0].obm 		= m_ip4->obm_oem1;
	f_ip4->modulObn[0].tv 		= m_ip4->tv_oem1;
	f_ip4->modulObn[0].o_oem 	= m_ip4->o_oem1;
	f_ip4->modulObn[0].reg 		= m_ip4->reg_oem1;
	f_ip4->modulObn[0].obzor 	= m_ip4->obzor_oem1;
	f_ip4->modulObn[0].peleng 	= ((float)((unsigned short)m_ip4->peleng1 )*M_PI/pow(2,15));
	f_ip4->modulObn[0].angleHor = ((float)((unsigned short)m_ip4->angleHor1)*M_PI/pow(2,15));
	f_ip4->modulObn[0].UM 		= ((float)((signed short)m_ip4->UM1 )*M_PI/pow(2,15));
	f_ip4->modulObn[0].angleVer = ((float)((unsigned short)m_ip4->angleVer1 )*M_PI/pow(2,15));
	
	f_ip4->modulObn[1].obm 		= m_ip4->obm_oem2;
	f_ip4->modulObn[1].tv 		= m_ip4->tv_oem2;
	f_ip4->modulObn[1].o_oem 	= m_ip4->o_oem2;
	f_ip4->modulObn[1].reg 		= m_ip4->reg_oem2;
	f_ip4->modulObn[1].obzor 	= m_ip4->obzor_oem2;
	f_ip4->modulObn[1].peleng 	= ((float)((unsigned short)m_ip4->peleng2 )*M_PI/pow(2,15));
	f_ip4->modulObn[1].angleHor = ((float)((unsigned short)m_ip4->angleHor2)*M_PI/pow(2,15));
	f_ip4->modulObn[1].UM 		= ((float)((signed short)m_ip4->UM2 )*M_PI/pow(2,15));
	f_ip4->modulObn[1].angleVer = ((float)((unsigned short)m_ip4->angleVer2 )*M_PI/pow(2,15));
	
	f_ip4->modulObn[2].obm 		= m_ip4->obm_oem3;
	f_ip4->modulObn[2].tv 		= m_ip4->tv_oem3;
	f_ip4->modulObn[2].o_oem 	= m_ip4->o_oem3;
	f_ip4->modulObn[2].reg 		= m_ip4->reg_oem3;
	f_ip4->modulObn[2].obzor 	= m_ip4->obzor_oem3;
	f_ip4->modulObn[2].peleng 	= ((float)((unsigned short)m_ip4->peleng3 )*M_PI/pow(2,15));
	f_ip4->modulObn[2].angleHor = ((float)((unsigned short)m_ip4->angleHor3)*M_PI/pow(2,15));
	f_ip4->modulObn[2].UM 		= ((float)((signed short)m_ip4->UM3 )*M_PI/pow(2,15));
	f_ip4->modulObn[2].angleVer = ((float)((unsigned short)m_ip4->angleVer3 )*M_PI/pow(2,15));
	
	f_ip4->modulObn[3].obm 		= m_ip4->obm_oem4;
	f_ip4->modulObn[3].tv 		= m_ip4->tv_oem4;
	f_ip4->modulObn[3].o_oem 	= m_ip4->o_oem4;
	f_ip4->modulObn[3].reg 		= m_ip4->reg_oem4;
	f_ip4->modulObn[3].obzor 	= m_ip4->obzor_oem4;
	f_ip4->modulObn[3].peleng 	= ((float)((unsigned short)m_ip4->peleng4 )*M_PI/pow(2,15));
	f_ip4->modulObn[3].angleHor = ((float)((unsigned short)m_ip4->angleHor4)*M_PI/pow(2,15));
	f_ip4->modulObn[3].UM 		= ((float)((signed short)m_ip4->UM4 )*M_PI/pow(2,15));
	f_ip4->modulObn[3].angleVer = ((float)((unsigned short)m_ip4->angleVer4 )*M_PI/pow(2,15));
	
	f_ip4->modulSopr[0].reg 		= m_ip4->reg_sopr11;
	f_ip4->modulSopr[0].ld 		= m_ip4->ld11;
	f_ip4->modulSopr[0].numberSfera = m_ip4->numberSfera[0];
	f_ip4->modulSopr[0].bDistance = m_ip4->bDistance11;

	f_ip4->modulSopr[1].reg 		= m_ip4->reg_sopr12;
	f_ip4->modulSopr[1].ld 		= m_ip4->ld12;
	f_ip4->modulSopr[1].numberSfera = m_ip4->numberSfera[1];
	f_ip4->modulSopr[1].bDistance = m_ip4->bDistance12;

	f_ip4->modulSopr[2].reg 		= m_ip4->reg_sopr21;
	f_ip4->modulSopr[2].ld 		= m_ip4->ld21;
	f_ip4->modulSopr[2].numberSfera = m_ip4->numberSfera[2];
	f_ip4->modulSopr[2].bDistance = m_ip4->bDistance21;

	f_ip4->modulSopr[3].reg 		= m_ip4->reg_sopr22;
	f_ip4->modulSopr[3].ld 		= m_ip4->ld22;
	f_ip4->modulSopr[3].numberSfera = m_ip4->numberSfera[3];
	f_ip4->modulSopr[3].bDistance = m_ip4->bDistance22;
	
	f_ip4->modulSopr[4].reg 		= m_ip4->reg_sopr31;
	f_ip4->modulSopr[4].ld 		= m_ip4->ld31;
	f_ip4->modulSopr[4].numberSfera = m_ip4->numberSfera[4];
	f_ip4->modulSopr[4].bDistance = m_ip4->bDistance31;

	f_ip4->modulSopr[5].reg 		= m_ip4->reg_sopr32;
	f_ip4->modulSopr[5].ld 		= m_ip4->ld32;
	f_ip4->modulSopr[5].numberSfera = m_ip4->numberSfera[5];
	f_ip4->modulSopr[5].bDistance = m_ip4->bDistance32;

	f_ip4->modulSopr[6].reg 		= m_ip4->reg_sopr41;
	f_ip4->modulSopr[6].ld 		= m_ip4->ld41;
	f_ip4->modulSopr[6].numberSfera = m_ip4->numberSfera[6];
	f_ip4->modulSopr[6].bDistance = m_ip4->bDistance41;

	f_ip4->modulSopr[7].reg 		= m_ip4->reg_sopr42;
	f_ip4->modulSopr[7].ld 		= m_ip4->ld42;
	f_ip4->modulSopr[7].numberSfera = m_ip4->numberSfera[7];
	f_ip4->modulSopr[7].bDistance = m_ip4->bDistance42;

}


//Преобразует  mSfera_Ip5Target_t в fSfera_Ip5Target_t
void conv_read_Sfera_IP5_oneTarget(mSfera_Ip5Target_t *mtgt_ip5, 
		fSfera_Ip5Tgt   * ftgt_ip5)
{
	ftgt_ip5->nSfera = mtgt_ip5->nSfera;
	ftgt_ip5->typeTarget = mtgt_ip5->typeTarget;
	ftgt_ip5->naznCU = mtgt_ip5->naznCU;
	ftgt_ip5->istCU = mtgt_ip5->istCU;
	ftgt_ip5->GO = mtgt_ip5->GO;
	ftgt_ip5->nIstCU = mtgt_ip5->nIstCU;
	ftgt_ip5->peleng = ((float)((unsigned short)mtgt_ip5->peleng )*M_PI/pow(2,15));
	ftgt_ip5->distance = ((float)((unsigned short)mtgt_ip5->distance)*128000.0/pow(2,15));
	ftgt_ip5->UM = ((float)((signed short)mtgt_ip5->UM )*M_PI/pow(2,15));
	if (mtgt_ip5->bVPeleng == 1) ftgt_ip5->vPeleng = ((float)((signed short)mtgt_ip5->vPeleng )*512.0/pow(2,14));
		else ftgt_ip5->vPeleng = 0;
	if (mtgt_ip5->bVUM == 1) ftgt_ip5->vUM = ((float)((signed short)mtgt_ip5->vUM )*512.0/pow(2,14));
		else ftgt_ip5->vUM = 0;
	if (mtgt_ip5->bVDistance == 1) ftgt_ip5->vDistance = ((float)((signed short)mtgt_ip5->vDistance )*512.0/pow(2,14));
		else ftgt_ip5->vDistance = 0;
}


//Преобразует  mSfera_Ip6Target_t в fSfera_Ip6Target_t
void conv_read_Sfera_IP6_OneTarget(mSfera_Ip6Target_t *mtgt_ip6, fSfera_Ip6Tgt   * ftgt_ip6)
{
	ftgt_ip6->nSfera = mtgt_ip6->nSfera;
	ftgt_ip6->TIP = mtgt_ip6->TIP;
	ftgt_ip6->P = ((float)((unsigned short)mtgt_ip6->P )*M_PI/pow(2,15));
	ftgt_ip6->UM = ((float)((signed short)mtgt_ip6->UM )*M_PI/pow(2,15));
}

