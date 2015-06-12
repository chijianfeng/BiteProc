#include "StdAfx.h"
#include "mySvmArth.h"
#include "Stringoper.h"
#include "SvmConfig.h"
const	CString TmpFileName = _T("~tmpTrainData.txt");
CmySvmArth::CmySvmArth(void)
{
	max_line_len = 1024;
	line = NULL;
	nr_fold = 0;
	m_pTestdata = NULL;
	
}

CmySvmArth::CmySvmArth(const char* parmpath)
{
	max_line_len = 1024;
	line = NULL;
	nr_fold = 0;
	m_pTestdata = NULL;
	if(parmpath==NULL)
		return;
	strcpy_s(m_nParamPath , parmpath);
}
CmySvmArth::~CmySvmArth(void)
{
	if(m_pTestdata!=NULL)
		delete[] m_pTestdata;
	if(line!=NULL)
		free(line);
}

bool CmySvmArth::LoadTrainData(const char* pPath)
{
	if(pPath==NULL)
		return false;
	if(!ChangeDataFormat(pPath))
		return false;
	Stringoper oper;
	char p[1024];
	oper.CStringtoChars(TmpFileName , p);
	read_problem(p);
	return true;
}

void CmySvmArth::SetTrainData(const double* pd , const int len)
{
	if(pd==NULL||len<=0)
		return;

	//add code here.
}


bool CmySvmArth::Train( char* path)
{
	const char *error_msg;
	error_msg = svm_check_parameter(&prob,&param);
	if(error_msg)
	{
		fprintf(stderr,"ERROR: %s\n",error_msg);
		free(prob.y);
		free(prob.x);
		free(x_space);
		free(line);
		line = NULL;
		x_space = NULL;
		return false;
	}
	if(cross_validation)
	{
		do_cross_validation();
	}
	else
	{
		model = svm_train(&prob,&param);
		if(path!=NULL&&svm_save_model(path,model))
		{
			fprintf(stderr, "can't save model to file %s\n", path);
			return false;
		}
		if(path!=NULL)
		{
			svm_free_and_destroy_model(&model);
		}
	}
	svm_destroy_param(&param);
	free(prob.y);
	free(prob.x);
	if(path!=NULL)
	{
		free(x_space);
		free(line);
		line = NULL;
		x_space = NULL;
	}
	return true;
}

void CmySvmArth::SetTestData(const double* pd , const int len)
{
	if(pd==NULL)
		return;
	if(m_pTestdata==NULL)
	{
		m_pTestdata = new struct svm_node[len+1];
	}
	for (int i = 0;i<len;i++)
	{
		m_pTestdata[i].index = i+1;
		m_pTestdata[i].value = pd[i];
	}
	m_pTestdata[len].index = -1;
	m_nSimDataLen = 1;
}

bool CmySvmArth::LoadTestData(const char *pPath)
{
	if(pPath==NULL)
		return false;

	return true;
}

bool CmySvmArth::Sim(double* res , int& len)
{
	if(model==NULL||res==NULL)
		return false;
	int svm_type=svm_get_svm_type(model);
	int nr_class=svm_get_nr_class(model);
	double *prob_estimates=NULL;
	len = m_nSimDataLen;
	if (predict_probability && (svm_type==C_SVC || svm_type==NU_SVC))
	{
		prob_estimates = new double[nr_class];
		*res = svm_predict_probability(model,m_pTestdata,prob_estimates);
		delete prob_estimates;
	}
	else
	{
		*res = svm_predict(model,m_pTestdata);
	}
	return true;
}

bool CmySvmArth::LoadModel(const char* pPath)
{
	if(pPath==NULL)
		return false;
	if((model=svm_load_model(pPath))==0)
	{
		fprintf(stderr,"can't open model file %s\n",pPath);
		return false;
	}
	return true;
}

bool CmySvmArth::SaveTrainModel(const char* pPath)
{
	if(pPath==NULL||model==NULL)
		return false;

	if(svm_save_model(pPath,model))
	{
		fprintf(stderr, "can't save model to file %s\n", pPath);
		return false;
	}
	svm_free_and_destroy_model(&model);
	if(x_space!=NULL)
	{
		free(x_space);
		x_space = NULL;
	}
	if(line!=NULL)
	{
		free(line);
		line = NULL;
	}
	return true;
}

void CmySvmArth::SetParamPath(const char* path)
{
	if(path==NULL)
		return;
	strcpy_s(m_nParamPath , path);
}

char* CmySvmArth::GetParamPath()
{
	return m_nParamPath;
}


void CmySvmArth::Config()
{
	//config the init svm model parameter.

	//for train.
	param.svm_type = C_SVC;
	param.kernel_type = RBF;
	param.degree = 3;
	param.gamma = 0;			// 1/num_features
	param.coef0 = 0;
	param.nu = 0.5;
	param.cache_size = 100;
	param.C = 1;
	param.eps = 1e-3;
	param.p = 0.1;
	param.shrinking = 1;
	param.probability = 0;
	param.nr_weight = 0;
	param.weight_label = NULL;
	param.weight = NULL;
	cross_validation = 0;

	CSvmConfig config;
	config.LoadArthConfig(m_nParamPath);
	param.svm_type = config.GetSvmType();
	param.kernel_type = config.GetSvmKernel();
	param.degree = config.GetSvmDegree();
	param.gamma = config.GetSvmGamma();
	param.coef0 = config.GetSvmCoef();
	param.nu = config.GetSvmNu();
	param.cache_size = config.GetSvmCachesize();
	param.C = config.GetSvmCost();
	param.eps = config.GetSvmEps();
	param.p = config.GetSvmEpsilon();
	param.shrinking = config.GetSvmShriking();
	param.probability = config.GetSvmPro_est();
	nr_fold = config.GetFlods();
	if(nr_fold>=2)
		cross_validation  = 1;
	else
		cross_validation = 0;
	param.weight = new double[128];
	int len = config.GetSvmWeight(param.weight);
	if(len<=0)
	{
		param.nr_weight = 0;
		delete param.weight;
		param.weight = NULL;
	}
	param.weight_label = new int[128];
	len = config.GetSvmWeightLable(param.weight_label);
	param.nr_weight = len;
	if(len<=0)
	{
		param.nr_weight = 0;
		delete param.weight_label;
		param.weight_label = NULL;
	}
	predict_probability = param.probability;
}

//private function

bool CmySvmArth::ChangeDataFormat(const char* pPath)
{
	CFileFind finder;
	Stringoper oper;
	if(!finder.FindFile(oper.tocstring(pPath)))
		return false;
	FILE *file = NULL;
	CFile fwrite;
	if((file=fopen(pPath,"r"))==NULL)
	{
		TRACE("Open Source file Failed\n");
		return false;
	}
	if(!fwrite.Open(TmpFileName , CFile::modeWrite|CFile::modeCreate))
	{
		TRACE("Open Dest file Failed\n");
		return false;
	}
	unsigned int num_input, num_output, num_data, i, j;
	unsigned int line = 1;
	//read the head.
	if(fscanf(file, "%u %u %u\n", &num_data, &num_input, &num_output) != 3)
	{
		TRACE("Read the File Head failed ");
		TRACE("Read error in Line:%d\n",line);
		return false;
	}
	line++;
	double *pd = new double[num_input];
	double *pt = new double[num_output];
	for (int i=0;i<num_data;i++)
	{
		for (int j=0;j<num_input;j++)
		{
			if(fscanf(file, "%le" " ",&pd[j]) != 1)
			{
				delete pd;
				delete pt;
				TRACE("Read error in Line:%d\n",line);
				return false;
			}
		}
		for (int j=0;j<num_output;j++)
		{
			if(fscanf(file, "%le" " ",&pt[j]) != 1)
			{
				delete pd;
				delete pt;
				TRACE("Read error in Line:%d\n",line);
				return false;
			}
		}
		line++;
		
		string str = pt[0]>=0?"+1 ":"-1 ";
		stringstream ss(stringstream::in | stringstream::out);
		for (int j=0;j<num_input;j++)
		{
			ss.clear();
			ss.str("");
			ss<<(j+1);
			str += ss.str();
			ss.str("");
			ss<<pd[j];
			str += ":"+ss.str()+" ";
		}
		str +="\r\n";
		fwrite.Write(str.c_str() , str.length());
	}
	delete pt;
	delete pd;
	fwrite.Close();
	fclose(file);
	return true;
}

void CmySvmArth::read_problem(const char *filename)
{
	int elements, max_index, inst_max_index, i, j;
	FILE *fp = fopen(filename,"r");
	char *endptr;
	char *idx, *val, *label;

	if(fp == NULL)
	{
		fprintf(stderr,"can't open input file %s\n",filename);
		exit(1);
	}

	prob.l = 0;
	elements = 0;

	max_line_len = 1024;
	line = Malloc(char,max_line_len);
	while(readline(fp)!=NULL)
	{
		char *p = strtok(line," \t"); // label

		// features
		while(1)
		{
			p = strtok(NULL," \t");
			if(p == NULL || *p == '\n') // check '\n' as ' ' may be after the last feature
				break;
			++elements;
		}
		++elements;
		++prob.l;
	}
	rewind(fp);

	prob.y = Malloc(double,prob.l);
	prob.x = Malloc(struct svm_node *,prob.l);
	x_space = Malloc(struct svm_node,elements);

	max_index = 0;
	j=0;
	for(i=0;i<prob.l;i++)
	{
		inst_max_index = -1; // strtol gives 0 if wrong format, and precomputed kernel has <index> start from 0
		readline(fp);
		prob.x[i] = &x_space[j];
		label = strtok(line," \t\n");
		if(label == NULL) // empty line
			exit_input_error(i+1);

		prob.y[i] = strtod(label,&endptr);
		if(endptr == label || *endptr != '\0')
			exit_input_error(i+1);

		while(1)
		{
			idx = strtok(NULL,":");
			val = strtok(NULL," \t");

			if(val == NULL)
				break;

			errno = 0;
			x_space[j].index = (int) strtol(idx,&endptr,10);
			if(endptr == idx || errno != 0 || *endptr != '\0' || x_space[j].index <= inst_max_index)
				exit_input_error(i+1);
			else
				inst_max_index = x_space[j].index;

			errno = 0;
			x_space[j].value = strtod(val,&endptr);
			if(endptr == val || errno != 0 || (*endptr != '\0' && !isspace(*endptr)))
				exit_input_error(i+1);

			++j;
		}

		if(inst_max_index > max_index)
			max_index = inst_max_index;
		x_space[j++].index = -1;
	}

	if(param.gamma == 0 && max_index > 0)
		param.gamma = 1.0/max_index;

	if(param.kernel_type == PRECOMPUTED)
		for(i=0;i<prob.l;i++)
		{
			if (prob.x[i][0].index != 0)
			{
				fprintf(stderr,"Wrong input format: first column must be 0:sample_serial_number\n");
				return ;
			}
			if ((int)prob.x[i][0].value <= 0 || (int)prob.x[i][0].value > max_index)
			{
				fprintf(stderr,"Wrong input format: sample_serial_number out of range\n");
				return ;
			}
		}

		fclose(fp);
}

void CmySvmArth::exit_input_error(int line_num)
{
	fprintf(stderr,"Wrong input format at line %d\n", line_num);
	return;
}

char* CmySvmArth::readline(FILE *input)
{
	int len;

	if(fgets(line,max_line_len,input) == NULL)
		return NULL;

	while(strrchr(line,'\n') == NULL)
	{
		max_line_len *= 2;
		line = (char *) realloc(line,max_line_len);
		len = (int) strlen(line);
		if(fgets(line+len,max_line_len-len,input) == NULL)
			break;
	}
	return line;
}

void CmySvmArth::do_cross_validation()
{
	int i;
	int total_correct = 0;
	double total_error = 0;
	double sumv = 0, sumy = 0, sumvv = 0, sumyy = 0, sumvy = 0;
	double *target = Malloc(double,prob.l);

	svm_cross_validation(&prob,&param,nr_fold,target);
	if(param.svm_type == EPSILON_SVR ||
		param.svm_type == NU_SVR)
	{
		for(i=0;i<prob.l;i++)
		{
			double y = prob.y[i];
			double v = target[i];
			total_error += (v-y)*(v-y);
			sumv += v;
			sumy += y;
			sumvv += v*v;
			sumyy += y*y;
			sumvy += v*y;
		}
		printf("Cross Validation Mean squared error = %g\n",total_error/prob.l);
		printf("Cross Validation Squared correlation coefficient = %g\n",
			((prob.l*sumvy-sumv*sumy)*(prob.l*sumvy-sumv*sumy))/
			((prob.l*sumvv-sumv*sumv)*(prob.l*sumyy-sumy*sumy))
			);
	}
	else
	{
		for(i=0;i<prob.l;i++)
			if(target[i] == prob.y[i])
				++total_correct;
		printf("Cross Validation Accuracy = %g%%\n",100.0*total_correct/prob.l);
	}
	free(target);
}

//extern interface
extern "C" _declspec(dllexport) CBaseArth* CreateArth(void *p=NULL)
{
	if(p==NULL)
		return new CmySvmArth();
	else
		return new CmySvmArth((char*)p);
}