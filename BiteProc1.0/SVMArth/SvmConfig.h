#pragma once
#include "basearthconfig.h"
class CSvmConfig :
	public BaseArthConfig
{
public:
	CSvmConfig(void);
	virtual ~CSvmConfig(void);
	bool	LoadArthConfig(const char *pPath) ;

	bool	SaveArthConfig(const char* pPath) ;
	/*
	0 -- C-SVC		(multi-class classification)
	1 -- nu-SVC		(multi-class classification)
	2 -- one-class SVM	
	3 -- epsilon-SVR	(regression)
	4 -- nu-SVR		(regression)
	*/
	void SetSvmType(int type);
	int	GetSvmType();

	/*
	0 -- linear: u'*v
	1 -- polynomial: (gamma*u'*v + coef0)^degree
	2 -- radial basis function: exp(-gamma*|u-v|^2)
	3 -- sigmoid: tanh(gamma*u'*v + coef0)
	4 -- precomputed kernel (kernel values in training_set_file)
	*/
	void SetSvmKernel(int type);
	int	GetSvmKernel();

	/*
	degree : set degree in kernel function (default 3)
	*/
	void SetSvmDegree(int d);
	int	GetSvmDegree();

	//gamma : set gamma in kernel function (default 1/num_features)
	void	SetSvmGamma(double g);
	double	GetSvmGamma();

	//coef0 : set coef0 in kernel function (default 0)
	void	SetSvmCoef(double c);
	double	GetSvmCoef();

	//cost : set the parameter C of C-SVC, epsilon-SVR, and nu-SVR (default 1)
	void	SetSvmCost(double c);
	double	GetSvmCost();

	//nu : set the parameter nu of nu-SVC, one-class SVM, and nu-SVR (default 0.5)
	void	SetSvmNu(double n);
	double	GetSvmNu();

	//epsilon : set the epsilon in loss function of epsilon-SVR (default 0.1)
	void	SetSvmEpsilon(double p);
	double	GetSvmEpsilon();

	//cachesize : set cache memory size in MB (default 100)
	void	SetSvmCachesize(double s);
	double	GetSvmCachesize();

	//eps : set tolerance of termination criterion (default 0.001)
	void	SetSvmEps(double e);
	double	GetSvmEps();

	//shrinking : whether to use the shrinking heuristics, 0 or 1 (default 1)
	void	SetSvmShrinking(int s);
	int		GetSvmShriking();

	//probability_estimates : whether to train a SVC or SVR model for probability estimates, 0 or 1 (default 0)
	void	SetSvmPro_est(int p);
	int		GetSvmPro_est();

	//weight : set the parameter C of class i to weight*C, for C-SVC (default 1)
	void	SetSvmWeight(const double *w , const int len);
	int		GetSvmWeight(double *w);

	//weight_lable for C_SVC
	void	SetSvmWeightLable(const int* l , const int len);
	int		GetSvmWeightLable(int* l);

	//n: n-fold cross validation mode
	void	SetFlods(int n);
	int		GetFlods();

	//get and set Svm model path.
	void	SetModelPath(const char* path);
	string	GetModelPath();

private:
	
};

