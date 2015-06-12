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

	TMatrix	m_pTMatrix;				// 指向矩阵的头指针

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
	//		[out]	cMatrix ----> 矩阵的子矩阵返回的结果
	//		[in]	nStartX ----> 子矩阵在矩阵中的起始坐标,对应行,索引从1开始
	//		[in]	nStartY ----> 子矩阵在矩阵中的起始坐标,对应列,索引从1开始
	//
	void CopySubMatrix(CMatrix& cMatrix,unsigned int nStartX,unsigned int nStartY);

	/////////////////////////////////////////////////////////////////////////
	// Copy Matrix
	//	Parameter:
	//		[in]	cMatrix ----> be copied matrix
	//
	void CopyMatrix(CMatrix& cMatrix);

	/////////////////////////////////////////////////////////////////////////
	// 将矩阵的所有的元素按列合成一列
	//	例如：
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
	// 对矩阵中所有的元素进行一次非线性变换:
	//		变换后的值y与变换前的值的关系是:
	//			y = f(x) = 1 / (1 + exp(-x))	( 0 < f(x) < 1)
	//
	CMatrix Sigmoid();

	/////////////////////////////////////////////////////////////////////////
	// 对矩阵中所有的元素进行一次非线性变换:
	//		变换后的值y与变换前的值的关系是:
	//			y = f'(x) = (1 / (1 + exp(-x)))'	( 0 < f(x) < 1)
	//			  = exp(-x)/((1 + exp(-x))*(1 + exp(-x)))
	//
	CMatrix SigmoidDerivative();


	/////////////////////////////////////////////////////////////////////////
	// 对矩阵中所有的元素进行一次非线性变换:
	//		变换后的值y与变换前的值的关系是:
	//			y = tanh(x) = (1 - exp(-x)) / (1 + exp(-x))
	//					 =  1 - 2 * exp(-x) / (1 + exp(-x))	( -1 < f(x) < 1)
	//
	CMatrix tanh(); 

	/////////////////////////////////////////////////////////////////////////
	// 对矩阵中所有的元素进行一次非线性变换:
	//		变换后的值y与变换前的值的关系是:
	//			y = tanh'(x) = ((1 - exp(-x)) / (1 + exp(-x)))'	( -1 < f(x) < 1)
	//					 = 	2*exp(-x)/((1 + exp(-x))*(1 + exp(-x)))
	//
	CMatrix tanhDerivative();

	/////////////////////////////////////////////////////////////////////////
	// 对矩阵中所有的元素进行一次非线性变换:
	//		变换后的值y与变换前的值的关系是:
	//			y = Tansig(x) = 2 / (1 + exp(-2 * x)) -1
	//
	CMatrix Tansig();

	/////////////////////////////////////////////////////////////////////////
	// 对矩阵中所有的元素进行一次非线性变换:
	//		变换后的值y与变换前的值的关系是:
	//			y = Tansig'(x) = (2 / (1 + exp(-2 * x)) -1)'
	//				= (2 / (1 + exp(-2 * x)) -1) * (2 / (1 + exp(-2 * x)) -1) -1
	//
	CMatrix TansigDerivative();

	/////////////////////////////////////////////////////////////////////////
	// 对矩阵中的元素进行一次操作:
	//		使所有行中的相对应的列元素相等
	//	Parameter:
	//		nRowIndex	---->	行索引值(the index starts from 0)
	//							以此行做为标准,使矩阵中其余的行相对应的列的值
	//							与此行相对应的列的值相等	
	//
	void MakeAllColumnElementsSameValue(unsigned int nRowIndex);

	/////////////////////////////////////////////////////////////////////////
	// 对矩阵中的元素进行一次操作:
	//		使矩阵变为单位阵
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
	//		CMatrix& cMatrix:		被拷贝的数据源
	//		unsigned int nIndex:	被拷贝的数据在对象中的开始索引位置
	// Purpose:
	//		This function will copy all the data of the cMatrix
	// Notes:
	//		The object must be column vector!!!
	//
	void GetMatrixData(CMatrix& cMatrix, unsigned int nIndex);

	/////////////////////////////////////////////////////////////////////////
	// Parameter:
	//		CMatrix& cMatrix:		被填充的矩阵
	//		unsigned int nIndex:	被拷贝的数据在对象中的开始索引位置
	// Purpose:
	//		This function will copy part of the object data into cMatrix
	// Notes:
	//		The object must be column vector!!!
	//
	void SetMatrixData(CMatrix& cMatrix, unsigned int nIndex);

	/////////////////////////////////////////////////////////////////////////
	// Parameter:
	//		CMatrix& cMatrix:		被拷贝的数据源
	//		unsigned int nIndex:	被拷贝的数据在对象中的开始索引位置
	//		unsigned int nRow:		被拷贝的数据在被拷贝对象中的行索引(从0开始)
	// Purpose:
	//		This function will copy all the data of the cMatrix
	// Notes:
	//		The object must be column vector!!!
	//
	void GetMatrixRowData(CMatrix& cMatrix, unsigned int nIndex, unsigned int nRow);

	/////////////////////////////////////////////////////////////////////////
	// Parameter:
	//		CMatrix& cMatrix:		被填充的矩阵
	//		unsigned int nIndex:	被拷贝的数据在对象中的开始索引位置
	//		unsigned int nRow:		被填充的数据在被填充对象中的行索引
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
	// 对矩阵进行拓展
	//	实现功能:
	//		对矩阵的列数进行拓展,nTimes是每列拓展的次数
	//
	void nncpyi(CMatrix &cMatrix, unsigned int nTimes);

	/////////////////////////////////////////////////////////////////////////
	// 对矩阵进行拓展
	//	实现功能:
	//		对矩阵的列数进行拓展
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
	// 对矩阵进行拓展
	//	实现功能:
	//		对矩阵的列数进行拓展,nTimes是每列拓展的次数
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
	// 设置矩阵的行列数
	//
	void SetMatrixRowAndCol(unsigned int nRow,unsigned int nCol);

private:

	unsigned int m_nRow;			// 矩阵所拥有的行数
	unsigned int m_nCol;			// 矩阵所拥有的列数


	/////////////////////////////////////////////////////////////////////////
	// 注意:
	//		在重新设置矩阵的行数和列数后,矩阵中的元素被重新初始化为0
	/////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////
	// 设置矩阵的行数
	//
	void SetMatrixRowNumber(unsigned int nRow);

	/////////////////////////////////////////////////////////////////////////
	// 设置矩阵的列数
	//
	void SetMatrixColNumber(unsigned int nCol);


	/////////////////////////////////////////////////////////////////////////
	// 交换矩阵的两行
	//
	void SwapMatrixRow(unsigned int nRow1,unsigned int nRow2);

	/////////////////////////////////////////////////////////////////////////
	// 交换矩阵的两列
	//
	void SwapMatrixCol(unsigned int nCol1,unsigned int nCol2);


};


/////////////////////////////////////////////////////////////////////////////
// overload operator 'double - CMatrix'
__declspec(dllimport) CMatrix operator - (double nValue,CMatrix& cMatrixB);

/////////////////////////////////////////////////////////////////////////
// 矩阵合并运算符
//	合并规则:
//		1. 参与合并运算的两个矩阵的行数必须相等;
//		2. 参与合并的两个矩阵的列数可以不相等;
//		3. 合并后返回的矩阵的行数与参与合并的矩阵的行数相等,列数是参与合并的
//			两个矩阵的列数之和;
//
__declspec(dllimport) CMatrix MergeMatrix(CMatrix& cMatrixA,CMatrix& cMatrixB);



#endif // _MATRIX_H