#pragma once
#include "basearthconfig.h"
class CTLDetecConfig :
	public BaseArthConfig
{
public:
	CTLDetecConfig(void);
	~CTLDetecConfig(void);

	bool	LoadArthConfig(const char *pPath) ;

	bool	SaveArthConfig(const char* pPath) ;

	void	Setleftend(int pos);
	int		Getleftend();

	void	Setrightstart(int pos);
	int		GetrightStart();

	void	SetRatio1(double d);
	double	GetRatio1();

	void	SetRatio2(double d);
	double	GetRatio2();

	void	SetRatio3(double d);
	double	GetRatio3();
};

