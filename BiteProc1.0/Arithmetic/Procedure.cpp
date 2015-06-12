/////////////////////////////////////////////////////////////////////////////
// Procedure.cpp
//		the implementation of the specified arithmetic
// Author : freeia
// E-mail : freeia@163.com
// Date : 3/20/2003
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Matrix.h"
#include "AllDef.h"
#include "FANN/fann.h"

#ifdef _DEBUG
#pragma  comment(lib,"..\\Debug\\fannfloatd.lib")
#pragma comment(lib ,"..\\Debug\\MatrixBase.lib")
#else
#pragma  comment(lib,"..\\Release\\fanndoubled.lib")
#pragma comment(lib ,"..\\Release\\MatrixBase.lib")
#endif

/////////////////////////////////////////////////////////////////////////////
//	Levenberg-Marquart		---->	��һ��ǰ������ʼ��				   //
/////////////////////////////////////////////////////////////////////////////
__declspec (dllexport) void LMForwardCalculateInit( int nInputLayerNumber,
	int nHideLayerNumber,
	int nOutputLayerNumber,
	CMatrix &matrixDemoDataInput,
	CMatrix &matrixInputLayerValue,
	CMatrix &matrixInputToHideWeightValue,
	CMatrix &matrixHideLayerValveValue,
	CMatrix &matrixHideToOutputWeightValue,
	CMatrix &matrixOutputLayerValveValue
	)
{

	/************************************************************************
	*					--------->Use Matlab Method <---------              *
	************************************************************************/


	/////////////////////////////////////////////////////////////////////////
	// ���������Ԫ�صľ���
	// �������:
	//		1. ������Ŀ��Ϊ���������;
	//		2. ������������������Ŀ��Ϊ���������;
	//		3. �����е�Ԫ�ؼ�Ϊ��Ӧ��������ֵ
	//

	CMatrix cMatrixInputLayerValue(matrixDemoDataInput.GetMatrixRowNumber (), nInputLayerNumber);

	// �õ�����������ֵ
	matrixDemoDataInput.CopySubMatrix (cMatrixInputLayerValue,(unsigned int)0,(unsigned int)0);

	CMatrix cMatrixTInputLayerValue = cMatrixInputLayerValue.Transpose ();

	matrixInputLayerValue.CopyMatrix (cMatrixTInputLayerValue);


	/////////////////////////////////////////////////////////////////////////
	// ����Ȩֵ���� -----> �ɵ��������������������֮���Ȩֵ��ΪԪ�����
	// �������:
	//		1. ������������������Ŀ��Ϊ��������;
	//		2. �������������������Ŀ��Ϊ���������;
	//		3. �Ծ����е�Ԫ�ؽ��������ʼ��,ֵ��(-1,1)֮��;
	//		4. �������������������������Ŀ����ȵ�;
	//		5. ��������ʹ�õ���ͬһ��Ȩֵ����.
	//

	CMatrix	cMatrixInputToHideWeightValue(nHideLayerNumber, nInputLayerNumber);

	// �����ʼ��������Ԫ�ص�ֵ
	cMatrixInputToHideWeightValue.RandomInitialize ();

	matrixInputToHideWeightValue.CopyMatrix (cMatrixInputToHideWeightValue);


	/////////////////////////////////////////////////////////////////////
	//	��������������ķ�ֵ����
	//	�������:
	//		1. ��������Ŀ��Ϊ��������;
	//		2. �������������������Ŀ��Ϊ���������;
	//		3. �����ʼ�������е�Ԫ��,ʹֵ��(-1,1)֮��;
	//		4. ������ÿ�е����ݶ��͵�һ���������Ӧ��λ�����.
	//

	CMatrix cMatrixHideLayerValveValue(nHideLayerNumber,(unsigned int)1);

	// �����ʼ��������Ԫ�ص�ֵ
	cMatrixHideLayerValveValue.RandomInitialize ();

	matrixHideLayerValveValue.CopyMatrix(cMatrixHideLayerValveValue);


	/////////////////////////////////////////////////////////////////////
	//	����Ȩֵ����	----->	�ɵ����������������������֮��Ȩֵ��ΪԪ��
	//							���
	//	�������:
	//		1. �������������������Ŀ��Ϊ���������;
	//		2. ������������������Ŀ��Ϊ���������;
	//		3. �Ծ����е�Ԫ�ؽ��������ʼ��,ֵ��(-1,1)֮��;
	//		4. �������������������������Ŀ����ȵ�;
	//		5. ��������ʹ�õ���ͬһ��Ȩֵ����.
	//

	CMatrix cMatrixHideToOutputWeightValue(nOutputLayerNumber, nHideLayerNumber);

	// �Ծ����Ԫ�������ʼ��
	cMatrixHideToOutputWeightValue.RandomInitialize ();

	matrixHideToOutputWeightValue.CopyMatrix (cMatrixHideToOutputWeightValue);


	/////////////////////////////////////////////////////////////////////
	//	���������������ķ�ֵ����
	//	�������:
	//		1. ��������Ŀ��Ϊ���������;
	//		2. ������������������Ŀ��Ϊ���������;
	//		3. �����ʼ�������е�Ԫ��,ʹֵ��(-1,1)֮��;
	//		4. ������ÿ�е����ݶ��͵�һ���������Ӧ��λ�����.
	//

	CMatrix cMatrixOutputLayerValveValue(nOutputLayerNumber,(unsigned int)1);

	// �����ʼ��������Ԫ�ص�ֵ
	cMatrixOutputLayerValveValue.RandomInitialize ();

	matrixOutputLayerValveValue.CopyMatrix(cMatrixOutputLayerValveValue);


}

/////////////////////////////////////////////////////////////////////////////
//	Levenberg-Marquart		---->		ǰ�����						   //
/////////////////////////////////////////////////////////////////////////////
__declspec(dllexport) void LMForwardCalculate ( int nInputLayerNumber,
	int nHideLayerNumber,
	int nOutputLayerNumber,
	bool bSimulateDataFlag,
	int nComboFunc,
	CMatrix &matrixDemoDataInput,
	CMatrix &matrixInputLayerValue,
	CMatrix &matrixInputToHideWeightValue,
	CMatrix &matrixHideLayerValveValue,
	CMatrix &matrixHideLayerOutput,
	CMatrix &matrixHideToOutputWeightValue,
	CMatrix &matrixOutputLayerOutput,
	CMatrix &matrixOutputLayerValveValue
	)
{

	if(bSimulateDataFlag)
	{

		CMatrix cMatrixInputLayerValue(matrixDemoDataInput.GetMatrixRowNumber (), nInputLayerNumber);

		// �õ�����������ֵ
		matrixDemoDataInput.CopySubMatrix (cMatrixInputLayerValue, (unsigned int)0, (unsigned int)0);

		CMatrix cMatrixTInputLayerValue = cMatrixInputLayerValue.Transpose ();

		matrixInputLayerValue.CopyMatrix (cMatrixTInputLayerValue);


	}

	/////////////////////////////////////////////////////////////////////////
	// �õ�����������������ľ�����
	// �������:
	//		1. ��������Ŀ��Ϊ��������;
	//		2. �������������������Ŀ��Ϊ���������;
	//		3. ����Ԫ���е�ֵ��Ϊ��Ӧ��������������ľ�����:
	//		   �� 
	//				cMatrixInputLayerValue * cMatrixInputToHideWeightValue 
	//				+ cMatrixHideLayerValveValue
	//			�õ�.
	//

	CMatrix cMatrixExHideLayerValveValue;
	cMatrixExHideLayerValveValue.nncpyi (matrixHideLayerValveValue, matrixDemoDataInput.GetMatrixRowNumber ());

	CMatrix cMatrixHideLayerPureInput(nHideLayerNumber, matrixDemoDataInput.GetMatrixRowNumber ());

	cMatrixHideLayerPureInput = matrixInputToHideWeightValue * matrixInputLayerValue;

	cMatrixHideLayerPureInput += cMatrixExHideLayerValveValue;


	/////////////////////////////////////////////////////////////////////
	//	�����������������������
	//	�������:
	//		1. ����������y�������������x�Ĺ�ϵ���ú�����ʾ
	//			y = f(x)
	//		2. �����ά����������ľ���������ά�����
	//

	CMatrix cMatrixHideLayerOutput(nHideLayerNumber, matrixDemoDataInput.GetMatrixRowNumber ());

	switch(nComboFunc)
	{
	case 0:
		cMatrixHideLayerOutput = cMatrixHideLayerPureInput.Sigmoid ();
		break;

	case 1:
		cMatrixHideLayerOutput = cMatrixHideLayerPureInput.tanh ();
		break;

	case 2:
		cMatrixHideLayerOutput = cMatrixHideLayerPureInput.Tansig();
		break;

	default:
		return;
	}

	matrixHideLayerOutput.CopyMatrix(cMatrixHideLayerOutput);

	/////////////////////////////////////////////////////////////////////
	//	�õ��������������ľ�����
	//	�������;
	//		1. ��������Ŀ��Ϊ���������;
	//		2. ������������������Ŀ��Ϊ���������;
	//		3. ������Ԫ�ص�ֵ��Ϊ��Ӧ�����������ľ�����:
	//			��
	//				cMatrixHideLayerOutput * cMatrixHideToOutputWeightValue
	//				+ cMatrixOutputLayerValveValue
	//			�õ�
	//

	CMatrix cMatrixExOutputLayerValveValue;
	cMatrixExOutputLayerValveValue.nncpyi (matrixOutputLayerValveValue, matrixDemoDataInput.GetMatrixRowNumber ());

	CMatrix cMatrixOutputLayerPureInput(nOutputLayerNumber, matrixDemoDataInput.GetMatrixRowNumber ());

	cMatrixOutputLayerPureInput = matrixHideToOutputWeightValue * cMatrixHideLayerOutput;
	cMatrixOutputLayerPureInput  += cMatrixExOutputLayerValveValue;


	/////////////////////////////////////////////////////////////////////
	//	����������������������
	//	�������:
	//		1. �����ά����õ������������������ľ�������ɵľ���һ��;
	//		2. ���������y��������������ù�ϵʽ
	//			y = f(x)
	//		��ʾ
	//

	CMatrix cMatrixOutputLayerOutput(nOutputLayerNumber, matrixDemoDataInput.GetMatrixRowNumber ());

	switch(nComboFunc)
	{
	case 0:
		cMatrixOutputLayerOutput = cMatrixOutputLayerPureInput.Sigmoid ();
		break;

	case 1:
		cMatrixOutputLayerOutput = cMatrixOutputLayerPureInput.tanh ();
		break;

	case 2:
		cMatrixOutputLayerOutput = cMatrixOutputLayerPureInput.Tansig ();
		break;

	default:
		return ;
	}

	matrixOutputLayerOutput.CopyMatrix(cMatrixOutputLayerOutput);

}


/////////////////////////////////////////////////////////////////////////////
//	Levenberg-Marquart		---->		��������						   //
/////////////////////////////////////////////////////////////////////////////
__declspec(dllexport) bool LMDemoDataTrainRepeat (	int nInputLayerNumber,
	int nHideLayerNumber,
	int nOutputLayerNumber,
	bool bSimulateDataFlag,
	int nComboFunc,
	double nSystemErrorOld,
	double nSystemErrorNew,
	double nSystemErrorLevel,
	double nSystemError,
	double nStep,
	UINT nMaxTrainTimes,
	UINT nTrainTimes,
	double& nCurSystemError,					//����ϵͳ���
	int&    nCurTrainTime,
	CMatrix &matrixDemoDataInput,
	CMatrix &matrixInputLayerValue,
	CMatrix &matrixInputToHideWeightValue,
	CMatrix &matrixHideLayerValveValue,
	CMatrix &matrixHideLayerOutput,
	CMatrix &matrixHideToOutputWeightValue,
	CMatrix &matrixOutputLayerOutput,
	CMatrix &matrixOutputLayerValveValue
	)
{

	LMForwardCalculate (nInputLayerNumber,
		nHideLayerNumber,
		nOutputLayerNumber,
		bSimulateDataFlag,
		nComboFunc,
		matrixDemoDataInput,
		matrixInputLayerValue,
		matrixInputToHideWeightValue,
		matrixHideLayerValveValue,
		matrixHideLayerOutput,
		matrixHideToOutputWeightValue,
		matrixOutputLayerOutput,
		matrixOutputLayerValveValue
		);


	/////////////////////////////////////////////////////////////////////
	//	�������������������delta����
	//	�������:
	//		1. ��������ĿΪ���������;
	//		2. ������������ĿΪ���������;
	//		3. �����е�Ԫ�ص�ֵyΪ:
	//				y = -(ǰ��������������ֵ - ������������ֵ) * f'(net)
	//


	CMatrix cMatrixTDemoOutput(matrixDemoDataInput.GetMatrixRowNumber (), nOutputLayerNumber);

	// �õ�����������������
	matrixDemoDataInput.CopySubMatrix (cMatrixTDemoOutput, (unsigned int)nInputLayerNumber, (unsigned int)0);

	CMatrix cMatrixDemoOutput = cMatrixTDemoOutput.Transpose ();

	// �õ����������������
	CMatrix cMatrixOutputLayerError(nOutputLayerNumber, matrixDemoDataInput.GetMatrixRowNumber ());

	cMatrixOutputLayerError = cMatrixDemoOutput - matrixOutputLayerOutput;


	nSystemErrorOld = cMatrixOutputLayerError.GetSystemError ();

	for(int nLoopTimes=1; nLoopTimes < nMaxTrainTimes; nLoopTimes++)	
	{
		if(nSystemErrorOld < nSystemErrorLevel)
		{
			nLoopTimes--;
			break;
		}
		if(IS_STOP)
			break;

		CMatrix	cMatrixExHideLayerOutput;
		cMatrixExHideLayerOutput.nncpyi (matrixHideLayerOutput, nOutputLayerNumber);


		CMatrix	cMatrixOutputLayerDelta (nOutputLayerNumber, matrixDemoDataInput.GetMatrixRowNumber());

		// ע��: �˴�'/' �� '���'!!!
		cMatrixOutputLayerDelta = 1 - matrixOutputLayerOutput / matrixOutputLayerOutput;

		CMatrix cMatrixExOutputLayerDelta;
		cMatrixExOutputLayerDelta.nncpyd (cMatrixOutputLayerDelta);

		cMatrixExOutputLayerDelta = cMatrixExOutputLayerDelta * (-1.0);


		CMatrix cMatrixTHideToOutputWeightValue (matrixHideToOutputWeightValue.GetMatrixColNumber(), matrixHideToOutputWeightValue.GetMatrixRowNumber());
		cMatrixTHideToOutputWeightValue = matrixHideToOutputWeightValue.Transpose();

		CMatrix cMatrixExHideLayerDelta;
		// ע��: �˴�'/' �� '���'!!!
		cMatrixExHideLayerDelta.CopyMatrix ( (1 - (cMatrixExHideLayerOutput / cMatrixExHideLayerOutput)) / ( cMatrixTHideToOutputWeightValue * cMatrixExOutputLayerDelta) );


		CMatrix	cMatrixExInputLayerValue;
		cMatrixExInputLayerValue.nncpyi (matrixInputLayerValue, nOutputLayerNumber);


		CMatrix	cMatrixJ11;
		cMatrixJ11.nncpy (cMatrixExHideLayerDelta.Transpose(), cMatrixExInputLayerValue.GetMatrixRowNumber ());

		CMatrix cMatrixJ12;
		cMatrixJ12.nncpyi(cMatrixExInputLayerValue.Transpose (), cMatrixExHideLayerDelta.GetMatrixRowNumber());


		CMatrix	cMatrixJ1;
		// ע��: �˴�'/' �� '���'!!!
		cMatrixJ1.CopyMatrix (cMatrixJ11 / cMatrixJ12);


		CMatrix cMatrixJ21;
		cMatrixJ21.nncpy (cMatrixExOutputLayerDelta.Transpose (), cMatrixExHideLayerOutput.GetMatrixRowNumber ());

		CMatrix cMatrixJ22;
		cMatrixJ22.nncpyi (cMatrixExHideLayerOutput.Transpose (), cMatrixExOutputLayerDelta.GetMatrixRowNumber ());

		CMatrix cMatrixJ2;
		// ע��: �˴�'/' �� '���'!!!
		cMatrixJ2.CopyMatrix (cMatrixJ21 / cMatrixJ22);


		CMatrix cMatrixZ;
		cMatrixZ.CopyMatrix (MergeMatrix(MergeMatrix(MergeMatrix(cMatrixJ1, cMatrixExHideLayerDelta.Transpose ()), cMatrixJ2), cMatrixExOutputLayerDelta.Transpose ()));


		CMatrix cMatrixMOutputLayerError;
		cMatrixMOutputLayerError.CopyMatrix ( cMatrixOutputLayerError.MergeColumnsToColumnVector () );


		CMatrix	cMatrixJE;
		cMatrixJE.CopyMatrix ( (cMatrixZ.Transpose ()) * cMatrixMOutputLayerError );


		CMatrix cMatrixJJ;
		cMatrixJJ.CopyMatrix ( (cMatrixZ.Transpose ()) * cMatrixZ );


		// �����µ�����㵽�������Ȩֵ
		CMatrix cMatrixNewInputToHideWeight;
		// ������µ�������ķ�ֵ
		CMatrix cMatrixNewHideLayerValve;
		// �����µ������㵽������Ȩֵ
		CMatrix cMatrixNewHideToOutputWeight;
		// �����µ������ķ�ֵ
		CMatrix cMatrixNewOutputLayerValve;

		// �����µ�������
		CMatrix cMatrixNewOutputLayerError(nOutputLayerNumber, matrixDemoDataInput.GetMatrixRowNumber ());

		/////////////////////////////////////////////////////////////////
		// the weight value is adjusted

		while(nStep <= MAX_ADJUST_VALUE)
		{
			CMatrix	cMatrixI (cMatrixZ.GetMatrixColNumber (), cMatrixZ.GetMatrixColNumber ());

			cMatrixI.Eye ();


			CMatrix cMatrixDX;
			cMatrixDX.CopyMatrix ( (((cMatrixJJ + cMatrixI * nStep).Inverse ()) * cMatrixJE) * (-1.0) );


			/////////////////////////////////////////////////////////////////////////
			// ���cMatrixDX����

			unsigned int nIndex = 0;

			// �õ�����㵽�������Ȩֵ��������
			CMatrix cMatrixInputToHideWeightChange(nHideLayerNumber, nInputLayerNumber);

			cMatrixDX.CopySubMatrixFromVector (cMatrixInputToHideWeightChange, nIndex);

			nIndex += nHideLayerNumber * nInputLayerNumber;

			// �õ������㷧ֵ��������
			CMatrix cMatrixHideLayerValveChange (nHideLayerNumber, (unsigned int)1);

			cMatrixDX.CopySubMatrixFromVector (cMatrixHideLayerValveChange, nIndex);


			nIndex += nHideLayerNumber;

			// �õ������㵽������Ȩֵ��������
			CMatrix cMatrixHideToOutputWeightChange (nOutputLayerNumber, nHideLayerNumber);

			cMatrixDX.CopySubMatrixFromVector (cMatrixHideToOutputWeightChange, nIndex);

			nIndex += nOutputLayerNumber * nHideLayerNumber;

			// �õ�����㷧ֵ������ֵ
			CMatrix cMatrixOutputValveChange (nOutputLayerNumber, (unsigned int)1);

			cMatrixDX.CopySubMatrixFromVector (cMatrixOutputValveChange, nIndex);

			cMatrixNewInputToHideWeight.CopyMatrix (matrixInputToHideWeightValue + cMatrixInputToHideWeightChange);

			cMatrixNewHideLayerValve.CopyMatrix (matrixHideLayerValveValue + cMatrixHideLayerValveChange);

			cMatrixNewHideToOutputWeight.CopyMatrix (matrixHideToOutputWeightValue + cMatrixHideToOutputWeightChange);

			cMatrixNewOutputLayerValve.CopyMatrix (matrixOutputLayerValveValue + cMatrixOutputValveChange);

			// ǰ�����
			LMForwardCalculate (nInputLayerNumber,
				nHideLayerNumber,
				nOutputLayerNumber,
				bSimulateDataFlag,
				nComboFunc,
				matrixDemoDataInput,
				matrixInputLayerValue,
				cMatrixNewInputToHideWeight,
				cMatrixNewHideLayerValve,
				matrixHideLayerOutput,
				cMatrixNewHideToOutputWeight,
				matrixOutputLayerOutput,
				cMatrixNewOutputLayerValve
				);


			cMatrixNewOutputLayerError = cMatrixDemoOutput - matrixOutputLayerOutput;
			nSystemErrorNew = 	cMatrixNewOutputLayerError.GetSystemError ();

			if(nSystemErrorNew < nSystemErrorOld)
			{
				break;
			}
			else
			{
				nStep *= 10;
			}

		}// End for while loop

		if ( nStep > MAX_ADJUST_VALUE)
		{
			nLoopTimes--;
			return false;
		}

		nStep	*= 0.1;

		// ��ֵ
		matrixInputToHideWeightValue = cMatrixNewInputToHideWeight;
		matrixHideLayerValveValue = cMatrixNewHideLayerValve;
		matrixHideToOutputWeightValue = cMatrixNewHideToOutputWeight;
		matrixOutputLayerValveValue = cMatrixNewOutputLayerValve;
		cMatrixOutputLayerError = cMatrixNewOutputLayerError;
		nSystemErrorOld = nSystemErrorNew;

		// ��ʾ���ݺͳ�������״̬
		nSystemError = nSystemErrorOld;
		nTrainTimes = nLoopTimes;

		//����ϵͳ���
		nCurSystemError = nSystemError;

		// ����ѵ������
		nCurTrainTime = nTrainTimes+1;

	}// End the "for" loop

	return true;
}

/////////////////////////////////////////////////////////////////////////////
//	Back propagation		---->		ǰ�����(Only for Training)		   //
/////////////////////////////////////////////////////////////////////////////
__declspec(dllexport) void BPForwardCalculate ( int nInputLayerNumber,
	int nHideLayerNumber,
	int nOutputLayerNumber,
	bool bSimulateDataFlag,
	int nComboFunc,
	CMatrix &matrixDemoDataInput,
	CMatrix &matrixInputLayerValue,
	CMatrix &matrixInputToHideWeightValue,
	CMatrix &matrixHideLayerValveValue,
	CMatrix &matrixHideLayerOutput,
	CMatrix &matrixHideToOutputWeightValue,
	CMatrix &matrixOutputLayerOutput,
	CMatrix &matrixOutputLayerValveValue,
	CMatrix &cMatrixExHideLayerValveValue,
	CMatrix &cMatrixExOutputLayerValveValue
	)
{

	if(bSimulateDataFlag)
	{

		CMatrix cMatrixInputLayerValue(matrixDemoDataInput.GetMatrixRowNumber (), nInputLayerNumber);

		// �õ�����������ֵ
		matrixDemoDataInput.CopySubMatrix (cMatrixInputLayerValue, (unsigned int)0, (unsigned int)0);

		CMatrix cMatrixTInputLayerValue = cMatrixInputLayerValue.Transpose ();

		matrixInputLayerValue.CopyMatrix (cMatrixTInputLayerValue);


	}

	/////////////////////////////////////////////////////////////////////////
	// �õ�����������������ľ�����
	// �������:
	//		1. ��������Ŀ��Ϊ��������;
	//		2. �������������������Ŀ��Ϊ���������;
	//		3. ����Ԫ���е�ֵ��Ϊ��Ӧ��������������ľ�����:
	//		   �� 
	//				cMatrixInputLayerValue * cMatrixInputToHideWeightValue 
	//				+ cMatrixHideLayerValveValue
	//			�õ�.
	//

	//CMatrix cMatrixExHideLayerValveValue;
	//cMatrixExHideLayerValveValue.nncpyi (matrixHideLayerValveValue, matrixDemoDataInput.GetMatrixRowNumber ());

	CMatrix cMatrixHideLayerPureInput(nHideLayerNumber, matrixDemoDataInput.GetMatrixRowNumber ());

	cMatrixHideLayerPureInput = matrixInputToHideWeightValue * matrixInputLayerValue;

	cMatrixHideLayerPureInput += cMatrixExHideLayerValveValue;

	/////////////////////////////////////////////////////////////////////
	//	�����������������������
	//	�������:
	//		1. ����������y�������������x�Ĺ�ϵ���ú�����ʾ
	//			y = f(x)
	//		2. �����ά����������ľ���������ά�����
	//

	CMatrix cMatrixHideLayerOutput(nHideLayerNumber, matrixDemoDataInput.GetMatrixRowNumber ());

	switch(nComboFunc)
	{
	case 0:
		cMatrixHideLayerOutput = cMatrixHideLayerPureInput.Sigmoid ();
		break;

	case 1:
		cMatrixHideLayerOutput = cMatrixHideLayerPureInput.tanh ();
		break;

	case 2:
		cMatrixHideLayerOutput = cMatrixHideLayerPureInput.Tansig();
		break;

	default:
		return;
	}

	matrixHideLayerOutput.CopyMatrix(cMatrixHideLayerOutput);

	/////////////////////////////////////////////////////////////////////
	//	�õ��������������ľ�����
	//	�������;
	//		1. ��������Ŀ��Ϊ���������;
	//		2. ������������������Ŀ��Ϊ���������;
	//		3. ������Ԫ�ص�ֵ��Ϊ��Ӧ�����������ľ�����:
	//			��
	//				cMatrixHideLayerOutput * cMatrixHideToOutputWeightValue
	//				+ cMatrixOutputLayerValveValue
	//			�õ�
	//

	//CMatrix cMatrixExOutputLayerValveValue;
	//cMatrixExOutputLayerValveValue.nncpyi (matrixOutputLayerValveValue, matrixDemoDataInput.GetMatrixRowNumber ());

	CMatrix cMatrixOutputLayerPureInput(nOutputLayerNumber, matrixDemoDataInput.GetMatrixRowNumber ());

	cMatrixOutputLayerPureInput = matrixHideToOutputWeightValue * cMatrixHideLayerOutput;
	cMatrixOutputLayerPureInput  += cMatrixExOutputLayerValveValue;


	/////////////////////////////////////////////////////////////////////
	//	����������������������
	//	�������:
	//		1. �����ά����õ������������������ľ�������ɵľ���һ��;
	//		2. ���������y��������������ù�ϵʽ
	//			y = f(x)
	//		��ʾ
	//

	CMatrix cMatrixOutputLayerOutput(nOutputLayerNumber, matrixDemoDataInput.GetMatrixRowNumber ());

	switch(nComboFunc)
	{
	case 0:
		cMatrixOutputLayerOutput = cMatrixOutputLayerPureInput.Sigmoid ();
		break;

	case 1:
		cMatrixOutputLayerOutput = cMatrixOutputLayerPureInput.tanh ();
		break;

	case 2:
		cMatrixOutputLayerOutput = cMatrixOutputLayerPureInput.Tansig ();
		break;

	default:
		return ;
	}

	matrixOutputLayerOutput.CopyMatrix(cMatrixOutputLayerOutput);

}

/////////////////////////////////////////////////////////////////////////////
//	Back propagation		---->		ǰ�����(Only for Simulating)	   //
/////////////////////////////////////////////////////////////////////////////
__declspec(dllexport) void BPForwardCalculate2( int nInputLayerNumber,
	int nHideLayerNumber,
	int nOutputLayerNumber,
	bool bSimulateDataFlag,
	int nComboFunc,
	CMatrix &matrixDemoDataInput,
	CMatrix &matrixInputLayerValue,
	CMatrix &matrixInputToHideWeightValue,
	CMatrix &matrixHideLayerValveValue,
	CMatrix &matrixHideLayerOutput,
	CMatrix &matrixHideToOutputWeightValue,
	CMatrix &matrixOutputLayerOutput,
	CMatrix &matrixOutputLayerValveValue
	)
{

	if(bSimulateDataFlag)
	{

		CMatrix cMatrixInputLayerValue(matrixDemoDataInput.GetMatrixRowNumber (), nInputLayerNumber);

		// �õ�����������ֵ
		matrixDemoDataInput.CopySubMatrix (cMatrixInputLayerValue, (unsigned int)0, (unsigned int)0);

		CMatrix cMatrixTInputLayerValue = cMatrixInputLayerValue.Transpose ();

		matrixInputLayerValue.CopyMatrix (cMatrixTInputLayerValue);


	}

	/////////////////////////////////////////////////////////////////////////
	// �õ�����������������ľ�����
	// �������:
	//		1. ��������Ŀ��Ϊ��������;
	//		2. �������������������Ŀ��Ϊ���������;
	//		3. ����Ԫ���е�ֵ��Ϊ��Ӧ��������������ľ�����:
	//		   �� 
	//				cMatrixInputLayerValue * cMatrixInputToHideWeightValue 
	//				+ cMatrixHideLayerValveValue
	//			�õ�.
	//

	CMatrix cMatrixExHideLayerValveValue;
	cMatrixExHideLayerValveValue.nncpyi (matrixHideLayerValveValue, matrixDemoDataInput.GetMatrixRowNumber ());

	CMatrix cMatrixHideLayerPureInput(nHideLayerNumber, matrixDemoDataInput.GetMatrixRowNumber ());

	cMatrixHideLayerPureInput = matrixInputToHideWeightValue * matrixInputLayerValue;

	cMatrixHideLayerPureInput += cMatrixExHideLayerValveValue;


	/////////////////////////////////////////////////////////////////////
	//	�����������������������
	//	�������:
	//		1. ����������y�������������x�Ĺ�ϵ���ú�����ʾ
	//			y = f(x)
	//		2. �����ά����������ľ���������ά�����
	//

	CMatrix cMatrixHideLayerOutput(nHideLayerNumber, matrixDemoDataInput.GetMatrixRowNumber ());

	switch(nComboFunc)
	{
	case 0:
		cMatrixHideLayerOutput = cMatrixHideLayerPureInput.Sigmoid ();
		break;

	case 1:
		cMatrixHideLayerOutput = cMatrixHideLayerPureInput.tanh ();
		break;

	case 2:
		cMatrixHideLayerOutput = cMatrixHideLayerPureInput.Tansig();
		break;

	default:
		return;
	}

	matrixHideLayerOutput.CopyMatrix(cMatrixHideLayerOutput);

	/////////////////////////////////////////////////////////////////////
	//	�õ��������������ľ�����
	//	�������;
	//		1. ��������Ŀ��Ϊ���������;
	//		2. ������������������Ŀ��Ϊ���������;
	//		3. ������Ԫ�ص�ֵ��Ϊ��Ӧ�����������ľ�����:
	//			��
	//				cMatrixHideLayerOutput * cMatrixHideToOutputWeightValue
	//				+ cMatrixOutputLayerValveValue
	//			�õ�
	//

	CMatrix cMatrixExOutputLayerValveValue;
	cMatrixExOutputLayerValveValue.nncpyi (matrixOutputLayerValveValue, matrixDemoDataInput.GetMatrixRowNumber ());

	CMatrix cMatrixOutputLayerPureInput(nOutputLayerNumber, matrixDemoDataInput.GetMatrixRowNumber ());

	cMatrixOutputLayerPureInput = matrixHideToOutputWeightValue * cMatrixHideLayerOutput;
	cMatrixOutputLayerPureInput  += cMatrixExOutputLayerValveValue;


	/////////////////////////////////////////////////////////////////////
	//	����������������������
	//	�������:
	//		1. �����ά����õ������������������ľ�������ɵľ���һ��;
	//		2. ���������y��������������ù�ϵʽ
	//			y = f(x)
	//		��ʾ
	//

	CMatrix cMatrixOutputLayerOutput(nOutputLayerNumber, matrixDemoDataInput.GetMatrixRowNumber ());

	switch(nComboFunc)
	{
	case 0:
		cMatrixOutputLayerOutput = cMatrixOutputLayerPureInput.Sigmoid ();
		break;

	case 1:
		cMatrixOutputLayerOutput = cMatrixOutputLayerPureInput.tanh ();
		break;

	case 2:
		cMatrixOutputLayerOutput = cMatrixOutputLayerPureInput.Tansig ();
		break;

	default:
		return ;
	}

	matrixOutputLayerOutput.CopyMatrix(cMatrixOutputLayerOutput);

}

/////////////////////////////////////////////////////////////////////////////
//	Back propagation		---->		��������						   //
/////////////////////////////////////////////////////////////////////////////
__declspec(dllexport) bool BPDemoDataTrainRepeat (	int nInputLayerNumber,
	int nHideLayerNumber,
	int nOutputLayerNumber,
	bool bSimulateDataFlag,
	int nComboFunc,
	double nSystemErrorOld,
	double nSystemErrorNew,
	double nSystemErrorLevel,
	double nSystemError,
	double nStep,
	UINT nMaxTrainTimes,
	UINT nTrainTimes,
	double& nCurSystemError,
	int&    nCurTrainTimes,
	CMatrix &matrixDemoDataInput,
	CMatrix &matrixInputLayerValue,
	CMatrix &matrixInputToHideWeightValue,
	CMatrix &matrixHideLayerValveValue,
	CMatrix &matrixHideLayerOutput,
	CMatrix &matrixHideToOutputWeightValue,
	CMatrix &matrixOutputLayerOutput,
	CMatrix &matrixOutputLayerValveValue
	)
{
	// ����BP�㷨����nStep�ĳ�ʼֵ
	nStep = 0.1;

	// ǰ�����
	LMForwardCalculate (nInputLayerNumber,
		nHideLayerNumber,
		nOutputLayerNumber,
		bSimulateDataFlag,
		nComboFunc,
		matrixDemoDataInput,
		matrixInputLayerValue,
		matrixInputToHideWeightValue,
		matrixHideLayerValveValue,
		matrixHideLayerOutput,
		matrixHideToOutputWeightValue,
		matrixOutputLayerOutput,
		matrixOutputLayerValveValue
		);


	/////////////////////////////////////////////////////////////////////
	//	�������������������delta����
	//	�������:
	//		1. ��������ĿΪ���������;
	//		2. ������������ĿΪ���������;
	//		3. �����е�Ԫ�ص�ֵyΪ:
	//				y = (ǰ��������������ֵ - ������������ֵ) .* f'(net)
	//
	CMatrix cMatrixTDemoOutput(matrixDemoDataInput.GetMatrixRowNumber (), nOutputLayerNumber);

	// �õ�����������������
	matrixDemoDataInput.CopySubMatrix (cMatrixTDemoOutput, (unsigned int)nInputLayerNumber, (unsigned int)0);

	CMatrix cMatrixDemoOutput = cMatrixTDemoOutput.Transpose ();

	// �õ����������������
	CMatrix cMatrixOutputLayerError(nOutputLayerNumber, matrixDemoDataInput.GetMatrixRowNumber ());
	cMatrixOutputLayerError = cMatrixDemoOutput - matrixOutputLayerOutput;

	nSystemErrorOld = cMatrixOutputLayerError.GetSystemError ();

	for(int nLoopTimes=1; nLoopTimes < nMaxTrainTimes; nLoopTimes++)	
	{
		if(nSystemErrorOld < nSystemErrorLevel)
		{
			nLoopTimes--;
			break;
		}
		if(IS_STOP)
			break;
		// ��������deltaֵ
		// ע��: �˴�'/' �� '���'!!!
		CMatrix	cMatrixOutputLayerDelta (nOutputLayerNumber, matrixDemoDataInput.GetMatrixRowNumber());
		cMatrixOutputLayerDelta = (matrixOutputLayerOutput - matrixOutputLayerOutput / matrixOutputLayerOutput) / cMatrixOutputLayerError;

		CMatrix cMatrixTHideToOutputWeightValue (matrixHideToOutputWeightValue.GetMatrixColNumber(), matrixHideToOutputWeightValue.GetMatrixRowNumber());
		cMatrixTHideToOutputWeightValue = matrixHideToOutputWeightValue.Transpose();

		// ���������deltaֵ
		// ע��: �˴�'/' �� '���'!!!
		CMatrix cMatrixHideLayerDelta;
		cMatrixHideLayerDelta.CopyMatrix ( (matrixHideLayerOutput - (matrixHideLayerOutput / matrixHideLayerOutput)) / ( cMatrixTHideToOutputWeightValue * cMatrixOutputLayerDelta) );

		// �����µ�����㵽�������Ȩֵ
		CMatrix cMatrixNewInputToHideWeight (matrixInputToHideWeightValue.GetMatrixRowNumber (), matrixInputToHideWeightValue.GetMatrixColNumber ());
		// ������µ�������ķ�ֵ
		CMatrix cMatrixNewHideLayerValve (nHideLayerNumber, matrixDemoDataInput.GetMatrixRowNumber ());
		// �����µ������㵽������Ȩֵ
		CMatrix cMatrixNewHideToOutputWeight (matrixHideToOutputWeightValue.GetMatrixRowNumber (), matrixHideToOutputWeightValue.GetMatrixColNumber ());
		// �����µ������ķ�ֵ
		CMatrix cMatrixNewOutputLayerValve (nOutputLayerNumber, matrixDemoDataInput.GetMatrixRowNumber ());
		// �����µ�������
		CMatrix cMatrixNewOutputLayerError(nOutputLayerNumber, matrixDemoDataInput.GetMatrixRowNumber ());


		// Ȩֵ�ͷ�ֵ����
		cMatrixNewHideToOutputWeight = cMatrixOutputLayerDelta * (matrixHideLayerOutput.Transpose ()) * (nStep);
		cMatrixNewOutputLayerValve = cMatrixOutputLayerDelta;

		cMatrixNewInputToHideWeight = cMatrixHideLayerDelta * (matrixInputLayerValue.Transpose ()) * (nStep);
		cMatrixNewHideLayerValve = cMatrixHideLayerDelta;

		// ��ֵ
		matrixInputToHideWeightValue += cMatrixNewInputToHideWeight;

		CMatrix cMatrixExHideLayerValveValue;
		cMatrixExHideLayerValveValue.nncpyi (matrixHideLayerValveValue, matrixDemoDataInput.GetMatrixRowNumber ());
		cMatrixExHideLayerValveValue += cMatrixNewHideLayerValve;

		matrixHideToOutputWeightValue += cMatrixNewHideToOutputWeight;

		CMatrix cMatrixExOutputLayerValveValue;
		cMatrixExOutputLayerValveValue.nncpyi (matrixOutputLayerValveValue, matrixDemoDataInput.GetMatrixRowNumber ());
		cMatrixExOutputLayerValveValue += cMatrixNewOutputLayerValve;

		// ǰ�����
		BPForwardCalculate (nInputLayerNumber,
			nHideLayerNumber,
			nOutputLayerNumber,
			bSimulateDataFlag,
			nComboFunc,
			matrixDemoDataInput,
			matrixInputLayerValue,
			matrixInputToHideWeightValue,
			matrixHideLayerValveValue,
			matrixHideLayerOutput,
			matrixHideToOutputWeightValue,
			matrixOutputLayerOutput,
			matrixOutputLayerValveValue,
			cMatrixExHideLayerValveValue,
			cMatrixExOutputLayerValveValue
			);


		cMatrixNewOutputLayerError = cMatrixDemoOutput - matrixOutputLayerOutput;
		nSystemErrorNew = 	cMatrixNewOutputLayerError.GetSystemError ();

		cMatrixOutputLayerError = cMatrixNewOutputLayerError;

		if(nSystemErrorNew < nSystemErrorOld)
		{
			nSystemErrorOld = nSystemErrorNew;
		}
		else
		{
			nStep *= -0.1;
		}

		// ��ʾ���ݺͳ�������״̬
		nSystemError = nSystemErrorOld;
		nTrainTimes = nLoopTimes;


		// ����ϵͳ���
		nCurSystemError = nSystemError;

		// ����ѵ������
		nCurTrainTimes = nTrainTimes+1;

	}// End the "for" loop

	return true;
}


int *curtime;
double *curerror;


struct fann *pAnn = NULL;

int FANN_API myCallback(struct fann *ann, struct fann_train_data *train,
	unsigned int max_epochs, unsigned int epochs_between_reports, 
	float desired_error, unsigned int epochs)
{
	*curtime = epochs;
	*curerror = fann_get_MSE(ann);
	return 0;
}


__declspec(dllexport) bool FANNTrainRepeat( int nInputLayerNumber,
	int nHideLayerNumber,
	int nOutputLayerNumber,
	bool bSimulateDataFlag,
	int nHideFunc,
	int nOutFunc,
	int nMaxTrainTime,
	double nSystemError,
	int& nCurTime,
	double& nCurError, 
	char *nDatapath,		/*train data path*/
	char *nDestPath			/*for the net path*/
	)
{
	curtime = &nCurTime;
	curerror = &nCurError;
	if(nDatapath==NULL||nDestPath==NULL)
		return false;
	struct fann *ann = fann_create_standard(3, nInputLayerNumber, nHideLayerNumber, nOutputLayerNumber);
	fann_set_activation_function_hidden(ann, (fann_activationfunc_enum)nHideFunc);
	fann_set_activation_function_output(ann, (fann_activationfunc_enum)nOutFunc);
	struct fann_train_data *data;
	data = fann_read_train_from_file(nDatapath);
	if(data==NULL)
		return false;
	fann_set_callback(ann , myCallback);

	//fann_set_training_algorithm(ann, FANN_TRAIN_RPROP);//change.

	//fann_init_weights(ann, data);

	fann_train_on_data(ann, data, nMaxTrainTime, 1, nSystemError);

	fann_save(ann, nDestPath);

	fann_destroy_train(data);
	fann_destroy(ann);
	return true;
}

__declspec(dllexport) bool SimFannNet(float *pd ,const  int num , double* outres)
{
	if(pd==NULL||outres==NULL)
		return false;
	if(pAnn==NULL)
		return false;
	if(num!=pAnn->num_input)
		return false;
	fann_reset_MSE(pAnn);
	fann_type *pdata = new fann_type[num];
	for (int i=0;i<num;i++)
	{
		pdata[i] = (fann_type)pd[i];
	}
	fann_type *calout = fann_run(pAnn , pdata);

	*outres = *calout;
	delete pdata;
	return true;
}

__declspec(dllexport) bool CreateNetWork(char* path)		//just for FANN
{
	if(path==NULL)
		return false;
	if(pAnn==NULL)
	{
		pAnn = fann_create_from_file(path);
		if(!pAnn)
			return false;
	}
	return true;
}

__declspec(dllexport) bool DelteNetWork()
{
	if(pAnn==NULL)
		return false;
	else
		fann_destroy(pAnn);
	return true;
}

__declspec(dllexport) void StopTrain(const bool b)
{
	IS_STOP = b;
}
