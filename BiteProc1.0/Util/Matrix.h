/////////////////////////////////////////////////////////////////////////////
// Matrix.h : 
//		Interface of the class CMatrix
// Author : freeia
// Modified Date : 3/11/2003
// E-mail : freeia@163.com
// Company/School : hdcad
/////////////////////////////////////////////////////////////////////////////

#ifndef _MATRIX_H
#define _MATRIX_H

#include <vector>

using namespace std;

typedef vector <double> VDOUBLE;
typedef vector <VDOUBLE> TMatrix;


class __declspec(dllimport) CMatrix  
{	

	/************************************************************************
	*				the interface function of the class CMatrix 			*
	************************************************************************/
public:

	/////////////////////////////////////////////////////////////////////////
	// Construction and Destruction	
	CMatrix();

	CMatrix(CMatrix& cMatrixB);

	virtual ~CMatrix();

	TMatrix	m_pTMatrix;				// ָ������ͷָ��

	/////////////////////////////////////////////////////////////////////////
	// According to the parameters nRow & nCol to construct a matrix
	CMatrix(unsigned int nRow, unsigned int nCol);


	/////////////////////////////////////////////////////////////////////////
	// This function initialize the matrix :
	//		the matrix which has been initialized has 0 row & 0 column, and
	// all elements in it is zeros.
	// 
	void Initialize();

	/////////////////////////////////////////////////////////////////////////
	// This function initialize the matrix :
	//		the matrix which has been initialized has 0 row & 0 column, and
	// all elements in it is zeros.
	// 
	void InitializeZero();

	/////////////////////////////////////////////////////////////////////////
	// To make random in the elements of the matrix and the elements of the 
	// matrix has been randomized between -1 and 1.These elements must be
	// decimal fractions.
	// 
	void RandomInitialize();

	/////////////////////////////////////////////////////////////////////////
	// Overload Operations

	// 'CMatrix + CMatrix'
	CMatrix operator + (CMatrix& cMatrixB);	
	// 'CMatrix - CMatrix'
	CMatrix operator - (CMatrix& cMatrixB);	
	// 'CMatrix * CMatrix'
	CMatrix operator * (CMatrix& cMatrixB);	
	// 'CMatrix * int'
	CMatrix operator * (double nValue);	
	// 'CMatrix = CMatrix'
	CMatrix& operator = (CMatrix& cMatrixB);	
	// 'CMatrix += CMatrix'
	CMatrix& operator += (CMatrix& cMatrixB);	
	// 'CMatrix .* CMatrix'
	CMatrix operator / (CMatrix& cMatrixB);	


	/////////////////////////////////////////////////////////////////////////
	// Transpose the matrix
	//
	CMatrix Transpose();

	/////////////////////////////////////////////////////////////////////////
	// Inverse the matrix
	//
	CMatrix Inverse();

	/////////////////////////////////////////////////////////////////////////
	// Load the data from the file and reset the rows and the colums of the 
	// matrix
	//
	bool LoadDataFromFile(CString& strFileName);

	//load the data from the stream buffer. the length = colnums*rows;
	bool LoadDataFromBuf(const double* pbuf, const int length);
	/////////////////////////////////////////////////////////////////////////
	// Save the data from the CMatrix object to the specified file
	//
	bool SaveDataToFile(CString& strFileName);
	
	/////////////////////////////////////////////////////////////////////////
	// Get the matrix Row Number
	//
	unsigned int GetMatrixRowNumber() const
	{
		return m_nRow;
	}

	/////////////////////////////////////////////////////////////////////////
	// Get the matrix Colum Number
	//
	unsigned int GetMatrixColNumber() const
	{
		return m_nCol;
	}


	/////////////////////////////////////////////////////////////////////////////
	// Load the data from the file and reset the rows and the colums of 
	// the matrixes.
	// Parameter:
	//		[in] strFileName
	//		[out]cMatrixInputToHideWeightValue
	//		[out]cMatrixHideLayerValveValue
	//		[out]cMatrixHideToOutputWeightValue
	//		[out]cMatrixOutputLayerValveValue
	//		[out]nInputLayerNumber
	//		[out]nHideLayerNumber
	//		[out]nOutputLayerNumber
	//		[out]nComboArithmetic
	//		[out]nComboFunc
	//
	bool LoadDataFromFileSpecial (	CString& strFileName,
									CMatrix& cMatrixInputToHideWeightValue,
									CMatrix& cMatrixHideLayerValveValue,
									CMatrix& cMatrixHideToOutputWeightValue,
									CMatrix& cMatrixOutputLayerValveValue,
									unsigned int &nInputLayerNumber,
									unsigned int &nHideLayerNumber,
									unsigned int &nOutputLayerNumber,
									int &nComboArithmetic,
									int	&nComboFunc);



	/////////////////////////////////////////////////////////////////////////
	//	Copy data from a vector to a matrix
	//	Parameter:
	//		[out]	cMatrix ----> the returned value
	//		[in]	nIndex	----> the index in vector
	//	Notes:
	//		the object copied must be vector!!!
	//
	void CopySubMatrixFromVector(CMatrix& cMatrix,unsigned int nIndex);

	/////////////////////////////////////////////////////////////////////////
	// Copy data from sub matrix to another matrix
	// Parameter:
	//		[out]	cMatrix ----> ������Ӿ��󷵻صĽ��
	//		[in]	nStartX ----> �Ӿ����ھ����е���ʼ����,��Ӧ��,������1��ʼ
	//		[in]	nStartY ----> �Ӿ����ھ����е���ʼ����,��Ӧ��,������1��ʼ
	//
	void CopySubMatrix(CMatrix& cMatrix,unsigned int nStartX,unsigned int nStartY);

	/////////////////////////////////////////////////////////////////////////
	// Copy Matrix
	//	Parameter:
	//		[in]	cMatrix ----> be copied matrix
	//
	void CopyMatrix(CMatrix& cMatrix);

	/////////////////////////////////////////////////////////////////////////
	// ����������е�Ԫ�ذ��кϳ�һ��
	//	���磺
	//		matrix = [
	//			1	2	3
	//			4	5	6
	//			7	8	9
	//				]
	//		CMatrix cMatrix = matrix.MergeColumnsToColumnVector();
	//		cMatrix = 
	//			[	1
	//				4	
	//				7
	//				2
	//				5
	//				8
	//				3
	//				6
	//				9	]
	//
	CMatrix MergeColumnsToColumnVector();

	/////////////////////////////////////////////////////////////////////////
	// �Ծ��������е�Ԫ�ؽ���һ�η����Ա任:
	//		�任���ֵy��任ǰ��ֵ�Ĺ�ϵ��:
	//			y = f(x) = 1 / (1 + exp(-x))	( 0 < f(x) < 1)
	//
	CMatrix Sigmoid();

	/////////////////////////////////////////////////////////////////////////
	// �Ծ��������е�Ԫ�ؽ���һ�η����Ա任:
	//		�任���ֵy��任ǰ��ֵ�Ĺ�ϵ��:
	//			y = f'(x) = (1 / (1 + exp(-x)))'	( 0 < f(x) < 1)
	//			  = exp(-x)/((1 + exp(-x))*(1 + exp(-x)))
	//
	CMatrix SigmoidDerivative();


	/////////////////////////////////////////////////////////////////////////
	// �Ծ��������е�Ԫ�ؽ���һ�η����Ա任:
	//		�任���ֵy��任ǰ��ֵ�Ĺ�ϵ��:
	//			y = tanh(x) = (1 - exp(-x)) / (1 + exp(-x))
	//					 =  1 - 2 * exp(-x) / (1 + exp(-x))	( -1 < f(x) < 1)
	//
	CMatrix tanh(); 

	/////////////////////////////////////////////////////////////////////////
	// �Ծ��������е�Ԫ�ؽ���һ�η����Ա任:
	//		�任���ֵy��任ǰ��ֵ�Ĺ�ϵ��:
	//			y = tanh'(x) = ((1 - exp(-x)) / (1 + exp(-x)))'	( -1 < f(x) < 1)
	//					 = 	2*exp(-x)/((1 + exp(-x))*(1 + exp(-x)))
	//
	CMatrix tanhDerivative();

	/////////////////////////////////////////////////////////////////////////
	// �Ծ��������е�Ԫ�ؽ���һ�η����Ա任:
	//		�任���ֵy��任ǰ��ֵ�Ĺ�ϵ��:
	//			y = Tansig(x) = 2 / (1 + exp(-2 * x)) -1
	//
	CMatrix Tansig();

	/////////////////////////////////////////////////////////////////////////
	// �Ծ��������е�Ԫ�ؽ���һ�η����Ա任:
	//		�任���ֵy��任ǰ��ֵ�Ĺ�ϵ��:
	//			y = Tansig'(x) = (2 / (1 + exp(-2 * x)) -1)'
	//				= (2 / (1 + exp(-2 * x)) -1) * (2 / (1 + exp(-2 * x)) -1) -1
	//
	CMatrix TansigDerivative();

	/////////////////////////////////////////////////////////////////////////
	// �Ծ����е�Ԫ�ؽ���һ�β���:
	//		ʹ�������е����Ӧ����Ԫ�����
	//	Parameter:
	//		nRowIndex	---->	������ֵ(the index starts from 0)
	//							�Դ�����Ϊ��׼,ʹ����������������Ӧ���е�ֵ
	//							��������Ӧ���е�ֵ���	
	//
	void MakeAllColumnElementsSameValue(unsigned int nRowIndex);

	/////////////////////////////////////////////////////////////////////////
	// �Ծ����е�Ԫ�ؽ���һ�β���:
	//		ʹ�����Ϊ��λ��
	//
	void Eye();

	/////////////////////////////////////////////////////////////////////////
	// Get System Error
	//
	double	GetSystemError() const;

	/////////////////////////////////////////////////////////////////////////
	// Make all the matrix elements to be changed into absolute value
	//
	CMatrix AbsoluteValue();

	/////////////////////////////////////////////////////////////////////////
	// Parameter:
	//		CMatrix& cMatrix:		������������Դ
	//		unsigned int nIndex:	�������������ڶ����еĿ�ʼ����λ��
	// Purpose:
	//		This function will copy all the data of the cMatrix
	// Notes:
	//		The object must be column vector!!!
	//
	void GetMatrixData(CMatrix& cMatrix, unsigned int nIndex);

	/////////////////////////////////////////////////////////////////////////
	// Parameter:
	//		CMatrix& cMatrix:		�����ľ���
	//		unsigned int nIndex:	�������������ڶ����еĿ�ʼ����λ��
	// Purpose:
	//		This function will copy part of the object data into cMatrix
	// Notes:
	//		The object must be column vector!!!
	//
	void SetMatrixData(CMatrix& cMatrix, unsigned int nIndex);

	/////////////////////////////////////////////////////////////////////////
	// Parameter:
	//		CMatrix& cMatrix:		������������Դ
	//		unsigned int nIndex:	�������������ڶ����еĿ�ʼ����λ��
	//		unsigned int nRow:		�������������ڱ����������е�������(��0��ʼ)
	// Purpose:
	//		This function will copy all the data of the cMatrix
	// Notes:
	//		The object must be column vector!!!
	//
	void GetMatrixRowData(CMatrix& cMatrix, unsigned int nIndex, unsigned int nRow);

	/////////////////////////////////////////////////////////////////////////
	// Parameter:
	//		CMatrix& cMatrix:		�����ľ���
	//		unsigned int nIndex:	�������������ڶ����еĿ�ʼ����λ��
	//		unsigned int nRow:		�����������ڱ��������е�������
	// Purpose:
	//		This function will copy part of the object data to fill the special 
	// row of the cMatrix
	//	Notes:
	//		The object must be column vector!!!
	//
	void SetMatrixRowData(CMatrix& cMatrix, unsigned int nIndex, unsigned int nRow);

	/////////////////////////////////////////////////////////////////////////
	// Get the total value of the matrix
	double GetTotalElementValue();

	/////////////////////////////////////////////////////////////////////////
	// �Ծ��������չ
	//	ʵ�ֹ���:
	//		�Ծ��������������չ,nTimes��ÿ����չ�Ĵ���
	//
	void nncpyi(CMatrix &cMatrix, unsigned int nTimes);

	/////////////////////////////////////////////////////////////////////////
	// �Ծ��������չ
	//	ʵ�ֹ���:
	//		�Ծ��������������չ
	//	matrix =	[ 
	//			1	2	3
	//			4	5	6
	//			7	8	9
	//				]
	//
	//		nncpyd(matrix)	=	[
	//			1	0	0	2	0	0	3	0	0
	//			0	4	0	0	5	0	0	6	0
	//			0	0	7	0	0	8	0	0	9
	//							]
	void nncpyd(CMatrix &cMatrix);

	/////////////////////////////////////////////////////////////////////////
	// �Ծ��������չ
	//	ʵ�ֹ���:
	//		�Ծ��������������չ,nTimes��ÿ����չ�Ĵ���
	//	matrix =	[ 
	//			1	2	3
	//			4	5	6
	//			7	8	9
	//				]
	//		nTimes = 2
	//
	//		nncpyd(matrix)	=	[
	//					1	2	3	1	2	3
	//					4	5	6	4	5	6
	//					7	8	9	7	8	9
	//							]
	//
	void nncpy (CMatrix& cMatrix, unsigned int nTimes);

	/////////////////////////////////////////////////////////////////////////
	// ���þ����������
	//
	void SetMatrixRowAndCol(unsigned int nRow,unsigned int nCol);

private:

	unsigned int m_nRow;			// ������ӵ�е�����
	unsigned int m_nCol;			// ������ӵ�е�����


	/////////////////////////////////////////////////////////////////////////
	// ע��:
	//		���������þ����������������,�����е�Ԫ�ر����³�ʼ��Ϊ0
	/////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////
	// ���þ��������
	//
	void SetMatrixRowNumber(unsigned int nRow);

	/////////////////////////////////////////////////////////////////////////
	// ���þ��������
	//
	void SetMatrixColNumber(unsigned int nCol);


	/////////////////////////////////////////////////////////////////////////
	// �������������
	//
	void SwapMatrixRow(unsigned int nRow1,unsigned int nRow2);

	/////////////////////////////////////////////////////////////////////////
	// �������������
	//
	void SwapMatrixCol(unsigned int nCol1,unsigned int nCol2);


};


/////////////////////////////////////////////////////////////////////////////
// overload operator 'double - CMatrix'
__declspec(dllimport) CMatrix operator - (double nValue,CMatrix& cMatrixB);

/////////////////////////////////////////////////////////////////////////
// ����ϲ������
//	�ϲ�����:
//		1. ����ϲ��������������������������;
//		2. ����ϲ�������������������Բ����;
//		3. �ϲ��󷵻صľ�������������ϲ��ľ�����������,�����ǲ���ϲ���
//			�������������֮��;
//
__declspec(dllimport) CMatrix MergeMatrix(CMatrix& cMatrixA,CMatrix& cMatrixB);



#endif // _MATRIX_H