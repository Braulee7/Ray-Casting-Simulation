#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "Util.h"
struct Image {
	Image() = default;
	Image(int width, int height) { 
		mImageData = nullptr;
		mWidth = width;
		mHeight = height;
	}
	~Image() { 
		if (mImageData)
			delete[] mImageData; 
	}

	void Resize(uint32_t width, uint32_t height) {
		mWidth = width;
		mHeight = height;
	}

	void SetImage() {
		mImageData = new uint32_t[mWidth * mHeight];
		memset(mImageData, 0, mWidth * mHeight * sizeof(uint32_t));
	}

	void DeleteImage() {
		delete[] mImageData;
		mImageData = nullptr;
	}

	int mWidth, mHeight;
	uint32_t* mImageData;
};
#endif // !IMAGE_HPP
