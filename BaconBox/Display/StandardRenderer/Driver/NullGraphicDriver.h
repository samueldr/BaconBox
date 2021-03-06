/**
 * @file
 * @ingroup GraphicDrivers
 */
#ifndef BB_NULL_GRAPHIC_DRIVER_H
#define BB_NULL_GRAPHIC_DRIVER_H

#include "BaconBox/Display/StandardRenderer/Driver/GraphicDriver.h"

namespace BaconBox {
	/**
	 * Null graphic driver.
	 * @ingroup GraphicDrivers
	 */
	class NullGraphicDriver : public GraphicDriver {
            friend class BaseEngine;
		BB_SINGLETON_DECL(NullGraphicDriver);
	public:
		void renderToTexture(const TextureInformation *textureInformation, unsigned int viewportWidth = 0, unsigned int viewportHeight = 0, unsigned int contextWidth = 0, unsigned int contextHeight = 0);
		void endRenderToTexture();

		/*
		 * Prepare the scene before rendering object.
		 * It clear the draw buffer and reset the transformation matrix with the given
		 * parameters.
		 * @param position Shift the matrix using this 2D vector.
		 * @param angle Apply a rotation to the matrix in degree.
		 * @param zoom Apply a scale factor to the matrix. 1 is unchanged, less than 1 zoom out,
		 * more than 1 zoom in.
		 * @param backgroundColor The scene's background color.
		 */
		void prepareScene(const Vector2 &position, float angle,
		                  const Vector2 &zoom, const Color &backgroundColor, bool clearScreen);


		void tearGraphicDriver();
		void drawShapeWithTextureColorColorOffset(const VertexArray &vertices,
												  const TextureInformation *textureInformation,
												  const TextureCoordinates &textureCoordinates,
												  const Color &color,
												  const Color &colorOffset, bool blend, int degenerationStride, int degenerationJump);
		
		void resetProjection();
		
		void drawShapeWithTexture(const VertexArray &vertices,
								  const TextureInformation *textureInformation,
								  const TextureCoordinates &textureCoordinates,
								  bool blend, int degenerationStride, int degenerationJump);

        void drawShapeWithColorColorOffset(const VertexArray &vertices,
                                           const Color &color,
                                           const Color &colorOffset,
                                           bool blend, int degenerationStride, int degenerationJump);


		void drawBatch(const DynamicBatch *batch, const TextureInformation *texInfo, const bool blend);

		void initializeGraphicDriver();

		/**
		 * Pushes the current matrix on the stack.
		 */
		void pushMatrix();

		/**
		 * Applies a translation on the current matrix.
		 * @param translation 2D translation to apply.
		 */
		void translate(const Vector2 &translation);
		
		void scale(const Vector2 &scale);
		void rotate(float angle);

		/**
		 * Loads the identity matrix as the current matrix.
		 */
		void loadIdentity();

		/**
		 * Pops the current matrix from the stack.
		 */
		void popMatrix();

		/**
		 * Load a texture into graphic memory.
		 * @param pixMap A pixmap object containing the buffer the driver must load.
		 */
		TextureInformation *loadTexture(PixMap *pixMap, TextureInformation *textureInfo = NULL);


        /**
         *  Remove a texture from graphic memory
         */
        void deleteTexture(TextureInformation * textureInfo);


		/**
		 * Finalizes all pending batches if there are any.
		 */
		void finalizeRender();

	private:
		/**
		 * Default constructor.
		 */
		NullGraphicDriver();

		/**
		 * Destructor.
		 */
		~NullGraphicDriver();
	};
}

#endif

