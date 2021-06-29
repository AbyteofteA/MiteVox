
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define _USE_MATH_DEFINES
#include <math.h>
#include <conio.h>

namespace aimods
{
	enum struct ActivationFunction 
	{ 
		IDENTITY = 0, 
		BINARY, 
		BIPOLAR, 
		RELU, 
		LEAKY_RELU,
		SIGMOID, 
		TANH, 
		AVERAGE, 
		MIN, 
		MAX 
	};

	template <typename T>
	inline T activationFunction(T weightedSum, ActivationFunction function)
	{
		T f = 0;

		switch (function)
		{

		case ActivationFunction::IDENTITY:
			f = weightedSum;
			break;

		case ActivationFunction::BINARY:
			if (weightedSum > 0)
				f = 1;
			else
				f = 0;
			break;

		case ActivationFunction::BIPOLAR:
			if (weightedSum > 0)
				f = 1;
			else
				f = -1;
			break;

		case ActivationFunction::RELU:
			if (weightedSum > 0)
				f = weightedSum;
			else
				f = 0;
			break;

		case ActivationFunction::LEAKY_RELU:
			if (weightedSum > 0)
				f = weightedSum;
			else
				f = weightedSum * 0.05f;
			break;

		case ActivationFunction::SIGMOID:
			f = 1.0f / (1.0f + (T)pow((T)M_E, -weightedSum));
			break;

		case ActivationFunction::TANH:
			f = (T)tanh(weightedSum);
			break;
		}

		return f;
	}

	template <typename T>
	inline T activationDerivative(T weightedSum, ActivationFunction function)
	{
		T der = 0;

		switch (function)
		{

		case ActivationFunction::IDENTITY:
			der = 1;
			break;

		case ActivationFunction::BINARY:
			if (weightedSum != 0)
				der = 0;
			else
				der = 0;	// ??? clarify
			break;

		case ActivationFunction::BIPOLAR:	// ??? clarify
			if (weightedSum != 0)
				der = 0;
			else
				der = 0;
			break;

		case ActivationFunction::RELU:
			if (weightedSum > 0)
				der = 1;
			else
				der = 0;
			break;

		case ActivationFunction::LEAKY_RELU:
			if (weightedSum > 0)
				der = 1;
			else
				der = 0.05f;
			break;

		case ActivationFunction::SIGMOID:
			der = (1.0f - 1.0f / (1.0f + (T)pow((T)M_E, -weightedSum))) / 
				(1.0f + (T)pow((T)M_E, -weightedSum));
			break;

		case ActivationFunction::TANH:
			der = 1.0f - (T)pow(tanh(weightedSum), 2.0f);
			break;
		}

		return der;
	}

}

#endif





/*

<<< Math >>>
Abs
Acos
Acosh
Asin
Asinh
Atan
Atanh
Add

<<< Bit >>>
And
BitShift

ArgMax
ArgMin

<<< CNN >>>
AveragePool

<<< Other >>>
BatchNormalization

Cast
Ceil
Clip
Compress
Concat
ConcatFromSequence
Constant
ConstantOfShape
Conv
ConvInteger
ConvTranspose
Cos
Cosh
CumSum
DepthToSpace
DequantizeLinear
Det
Div
Dropout
Einsum
Elu
Equal
Erf
Exp
Expand
EyeLike
Flatten
Floor
GRU
Gather
GatherElements
GatherND
Gemm
GlobalAveragePool
GlobalLpPool
GlobalMaxPool
Greater
HardSigmoid
Hardmax
Identity
If
InstanceNormalization
IsInf
IsNaN
LRN
LSTM
LeakyRelu
Less
Log
Loop
LpNormalization
LpPool
MatMul
MatMulInteger
Max
MaxPool
MaxRoiPool
MaxUnpool
Mean
Min
Mod
Mul
Multinomial
Neg
NonMaxSuppression
NonZero
Not
OneHot
Or
PRelu
Pad
Pow
QLinearConv
QLinearMatMul
QuantizeLinear
RNN
RandomNormal
RandomNormalLike
RandomUniform
RandomUniformLike
Reciprocal
ReduceL1
ReduceL2
ReduceLogSum
ReduceLogSumExp
ReduceMax
ReduceMean
ReduceMin
ReduceProd
ReduceSum
ReduceSumSquare
Relu
Reshape
Resize
ReverseSequence
RoiAlign
Round
Scan
Scatter (deprecated)
ScatterElements
ScatterND
Selu
SequenceAt
SequenceConstruct
SequenceEmpty
SequenceErase
SequenceInsert
SequenceLength
Shape
Shrink
Sigmoid
Sign
Sin
Sinh
Size
Slice
Softplus
Softsign
SpaceToDepth
Split
SplitToSequence
Sqrt
Squeeze
StringNormalizer
Sub
Sum
Tan
Tanh
TfIdfVectorizer
ThresholdedRelu
Tile
TopK
Transpose
Trilu
Unique
Unsqueeze
Upsample (deprecated)
Where
Xor
Function
Celu
DynamicQuantizeLinear
GreaterOrEqual
HardSwish
LessOrEqual
LogSoftmax
MeanVarianceNormalization
NegativeLogLikelihoodLoss
Range
Softmax
SoftmaxCrossEntropyLoss

*/