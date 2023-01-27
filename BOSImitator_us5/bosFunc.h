#ifndef BOSFUNC_H_
#define BOSFUNC_H_

#include <commonTypes.h>
#include <modeling.h>
#include <typesRls.h>


int getVisualMarksBosImitator(
            int sizeCoordinats,
            ObjectData  * pobjectDatas[],		// массив координат
            Coordinat  * pourShip,
            float pelengAntennaOld,
            float pelengAntenna,
            int nVisualMarks,
            VisualMark visualMarks[]
            );
void getChannelsBosImitator(
            int sizeCoordinats, 	
            ObjectData  * pobjectDatas[],
            Coordinat  * pourShip,            	
            int sizeTargets,
            RlsTarget  prlsTargets[],            
            float currTime,
            float timeLoss,
            float pelengOld,
            float peleng,
            int nVisualMarks,
            VisualMark visualMarks[]);

#endif /*BOSFUNC_H_*/
