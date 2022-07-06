// DEFINE, MACRO
//////////////////////////////////////////////////////////////////////////

#define INF						1234567890
#define MAX_INT					2147483647
#define PI						3.1415927f
#define CUTZERO					0.000125f
#define CUTONE					0.999875f
#define IS_ZERO(__FLOAT)		((__FLOAT) < CUTZERO && (__FLOAT) > -CUTZERO)
#define IS_ONE(__FLOAT)			((__FLOAT) < 1.f+CUTZERO && (__FLOAT) > CUTONE)

#define MIN(__VAL1,__VAL2)			((__VAL1 < __VAL2)?__VAL1:__VAL2)
#define MAX(__VAL1,__VAL2)			((__VAL1 > __VAL2)?__VAL1:__VAL2)
#define BETWEEN(__VAL,__MIN,__MAX)	((__VAL < __MIN)?__MIN:((__VAL > __MAX)?__MAX:__VAL))
#define SQR(__VAL)					((__VAL)*(__VAL))
#define D2R(__DEGREE)				((__DEGREE)/180.f*PI)
#define R2D(__RADIAN)				((__RADIAN)*180.f/PI)
#define RAND_RANGE(__MIN,__MAX)		((rand()%((__MAX)-(__MIN)+1))+__MIN)
#define RAND_RANGE_F(__MIN,__MAX)	((rand()%1000)/1000.f*((__MAX)-(__MIN))+(__MIN))
#define RAND_F(__VAL)				((rand()%1000)/1000.f*(__VAL))

//////////////////////////////////////////////////////////////////////////