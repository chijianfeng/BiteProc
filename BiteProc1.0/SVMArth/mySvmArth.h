#pragma once
#include "basearth.h"
#include "svm.h"
#define Malloc(type,n) (type *)malloc((n)*sizeof(type))


class CmySvmArth :
	public CBaseArth
{
public:
	CmySvmArth(void);
	CmySvmArth(const char* parmpath);
	virtual ~CmySvmArth(void);

	bool	LoadTrainData(const char* pPath);

    void	SetTrainData(const double* pd , const int len);

	bool	Train( char* path=NULL);

	void	SetTestData(const double* pd , const int len);

	bool	Sim(double *res ,  int& len);

	bool	SaveTrainModel(const char* pPath);

	bool	LoadModel(const char* pPath);

	void	SetParamPath(const char* path);
	
	char*	GetParamPath();

	void	Config();

private: 
	bool	LoadTestData(const char *pPath);
	bool	ChangeDataFormat(const char* pPath);
	void	read_problem(const char *filename);
	void	exit_input_error(int line_num);
	char*	readline(FILE *input);
	void	do_cross_validation();

	char	*line;
	int	    max_line_len;
	struct svm_parameter param;		
	struct svm_problem prob;		
	struct svm_model *model;
	struct svm_node *x_space;
	int		cross_validation;
	int		nr_fold;
	struct svm_node* m_pTestdata;
	char  	m_nParamPath[1024];					//Arth config parament.

	int		predict_probability;				//for sim test.
	int		m_nSimDataLen;					
};

