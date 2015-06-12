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
//	Levenberg-Marquart		---->	第一次前向计算初始化				   //
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
	// 构造输入层元素的矩阵
	// 构造规则:
	//		1. 样本数目做为矩阵的行数;
	//		2. 单个样本的输入层的数目做为矩阵的列数;
	//		3. 矩阵中的元素即为对应的输入层的值
	//

	CMatrix cMatrixInputLayerValue(matrixDemoDataInput.GetMatrixRowNumber (), nInputLayerNumber);

	// 得到样本的输入值
	matrixDemoDataInput.CopySubMatrix (cMatrixInputLayerValue,(unsigned int)0,(unsigned int)0);

	CMatrix cMatrixTInputLayerValue = cMatrixInputLayerValue.Transpose ();

	matrixInputLayerValue.CopyMatrix (cMatrixTInputLayerValue);


	/////////////////////////////////////////////////////////////////////////
	// 构造权值矩阵 -----> 由单个样本输入层与隐含层之间的权值做为元素组成
	// 构造规则:
	//		1. 单个样本的输入层的数目做为矩阵行数;
	//		2. 单个样本的隐含层的数目做为矩阵的列数;
	//		3. 对矩阵中的元素进行随机初始化,值在(-1,1)之间;
	//		4. 所有样本的输入层和隐含层的数目是相等的;
	//		5. 所有样本使用的是同一个权值矩阵.
	//

	CMatrix	cMatrixInputToHideWeightValue(nHideLayerNumber, nInputLayerNumber);

	// 随机初始化矩阵内元素的值
	cMatrixInputToHideWeightValue.RandomInitialize ();

	matrixInputToHideWeightValue.CopyMatrix (cMatrixInputToHideWeightValue);


	/////////////////////////////////////////////////////////////////////
	//	构造样本隐含层的阀值矩阵
	//	构造规则:
	//		1. 样本的数目做为矩阵行数;
	//		2. 单个样本的隐含层的数目做为矩阵的列数;
	//		3. 随机初始化矩阵中的元素,使值在(-1,1)之间;
	//		4. 矩阵中每行的数据都和第一行数据相对应的位置相等.
	//

	CMatrix cMatrixHideLayerValveValue(nHideLayerNumber,(unsigned int)1);

	// 随机初始化矩阵内元素的值
	cMatrixHideLayerValveValue.RandomInitialize ();

	matrixHideLayerValveValue.CopyMatrix(cMatrixHideLayerValveValue);


	/////////////////////////////////////////////////////////////////////
	//	构造权值矩阵	----->	由单个样本的隐含层与输出层之间权值做为元素
	//							组成
	//	构造规则:
	//		1. 单个样本的隐含层的数目做为矩阵的行数;
	//		2. 单个样本的输出层的数目做为矩阵的列数;
	//		3. 对矩阵中的元素进行随机初始化,值在(-1,1)之间;
	//		4. 所有样本的隐含层和输出层的数目是相等的;
	//		5. 所有样本使用的是同一个权值矩阵.
	//

	CMatrix cMatrixHideToOutputWeightValue(nOutputLayerNumber, nHideLayerNumber);

	// 对矩阵的元素随机初始化
	cMatrixHideToOutputWeightValue.RandomInitialize ();

	matrixHideToOutputWeightValue.CopyMatrix (cMatrixHideToOutputWeightValue);


	/////////////////////////////////////////////////////////////////////
	//	构造样本的输出层的阀值矩阵
	//	构造规则:
	//		1. 样本的数目做为矩阵的行数;
	//		2. 单个样本的输出层的数目做为矩阵的列数;
	//		3. 随机初始化矩阵中的元素,使值在(-1,1)之间;
	//		4. 矩阵中每行的数据都和第一行数据相对应的位置相等.
	//

	CMatrix cMatrixOutputLayerValveValue(nOutputLayerNumber,(unsigned int)1);

	// 随机初始化矩阵内元素的值
	cMatrixOutputLayerValveValue.RandomInitialize ();

	matrixOutputLayerValveValue.CopyMatrix(cMatrixOutputLayerValveValue);


}

/////////////////////////////////////////////////////////////////////////////
//	Levenberg-Marquart		---->		前向计算						   //
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

		// 得到样本的输入值
		matrixDemoDataInput.CopySubMatrix (cMatrixInputLayerValue, (unsigned int)0, (unsigned int)0);

		CMatrix cMatrixTInputLayerValue = cMatrixInputLayerValue.Transpose ();

		matrixInputLayerValue.CopyMatrix (cMatrixTInputLayerValue);


	}

	/////////////////////////////////////////////////////////////////////////
	// 得到所有样本的隐含层的净输入
	// 构造规则:
	//		1. 样本的数目做为矩阵行数;
	//		2. 单个样本的隐含层的数目做为矩阵的列数;
	//		3. 矩阵元素中的值即为对应的样本的隐含层的净输入:
	//		   由 
	//				cMatrixInputLayerValue * cMatrixInputToHideWeightValue 
	//				+ cMatrixHideLayerValveValue
	//			得到.
	//

	CMatrix cMatrixExHideLayerValveValue;
	cMatrixExHideLayerValveValue.nncpyi (matrixHideLayerValveValue, matrixDemoDataInput.GetMatrixRowNumber ());

	CMatrix cMatrixHideLayerPureInput(nHideLayerNumber, matrixDemoDataInput.GetMatrixRowNumber ());

	cMatrixHideLayerPureInput = matrixInputToHideWeightValue * matrixInputLayerValue;

	cMatrixHideLayerPureInput += cMatrixExHideLayerValveValue;


	/////////////////////////////////////////////////////////////////////
	//	算出所有样本的隐含层的输出
	//	构造规则:
	//		1. 隐含层的输出y与隐含层的输入x的关系可用函数表示
	//			y = f(x)
	//		2. 矩阵的维数和隐含层的净输入矩阵的维数相等
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
	//	得到所有样本输出层的净输入
	//	构造规则;
	//		1. 样本的数目做为矩阵的行数;
	//		2. 单个样本的输出层的数目做为矩阵的列数;
	//		3. 矩阵中元素的值即为对应样本的输出层的净输入:
	//			由
	//				cMatrixHideLayerOutput * cMatrixHideToOutputWeightValue
	//				+ cMatrixOutputLayerValveValue
	//			得到
	//

	CMatrix cMatrixExOutputLayerValveValue;
	cMatrixExOutputLayerValveValue.nncpyi (matrixOutputLayerValveValue, matrixDemoDataInput.GetMatrixRowNumber ());

	CMatrix cMatrixOutputLayerPureInput(nOutputLayerNumber, matrixDemoDataInput.GetMatrixRowNumber ());

	cMatrixOutputLayerPureInput = matrixHideToOutputWeightValue * cMatrixHideLayerOutput;
	cMatrixOutputLayerPureInput  += cMatrixExOutputLayerValveValue;


	/////////////////////////////////////////////////////////////////////
	//	算出所有样本的输出层的输出
	//	构造规则:
	//		1. 矩阵的维数与得到的所有样本的输出层的净输入组成的矩阵一样;
	//		2. 输出层的输出y和输出层的输入可用关系式
	//			y = f(x)
	//		表示
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
//	Levenberg-Marquart		---->		反馈计算						   //
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
	double& nCurSystemError,					//返回系统误差
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
	//	算出所有样本的输出层的delta矩阵
	//	构造规则:
	//		1. 样本的数目为矩阵的行数;
	//		2. 样本输出层的数目为矩阵的列数;
	//		3. 矩阵中的元素的值y为:
	//				y = -(前向计算出的输出层的值 - 样本的输出层的值) * f'(net)
	//


	CMatrix cMatrixTDemoOutput(matrixDemoDataInput.GetMatrixRowNumber (), nOutputLayerNumber);

	// 得到样本中输出层的数据
	matrixDemoDataInput.CopySubMatrix (cMatrixTDemoOutput, (unsigned int)nInputLayerNumber, (unsigned int)0);

	CMatrix cMatrixDemoOutput = cMatrixTDemoOutput.Transpose ();

	// 得到样本中输出层的误差
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

		// 注意: 此处'/' 是 '点乘'!!!
		cMatrixOutputLayerDelta = 1 - matrixOutputLayerOutput / matrixOutputLayerOutput;

		CMatrix cMatrixExOutputLayerDelta;
		cMatrixExOutputLayerDelta.nncpyd (cMatrixOutputLayerDelta);

		cMatrixExOutputLayerDelta = cMatrixExOutputLayerDelta * (-1.0);


		CMatrix cMatrixTHideToOutputWeightValue (matrixHideToOutputWeightValue.GetMatrixColNumber(), matrixHideToOutputWeightValue.GetMatrixRowNumber());
		cMatrixTHideToOutputWeightValue = matrixHideToOutputWeightValue.Transpose();

		CMatrix cMatrixExHideLayerDelta;
		// 注意: 此处'/' 是 '点乘'!!!
		cMatrixExHideLayerDelta.CopyMatrix ( (1 - (cMatrixExHideLayerOutput / cMatrixExHideLayerOutput)) / ( cMatrixTHideToOutputWeightValue * cMatrixExOutputLayerDelta) );


		CMatrix	cMatrixExInputLayerValue;
		cMatrixExInputLayerValue.nncpyi (matrixInputLayerValue, nOutputLayerNumber);


		CMatrix	cMatrixJ11;
		cMatrixJ11.nncpy (cMatrixExHideLayerDelta.Transpose(), cMatrixExInputLayerValue.GetMatrixRowNumber ());

		CMatrix cMatrixJ12;
		cMatrixJ12.nncpyi(cMatrixExInputLayerValue.Transpose (), cMatrixExHideLayerDelta.GetMatrixRowNumber());


		CMatrix	cMatrixJ1;
		// 注意: 此处'/' 是 '点乘'!!!
		cMatrixJ1.CopyMatrix (cMatrixJ11 / cMatrixJ12);


		CMatrix cMatrixJ21;
		cMatrixJ21.nncpy (cMatrixExOutputLayerDelta.Transpose (), cMatrixExHideLayerOutput.GetMatrixRowNumber ());

		CMatrix cMatrixJ22;
		cMatrixJ22.nncpyi (cMatrixExHideLayerOutput.Transpose (), cMatrixExOutputLayerDelta.GetMatrixRowNumber ());

		CMatrix cMatrixJ2;
		// 注意: 此处'/' 是 '点乘'!!!
		cMatrixJ2.CopyMatrix (cMatrixJ21 / cMatrixJ22);


		CMatrix cMatrixZ;
		cMatrixZ.CopyMatrix (MergeMatrix(MergeMatrix(MergeMatrix(cMatrixJ1, cMatrixExHideLayerDelta.Transpose ()), cMatrixJ2), cMatrixExOutputLayerDelta.Transpose ()));


		CMatrix cMatrixMOutputLayerError;
		cMatrixMOutputLayerError.CopyMatrix ( cMatrixOutputLayerError.MergeColumnsToColumnVector () );


		CMatrix	cMatrixJE;
		cMatrixJE.CopyMatrix ( (cMatrixZ.Transpose ()) * cMatrixMOutputLayerError );


		CMatrix cMatrixJJ;
		cMatrixJJ.CopyMatrix ( (cMatrixZ.Transpose ()) * cMatrixZ );


		// 定义新的输入层到隐含层的权值
		CMatrix cMatrixNewInputToHideWeight;
		// 定义的新的隐含层的阀值
		CMatrix cMatrixNewHideLayerValve;
		// 定义新的隐含层到输出层的权值
		CMatrix cMatrixNewHideToOutputWeight;
		// 定义新的输出层的阀值
		CMatrix cMatrixNewOutputLayerValve;

		// 定义新的误差矩阵
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
			// 拆分cMatrixDX矩阵

			unsigned int nIndex = 0;

			// 得到输入层到隐含层的权值的修正量
			CMatrix cMatrixInputToHideWeightChange(nHideLayerNumber, nInputLayerNumber);

			cMatrixDX.CopySubMatrixFromVector (cMatrixInputToHideWeightChange, nIndex);

			nIndex += nHideLayerNumber * nInputLayerNumber;

			// 得到隐含层阀值的修正量
			CMatrix cMatrixHideLayerValveChange (nHideLayerNumber, (unsigned int)1);

			cMatrixDX.CopySubMatrixFromVector (cMatrixHideLayerValveChange, nIndex);


			nIndex += nHideLayerNumber;

			// 得到隐含层到输出层的权值的修正量
			CMatrix cMatrixHideToOutputWeightChange (nOutputLayerNumber, nHideLayerNumber);

			cMatrixDX.CopySubMatrixFromVector (cMatrixHideToOutputWeightChange, nIndex);

			nIndex += nOutputLayerNumber * nHideLayerNumber;

			// 得到输出层阀值的修正值
			CMatrix cMatrixOutputValveChange (nOutputLayerNumber, (unsigned int)1);

			cMatrixDX.CopySubMatrixFromVector (cMatrixOutputValveChange, nIndex);

			cMatrixNewInputToHideWeight.CopyMatrix (matrixInputToHideWeightValue + cMatrixInputToHideWeightChange);

			cMatrixNewHideLayerValve.CopyMatrix (matrixHideLayerValveValue + cMatrixHideLayerValveChange);

			cMatrixNewHideToOutputWeight.CopyMatrix (matrixHideToOutputWeightValue + cMatrixHideToOutputWeightChange);

			cMatrixNewOutputLayerValve.CopyMatrix (matrixOutputLayerValveValue + cMatrixOutputValveChange);

			// 前向计算
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

		// 赋值
		matrixInputToHideWeightValue = cMatrixNewInputToHideWeight;
		matrixHideLayerValveValue = cMatrixNewHideLayerValve;
		matrixHideToOutputWeightValue = cMatrixNewHideToOutputWeight;
		matrixOutputLayerValveValue = cMatrixNewOutputLayerValve;
		cMatrixOutputLayerError = cMatrixNewOutputLayerError;
		nSystemErrorOld = nSystemErrorNew;

		// 显示数据和程序运行状态
		nSystemError = nSystemErrorOld;
		nTrainTimes = nLoopTimes;

		//返回系统误差
		nCurSystemError = nSystemError;

		// 返回训练次数
		nCurTrainTime = nTrainTimes+1;

	}// End the "for" loop

	return true;
}

/////////////////////////////////////////////////////////////////////////////
//	Back propagation		---->		前向计算(Only for Training)		   //
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

		// 得到样本的输入值
		matrixDemoDataInput.CopySubMatrix (cMatrixInputLayerValue, (unsigned int)0, (unsigned int)0);

		CMatrix cMatrixTInputLayerValue = cMatrixInputLayerValue.Transpose ();

		matrixInputLayerValue.CopyMatrix (cMatrixTInputLayerValue);


	}

	/////////////////////////////////////////////////////////////////////////
	// 得到所有样本的隐含层的净输入
	// 构造规则:
	//		1. 样本的数目做为矩阵行数;
	//		2. 单个样本的隐含层的数目做为矩阵的列数;
	//		3. 矩阵元素中的值即为对应的样本的隐含层的净输入:
	//		   由 
	//				cMatrixInputLayerValue * cMatrixInputToHideWeightValue 
	//				+ cMatrixHideLayerValveValue
	//			得到.
	//

	//CMatrix cMatrixExHideLayerValveValue;
	//cMatrixExHideLayerValveValue.nncpyi (matrixHideLayerValveValue, matrixDemoDataInput.GetMatrixRowNumber ());

	CMatrix cMatrixHideLayerPureInput(nHideLayerNumber, matrixDemoDataInput.GetMatrixRowNumber ());

	cMatrixHideLayerPureInput = matrixInputToHideWeightValue * matrixInputLayerValue;

	cMatrixHideLayerPureInput += cMatrixExHideLayerValveValue;

	/////////////////////////////////////////////////////////////////////
	//	算出所有样本的隐含层的输出
	//	构造规则:
	//		1. 隐含层的输出y与隐含层的输入x的关系可用函数表示
	//			y = f(x)
	//		2. 矩阵的维数和隐含层的净输入矩阵的维数相等
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
	//	得到所有样本输出层的净输入
	//	构造规则;
	//		1. 样本的数目做为矩阵的行数;
	//		2. 单个样本的输出层的数目做为矩阵的列数;
	//		3. 矩阵中元素的值即为对应样本的输出层的净输入:
	//			由
	//				cMatrixHideLayerOutput * cMatrixHideToOutputWeightValue
	//				+ cMatrixOutputLayerValveValue
	//			得到
	//

	//CMatrix cMatrixExOutputLayerValveValue;
	//cMatrixExOutputLayerValveValue.nncpyi (matrixOutputLayerValveValue, matrixDemoDataInput.GetMatrixRowNumber ());

	CMatrix cMatrixOutputLayerPureInput(nOutputLayerNumber, matrixDemoDataInput.GetMatrixRowNumber ());

	cMatrixOutputLayerPureInput = matrixHideToOutputWeightValue * cMatrixHideLayerOutput;
	cMatrixOutputLayerPureInput  += cMatrixExOutputLayerValveValue;


	/////////////////////////////////////////////////////////////////////
	//	算出所有样本的输出层的输出
	//	构造规则:
	//		1. 矩阵的维数与得到的所有样本的输出层的净输入组成的矩阵一样;
	//		2. 输出层的输出y和输出层的输入可用关系式
	//			y = f(x)
	//		表示
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
//	Back propagation		---->		前向计算(Only for Simulating)	   //
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

		// 得到样本的输入值
		matrixDemoDataInput.CopySubMatrix (cMatrixInputLayerValue, (unsigned int)0, (unsigned int)0);

		CMatrix cMatrixTInputLayerValue = cMatrixInputLayerValue.Transpose ();

		matrixInputLayerValue.CopyMatrix (cMatrixTInputLayerValue);


	}

	/////////////////////////////////////////////////////////////////////////
	// 得到所有样本的隐含层的净输入
	// 构造规则:
	//		1. 样本的数目做为矩阵行数;
	//		2. 单个样本的隐含层的数目做为矩阵的列数;
	//		3. 矩阵元素中的值即为对应的样本的隐含层的净输入:
	//		   由 
	//				cMatrixInputLayerValue * cMatrixInputToHideWeightValue 
	//				+ cMatrixHideLayerValveValue
	//			得到.
	//

	CMatrix cMatrixExHideLayerValveValue;
	cMatrixExHideLayerValveValue.nncpyi (matrixHideLayerValveValue, matrixDemoDataInput.GetMatrixRowNumber ());

	CMatrix cMatrixHideLayerPureInput(nHideLayerNumber, matrixDemoDataInput.GetMatrixRowNumber ());

	cMatrixHideLayerPureInput = matrixInputToHideWeightValue * matrixInputLayerValue;

	cMatrixHideLayerPureInput += cMatrixExHideLayerValveValue;


	/////////////////////////////////////////////////////////////////////
	//	算出所有样本的隐含层的输出
	//	构造规则:
	//		1. 隐含层的输出y与隐含层的输入x的关系可用函数表示
	//			y = f(x)
	//		2. 矩阵的维数和隐含层的净输入矩阵的维数相等
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
	//	得到所有样本输出层的净输入
	//	构造规则;
	//		1. 样本的数目做为矩阵的行数;
	//		2. 单个样本的输出层的数目做为矩阵的列数;
	//		3. 矩阵中元素的值即为对应样本的输出层的净输入:
	//			由
	//				cMatrixHideLayerOutput * cMatrixHideToOutputWeightValue
	//				+ cMatrixOutputLayerValveValue
	//			得到
	//

	CMatrix cMatrixExOutputLayerValveValue;
	cMatrixExOutputLayerValveValue.nncpyi (matrixOutputLayerValveValue, matrixDemoDataInput.GetMatrixRowNumber ());

	CMatrix cMatrixOutputLayerPureInput(nOutputLayerNumber, matrixDemoDataInput.GetMatrixRowNumber ());

	cMatrixOutputLayerPureInput = matrixHideToOutputWeightValue * cMatrixHideLayerOutput;
	cMatrixOutputLayerPureInput  += cMatrixExOutputLayerValveValue;


	/////////////////////////////////////////////////////////////////////
	//	算出所有样本的输出层的输出
	//	构造规则:
	//		1. 矩阵的维数与得到的所有样本的输出层的净输入组成的矩阵一样;
	//		2. 输出层的输出y和输出层的输入可用关系式
	//			y = f(x)
	//		表示
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
//	Back propagation		---->		反馈计算						   //
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
	// 根据BP算法修正nStep的初始值
	nStep = 0.1;

	// 前向计算
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
	//	算出所有样本的输出层的delta矩阵
	//	构造规则:
	//		1. 样本的数目为矩阵的行数;
	//		2. 样本输出层的数目为矩阵的列数;
	//		3. 矩阵中的元素的值y为:
	//				y = (前向计算出的输出层的值 - 样本的输出层的值) .* f'(net)
	//
	CMatrix cMatrixTDemoOutput(matrixDemoDataInput.GetMatrixRowNumber (), nOutputLayerNumber);

	// 得到样本中输出层的数据
	matrixDemoDataInput.CopySubMatrix (cMatrixTDemoOutput, (unsigned int)nInputLayerNumber, (unsigned int)0);

	CMatrix cMatrixDemoOutput = cMatrixTDemoOutput.Transpose ();

	// 得到样本中输出层的误差
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
		// 求输出层的delta值
		// 注意: 此处'/' 是 '点乘'!!!
		CMatrix	cMatrixOutputLayerDelta (nOutputLayerNumber, matrixDemoDataInput.GetMatrixRowNumber());
		cMatrixOutputLayerDelta = (matrixOutputLayerOutput - matrixOutputLayerOutput / matrixOutputLayerOutput) / cMatrixOutputLayerError;

		CMatrix cMatrixTHideToOutputWeightValue (matrixHideToOutputWeightValue.GetMatrixColNumber(), matrixHideToOutputWeightValue.GetMatrixRowNumber());
		cMatrixTHideToOutputWeightValue = matrixHideToOutputWeightValue.Transpose();

		// 求隐含层的delta值
		// 注意: 此处'/' 是 '点乘'!!!
		CMatrix cMatrixHideLayerDelta;
		cMatrixHideLayerDelta.CopyMatrix ( (matrixHideLayerOutput - (matrixHideLayerOutput / matrixHideLayerOutput)) / ( cMatrixTHideToOutputWeightValue * cMatrixOutputLayerDelta) );

		// 定义新的输入层到隐含层的权值
		CMatrix cMatrixNewInputToHideWeight (matrixInputToHideWeightValue.GetMatrixRowNumber (), matrixInputToHideWeightValue.GetMatrixColNumber ());
		// 定义的新的隐含层的阀值
		CMatrix cMatrixNewHideLayerValve (nHideLayerNumber, matrixDemoDataInput.GetMatrixRowNumber ());
		// 定义新的隐含层到输出层的权值
		CMatrix cMatrixNewHideToOutputWeight (matrixHideToOutputWeightValue.GetMatrixRowNumber (), matrixHideToOutputWeightValue.GetMatrixColNumber ());
		// 定义新的输出层的阀值
		CMatrix cMatrixNewOutputLayerValve (nOutputLayerNumber, matrixDemoDataInput.GetMatrixRowNumber ());
		// 定义新的误差矩阵
		CMatrix cMatrixNewOutputLayerError(nOutputLayerNumber, matrixDemoDataInput.GetMatrixRowNumber ());


		// 权值和阀值调整
		cMatrixNewHideToOutputWeight = cMatrixOutputLayerDelta * (matrixHideLayerOutput.Transpose ()) * (nStep);
		cMatrixNewOutputLayerValve = cMatrixOutputLayerDelta;

		cMatrixNewInputToHideWeight = cMatrixHideLayerDelta * (matrixInputLayerValue.Transpose ()) * (nStep);
		cMatrixNewHideLayerValve = cMatrixHideLayerDelta;

		// 赋值
		matrixInputToHideWeightValue += cMatrixNewInputToHideWeight;

		CMatrix cMatrixExHideLayerValveValue;
		cMatrixExHideLayerValveValue.nncpyi (matrixHideLayerValveValue, matrixDemoDataInput.GetMatrixRowNumber ());
		cMatrixExHideLayerValveValue += cMatrixNewHideLayerValve;

		matrixHideToOutputWeightValue += cMatrixNewHideToOutputWeight;

		CMatrix cMatrixExOutputLayerValveValue;
		cMatrixExOutputLayerValveValue.nncpyi (matrixOutputLayerValveValue, matrixDemoDataInput.GetMatrixRowNumber ());
		cMatrixExOutputLayerValveValue += cMatrixNewOutputLayerValve;

		// 前向计算
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

		// 显示数据和程序运行状态
		nSystemError = nSystemErrorOld;
		nTrainTimes = nLoopTimes;


		// 返回系统误差
		nCurSystemError = nSystemError;

		// 返回训练次数
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
