#include <iostream>
#include <cstdlib>

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkResampleImageFilter.h"
#include "itkIdentityTransform.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkBSplineInterpolateImageFunction.h"


int main(int argc, char * argv[])
{
	if (argc < 5)
	{
		std::cerr << "Usage:\n  " << argv[0]
				  << " <FixedImagePath> <MovingImagePath> <OutputImagePath> <interpolator type>\n";
		return EXIT_FAILURE;
	}

	const char * fixedImageName  = argv[1];
	const char * movingImageName = argv[2];
	const char * outputPath      = argv[3];
	const std::string interpName = argv[4];

	constexpr unsigned int Dimension = 3;
	using ImagePixelType = double;
	using ImageType = itk::Image<ImagePixelType, Dimension>;

	using ReaderType = itk::ImageFileReader<ImageType>;

	auto fixedReader = ReaderType::New();
	fixedReader->SetFileName(fixedImageName);
	fixedReader->Update();
	auto fixedImage = fixedReader->GetOutput();

	auto movingReader = ReaderType::New();
	movingReader->SetFileName(movingImageName);
	movingReader->Update();
	auto movingImage = movingReader->GetOutput();

	using TransformType = itk::IdentityTransform<ImagePixelType, Dimension>;
	using ResampleType  = itk::ResampleImageFilter<ImageType, ImageType>;

	auto resample = ResampleType::New();
	resample->SetInput(movingImage); 
	resample->SetTransform(TransformType::New());
	resample->SetDefaultPixelValue(0);


	if (interpName == "linear")
	{
		using LinearType =
			itk::LinearInterpolateImageFunction<ImageType, ImagePixelType>;

		resample->SetInterpolator(LinearType::New());
	}
	else if (interpName == "BSpline3")
	{
		using BSplineType =
			itk::BSplineInterpolateImageFunction<ImageType, ImagePixelType>;

		auto bspline = BSplineType::New();
		bspline->SetSplineOrder(3);
		resample->SetInterpolator(bspline);
	}
	else if (interpName == "BSpline5")
	{
		using BSplineType =
			itk::BSplineInterpolateImageFunction<ImageType, ImagePixelType>;

		auto bspline = BSplineType::New();
		bspline->SetSplineOrder(5);
		resample->SetInterpolator(bspline);
	}
	else
	{
		std::cerr << "Invalid interpolator. Choices: "
				  << "\"linear\", \"BSpline3\", \"BSpline5\""
				  << std::endl;
		return EXIT_FAILURE;
	}

  resample->SetOutputSpacing(fixedImage->GetSpacing());
  resample->SetOutputOrigin(fixedImage->GetOrigin());
  resample->SetOutputDirection(fixedImage->GetDirection());
  resample->SetSize(fixedImage->GetLargestPossibleRegion().GetSize());
  resample->SetOutputStartIndex(fixedImage->GetLargestPossibleRegion().GetIndex());

  resample->Update();
  auto movingResampled = resample->GetOutput();

  using WriterType = itk::ImageFileWriter<ImageType>;
  auto writer = WriterType::New();
  writer->SetFileName(outputPath);
  writer->SetInput(movingResampled);
  writer->Update();

  return EXIT_SUCCESS;
}
