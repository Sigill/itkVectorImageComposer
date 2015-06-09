#include <log4cxx/logger.h>
#include <log4cxx/consoleappender.h>
#include <log4cxx/patternlayout.h>
#include <log4cxx/basicconfigurator.h>

#include <itkVectorImage.h>
#include <itkImageFileWriter.h>
#include <itkComposeImageFilter.h>
#include <itkVectorImageToImageAdaptor.h>
#include <itkShiftScaleImageFilter.h>

typedef itk::VectorImage< float, 3 > OutputImageType;
typedef itk::Image< float, 3 > ScalarImageType;

#include "image_loader.h"

typedef itk::VectorImageToImageAdaptor< float, 3 > VectorImageAdaptor;
typedef itk::ShiftScaleImageFilter< VectorImageAdaptor, ScalarImageType > ShiftScaleImageFilterType;
typedef itk::ComposeImageFilter< ScalarImageType, OutputImageType > ComposeFilterType;
typedef itk::ImageFileWriter< OutputImageType > OutputImageWriter;

int main(int argc, char** argv)
{
	log4cxx::BasicConfigurator::configure(
		log4cxx::AppenderPtr(new log4cxx::ConsoleAppender(
				log4cxx::LayoutPtr(new log4cxx::PatternLayout("\%-5p - [%c] - \%m\%n")),
				log4cxx::ConsoleAppender::getSystemErr()
			)
		)
	);

	log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("main"));

	if(argc < 3)
	{
		LOG4CXX_FATAL(logger, "You need to specify the image and an output path");
		return EXIT_FAILURE;
	}

	OutputImageType::Pointer composite_image;
	try {
		composite_image = ImageLoader< OutputImageType >::load(argv[1]);
		LOG4CXX_INFO(logger, "Image has " << composite_image->GetNumberOfComponentsPerPixel() << " component(s)");
	} catch (ImageLoadingException &ex) {
		LOG4CXX_FATAL(logger, ex.what());
		return EXIT_FAILURE;
	}

	ComposeFilterType::Pointer compose_filter = ComposeFilterType::New();

	for(int i = 0; i < composite_image->GetNumberOfComponentsPerPixel(); ++i)
	{
		VectorImageAdaptor::Pointer scalar_adaptor = VectorImageAdaptor::New();
		scalar_adaptor->SetImage(composite_image);
		scalar_adaptor->SetExtractComponentIndex(i);

		ShiftScaleImageFilterType::Pointer normalizer = ShiftScaleImageFilterType::New();
		normalizer->SetShift(0.0);
		normalizer->SetScale(1.0/255.0);
		normalizer->SetInput(scalar_adaptor);
		normalizer->Update();

		compose_filter->SetInput(i, normalizer->GetOutput());
		LOG4CXX_DEBUG(logger, "Component " << i << " normalized");
	}

	compose_filter->Update();

	OutputImageWriter::Pointer writer = OutputImageWriter::New();
	writer->SetInput(compose_filter->GetOutput());
	writer->SetFileName(argv[2]);
	writer->Update();

	return EXIT_SUCCESS;
}

