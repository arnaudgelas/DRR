#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRegionOfInterestImageFilter.h"
#include "itkThresholdImageFilter.h"
#include "itkAccumulateImageFilter.h"

int main( int argc, char* argv[] )
{
  if( argc != 13 )
    {
    std::cerr << "Usage: "<< std::endl;
    std::cerr << argv[0];
    std::cerr << " <InputFileName> <OutputFileName> <AccumulateDimension>"
              << " <Xmin> <Ymin> <Zmin> <Xmax> <Ymax> <Zmax>"
              << " <lowerThres> <upperThres> <outsideVal>";
    std::cerr << std::endl;
    return EXIT_FAILURE;
    }

  const unsigned int Dimension = 3;
  typedef short InputPixelType;
  typedef itk::Image< InputPixelType, Dimension > InputImageType;

  unsigned int k = 1;
  const char * inputFileName = argv[k++];
  const char * outputFileName = argv[k++];
  unsigned int accumulateDimension = static_cast< unsigned int >( atoi( argv[k++] ) );

  InputImageType::IndexType minBound, maxBound;
  minBound[0] = static_cast< unsigned int >( atoi( argv[k++] ) );
  minBound[1] = static_cast< unsigned int >( atoi( argv[k++] ) );
  minBound[2] = static_cast< unsigned int >( atoi( argv[k++] ) );

  maxBound[0] = static_cast< unsigned int >( atoi( argv[k++] ) );
  maxBound[1] = static_cast< unsigned int >( atoi( argv[k++] ) );
  maxBound[2] = static_cast< unsigned int >( atoi( argv[k++] ) );

  InputPixelType lowerThreshold = static_cast< InputPixelType >( atoi( argv[k++] ) );
  InputPixelType upperThreshold = static_cast< InputPixelType >( atoi( argv[k++] ) );
  InputPixelType outsideValue = static_cast< InputPixelType >( atoi( argv[k++] ) );

  typedef itk::ImageFileReader< InputImageType >  InputReaderType;
  InputReaderType::Pointer reader = InputReaderType::New();
  reader->SetFileName( inputFileName );

  InputImageType::SizeType roiSize;
  roiSize[0] = maxBound[0] - minBound[0];
  roiSize[1] = maxBound[1] - minBound[1];
  roiSize[2] = maxBound[2] - minBound[2];

  InputImageType::RegionType roiRegion( minBound, roiSize );

  typedef itk::RegionOfInterestImageFilter< InputImageType, InputImageType > ROIFilterType;
  ROIFilterType::Pointer roiFilter = ROIFilterType::New();
  roiFilter->SetInput( reader->GetOutput() );
  roiFilter->SetRegionOfInterest( roiRegion );

  typedef itk::ThresholdImageFilter< InputImageType > ThresholdFilterType;
  ThresholdFilterType::Pointer threshold = ThresholdFilterType::New();
  threshold->SetInput( roiFilter->GetOutput() );
  threshold->ThresholdOutside(lowerThreshold, upperThreshold);
  threshold->SetOutsideValue( outsideValue );

  typedef double      OutputPixelType;
  typedef itk::Image< OutputPixelType, Dimension > OutputImageType;;

  typedef itk::AccumulateImageFilter< InputImageType, OutputImageType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( threshold->GetOutput() );
  filter->SetAccumulateDimension( accumulateDimension );

  typedef itk::ImageFileWriter< OutputImageType > WriterType;
//  typedef itk::ImageFileWriter< InputImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( outputFileName );
  writer->SetInput( filter->GetOutput() );
//  writer->SetInput( roiFilter->GetOutput() );
  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & error )
    {
    std::cerr << "Error: " << error << std::endl;
    return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}
