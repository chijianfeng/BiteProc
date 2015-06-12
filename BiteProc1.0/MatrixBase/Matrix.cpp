/////////////////////////////////////////////////////////////////////////////
// Matrix.cpp : Implementation of the class Matrix
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Matrix.h"
#include <math.h>
#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include <sstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMatrix::CMatrix()
{
	m_nRow = 0;
	m_nCol = 0;

	m_pTMatrix.resize (m_nRow);
	for(unsigned int i=0; i < m_nRow; i++)
	{
		for(unsigned int j=0; j < m_nCol; j++)
		{
			m_pTMatrix[i].resize (m_nCol);
			m_pTMatrix[i][j] = (double) 0;
		}
	}
}


CMatrix::~CMatrix()
{	

}


CMatrix::CMatrix(unsigned int nRow,unsigned int nCol)
{
	// 动态分配二维数组
	TMatrix tMatrix;
	tMatrix.resize (nRow);

	for(unsigned int i=0; i < nRow; i++)
	{
		for(unsigned int j=0; j < nCol; j++)
		{
			tMatrix[i].resize(nCol);
			tMatrix[i][j] = (double) 0;
		}
	}

	// 对对象变量赋值
	m_nRow	= nRow;
	m_nCol	= nCol;
	m_pTMatrix = tMatrix;

}


CMatrix::CMatrix(CMatrix& cMatrixB)
{
	// Initialize the variable
	m_nRow = cMatrixB.m_nRow ;
	m_nCol = cMatrixB.m_nCol ;
	m_pTMatrix = cMatrixB.m_pTMatrix ;

	// Copy Data
	for(unsigned int i=0; i< cMatrixB.m_nRow; i++)
	{
		for(unsigned int j=0; j < cMatrixB.m_nCol; j++)
		{
			m_pTMatrix [i][j] = cMatrixB.m_pTMatrix [i][j];
		}
	}

}


/////////////////////////////////////////////////////////////////////////////
// CMatrix member functions
//

CMatrix CMatrix::operator +(CMatrix& cMatrixB)
{
	// 要满足矩阵相加的条件: 行列数目相等!
	if(m_nRow != cMatrixB.m_nRow || m_nCol != cMatrixB.m_nCol )
	{
		::AfxMessageBox (TEXT("执行相加的两个矩阵维数不相等!"),MB_OK | MB_ICONERROR);
	}

	CMatrix	cMatrix = *this;

	for(unsigned int i=0; i < m_nRow; i++)
	{
		for(unsigned int j=0; j < m_nCol; j++)
		{
			cMatrix.m_pTMatrix [i][j] = m_pTMatrix [i][j] + cMatrixB.m_pTMatrix [i][j];
		}
	}
	return	cMatrix;
}


CMatrix CMatrix::operator -(CMatrix& cMatrixB)
{
	// 要满足矩阵相加的条件: 行列数数目相等!
	if(m_nRow != cMatrixB.m_nRow || m_nCol != cMatrixB.m_nCol )
	{
		::AfxMessageBox (TEXT("执行相减的两个矩阵维数不相等!"),MB_OK | MB_ICONERROR);
	}

	CMatrix cMatrix = *this;

	for(unsigned int i=0; i < m_nRow; i++)
	{
		for(unsigned int j=0; j < m_nCol; j++)
		{
			cMatrix.m_pTMatrix [i][j] = m_pTMatrix [i][j] - cMatrixB.m_pTMatrix [i][j];
		}
	}
	return	cMatrix;
}


CMatrix CMatrix::operator *(CMatrix& cMatrixB)
{
	if( m_nCol != cMatrixB.m_nRow )
	{
		::AfxMessageBox (TEXT("执行相乘的两个矩阵维数不满足相乘的条件!"),MB_OK | MB_ICONERROR);
	}

	CMatrix cResultMatrix(m_nRow,cMatrixB.m_nCol);

	for(unsigned int i=0; i < m_nRow; i++)
	{
		for(unsigned int j=0; j < cMatrixB.m_nCol; j++)
		{
			for(unsigned int m=0; m < m_nCol; m++)
			{
				cResultMatrix.m_pTMatrix [i][j] +=  m_pTMatrix [i][m] * cMatrixB.m_pTMatrix [m][j];
			}
		}
	}

	return cResultMatrix;
}


CMatrix CMatrix::operator * (double nValue)
{
	CMatrix cMatrix = *this;

	for(unsigned int i=0; i < m_nRow; i++)
	{
		for(unsigned int j=0; j < m_nCol; j++)
		{
			cMatrix.m_pTMatrix [i][j] =m_pTMatrix [i][j] * nValue;
		}
	}

	return cMatrix;
}


CMatrix& CMatrix::operator =(CMatrix& cMatrixB)
{
	if( (m_nRow != cMatrixB.m_nRow) || (m_nCol != cMatrixB.m_nCol) )
	{
		::AfxMessageBox(TEXT("等号左右两边的矩阵的维数不相等!"),MB_OK | MB_ICONERROR);
		return *this;	// return invalid value
	}	

	// 给变量赋值
	m_nRow = cMatrixB.m_nRow ;
	m_nCol = cMatrixB.m_nCol ;
	m_pTMatrix = cMatrixB.m_pTMatrix ;

	// 赋值操作
	for(unsigned int i=0; i < cMatrixB.m_nRow; i++)
	{
		for(unsigned int j=0; j< cMatrixB.m_nCol; j++)
		{
			m_pTMatrix [i][j] = cMatrixB.m_pTMatrix [i][j];
		}
	}

	return *this;
}


CMatrix& CMatrix::operator += (CMatrix& cMatrixB)
{
	if(m_nRow != cMatrixB.m_nRow || m_nCol != cMatrixB.m_nCol )
	{
		//printf("错误!执行相加的两个矩阵维数不相等!\n");
		::AfxMessageBox (TEXT("运算符的两边矩阵的维数不相等!"),MB_OK | MB_ICONERROR);
		return *this;	// return invalid value
	}

	// 赋值操作
	for(unsigned int i=0; i < cMatrixB.m_nRow; i++)
	{
		for(unsigned int j=0; j< cMatrixB.m_nCol; j++)
		{
			m_pTMatrix [i][j] += cMatrixB.m_pTMatrix [i][j];
		}
	}

	return *this;

}


CMatrix CMatrix::Transpose()
{
	CMatrix cMatrix(m_nCol,m_nRow);

	for(unsigned int i=0; i < m_nRow; i++)
	{
		for(unsigned int j=0; j < m_nCol; j++)
		{
			cMatrix.m_pTMatrix [j][i] = m_pTMatrix [i][j];
		}
	}

	return cMatrix;
}

/////////////////////////////////////////////////////////////////////////////
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
/////////////////////////////////////////////////////////////////////////////

CMatrix CMatrix::MergeColumnsToColumnVector()
{
	CMatrix cMatrix(m_nRow * m_nCol,(unsigned int)1);

	// 对矩阵赋值
	for(unsigned int j=0; j < m_nCol; j++)
	{
		for(unsigned int i=0; i < m_nRow; i++)
		{
			cMatrix.m_pTMatrix [i + j * m_nRow][(unsigned int)0] = m_pTMatrix [i][j];
		}
	}

	return cMatrix;

}

/////////////////////////////////////////////////////////////////////////////
// Get the total value of the matrix
/////////////////////////////////////////////////////////////////////////////

double CMatrix::GetTotalElementValue()
{
	double	nTotalValue = 0;

	for(unsigned int i=0; i < m_nRow; i++)
	{
		for( unsigned int j=0; j < m_nCol; j++)
		{
			nTotalValue += m_pTMatrix [i][j];
		}
	}

	return nTotalValue;
}

/////////////////////////////////////////////////////////////////////////////
// Get System Error
/////////////////////////////////////////////////////////////////////////////

double	CMatrix::GetSystemError() const
{
	double	nSystemError = 0;

	for(unsigned int i=0; i < m_nRow; i++)
	{
		for( unsigned int j=0; j < m_nCol; j++)
		{
			nSystemError += m_pTMatrix [i][j] * m_pTMatrix [i][j];
		}
	}

	return nSystemError;

}

/////////////////////////////////////////////////////////////////////////////
// Make all the matrix elements to be changed into absolute value
/////////////////////////////////////////////////////////////////////////////

CMatrix CMatrix::AbsoluteValue ()
{
	CMatrix cMatrix = *this;

	for(unsigned int i=0; i < m_nRow; i++)
	{
		for(unsigned int j=0; j < m_nCol; j++)
		{
			cMatrix.m_pTMatrix [i][j] = fabs( m_pTMatrix [i][j]);

		}

	}

	return cMatrix;

}


CMatrix CMatrix::Inverse()
{
	/////////////////////////////////////////////////////////////////////////
	// Using Gauss - Jordan Method
	// 参考书目: 计算机数值方法 --->施吉林 陈桂枝
	/////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////
	// 判断是否是可逆阵:
	//		可逆阵一定是方阵!!!

	if ( m_nRow != m_nCol)
	{
		//printf("错误!矩阵的行列数不相等,是非可逆阵!\n");
		::AfxMessageBox (TEXT("矩阵的行列数不相等,是非可逆阵!"),MB_OK | MB_ICONERROR);
	}

	CMatrix cMatrix = *this;

	//***********************************************************************
	// 思路:(非常规思维!)
	//		动态分配整型数组(2*m_nCol)来存储每次交换的行坐标的值
	//		不论有没有行交换都记录在数组中,
	//		1.没进行行交换的两个数据相等,在SwapMatrixRow()函数中
	//		检测到两个值相等立即返回,在SwapMatrixCol()函数中也一样,
	//		检测到两个值相等立即返回,不占用系统资源;
	//		2.不相等的就交换
	//***********************************************************************

	//	分配内存
	int *pIntArray = new int [2*m_nCol];

	// nSetp --- 约化步数,按列展开	
	for(unsigned int k=0; k < cMatrix.m_nCol; k++)
	{
		/////////////////////////////////////////////////////////////////////
		// 进行行交换 ---> 游戏规则:
		// 为保证计算过程的数值稳定性,在第k步约化时,先在{a(ik)}|i=k->n中选按
		// 模最大者作为约化主元素,并交换矩阵相应的行

		// 标记主元素
		double nMaxElement = cMatrix.m_pTMatrix [k][k];
		// 标记主元素所在的行数
		unsigned int nMainRow = k;

		for(unsigned int nCount = k+1; nCount < cMatrix.m_nCol; nCount++)
		{
			if( fabs(nMaxElement) < fabs(cMatrix.m_pTMatrix [nCount][k]) )
			{
				nMaxElement = cMatrix.m_pTMatrix [nCount][k];
				nMainRow = nCount;
			}
		}

		// 将欲交换的行数存在数组中
		pIntArray [2*k] = k;
		pIntArray [2*k+1] = nMainRow;


		// 交换行
		cMatrix.SwapMatrixRow(k,nMainRow);

		//Display();

		//	判断是否是可逆阵
		if(cMatrix.m_pTMatrix [k][k] == 0)
		{
			//printf("错误!此矩阵为非可逆阵!\n");
			::AfxMessageBox (TEXT("此矩阵为非可逆阵,没有逆矩阵!"),MB_OK | MB_ICONERROR);
		}

		cMatrix.m_pTMatrix [k][k] = 1/(cMatrix.m_pTMatrix [k][k]);


		// 算主列
		for(unsigned int i=0; i < cMatrix.m_nRow; i++)
		{	
			if( i != k)
				cMatrix.m_pTMatrix [i][k] = -(cMatrix.m_pTMatrix [k][k]) * (cMatrix.m_pTMatrix [i][k]); 

			//int nTempValue = m_pTMatrix [i][k];

		}

		//printf("\n");

		// 约化非主行
		for(unsigned int m=0; m < cMatrix.m_nRow; m++)
		{
			if ( m == k)
				continue;

			for(unsigned int n=0; n < cMatrix.m_nCol; n++)
			{
				if ( n == k)
					continue;

				cMatrix.m_pTMatrix [m][n] += cMatrix.m_pTMatrix [m][k] * cMatrix.m_pTMatrix [k][n];

				//printf("%10f ",m_pTMatrix [m][n]);

			}

			//printf("\n");

		}

		// 算主行
		for(unsigned int j=0; j < cMatrix.m_nCol; j++)
		{
			if( j != k)
				cMatrix.m_pTMatrix [k][j] = (cMatrix.m_pTMatrix [k][k]) * (cMatrix.m_pTMatrix [k][j]);

		}

	}


	/////////////////////////////////////////////////////////////////////////
	// 进行列交换 ---> 对交换行后的矩阵进行列交换 ---> 还原矩阵
	// 游戏规则:
	// 将开始矩阵中进行的行交换 ---> 现用相对应的列交换进行还原,即可得到所求的
	// 逆矩阵

	for(int i=2*m_nCol-1; i > 0; i--)
	{
		cMatrix.SwapMatrixCol(pIntArray[i],pIntArray[i-1]);
		i--;
	}

	delete []pIntArray;

	return cMatrix;

}


void CMatrix::SwapMatrixRow(unsigned int nRow1,unsigned int nRow2)
{
	if( nRow1 == nRow2)
		return;

	double *pArray = new double;

	for(unsigned int i=0; i < m_nCol; i++)
	{
		// Swap the datum of the two rows
		pArray[0] = m_pTMatrix [nRow1][i];
		m_pTMatrix [nRow1][i] = m_pTMatrix [nRow2][i];
		m_pTMatrix [nRow2][i] = pArray[0];
	}

	delete pArray;
}


void CMatrix::SwapMatrixCol(unsigned int nCol1,unsigned int nCol2)
{
	if( nCol1 == nCol2)
		return;

	double *pArray = new double;
	for(unsigned int i=0; i < m_nRow; i++)
	{
		// Swap the datum of the two columns
		pArray[0] = m_pTMatrix [i][nCol1];
		m_pTMatrix [i][nCol1] = m_pTMatrix [i][nCol2];
		m_pTMatrix [i][nCol2] = pArray[0];
	}

	delete pArray;
}


bool CMatrix::LoadDataFromFile(CString& strFileName)
{
	CStdioFile dataFile;
	LPCTSTR	lpszFileName = _T("");

	// CString convert to LPCTSTR
	strFileName.TrimLeft ();
	strFileName.TrimRight ();
	//strFileName.Format (lpszFileName);

	lpszFileName = (LPCTSTR)strFileName;

	if(!dataFile.Open (lpszFileName,CFile::modeRead | CFile::typeText))
	{
		::AfxMessageBox (TEXT("不能打开要读取数据的文件!"),MB_OK | MB_ICONERROR);
		dataFile.Close ();
		return FALSE;
	}

	// 用来存储提取文本文件中一行的数据
	CString	strData;				 
	// 用来记录文本文件中一共有多少行数据?
	unsigned int	nRow = 0;		
	stringstream ss;         //used for reading the data...
	/////////////////////////////////////////////////////////////////////////
	// Step 1: 得到文件的行列数目并根据文本文件的行列数目来设置对象(矩阵)的行
	// 列数目
	//

	while(dataFile.ReadString (strData) != FALSE)
	{
		++nRow;
	}

	// 根据文本文件的数据的行数设置对象(矩阵)的行数
	m_nRow = nRow;
	SetMatrixRowNumber(m_nRow);

	// 重新定位当前文件指针到文件开头
	dataFile.SeekToBegin ();

	dataFile.ReadString (strData);
	strData.TrimLeft ();
	strData.TrimRight ();

	TCHAR	SPACE_CHARACTER = ' ';
	TCHAR   SPACE_TAB = '	';
	// 用来记录文本文件中一行有多少列?
	unsigned int	nCol = 0;						

	// 空格符在字符串中的位置索引
	int nIndex = 0;
	do
	{
		nIndex = strData.Find (SPACE_CHARACTER);
		if(nIndex==-1)
			nIndex = strData.Find(SPACE_TAB);
		//ss<<strData;
		//ss.
		++nCol;

		// 提取字符串的子字符串,即提取一个double型实数数据
		//CString strDoubleNumber = strData.Left (nIndex);	

		// 将字符串转换为double型实数
		//double RealNumber = atof(strDoubleNumber);

		//int nTempNumber = strData.GetLength ();

		strData = strData.Right (strData.GetLength () - nIndex -1);

		// 去掉多余的空格
		strData.TrimLeft (SPACE_TAB);

		// Use for debugging
		//int nTempNum = strData.GetLength ();

	}while(nIndex != -1);

	// 根据文本文件的数据的列数设置对象(矩阵)的列数
	m_nCol = nCol;
	SetMatrixColNumber(m_nCol);

	// End of Getting the Rows and Cols of the Text File
	/////////////////////////////////////////////////////////////////////////


	/////////////////////////////////////////////////////////////////////////
	// Step 2: 根据文本文件中的数据对矩阵赋值,并检测每行的列数是否和第一行的
	// 列数相等,不相等提示出错信息
	//

	// 重新定位当前文件指针到文件开头
	dataFile.SeekToBegin ();

	// 对矩阵中的元素装入文本文件的数据
	for(unsigned int i=0; i < m_nRow; i++)
	{
		dataFile.ReadString (strData);
		strData.TrimLeft ();
		strData.TrimRight ();

		// 验证每行的列数是否与第一行的列数相等
		unsigned int nVerifyColNum = 0;

		do
		{
			nIndex = strData.Find (SPACE_CHARACTER);
			if(nIndex==-1)
				nIndex = strData.Find(SPACE_TAB);
			++nVerifyColNum;

			if(nIndex != -1)
			{
				// 提取字符串的子字符串,即提取一个double型实数数据
				CString strDoubleNumber = strData.Left (nIndex);	

				// 将字符串转换为double型实数
				string s;
				s = CStringtoString(strDoubleNumber);
				double RealNumber = atof(s.c_str());
				

				m_pTMatrix [i][nVerifyColNum - 1] = RealNumber;
				//int nTempNumber = strData.GetLength ();

				strData = strData.Right (strData.GetLength () - nIndex -1);

				// 去掉多余的空格
				strData.TrimLeft (SPACE_TAB);
				strData.TrimLeft(SPACE_CHARACTER);
				// Using for debugging
				//double nReadNumber = m_pTMatrix [i][nVerifyColNum - 1];

				// Using for debugging
				//int nTempNum = strData.GetLength ();
			}
			else
			{
				string s;
				s = CStringtoString(strData);
				double RealNumber = atof(s.c_str());
				

				m_pTMatrix [i][nVerifyColNum - 1] = RealNumber;
			}

		}while(nIndex != -1);

		if(nVerifyColNum != m_nCol)
		{
			CString strRowNumber;
			strRowNumber.Format(_T("%d"),i + 1);

			CString strColNumber;
			strColNumber.Format(_T("%d"),m_nCol);

			CString strVerifyColNumber;
			strVerifyColNumber.Format(_T("%d"),nVerifyColNum);

			CString strMessage =  CString(TEXT("文本文件第")) + strRowNumber + CString(TEXT("行一共有")) + strVerifyColNumber + CString(TEXT("列,与第一行中的列数")) + strColNumber + CString(TEXT("不相等!"));

			LPCTSTR lpszText = _T("");
			lpszText = (LPCTSTR)strMessage;

			//strMessage.FormatMessage (lpszText);
			//::AfxMessageBox (lpszText,MB_OK);


			::AfxMessageBox (lpszText,MB_OK | MB_ICONERROR);
			dataFile.Close ();
			return false;
		}


	}


	dataFile.Close ();

	return TRUE;
}

bool CMatrix::LoadDataFromBuf(const double* pbuf, const int length)
{
	if(length!=m_nCol*m_nRow||pbuf==NULL)
		return false;
	for (int i=0;i<m_nRow;i++)
	{
		for(int j=0;j<m_nCol;j++)
		{
			m_pTMatrix[i][j] = pbuf[i*m_nCol+j];
		}
	}
	return true;
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
/////////////////////////////////////////////////////////////////////////////

bool CMatrix::LoadDataFromFileSpecial (CString& strFileName,
	CMatrix& cMatrixInputToHideWeightValue,
	CMatrix& cMatrixHideLayerValveValue,
	CMatrix& cMatrixHideToOutputWeightValue,
	CMatrix& cMatrixOutputLayerValveValue,
	unsigned int &nInputLayerNumber,
	unsigned int &nHideLayerNumber,
	unsigned int &nOutputLayerNumber,
	int &nComboArithmetic,
	int	&nComboFunc)
{
	CStdioFile dataFile;
	LPCTSTR	lpszFileName = _T("");

	// CString convert to LPCTSTR
	strFileName.TrimLeft ();
	strFileName.TrimRight ();

	lpszFileName = (LPCTSTR)strFileName;

	TRY 
	{
		dataFile.Open(lpszFileName, CFile::modeRead | CFile::typeText);
	}
	CATCH (CFileException, e)
	{
			::AfxMessageBox(TEXT("不能打开要读取的数据文件!!!"), MB_OK | MB_ICONERROR);
			dataFile.Close();
			return FALSE;
	}
	END_CATCH
	

	// 提取网络参数
	dataFile.SeekToBegin ();

	// 用来存储提取文本文件中一行的数据
	CString	strData;	
	// 注释标志符号
	TCHAR cFirstCharacter = '#';
	// 空格符号
	TCHAR SPACE_CHARACTER = ' ';
	TCHAR SPACE_TAB = '	';
	// 空格符在字符串中的位置索引
	int nIndex = 0;

	// 读取文件中的网络的输入层数目
	while(dataFile.ReadString (strData))
	{
		strData.TrimLeft ();
		strData.TrimRight ();

		if((strData.Find (cFirstCharacter)) == 0)
		{
			continue;
		}
		else
		{
			string s;
			s = CStringtoString(strData);
			nInputLayerNumber = (unsigned int)atoi(s.c_str());

			break;

		}

	}

	// 读取文件中的网络的隐含层数目
	while(dataFile.ReadString (strData))
	{
		strData.TrimLeft ();
		strData.TrimRight ();

		if((strData.Find (cFirstCharacter)) == 0)
		{
			continue;
		}
		else
		{
			string s;
			s = CStringtoString(strData);
			nHideLayerNumber = (unsigned int)atoi(s.c_str());

			break;
		}

	}

	// 读取文件中的网络的输出层数目
	while(dataFile.ReadString (strData))
	{
		strData.TrimLeft ();
		strData.TrimRight ();

		if((strData.Find (cFirstCharacter)) == 0)
		{
			continue;
		}
		else
		{
			
			LPCSTR strOutputLayerNumber =  (LPCSTR)strData.GetBuffer(0);
			strData.ReleaseBuffer();

			nOutputLayerNumber = (unsigned int)atoi(strOutputLayerNumber);

			break;
		}

	}

	// 读取文件中的训练网络所使用的算法的索引值
	while(dataFile.ReadString (strData))
	{
		strData.TrimLeft ();
		strData.TrimRight ();

		if((strData.Find (cFirstCharacter)) == 0)
		{
			continue;
		}
		else
		{

			LPCSTR strComboArithmetic = (LPCSTR)strData.GetBuffer(0);
			strData.ReleaseBuffer();

			nComboArithmetic = atoi(strComboArithmetic);

			break;
		}

	}

	// 读取文件中的网络的使用的函数的索引值
	while(dataFile.ReadString (strData))
	{
		strData.TrimLeft ();
		strData.TrimRight ();

		if((strData.Find (cFirstCharacter)) == 0)
		{
			continue;
		}
		else
		{
			
			LPCSTR strComboFunc =  (LPCSTR)strData.GetBuffer(0);
			strData.ReleaseBuffer();

			nComboFunc = atoi(strComboFunc);

			break;
		}

	}


	// 根据网络参数设置输入层到隐含层的权值矩阵的行列数
	cMatrixInputToHideWeightValue.SetMatrixRowAndCol (nHideLayerNumber,nInputLayerNumber);

	// 验证矩阵的行数
	unsigned int nVerifyRowNum = 0;

	// 读取文件中的输入层到隐含层的权值矩阵到相应的矩阵对象中
	while(dataFile.ReadString (strData))
	{
		strData.TrimLeft ();
		strData.TrimRight ();

		if((strData.Find (cFirstCharacter)) == 0)
		{
			continue;
		}
		else
		{

			// 装入文本文件的数据到相对应的矩阵中

			strData.TrimLeft ();
			strData.TrimRight ();

			// 验证每行的列数是否与第一行的列数相等
			unsigned int nVerifyColNum = 0;

			do
			{
				nIndex = strData.Find (SPACE_CHARACTER);

				++nVerifyColNum;

				if(nIndex != -1)
				{
					// 提取字符串的子字符串,即提取一个double型实数数据
					CString strDoubleNumber = strData.Left (nIndex);	

					// 将字符串转换为double型实数

					string s;
					s = CStringtoString(strDoubleNumber);
					double RealNumber = atof(s.c_str());

					cMatrixInputToHideWeightValue.m_pTMatrix [nVerifyRowNum][nVerifyColNum - 1] = RealNumber;

					strData = strData.Right (strData.GetLength () - nIndex -1);

					// 去掉多余的空格
					strData.TrimLeft ();

				}
				else
				{
					string s;
					s = CStringtoString(strData);
					double RealNumber = atof(s.c_str());

					cMatrixInputToHideWeightValue.m_pTMatrix [nVerifyRowNum][nVerifyColNum - 1] = RealNumber;
				}

			}while(nIndex != -1);

			if(nVerifyColNum != cMatrixInputToHideWeightValue.m_nCol)
			{
				CString strRowNumber;
				strRowNumber.Format(_T("%d"),nVerifyRowNum + 1);

				CString strColNumber;
				strColNumber.Format(_T("%d"),m_nCol);

				CString strVerifyColNumber;
				strVerifyColNumber.Format(_T("%d"),nVerifyColNum);

				CString strMessage =  CString(TEXT("文本文件第")) + strRowNumber + CString(TEXT("行一共有")) + strVerifyColNumber + CString(TEXT("列,与第一行中的列数")) + strColNumber + CString(TEXT("不相等!"));

				LPCTSTR lpszText = _T("");
				lpszText = (LPCTSTR)strMessage;

				::AfxMessageBox (lpszText,MB_OK | MB_ICONERROR);
				dataFile.Close ();
				return FALSE;
			}

			++nVerifyRowNum;

			if( nVerifyRowNum == cMatrixInputToHideWeightValue.m_nRow )
			{
				break;
			}

		}

	}

	// Using for debugging
	//CString strInputToHideWeightValue = TEXT("TempInputToHideWeightValue.txt");
	//cMatrixInputToHideWeightValue.SaveDataToFile (strInputToHideWeightValue);



	// 根据网络参数设置隐含层的阀值矩阵的行列数
	cMatrixHideLayerValveValue.SetMatrixRowAndCol (nHideLayerNumber,(unsigned int)1);

	nIndex = 0;
	nVerifyRowNum = 0;

	// 读取文件中的隐含层的阀值矩阵到相应的矩阵对象中
	while(dataFile.ReadString (strData))
	{
		strData.TrimLeft ();
		strData.TrimRight ();

		if((strData.Find (cFirstCharacter)) == 0)
		{
			continue;
		}
		else
		{

			// 装入文本文件的数据到相对应的矩阵中

			strData.TrimLeft ();
			strData.TrimRight ();

			// 验证每行的列数是否与第一行的列数相等
			unsigned int nVerifyColNum = 0;

			do
			{
				nIndex = strData.Find (SPACE_CHARACTER);

				++nVerifyColNum;

				if(nIndex != -1)
				{
					// 提取字符串的子字符串,即提取一个double型实数数据
					CString strDoubleNumber = strData.Left (nIndex);	

					// 将字符串转换为double型实数
					string s;
					s = CStringtoString(strDoubleNumber);
					double RealNumber = atof(s.c_str());

					cMatrixHideLayerValveValue.m_pTMatrix [nVerifyRowNum][nVerifyColNum - 1] = RealNumber;

					strData = strData.Right (strData.GetLength () - nIndex -1);

					// 去掉多余的空格
					strData.TrimLeft ();

				}
				else
				{
					string s;
					s = CStringtoString(strData);
					double RealNumber = atof(s.c_str());

					cMatrixHideLayerValveValue.m_pTMatrix [nVerifyRowNum][nVerifyColNum - 1] = RealNumber;
				}

			}while(nIndex != -1);

			if(nVerifyColNum != cMatrixHideLayerValveValue.m_nCol)
			{
				CString strRowNumber;
				strRowNumber.Format(_T("%d"),nVerifyRowNum + 1);

				CString strColNumber;
				strColNumber.Format(_T("%d"),m_nCol);

				CString strVerifyColNumber;
				strVerifyColNumber.Format(_T("%d"),nVerifyColNum);

				CString strMessage =  CString(TEXT("文本文件第")) + strRowNumber + CString(TEXT("行一共有")) + strVerifyColNumber + CString(TEXT("列,与第一行中的列数")) + strColNumber + CString(TEXT("不相等!"));

				LPCTSTR lpszText = _T("");
				lpszText = (LPCTSTR)strMessage;

				::AfxMessageBox (lpszText,MB_OK | MB_ICONERROR);
				dataFile.Close ();
				return FALSE;
			}

			++nVerifyRowNum;

			if( nVerifyRowNum == cMatrixHideLayerValveValue.m_nRow )
			{
				break;
			}

		}

	}

	// Using for debugging
	//CString strHideLayerValveValue = TEXT("TempHideLayerValveValue.txt");
	//cMatrixHideLayerValveValue.SaveDataToFile (strHideLayerValveValue);

	// 根据网络参数设置隐含层到输出层的权值矩阵的行列数
	cMatrixHideToOutputWeightValue.SetMatrixRowAndCol (nOutputLayerNumber,nHideLayerNumber);

	nIndex = 0;
	nVerifyRowNum = 0;

	// 读取文件中的隐含层到输出层的权值矩阵到相应的矩阵对象中
	while(dataFile.ReadString (strData))
	{
		strData.TrimLeft ();
		strData.TrimRight ();

		if((strData.Find (cFirstCharacter)) == 0)
		{
			continue;
		}
		else
		{

			// 装入文本文件的数据到相对应的矩阵中

			strData.TrimLeft ();
			strData.TrimRight ();

			// 验证每行的列数是否与第一行的列数相等
			unsigned int nVerifyColNum = 0;

			do
			{
				nIndex = strData.Find (SPACE_CHARACTER);

				++nVerifyColNum;

				if(nIndex != -1)
				{
					// 提取字符串的子字符串,即提取一个double型实数数据
					CString strDoubleNumber = strData.Left (nIndex);	

					// 将字符串转换为double型实数
					string s;
					s=  CStringtoString(strDoubleNumber);
					double RealNumber = atof(s.c_str());

					cMatrixHideToOutputWeightValue.m_pTMatrix [nVerifyRowNum][nVerifyColNum - 1] = RealNumber;

					strData = strData.Right (strData.GetLength () - nIndex -1);

					// 去掉多余的空格
					strData.TrimLeft ();

				}
				else
				{
					string s;
					s = CStringtoString(strData);
					double RealNumber = atof(s.c_str());

					cMatrixHideToOutputWeightValue.m_pTMatrix [nVerifyRowNum][nVerifyColNum - 1] = RealNumber;
				}

			}while(nIndex != -1);

			if(nVerifyColNum != cMatrixHideToOutputWeightValue.m_nCol)
			{
				CString strRowNumber;
				strRowNumber.Format(_T("%d"),nVerifyRowNum + 1);

				CString strColNumber;
				strColNumber.Format(_T("%d"),m_nCol);

				CString strVerifyColNumber;
				strVerifyColNumber.Format(_T("%d"),nVerifyColNum);

				CString strMessage =  CString(TEXT("文本文件第")) + strRowNumber + CString(TEXT("行一共有")) + strVerifyColNumber + CString(TEXT("列,与第一行中的列数")) + strColNumber + CString(TEXT("不相等!"));

				LPCTSTR lpszText = _T("");
				lpszText = (LPCTSTR)strMessage;

				::AfxMessageBox (lpszText,MB_OK | MB_ICONERROR);
				dataFile.Close ();
				return FALSE;
			}

			++nVerifyRowNum;

			if( nVerifyRowNum == cMatrixHideToOutputWeightValue.m_nRow )
			{
				break;
			}

		}

	}

	// Using for debugging
	//CString strHideToOutputWeightValue = TEXT("TempHideToOutputWeightValue.txt");
	//cMatrixHideToOutputWeightValue.SaveDataToFile (strHideToOutputWeightValue);

	// 根据网络参数设置输出层的阀值矩阵的行列数
	cMatrixOutputLayerValveValue.SetMatrixRowAndCol (nOutputLayerNumber,(unsigned int)1);

	nIndex = 0;
	nVerifyRowNum = 0;

	// 读取文件中的输出层的阀值矩阵到相应的矩阵对象中
	while(dataFile.ReadString (strData))
	{
		strData.TrimLeft ();
		strData.TrimRight ();

		if((strData.Find (cFirstCharacter)) == 0)
		{
			continue;
		}
		else
		{

			// 装入文本文件的数据到相对应的矩阵中

			strData.TrimLeft ();
			strData.TrimRight ();

			// 验证每行的列数是否与第一行的列数相等
			unsigned int nVerifyColNum = 0;

			do
			{
				nIndex = strData.Find (SPACE_CHARACTER);

				++nVerifyColNum;

				if(nIndex != -1)
				{
					// 提取字符串的子字符串,即提取一个double型实数数据
					CString strDoubleNumber = strData.Left (nIndex);	

					// 将字符串转换为double型实数
					string s;
					s = CStringtoString(strDoubleNumber);
					double RealNumber = atof(s.c_str());

					cMatrixOutputLayerValveValue.m_pTMatrix [nVerifyRowNum][nVerifyColNum - 1] = RealNumber;

					strData = strData.Right (strData.GetLength () - nIndex -1);

					// 去掉多余的空格
					strData.TrimLeft ();

				}
				else
				{
					string s = CStringtoString(strData);
					double RealNumber = atof(s.c_str());

					cMatrixOutputLayerValveValue.m_pTMatrix [nVerifyRowNum][nVerifyColNum - 1] = RealNumber;
				}

			}while(nIndex != -1);

			if(nVerifyColNum != cMatrixOutputLayerValveValue.m_nCol)
			{
				CString strRowNumber;
				strRowNumber.Format(_T("%d"),nVerifyRowNum + 1);

				CString strColNumber;
				strColNumber.Format(_T("%d"),m_nCol);

				CString strVerifyColNumber;
				strVerifyColNumber.Format(_T("%d"),nVerifyColNum);

				CString strMessage =  CString(TEXT("文本文件第")) + strRowNumber + CString(TEXT("行一共有")) + strVerifyColNumber + CString(TEXT("列,与第一行中的列数")) + strColNumber + CString(TEXT("不相等!"));

				LPCTSTR lpszText = _T("");
				lpszText = (LPCTSTR)strMessage;

				::AfxMessageBox (lpszText,MB_OK | MB_ICONERROR);
				dataFile.Close ();
				return FALSE;
			}

			++nVerifyRowNum;

			if( nVerifyRowNum == cMatrixOutputLayerValveValue.m_nRow )
			{
				break;
			}

		}

	}

	// Using for debugging
	//CString strOutputLayerValveValue = TEXT("TempOutputLayerValveValue.txt");
	//cMatrixOutputLayerValveValue.SaveDataToFile (strOutputLayerValveValue);



	dataFile.Close ();

	return TRUE;
}

string CMatrix::CStringtoString(CString str)
{
	string s;
#if _MSC_VER<=1200					//vc6.0 or eariler version
	if (str.IsEmpty())
	{
		return "";
	}
	s = str.GetBuffer(0);
	str.ReleaseBuffer(0);
	return s;
#endif	
	USES_CONVERSION;
	s = (W2A(str));
	return s;
}

bool CMatrix::SaveDataToFile (CString& strFileName)
{
	CFile dataFile;
	LPCTSTR	lpszFileName = _T("");

	// CString convert to LPCTSTR
	strFileName.TrimLeft ();
	strFileName.TrimRight ();

	lpszFileName = (LPCTSTR)strFileName;

	if(!dataFile.Open (lpszFileName,CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite ))
	{
		::AfxMessageBox(TEXT("不能创建文件!"),MB_OK | MB_ICONERROR);
		dataFile.Close ();
		return false;
	}

	dataFile.SeekToEnd ();

	// 将对象(矩阵)中的数据写进文件
	for(unsigned int i=0; i < m_nRow; i++)
	{
		for(unsigned int j=0; j < m_nCol; j++)
		{
			//CString strRealNumber;

			//strRealNumber.Format (_T("%.16f  "), m_pTMatrix [i][j]);

			// Using for debugging
			double nReadNumber = m_pTMatrix [i][j];
			stringstream ss;
			ss.str("");
			ss<<nReadNumber<<" ";

			//dataFile.Write(strRealNumber.GetBuffer(0) , strRealNumber.GetLength()+1);
			//strRealNumber.ReleaseBuffer();
			dataFile.Write(ss.str().c_str() , ss.str().length()*sizeof(char));
		}

		if( i != m_nRow - 1)
		{
			//char ReturnNewline[] = "\r\n";
			char ReturnNewline[] = "\r\n";

			dataFile.Write (ReturnNewline, (sizeof(ReturnNewline) - 1)/sizeof(char));
		}

	}


	dataFile.Close ();
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// 对矩阵中的元素进行一次操作:
//		使矩阵变为单位阵
/////////////////////////////////////////////////////////////////////////////

void CMatrix::Eye()
{
	// Verify whether the rows is equal to the columns or not
	if(m_nRow != m_nCol)
	{
		::AfxMessageBox (TEXT("此矩阵的行列数不相等!不能转变为单位阵!"),MB_OK | MB_ICONERROR);
		return;
	}

	for(unsigned int i=0; i < m_nRow; i++)
	{
		for(unsigned int j=0; j < m_nCol; j++)
		{
			if(i == j)
			{
				m_pTMatrix [i][j] =	1;
			}
			else
			{
				m_pTMatrix [i][j] =	0;
			}
		}

	}


}


/////////////////////////////////////////////////////////////////////////////
// Parameter:
//		CMatrix& cMatrix:		被拷贝的数据源
//		unsigned int nIndex:	被拷贝的数据在对象中的开始索引位置,从0开始
// Purpose:
//		This function will copy all the data of the cMatrix
// Notes:
//		此对象必须是列向量!!!
/////////////////////////////////////////////////////////////////////////////

void CMatrix::GetMatrixData(CMatrix& cMatrix, unsigned int nIndex)
{
	if(m_nCol != 1)
	{
		::AfxMessageBox (TEXT("拷贝的矩阵不是列向量!"),MB_OK | MB_ICONERROR);
		return;
	}

	if((m_nRow - nIndex) < (cMatrix.m_nRow * cMatrix.m_nCol))
	{
		::AfxMessageBox (TEXT("拷贝矩阵的空间容量不足!"),MB_OK | MB_ICONERROR);
		return;
	}

	for(unsigned int i=0; i < cMatrix.m_nRow; i++)
	{
		for(unsigned int j=0; j < cMatrix.m_nCol; j++)
		{
			m_pTMatrix [nIndex + i * cMatrix.m_nCol + j][0] = cMatrix.m_pTMatrix [i][j];
		}

	}

}


/////////////////////////////////////////////////////////////////////////////
// Parameter:
//		CMatrix& cMatrix:		被填充的矩阵
//		unsigned int nIndex:	被拷贝的数据在对象中的开始索引位置
// Purpose:
//		This function will copy part of the object data into cMatrix
// Notes:
//		The object must be column vector!!!
/////////////////////////////////////////////////////////////////////////////

void CMatrix::SetMatrixData(CMatrix& cMatrix, unsigned int nIndex)
{
	// Verify whether the colunm number is 1
	if(m_nCol != 1)
	{
		::AfxMessageBox (TEXT("本矩阵对象不是列向量,不满足条件!"),MB_OK | MB_ICONERROR);
		return;
	}

	// Verify whether the number of the object element is enough to be copyed
	if((m_nRow - nIndex) < (cMatrix.m_nRow * cMatrix.m_nCol))
	{
		::AfxMessageBox (TEXT("对象中的元素数量不足!"),MB_OK | MB_ICONERROR);
		return;
	}


	for(unsigned int i=0; i < cMatrix.m_nRow; i++)
	{
		for(unsigned int j=0; j < cMatrix.m_nCol; j++)
		{
			cMatrix.m_pTMatrix [i][j] = m_pTMatrix [nIndex + i * cMatrix.m_nCol + j][0];

			// Using for debugging
			//unsigned int nIndexNumber = nIndex + i * cMatrix.m_nRow + j;
			//double nData = cMatrix.m_pTMatrix [i][j];

		}
	}

}


/////////////////////////////////////////////////////////////////////////////
// Parameter:
//		CMatrix& cMatrix:		被填充的矩阵
//		unsigned int nIndex:	被拷贝的数据在对象中的开始索引位置
//		unsigned int nRow:		被填充的数据在被填充对象中的行索引
// Purpose:
//		This function will copy part of the object data to fill the special 
// row of the cMatrix
//	Notes:
//		The object must be column vector!!!
/////////////////////////////////////////////////////////////////////////////

void CMatrix::SetMatrixRowData(CMatrix& cMatrix, unsigned int nIndex, unsigned int nRow)
{
	// Verify whether the column number is 1
	if(m_nCol != 1)
	{
		::AfxMessageBox (TEXT("本矩阵对象不是列向量,不满足条件!"),MB_OK | MB_ICONERROR);
		return;
	}

	// Verify whether the number of the object element is enough to be copyed
	if((m_nRow - nIndex) < cMatrix.m_nCol )
	{
		::AfxMessageBox (TEXT("对象的元素数量不足!"),MB_OK | MB_ICONERROR);
		return;
	}

	for(unsigned int i=0; i < cMatrix.m_nCol; i++)
	{
		cMatrix.m_pTMatrix [nRow][i] = m_pTMatrix [nIndex + i][(unsigned int)0];
	}

}


/////////////////////////////////////////////////////////////////////////////
// Parameter:
//		CMatrix& cMatrix:		被拷贝的数据源
//		unsigned int nIndex:	被拷贝的数据在对象中的开始索引位置
//		unsigned int nRow:		被拷贝的数据在被拷贝对象中的行索引(从0开始)
// Purpose:
//		This function will copy all the data of the cMatrix
//	Notes:
//		此对象必须是列向量!!!
/////////////////////////////////////////////////////////////////////////////

void CMatrix::GetMatrixRowData(CMatrix& cMatrix, unsigned int nIndex, unsigned int nRow)
{
	if(m_nCol != 1)
	{
		::AfxMessageBox (TEXT("拷贝的矩阵不是列向量!"),MB_OK | MB_ICONERROR);
		return;
	}

	if((m_nRow - nIndex) < cMatrix.m_nCol)
	{
		::AfxMessageBox (TEXT("拷贝矩阵的空间容量不足!"),MB_OK | MB_ICONERROR);
		return;
	}

	for(unsigned int i=0; i < cMatrix.m_nCol; i++)
	{
		m_pTMatrix [nIndex + i][(unsigned int)0] = cMatrix.m_pTMatrix [nRow][i];
	}

}

void CMatrix::SetMatrixRowNumber(unsigned int nRow)
{
	m_nRow = nRow;

	m_pTMatrix.resize (m_nRow);
	for(unsigned int i=0; i < m_nRow; i++)
	{
		for(unsigned int j=0; j < m_nCol; j++)
		{
			m_pTMatrix[i].resize (m_nCol);
			m_pTMatrix[i][j] = (double) 0;
		}
	}

}


void CMatrix::SetMatrixColNumber(unsigned int nCol)
{
	m_nCol = nCol;

	m_pTMatrix.resize (m_nRow);
	for(unsigned int i=0; i < m_nRow; i++)
	{
		for(unsigned int j=0; j < m_nCol; j++)
		{
			m_pTMatrix[i].resize (m_nCol);
			m_pTMatrix[i][j] = (double) 0;
		}
	}

}

/////////////////////////////////////////////////////////////////////////
// 设置矩阵的行列数
void CMatrix::SetMatrixRowAndCol(unsigned int nRow,unsigned int nCol)
{
	m_nRow = nRow;
	m_nCol = nCol;

	// 分配内存
	m_pTMatrix.resize (m_nRow);
	for(unsigned int i=0; i < m_nRow; i++)
	{
		for(unsigned int j=0; j < m_nCol; j++)
		{
			m_pTMatrix[i].resize (m_nCol);
			m_pTMatrix[i][j] = (double) 0;
		}
	}

}


/////////////////////////////////////////////////////////////////////////////
// Initialize()
// 矩阵初始化函数,矩阵的行列数目被初始化为零,矩阵中的元素全部初始化为零
/////////////////////////////////////////////////////////////////////////////

void CMatrix::Initialize()
{
	m_nRow = 0;
	m_nCol = 0;

	m_pTMatrix.resize (m_nRow);
	for(unsigned int i=0; i < m_nRow; i++)
	{
		for(unsigned int j=0; j < m_nCol; j++)
		{
			m_pTMatrix[i].resize (m_nCol);
			m_pTMatrix[i][j] = (double) 0;
		}
	}

}

/////////////////////////////////////////////////////////////////////////////
// InitializeZero()
// 矩阵初始化函数,矩阵的行列数目被初始化为零,矩阵中的元素全部初始化为零
/////////////////////////////////////////////////////////////////////////////

void CMatrix::InitializeZero()
{
	m_nRow = 0;
	m_nCol = 0;

	m_pTMatrix.resize (m_nRow);
	for(unsigned int i=0; i < m_nRow; i++)
	{
		for(unsigned int j=0; j < m_nCol; j++)
		{
			m_pTMatrix[i].resize (m_nCol);
			m_pTMatrix[i][j] = (double) 0;
		}
	}

}

/////////////////////////////////////////////////////////////////////////////
// RandomInitialize()
// 将矩阵中的元素随机初始化函数,元素的值在(-1,1)之间的小数
/////////////////////////////////////////////////////////////////////////////

void CMatrix::RandomInitialize ()
{
	for(unsigned int i=0; i < m_nRow; i++)
	{
		for(unsigned int j=0; j < m_nCol; j++)
		{
			m_pTMatrix [i][j] = (double)(rand() - (0.5*RAND_MAX)) / (0.5*RAND_MAX);
		}
	}

}


/////////////////////////////////////////////////////////////////////////////
// 拷贝矩阵的子矩阵元素到另外一个矩阵中
// Parameter:
//		[out]	cMatrix ----> 矩阵的子矩阵返回的结果
//		[in]	nStartX ----> 子矩阵在矩阵中的起始坐标,对应行,索引从1开始
//		[in]	nStartY ----> 子矩阵在矩阵中的起始坐标,对应列,索引从1开始
/////////////////////////////////////////////////////////////////////////////

void CMatrix::CopySubMatrix(CMatrix& cMatrix,unsigned int nStartX,unsigned int nStartY)
{
	if((m_nRow  < cMatrix.m_nRow + nStartY ) | (m_nCol  < cMatrix.m_nCol + nStartX))
	{
		::AfxMessageBox (TEXT("被拷贝的矩阵维数小于要拷贝的矩阵所需要的维数!"),MB_OK | MB_ICONERROR);
		return;
	}

	for(unsigned int i=0;  i < cMatrix.m_nRow; i++)
	{
		for(unsigned int j=0; j < cMatrix.m_nCol; j++)
		{
			cMatrix.m_pTMatrix [i][j] = m_pTMatrix [nStartY + i][nStartX + j];	
		}
	}

}

/////////////////////////////////////////////////////////////////////////////
// Copy Matrix
//	Parameter:
//		[in]	cMatrix ----> 被拷贝的矩阵
/////////////////////////////////////////////////////////////////////////////

void CMatrix::CopyMatrix(CMatrix& cMatrix)
{
	m_nRow	= cMatrix.m_nRow ;
	m_nCol	= cMatrix.m_nCol ;

	m_pTMatrix	= cMatrix.m_pTMatrix ;

	for(unsigned int i=0; i < m_nRow; i++)
	{
		for(unsigned int j=0; j < m_nCol; j++)
		{
			m_pTMatrix [i][j] = cMatrix.m_pTMatrix [i][j];
		}

	}

}

/////////////////////////////////////////////////////////////////////////////
//	从一个列向量中拷贝数据到一个矩阵中
//	Parameter:
//		[out]	cMatrix ----> 函数的返回结果
//		[in]	nIndex	----> 在列向量中的索引值
//	Notes:
//		被拷贝的对象必须是列向量!!!
/////////////////////////////////////////////////////////////////////////////

void CMatrix::CopySubMatrixFromVector(CMatrix& cMatrix,unsigned int nIndex)
{
	if(m_nCol != 1)
	{
		::AfxMessageBox (TEXT("被拷贝的矩阵不是列向量!!!"),MB_OK | MB_ICONERROR);
		return;
	}

	for(unsigned int j=0; j < cMatrix.m_nCol; j++)
	{
		for(unsigned int i=0; i < cMatrix.m_nRow; i++)
		{
			cMatrix.m_pTMatrix [i][j] = m_pTMatrix [nIndex + j * cMatrix.m_nRow + i ][(unsigned int)0];
		}
	}

}

/////////////////////////////////////////////////////////////////////////////
// 对矩阵进列拓展
//	实现功能:
//		对矩阵的列数进行拓展,nTimes是每列拓展的次数
/////////////////////////////////////////////////////////////////////////////

void CMatrix::nncpyi(CMatrix &cMatrix, unsigned int nTimes)
{
	m_nRow	=	cMatrix.m_nRow ;
	m_nCol	=	cMatrix.m_nCol *	nTimes;

	// 根据空间分配内存
	m_pTMatrix.resize (m_nRow);
	for(unsigned int i=0; i < m_nRow; i++)
	{
		for(unsigned int j=0; j < m_nCol; j++)
		{
			m_pTMatrix[i].resize (m_nCol);
			m_pTMatrix[i][j] = (double) 0;
		}
	}

	// 赋值
	for(int i=0; i < m_nRow; i++)
	{
		for(unsigned int j=0; j < cMatrix.m_nCol; j++)
		{
			for(unsigned int k=0; k < nTimes; k++)
			{
				m_pTMatrix [i][j * nTimes + k] = cMatrix.m_pTMatrix [i][j];
			}
		}
	}

}

/////////////////////////////////////////////////////////////////////////////
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
/////////////////////////////////////////////////////////////////////////////

void CMatrix::nncpyd(CMatrix &cMatrix)
{
	m_nRow	=	cMatrix.m_nRow ;
	m_nCol	=	cMatrix.m_nCol * cMatrix.m_nRow ;

	// 根据空间分配内存
	m_pTMatrix.resize (m_nRow);
	for(unsigned int i=0; i < m_nRow; i++)
	{
		for(unsigned int j=0; j < m_nCol; j++)
		{
			m_pTMatrix[i].resize (m_nCol);
			m_pTMatrix[i][j] = (double) 0;
		}
	}

	// 给矩阵赋值
	for(int i=0; i < m_nRow; i++)
	{
		for(unsigned int j=0; j < cMatrix.m_nCol; j++)
		{
			for(unsigned int k=0; k < cMatrix.m_nRow; k++)
			{
				if(i == (j * cMatrix.m_nRow + k) % cMatrix.m_nRow )
					m_pTMatrix [i][j * cMatrix.m_nRow + k] = cMatrix.m_pTMatrix [i][j];
			}
		}
	}

}

/////////////////////////////////////////////////////////////////////////////
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
/////////////////////////////////////////////////////////////////////////////

void CMatrix::nncpy(CMatrix& cMatrix,unsigned int nTimes)
{
	m_nRow = cMatrix.m_nRow ;
	m_nCol = cMatrix.m_nCol * nTimes;

	// 根据空间分配内存
	m_pTMatrix.resize (m_nRow);
	for(unsigned int i=0; i < m_nRow; i++)
	{
		for(unsigned int j=0; j < m_nCol; j++)
		{
			m_pTMatrix[i].resize (m_nCol);
			m_pTMatrix[i][j] = (double) 0;
		}
	}

	// 对矩阵赋值
	for(int i=0; i < m_nRow; i++)
	{
		for(unsigned int j=0; j < nTimes; j++)
		{
			for(unsigned int k=0; k < cMatrix.m_nCol; k++)
			{
				m_pTMatrix [i][j * cMatrix.m_nCol + k] = cMatrix.m_pTMatrix [i][k];
			}
		}
	}

}

/////////////////////////////////////////////////////////////////////////////
// 对矩阵中所有的元素进行一次非线性变换:
//		变换后的值y与变换前的值的关系是:
//			y = f(x) = 1 / (1 + exp(-x))	( 0 < f(x) < 1)
//
/////////////////////////////////////////////////////////////////////////////

CMatrix CMatrix::Sigmoid()
{
	CMatrix cMatrix = *this;

	for(unsigned int i=0; i < m_nRow; i++)
	{
		for(unsigned int j=0; j < m_nCol; j++)
		{
			cMatrix.m_pTMatrix [i][j] = 1 / (1 + exp(-m_pTMatrix [i][j]));
		}

	}

	return cMatrix;
}


/////////////////////////////////////////////////////////////////////////////
// 对矩阵中所有的元素进行一次非线性变换:
//		变换后的值y与变换前的值的关系是:
//			y = tanh(x) = (1 - exp(-x)) / (1 + exp(-x))
//					 =  1 - 2 * exp(-x) / (1 + exp(-x))	( -1 < f(x) < 1)
//
/////////////////////////////////////////////////////////////////////////////

CMatrix CMatrix::tanh ()
{
	CMatrix cMatrix = *this;

	for(unsigned int i=0; i < m_nRow; i++)
	{
		for(unsigned int j=0; j < m_nCol; j++)
		{
			cMatrix.m_pTMatrix [i][j] = 1 - (2 * exp(-m_pTMatrix [i][j])) / (1 + exp(-m_pTMatrix [i][j]));
		}

	}

	return cMatrix;
}

/////////////////////////////////////////////////////////////////////////////
// 对矩阵中所有的元素进行一次非线性变换:
//		变换后的值y与变换前的值的关系是:
//			y = Tansig(x) = 2 / (1 + exp(-2 * x)) -1
/////////////////////////////////////////////////////////////////////////////

CMatrix CMatrix::Tansig()
{
	CMatrix cMatrix = *this;

	for(unsigned int i=0; i < m_nRow; i++)
	{
		for(unsigned int j=0; j < m_nCol; j++)
		{
			cMatrix.m_pTMatrix [i][j] = 2 / (1 + exp(- 2 * m_pTMatrix [i][j])) - 1;
		}
	}

	return cMatrix;

}

/////////////////////////////////////////////////////////////////////////////
// 对矩阵中所有的元素进行一次非线性变换:
//		变换后的值y与变换前的值的关系是:
//			y = Tansig'(x) = (2 / (1 + exp(-2 * x)) -1)'
//				= (2 / (1 + exp(-2 * x)) -1) * (2 / (1 + exp(-2 * x)) -1) -1
/////////////////////////////////////////////////////////////////////////////

CMatrix CMatrix::TansigDerivative()
{
	CMatrix cMatrix = *this;

	for(unsigned int i=0; i < m_nRow; i++)
	{
		for(unsigned int j=0; j < m_nCol; j++)
		{
			cMatrix.m_pTMatrix [i][j] = (2 / (1 + exp(- 2 * m_pTMatrix [i][j])) - 1) * (2 / (1 + exp(- 2 * m_pTMatrix [i][j])) - 1) - 1;
		}
	}

	return cMatrix;

}

/////////////////////////////////////////////////////////////////////////////
// 对矩阵中的元素进行一次操作:
//		使所有行中的相对应的列元素相等
//	Parameter:
//		nRowIndex	---->	行索引值(从零开始)
//							以此行做为标准,使矩阵中其余的行相对应的列的值
//							与此行相对应的列的值相等
/////////////////////////////////////////////////////////////////////////////

void CMatrix::MakeAllColumnElementsSameValue(unsigned int nRowIndex)
{
	for(unsigned int i=0; i < m_nRow; i++)
	{
		if(i == nRowIndex)
			continue;

		for(unsigned int j=0; j < m_nCol; j++)
		{
			m_pTMatrix [i][j] = m_pTMatrix [nRowIndex][j];
		}
	}

}


/////////////////////////////////////////////////////////////////////////////
// 对矩阵中所有的元素进行一次非线性变换:
//		变换后的值y与变换前的值的关系是:
//			y = f'(x) = (1 / (1 + exp(-x)))'	( 0 < f(x) < 1)
//			  = exp(-x)/((1 + exp(-x))*(1 + exp(-x)))
/////////////////////////////////////////////////////////////////////////////

CMatrix CMatrix::SigmoidDerivative()
{
	CMatrix cMatrix = *this;

	for(unsigned int i=0; i < m_nRow; i++)
	{
		for(unsigned int j=0; j < m_nCol; j++)
		{
			cMatrix.m_pTMatrix [i][j] = exp(-m_pTMatrix [i][j]) / ((1 + exp(-m_pTMatrix [i][j])) * (1 + exp(-m_pTMatrix [i][j])));
		}

	}

	return cMatrix;
}


/////////////////////////////////////////////////////////////////////////////
// 对矩阵中所有的元素进行一次非线性变换:
//		变换后的值y与变换前的值的关系是:
//			y = tanh'(x) = ((1 - exp(-x)) / (1 + exp(-x)))'	( -1 < f(x) < 1)
//					 = 	2*exp(-x)/((1 + exp(-x))*(1 + exp(-x)))
/////////////////////////////////////////////////////////////////////////////

CMatrix CMatrix::tanhDerivative()
{
	CMatrix cMatrix = *this;

	for(unsigned int i=0; i < m_nRow; i++)
	{
		for(unsigned int j=0; j < m_nCol; j++)
		{
			cMatrix.m_pTMatrix [i][j] = 2 * exp(-m_pTMatrix [i][j]) / ((1 + exp(-m_pTMatrix [i][j])) * (1 + exp(-m_pTMatrix [i][j])));
		}

	}

	return cMatrix;
}


/////////////////////////////////////////////////////////////////////////////
// 实现对点乘操作符的重载
/////////////////////////////////////////////////////////////////////////////

CMatrix CMatrix::operator / (CMatrix& cMatrixB)
{
	CMatrix cMatrix = *this;

	if( (m_nRow != cMatrixB.m_nRow) || (m_nCol != cMatrixB.m_nCol) )
	{
		::AfxMessageBox (TEXT("两个矩阵的维数不相等,不满足矩阵点乘的条件!"),MB_OK | MB_ICONERROR);
		return cMatrix;	// return a invalid value
	}

	for(unsigned int i=0; i < m_nRow; i++)
	{
		for(unsigned int j=0; j < m_nCol; j++)
		{
			cMatrix.m_pTMatrix [i][j] = m_pTMatrix [i][j] * cMatrixB.m_pTMatrix [i][j];
		}

	}

	return cMatrix;

}



//***************************************************************************
// ordinary function
//

/////////////////////////////////////////////////////////////////////////////
// 重载 'double - CMatrix' 运算符
/////////////////////////////////////////////////////////////////////////////

CMatrix operator - (double nValue,CMatrix& cMatrixB)
{
	CMatrix	cMatrix = cMatrixB;

	for(unsigned int i=0; i < cMatrix.GetMatrixRowNumber (); i++)
	{
		for(unsigned int j=0; j < cMatrix.GetMatrixColNumber (); j++)
		{
			cMatrix.m_pTMatrix [i][j] = nValue - cMatrixB.m_pTMatrix [i][j];
		}
	}

	return cMatrix;
}



/////////////////////////////////////////////////////////////////////////////
// 矩阵合并运算符
//	合并规则:
//		1. 参与合并运算的两个矩阵的行数必须相等;
//		2. 参与合并的两个矩阵的列数可以不相等;
//		3. 合并后返回的矩阵的行数与参与合并的矩阵的行数相等,列数是参与合并的
//			两个矩阵的列数之和;
/////////////////////////////////////////////////////////////////////////////

CMatrix MergeMatrix(CMatrix& cMatrixA,CMatrix& cMatrixB)
{
	//	条件检测
	if( cMatrixA.GetMatrixRowNumber () != cMatrixB.GetMatrixRowNumber () )
	{
		::AfxMessageBox (TEXT("参与合并的两个矩阵的行数不相等!"),MB_OK | MB_ICONERROR);

		return cMatrixA;	// return invalid value
	}

	CMatrix cMatrix(cMatrixA.GetMatrixRowNumber (),cMatrixA.GetMatrixColNumber () + cMatrixB.GetMatrixColNumber ());

	for(unsigned int i=0; i < cMatrixA.GetMatrixRowNumber (); i++)
	{
		for(unsigned int j=0; j < cMatrixA.GetMatrixColNumber (); j++)
		{
			cMatrix.m_pTMatrix [i][j] = cMatrixA.m_pTMatrix [i][j];
		}

		for(unsigned int k=0; k < cMatrixB.GetMatrixColNumber (); k++)
		{
			cMatrix.m_pTMatrix [i][cMatrixA.GetMatrixColNumber () + k] = cMatrixB.m_pTMatrix [i][k];
		}

	}


	return cMatrix;
}

// End of ordinary function
//***************************************************************************