#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H

#include <stdexcept>

class ImageLoadingException : public std::runtime_error
{
public:
	ImageLoadingException ( const std::string &err ) : std::runtime_error (err) {}
};


template <typename TImageType>
class ImageLoader
{
private:
	typedef ImageLoader Self;
	typedef TImageType ImageType;

public:
	/**
	 * Load an image either as a single file or as a serie of files.
	 * @param[in] filename The file to load of the folder containing the files. Must exists.
	 */
	static typename TImageType::Pointer load(const std::string filename);

private:
	/**
	 * Load an image as a single file.
	 * @param[in] filename The file to load. Must exists.
	 */
	static typename TImageType::Pointer loadImage(const std::string filename);

	/**
	 * Load an image as a serie of files.
	 * @param[in] filename The folder containing the files. Must be a directory.
	 */
	static typename TImageType::Pointer loadImageSerie(const std::string filename);

};

#include "image_loader.hxx"

#endif /* IMAGE_LOADER_H */
