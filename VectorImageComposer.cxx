#include <log4cxx/logger.h>
#include <log4cxx/consoleappender.h>
#include <log4cxx/patternlayout.h>
#include <log4cxx/basicconfigurator.h>

#include <itkVectorImage.h>
#include <itkImageFileWriter.h>
#include "itkComposeVectorImageFilter.h"

typedef itk::VectorImage< float, 3 > OutputImageType;

#include "image_loader.h"

typedef itk::ComposeVectorImageFilter< OutputImageType, OutputImageType> JoinImageFilterType;
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
		LOG4CXX_FATAL(logger, "You need to specify at least one image and an output path");
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

	for(int i = 2; i < argc - 1; ++i)
	{
		OutputImageType::Pointer next_image;
		try {
			next_image = ImageLoader< OutputImageType >::load(argv[i]);
			LOG4CXX_INFO(logger, "Image has " << next_image->GetNumberOfComponentsPerPixel() << " component(s)");
		} catch (ImageLoadingException &ex) {
			LOG4CXX_FATAL(logger, ex.what());
			return EXIT_FAILURE;
		}

		JoinImageFilterType::Pointer joinFilter = JoinImageFilterType::New();
		joinFilter->SetInput1(composite_image);
		joinFilter->SetInput2(next_image);
		joinFilter->Update();

		composite_image = joinFilter->GetOutput();
	}

	OutputImageWriter::Pointer writer = OutputImageWriter::New();
	writer->SetInput(composite_image);
	writer->SetFileName(argv[argc - 1]);
	writer->Update();

	return EXIT_SUCCESS;
}

