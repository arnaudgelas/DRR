#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkThresholdImageFilter.h"
#include "itkAccumulateImageFilter.h"

int main( int argc, char* argv[] )
{
  if( argc != 7 )
    {
    std::cerr << "Usage: "<< std::endl;
    std::cerr << argv[0];
    std::cerr << " <InputFileName> <OutputFileName> <AccumulateDimension> <lowerThres> <upperThres> <outsideVal>";
    std::cerr << std::endl;
    return EXIT_FAILURE;
    }

  const unsigned int Dimension = 3;
  typedef short InputPixelType;
  typedef itk::Image< InputPixelType, Dimension > InputImageType;

  const char * inputFileName = argv[1];
  const char * outputFileName = argv[2];
  unsigned int accumulateDimension = static_cast< unsigned int >( atoi( argv[3] ) );

  InputPixelType lowerThreshold = static_cast< InputPixelType >( atoi( argv[4] ) );
  InputPixelType upperThreshold = static_cast< InputPixelType >( atoi( argv[5] ) );
  InputPixelType outsideValue = static_cast< InputPixelType >( atoi( argv[6] ) );

  typedef itk::ImageFileReader< InputImageType >  InputReaderType;
  InputReaderType::Pointer reader = InputReaderType::New();
  reader->SetFileName( inputFileName );

  typedef itk::ThresholdImageFilter< InputImageType > ThresholdFilterType;
  ThresholdFilterType::Pointer threshold = ThresholdFilterType::New();
  threshold->SetInput( reader->GetOutput() );
  threshold->ThresholdOutside(lowerThreshold, upperThreshold);
  threshold->SetOutsideValue( outsideValue );

  typedef double      OutputPixelType;
  typedef itk::Image< OutputPixelType, Dimension > OutputImageType;;

  typedef itk::AccumulateImageFilter< InputImageType, OutputImageType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( threshold->GetOutput() );
  filter->SetAccumulateDimension( accumulateDimension );

  typedef itk::ImageFileWriter< OutputImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( outputFileName );
  writer->SetInput( filter->GetOutput() );
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
