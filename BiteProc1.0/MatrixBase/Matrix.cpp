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
	// ��̬�����ά����
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

	// �Զ��������ֵ
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
	// Ҫ���������ӵ�����: ������Ŀ���!
	if(m_nRow != cMatrixB.m_nRow || m_nCol != cMatrixB.m_nCol )
	{
		::AfxMessageBox (TEXT("ִ����ӵ���������ά�������!"),MB_OK | MB_ICONERROR);
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
	// Ҫ���������ӵ�����: ��������Ŀ���!
	if(m_nRow != cMatrixB.m_nRow || m_nCol != cMatrixB.m_nCol )
	{
		::AfxMessageBox (TEXT("ִ���������������ά�������!"),MB_OK | MB_ICONERROR);
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
		::AfxMessageBox (TEXT("ִ����˵���������ά����������˵�����!"),MB_OK | MB_ICONERROR);
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
		::AfxMessageBox(TEXT("�Ⱥ��������ߵľ����ά�������!"),MB_OK | MB_ICONERROR);
		return *this;	// return invalid value
	}	

	// ��������ֵ
	m_nRow = cMatrixB.m_nRow ;
	m_nCol = cMatrixB.m_nCol ;
	m_pTMatrix = cMatrixB.m_pTMatrix ;

	// ��ֵ����
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
		//printf("����!ִ����ӵ���������ά�������!\n");
		::AfxMessageBox (TEXT("����������߾����ά�������!"),MB_OK | MB_ICONERROR);
		return *this;	// return invalid value
	}

	// ��ֵ����
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
/////////////////////////////////////////////////////////////////////////////

CMatrix CMatrix::MergeColumnsToColumnVector()
{
	CMatrix cMatrix(m_nRow * m_nCol,(unsigned int)1);

	// �Ծ���ֵ
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
	// �ο���Ŀ: �������ֵ���� --->ʩ���� �¹�֦
	/////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////
	// �ж��Ƿ��ǿ�����:
	//		������һ���Ƿ���!!!

	if ( m_nRow != m_nCol)
	{
		//printf("����!����������������,�Ƿǿ�����!\n");
		::AfxMessageBox (TEXT("����������������,�Ƿǿ�����!"),MB_OK | MB_ICONERROR);
	}

	CMatrix cMatrix = *this;

	//***********************************************************************
	// ˼·:(�ǳ���˼ά!)
	//		��̬������������(2*m_nCol)���洢ÿ�ν������������ֵ
	//		������û���н�������¼��������,
	//		1.û�����н����������������,��SwapMatrixRow()������
	//		��⵽����ֵ�����������,��SwapMatrixCol()������Ҳһ��,
	//		��⵽����ֵ�����������,��ռ��ϵͳ��Դ;
	//		2.����ȵľͽ���
	//***********************************************************************

	//	�����ڴ�
	int *pIntArray = new int [2*m_nCol];

	// nSetp --- Լ������,����չ��	
	for(unsigned int k=0; k < cMatrix.m_nCol; k++)
	{
		/////////////////////////////////////////////////////////////////////
		// �����н��� ---> ��Ϸ����:
		// Ϊ��֤������̵���ֵ�ȶ���,�ڵ�k��Լ��ʱ,����{a(ik)}|i=k->n��ѡ��
		// ģ�������ΪԼ����Ԫ��,������������Ӧ����

		// �����Ԫ��
		double nMaxElement = cMatrix.m_pTMatrix [k][k];
		// �����Ԫ�����ڵ�����
		unsigned int nMainRow = k;

		for(unsigned int nCount = k+1; nCount < cMatrix.m_nCol; nCount++)
		{
			if( fabs(nMaxElement) < fabs(cMatrix.m_pTMatrix [nCount][k]) )
			{
				nMaxElement = cMatrix.m_pTMatrix [nCount][k];
				nMainRow = nCount;
			}
		}

		// ������������������������
		pIntArray [2*k] = k;
		pIntArray [2*k+1] = nMainRow;


		// ������
		cMatrix.SwapMatrixRow(k,nMainRow);

		//Display();

		//	�ж��Ƿ��ǿ�����
		if(cMatrix.m_pTMatrix [k][k] == 0)
		{
			//printf("����!�˾���Ϊ�ǿ�����!\n");
			::AfxMessageBox (TEXT("�˾���Ϊ�ǿ�����,û�������!"),MB_OK | MB_ICONERROR);
		}

		cMatrix.m_pTMatrix [k][k] = 1/(cMatrix.m_pTMatrix [k][k]);


		// ������
		for(unsigned int i=0; i < cMatrix.m_nRow; i++)
		{	
			if( i != k)
				cMatrix.m_pTMatrix [i][k] = -(cMatrix.m_pTMatrix [k][k]) * (cMatrix.m_pTMatrix [i][k]); 

			//int nTempValue = m_pTMatrix [i][k];

		}

		//printf("\n");

		// Լ��������
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

		// ������
		for(unsigned int j=0; j < cMatrix.m_nCol; j++)
		{
			if( j != k)
				cMatrix.m_pTMatrix [k][j] = (cMatrix.m_pTMatrix [k][k]) * (cMatrix.m_pTMatrix [k][j]);

		}

	}


	/////////////////////////////////////////////////////////////////////////
	// �����н��� ---> �Խ����к�ľ�������н��� ---> ��ԭ����
	// ��Ϸ����:
	// ����ʼ�����н��е��н��� ---> �������Ӧ���н������л�ԭ,���ɵõ������
	// �����

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
		::AfxMessageBox (TEXT("���ܴ�Ҫ��ȡ���ݵ��ļ�!"),MB_OK | MB_ICONERROR);
		dataFile.Close ();
		return FALSE;
	}

	// �����洢��ȡ�ı��ļ���һ�е�����
	CString	strData;				 
	// ������¼�ı��ļ���һ���ж���������?
	unsigned int	nRow = 0;		
	stringstream ss;         //used for reading the data...
	/////////////////////////////////////////////////////////////////////////
	// Step 1: �õ��ļ���������Ŀ�������ı��ļ���������Ŀ�����ö���(����)����
	// ����Ŀ
	//

	while(dataFile.ReadString (strData) != FALSE)
	{
		++nRow;
	}

	// �����ı��ļ������ݵ��������ö���(����)������
	m_nRow = nRow;
	SetMatrixRowNumber(m_nRow);

	// ���¶�λ��ǰ�ļ�ָ�뵽�ļ���ͷ
	dataFile.SeekToBegin ();

	dataFile.ReadString (strData);
	strData.TrimLeft ();
	strData.TrimRight ();

	TCHAR	SPACE_CHARACTER = ' ';
	TCHAR   SPACE_TAB = '	';
	// ������¼�ı��ļ���һ���ж�����?
	unsigned int	nCol = 0;						

	// �ո�����ַ����е�λ������
	int nIndex = 0;
	do
	{
		nIndex = strData.Find (SPACE_CHARACTER);
		if(nIndex==-1)
			nIndex = strData.Find(SPACE_TAB);
		//ss<<strData;
		//ss.
		++nCol;

		// ��ȡ�ַ��������ַ���,����ȡһ��double��ʵ������
		//CString strDoubleNumber = strData.Left (nIndex);	

		// ���ַ���ת��Ϊdouble��ʵ��
		//double RealNumber = atof(strDoubleNumber);

		//int nTempNumber = strData.GetLength ();

		strData = strData.Right (strData.GetLength () - nIndex -1);

		// ȥ������Ŀո�
		strData.TrimLeft (SPACE_TAB);

		// Use for debugging
		//int nTempNum = strData.GetLength ();

	}while(nIndex != -1);

	// �����ı��ļ������ݵ��������ö���(����)������
	m_nCol = nCol;
	SetMatrixColNumber(m_nCol);

	// End of Getting the Rows and Cols of the Text File
	/////////////////////////////////////////////////////////////////////////


	/////////////////////////////////////////////////////////////////////////
	// Step 2: �����ı��ļ��е����ݶԾ���ֵ,�����ÿ�е������Ƿ�͵�һ�е�
	// �������,�������ʾ������Ϣ
	//

	// ���¶�λ��ǰ�ļ�ָ�뵽�ļ���ͷ
	dataFile.SeekToBegin ();

	// �Ծ����е�Ԫ��װ���ı��ļ�������
	for(unsigned int i=0; i < m_nRow; i++)
	{
		dataFile.ReadString (strData);
		strData.TrimLeft ();
		strData.TrimRight ();

		// ��֤ÿ�е������Ƿ����һ�е��������
		unsigned int nVerifyColNum = 0;

		do
		{
			nIndex = strData.Find (SPACE_CHARACTER);
			if(nIndex==-1)
				nIndex = strData.Find(SPACE_TAB);
			++nVerifyColNum;

			if(nIndex != -1)
			{
				// ��ȡ�ַ��������ַ���,����ȡһ��double��ʵ������
				CString strDoubleNumber = strData.Left (nIndex);	

				// ���ַ���ת��Ϊdouble��ʵ��
				string s;
				s = CStringtoString(strDoubleNumber);
				double RealNumber = atof(s.c_str());
				

				m_pTMatrix [i][nVerifyColNum - 1] = RealNumber;
				//int nTempNumber = strData.GetLength ();

				strData = strData.Right (strData.GetLength () - nIndex -1);

				// ȥ������Ŀո�
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

			CString strMessage =  CString(TEXT("�ı��ļ���")) + strRowNumber + CString(TEXT("��һ����")) + strVerifyColNumber + CString(TEXT("��,���һ���е�����")) + strColNumber + CString(TEXT("�����!"));

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
			::AfxMessageBox(TEXT("���ܴ�Ҫ��ȡ�������ļ�!!!"), MB_OK | MB_ICONERROR);
			dataFile.Close();
			return FALSE;
	}
	END_CATCH
	

	// ��ȡ�������
	dataFile.SeekToBegin ();

	// �����洢��ȡ�ı��ļ���һ�е�����
	CString	strData;	
	// ע�ͱ�־����
	TCHAR cFirstCharacter = '#';
	// �ո����
	TCHAR SPACE_CHARACTER = ' ';
	TCHAR SPACE_TAB = '	';
	// �ո�����ַ����е�λ������
	int nIndex = 0;

	// ��ȡ�ļ��е�������������Ŀ
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

	// ��ȡ�ļ��е��������������Ŀ
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

	// ��ȡ�ļ��е�������������Ŀ
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

	// ��ȡ�ļ��е�ѵ��������ʹ�õ��㷨������ֵ
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

	// ��ȡ�ļ��е������ʹ�õĺ���������ֵ
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


	// �������������������㵽�������Ȩֵ�����������
	cMatrixInputToHideWeightValue.SetMatrixRowAndCol (nHideLayerNumber,nInputLayerNumber);

	// ��֤���������
	unsigned int nVerifyRowNum = 0;

	// ��ȡ�ļ��е�����㵽�������Ȩֵ������Ӧ�ľ��������
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

			// װ���ı��ļ������ݵ����Ӧ�ľ�����

			strData.TrimLeft ();
			strData.TrimRight ();

			// ��֤ÿ�е������Ƿ����һ�е��������
			unsigned int nVerifyColNum = 0;

			do
			{
				nIndex = strData.Find (SPACE_CHARACTER);

				++nVerifyColNum;

				if(nIndex != -1)
				{
					// ��ȡ�ַ��������ַ���,����ȡһ��double��ʵ������
					CString strDoubleNumber = strData.Left (nIndex);	

					// ���ַ���ת��Ϊdouble��ʵ��

					string s;
					s = CStringtoString(strDoubleNumber);
					double RealNumber = atof(s.c_str());

					cMatrixInputToHideWeightValue.m_pTMatrix [nVerifyRowNum][nVerifyColNum - 1] = RealNumber;

					strData = strData.Right (strData.GetLength () - nIndex -1);

					// ȥ������Ŀո�
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

				CString strMessage =  CString(TEXT("�ı��ļ���")) + strRowNumber + CString(TEXT("��һ����")) + strVerifyColNumber + CString(TEXT("��,���һ���е�����")) + strColNumber + CString(TEXT("�����!"));

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



	// ���������������������ķ�ֵ�����������
	cMatrixHideLayerValveValue.SetMatrixRowAndCol (nHideLayerNumber,(unsigned int)1);

	nIndex = 0;
	nVerifyRowNum = 0;

	// ��ȡ�ļ��е�������ķ�ֵ������Ӧ�ľ��������
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

			// װ���ı��ļ������ݵ����Ӧ�ľ�����

			strData.TrimLeft ();
			strData.TrimRight ();

			// ��֤ÿ�е������Ƿ����һ�е��������
			unsigned int nVerifyColNum = 0;

			do
			{
				nIndex = strData.Find (SPACE_CHARACTER);

				++nVerifyColNum;

				if(nIndex != -1)
				{
					// ��ȡ�ַ��������ַ���,����ȡһ��double��ʵ������
					CString strDoubleNumber = strData.Left (nIndex);	

					// ���ַ���ת��Ϊdouble��ʵ��
					string s;
					s = CStringtoString(strDoubleNumber);
					double RealNumber = atof(s.c_str());

					cMatrixHideLayerValveValue.m_pTMatrix [nVerifyRowNum][nVerifyColNum - 1] = RealNumber;

					strData = strData.Right (strData.GetLength () - nIndex -1);

					// ȥ������Ŀո�
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

				CString strMessage =  CString(TEXT("�ı��ļ���")) + strRowNumber + CString(TEXT("��һ����")) + strVerifyColNumber + CString(TEXT("��,���һ���е�����")) + strColNumber + CString(TEXT("�����!"));

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

	// ��������������������㵽������Ȩֵ�����������
	cMatrixHideToOutputWeightValue.SetMatrixRowAndCol (nOutputLayerNumber,nHideLayerNumber);

	nIndex = 0;
	nVerifyRowNum = 0;

	// ��ȡ�ļ��е������㵽������Ȩֵ������Ӧ�ľ��������
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

			// װ���ı��ļ������ݵ����Ӧ�ľ�����

			strData.TrimLeft ();
			strData.TrimRight ();

			// ��֤ÿ�е������Ƿ����һ�е��������
			unsigned int nVerifyColNum = 0;

			do
			{
				nIndex = strData.Find (SPACE_CHARACTER);

				++nVerifyColNum;

				if(nIndex != -1)
				{
					// ��ȡ�ַ��������ַ���,����ȡһ��double��ʵ������
					CString strDoubleNumber = strData.Left (nIndex);	

					// ���ַ���ת��Ϊdouble��ʵ��
					string s;
					s=  CStringtoString(strDoubleNumber);
					double RealNumber = atof(s.c_str());

					cMatrixHideToOutputWeightValue.m_pTMatrix [nVerifyRowNum][nVerifyColNum - 1] = RealNumber;

					strData = strData.Right (strData.GetLength () - nIndex -1);

					// ȥ������Ŀո�
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

				CString strMessage =  CString(TEXT("�ı��ļ���")) + strRowNumber + CString(TEXT("��һ����")) + strVerifyColNumber + CString(TEXT("��,���һ���е�����")) + strColNumber + CString(TEXT("�����!"));

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

	// ��������������������ķ�ֵ�����������
	cMatrixOutputLayerValveValue.SetMatrixRowAndCol (nOutputLayerNumber,(unsigned int)1);

	nIndex = 0;
	nVerifyRowNum = 0;

	// ��ȡ�ļ��е������ķ�ֵ������Ӧ�ľ��������
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

			// װ���ı��ļ������ݵ����Ӧ�ľ�����

			strData.TrimLeft ();
			strData.TrimRight ();

			// ��֤ÿ�е������Ƿ����һ�е��������
			unsigned int nVerifyColNum = 0;

			do
			{
				nIndex = strData.Find (SPACE_CHARACTER);

				++nVerifyColNum;

				if(nIndex != -1)
				{
					// ��ȡ�ַ��������ַ���,����ȡһ��double��ʵ������
					CString strDoubleNumber = strData.Left (nIndex);	

					// ���ַ���ת��Ϊdouble��ʵ��
					string s;
					s = CStringtoString(strDoubleNumber);
					double RealNumber = atof(s.c_str());

					cMatrixOutputLayerValveValue.m_pTMatrix [nVerifyRowNum][nVerifyColNum - 1] = RealNumber;

					strData = strData.Right (strData.GetLength () - nIndex -1);

					// ȥ������Ŀո�
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

				CString strMessage =  CString(TEXT("�ı��ļ���")) + strRowNumber + CString(TEXT("��һ����")) + strVerifyColNumber + CString(TEXT("��,���һ���е�����")) + strColNumber + CString(TEXT("�����!"));

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
		::AfxMessageBox(TEXT("���ܴ����ļ�!"),MB_OK | MB_ICONERROR);
		dataFile.Close ();
		return false;
	}

	dataFile.SeekToEnd ();

	// ������(����)�е�����д���ļ�
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
// �Ծ����е�Ԫ�ؽ���һ�β���:
//		ʹ�����Ϊ��λ��
/////////////////////////////////////////////////////////////////////////////

void CMatrix::Eye()
{
	// Verify whether the rows is equal to the columns or not
	if(m_nRow != m_nCol)
	{
		::AfxMessageBox (TEXT("�˾���������������!����ת��Ϊ��λ��!"),MB_OK | MB_ICONERROR);
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
//		CMatrix& cMatrix:		������������Դ
//		unsigned int nIndex:	�������������ڶ����еĿ�ʼ����λ��,��0��ʼ
// Purpose:
//		This function will copy all the data of the cMatrix
// Notes:
//		�˶��������������!!!
/////////////////////////////////////////////////////////////////////////////

void CMatrix::GetMatrixData(CMatrix& cMatrix, unsigned int nIndex)
{
	if(m_nCol != 1)
	{
		::AfxMessageBox (TEXT("�����ľ�����������!"),MB_OK | MB_ICONERROR);
		return;
	}

	if((m_nRow - nIndex) < (cMatrix.m_nRow * cMatrix.m_nCol))
	{
		::AfxMessageBox (TEXT("��������Ŀռ���������!"),MB_OK | MB_ICONERROR);
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
//		CMatrix& cMatrix:		�����ľ���
//		unsigned int nIndex:	�������������ڶ����еĿ�ʼ����λ��
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
		::AfxMessageBox (TEXT("�����������������,����������!"),MB_OK | MB_ICONERROR);
		return;
	}

	// Verify whether the number of the object element is enough to be copyed
	if((m_nRow - nIndex) < (cMatrix.m_nRow * cMatrix.m_nCol))
	{
		::AfxMessageBox (TEXT("�����е�Ԫ����������!"),MB_OK | MB_ICONERROR);
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
//		CMatrix& cMatrix:		�����ľ���
//		unsigned int nIndex:	�������������ڶ����еĿ�ʼ����λ��
//		unsigned int nRow:		�����������ڱ��������е�������
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
		::AfxMessageBox (TEXT("�����������������,����������!"),MB_OK | MB_ICONERROR);
		return;
	}

	// Verify whether the number of the object element is enough to be copyed
	if((m_nRow - nIndex) < cMatrix.m_nCol )
	{
		::AfxMessageBox (TEXT("�����Ԫ����������!"),MB_OK | MB_ICONERROR);
		return;
	}

	for(unsigned int i=0; i < cMatrix.m_nCol; i++)
	{
		cMatrix.m_pTMatrix [nRow][i] = m_pTMatrix [nIndex + i][(unsigned int)0];
	}

}


/////////////////////////////////////////////////////////////////////////////
// Parameter:
//		CMatrix& cMatrix:		������������Դ
//		unsigned int nIndex:	�������������ڶ����еĿ�ʼ����λ��
//		unsigned int nRow:		�������������ڱ����������е�������(��0��ʼ)
// Purpose:
//		This function will copy all the data of the cMatrix
//	Notes:
//		�˶��������������!!!
/////////////////////////////////////////////////////////////////////////////

void CMatrix::GetMatrixRowData(CMatrix& cMatrix, unsigned int nIndex, unsigned int nRow)
{
	if(m_nCol != 1)
	{
		::AfxMessageBox (TEXT("�����ľ�����������!"),MB_OK | MB_ICONERROR);
		return;
	}

	if((m_nRow - nIndex) < cMatrix.m_nCol)
	{
		::AfxMessageBox (TEXT("��������Ŀռ���������!"),MB_OK | MB_ICONERROR);
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
// ���þ����������
void CMatrix::SetMatrixRowAndCol(unsigned int nRow,unsigned int nCol)
{
	m_nRow = nRow;
	m_nCol = nCol;

	// �����ڴ�
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
// �����ʼ������,�����������Ŀ����ʼ��Ϊ��,�����е�Ԫ��ȫ����ʼ��Ϊ��
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
// �����ʼ������,�����������Ŀ����ʼ��Ϊ��,�����е�Ԫ��ȫ����ʼ��Ϊ��
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
// �������е�Ԫ�������ʼ������,Ԫ�ص�ֵ��(-1,1)֮���С��
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
// ����������Ӿ���Ԫ�ص�����һ��������
// Parameter:
//		[out]	cMatrix ----> ������Ӿ��󷵻صĽ��
//		[in]	nStartX ----> �Ӿ����ھ����е���ʼ����,��Ӧ��,������1��ʼ
//		[in]	nStartY ----> �Ӿ����ھ����е���ʼ����,��Ӧ��,������1��ʼ
/////////////////////////////////////////////////////////////////////////////

void CMatrix::CopySubMatrix(CMatrix& cMatrix,unsigned int nStartX,unsigned int nStartY)
{
	if((m_nRow  < cMatrix.m_nRow + nStartY ) | (m_nCol  < cMatrix.m_nCol + nStartX))
	{
		::AfxMessageBox (TEXT("�������ľ���ά��С��Ҫ�����ľ�������Ҫ��ά��!"),MB_OK | MB_ICONERROR);
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
//		[in]	cMatrix ----> �������ľ���
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
//	��һ���������п������ݵ�һ��������
//	Parameter:
//		[out]	cMatrix ----> �����ķ��ؽ��
//		[in]	nIndex	----> ���������е�����ֵ
//	Notes:
//		�������Ķ��������������!!!
/////////////////////////////////////////////////////////////////////////////

void CMatrix::CopySubMatrixFromVector(CMatrix& cMatrix,unsigned int nIndex)
{
	if(m_nCol != 1)
	{
		::AfxMessageBox (TEXT("�������ľ�����������!!!"),MB_OK | MB_ICONERROR);
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
// �Ծ��������չ
//	ʵ�ֹ���:
//		�Ծ��������������չ,nTimes��ÿ����չ�Ĵ���
/////////////////////////////////////////////////////////////////////////////

void CMatrix::nncpyi(CMatrix &cMatrix, unsigned int nTimes)
{
	m_nRow	=	cMatrix.m_nRow ;
	m_nCol	=	cMatrix.m_nCol *	nTimes;

	// ���ݿռ�����ڴ�
	m_pTMatrix.resize (m_nRow);
	for(unsigned int i=0; i < m_nRow; i++)
	{
		for(unsigned int j=0; j < m_nCol; j++)
		{
			m_pTMatrix[i].resize (m_nCol);
			m_pTMatrix[i][j] = (double) 0;
		}
	}

	// ��ֵ
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
/////////////////////////////////////////////////////////////////////////////

void CMatrix::nncpyd(CMatrix &cMatrix)
{
	m_nRow	=	cMatrix.m_nRow ;
	m_nCol	=	cMatrix.m_nCol * cMatrix.m_nRow ;

	// ���ݿռ�����ڴ�
	m_pTMatrix.resize (m_nRow);
	for(unsigned int i=0; i < m_nRow; i++)
	{
		for(unsigned int j=0; j < m_nCol; j++)
		{
			m_pTMatrix[i].resize (m_nCol);
			m_pTMatrix[i][j] = (double) 0;
		}
	}

	// ������ֵ
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
/////////////////////////////////////////////////////////////////////////////

void CMatrix::nncpy(CMatrix& cMatrix,unsigned int nTimes)
{
	m_nRow = cMatrix.m_nRow ;
	m_nCol = cMatrix.m_nCol * nTimes;

	// ���ݿռ�����ڴ�
	m_pTMatrix.resize (m_nRow);
	for(unsigned int i=0; i < m_nRow; i++)
	{
		for(unsigned int j=0; j < m_nCol; j++)
		{
			m_pTMatrix[i].resize (m_nCol);
			m_pTMatrix[i][j] = (double) 0;
		}
	}

	// �Ծ���ֵ
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
// �Ծ��������е�Ԫ�ؽ���һ�η����Ա任:
//		�任���ֵy��任ǰ��ֵ�Ĺ�ϵ��:
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
// �Ծ��������е�Ԫ�ؽ���һ�η����Ա任:
//		�任���ֵy��任ǰ��ֵ�Ĺ�ϵ��:
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
// �Ծ��������е�Ԫ�ؽ���һ�η����Ա任:
//		�任���ֵy��任ǰ��ֵ�Ĺ�ϵ��:
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
// �Ծ��������е�Ԫ�ؽ���һ�η����Ա任:
//		�任���ֵy��任ǰ��ֵ�Ĺ�ϵ��:
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
// �Ծ����е�Ԫ�ؽ���һ�β���:
//		ʹ�������е����Ӧ����Ԫ�����
//	Parameter:
//		nRowIndex	---->	������ֵ(���㿪ʼ)
//							�Դ�����Ϊ��׼,ʹ����������������Ӧ���е�ֵ
//							��������Ӧ���е�ֵ���
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
// �Ծ��������е�Ԫ�ؽ���һ�η����Ա任:
//		�任���ֵy��任ǰ��ֵ�Ĺ�ϵ��:
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
// �Ծ��������е�Ԫ�ؽ���һ�η����Ա任:
//		�任���ֵy��任ǰ��ֵ�Ĺ�ϵ��:
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
// ʵ�ֶԵ�˲�����������
/////////////////////////////////////////////////////////////////////////////

CMatrix CMatrix::operator / (CMatrix& cMatrixB)
{
	CMatrix cMatrix = *this;

	if( (m_nRow != cMatrixB.m_nRow) || (m_nCol != cMatrixB.m_nCol) )
	{
		::AfxMessageBox (TEXT("���������ά�������,����������˵�����!"),MB_OK | MB_ICONERROR);
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
// ���� 'double - CMatrix' �����
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
// ����ϲ������
//	�ϲ�����:
//		1. ����ϲ��������������������������;
//		2. ����ϲ�������������������Բ����;
//		3. �ϲ��󷵻صľ�������������ϲ��ľ�����������,�����ǲ���ϲ���
//			�������������֮��;
/////////////////////////////////////////////////////////////////////////////

CMatrix MergeMatrix(CMatrix& cMatrixA,CMatrix& cMatrixB)
{
	//	�������
	if( cMatrixA.GetMatrixRowNumber () != cMatrixB.GetMatrixRowNumber () )
	{
		::AfxMessageBox (TEXT("����ϲ���������������������!"),MB_OK | MB_ICONERROR);

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