/**
 * @file
 * @ingroup Display
 */
#ifndef BB_TEXTURE_INFORMATION_H
#define BB_TEXTURE_INFORMATION_H

#include <iostream>
#include <string>
#include "BaconBox/PlatformFlagger.h"

#include "BaconBox/Display/ColorFormat.h"

namespace BaconBox {
	/**
	 * Struct containing information about a texture. Will contain different
	 * informations depending on the platform.
	 * @ingroup Display
	 */
	struct TextureInformation {
		/**
		 * Outputs the Texture's content.
		 * @param output The ostream in which Texture is output.
		 * @param t Texture to output in the ostream.
		 * @return Resulting ostream.
		 */
#ifndef SWIG
		friend std::ostream &operator<<(std::ostream &output,
		                                const TextureInformation &t);
#endif
		/**
		 * Default constructor.
		 */
		TextureInformation();
#if defined (BB_OPENGL) || defined (BB_OPENGLES)
		/**
		 * Parametrized constructor for the OpenGL implementation of
		 * Texture.
		 * @param newTextureId Texture ID to be used.
		 * @param newImageWidth Image width in pixels to be used.
		 * @param newImageHeight Image height in pixels to be used.
		 */
		TextureInformation(unsigned int newTextureId,
		                   unsigned int newImageWidth,
		                   unsigned int newImageHeight, 
				   const std::string & path);

		/// OpenGL's texture's ID.
		int textureId;
#else
		TextureInformation(unsigned int newImageWidth,
		                   unsigned int newImageHeight, const std::string & path);
#endif
		bool isLoaded();

		/// Color format of the texture
		ColorFormat::type colorFormat;

		/// Texture width powered to 2
		unsigned int poweredWidth;

		/// Texture height powered to 2
		unsigned int poweredHeight;

		/// Texture's image width.
		unsigned int imageWidth;

		/// Texture's image height.
		unsigned int imageHeight;
		
		std::string path;
		
		std::string key;
	};
}
#endif
