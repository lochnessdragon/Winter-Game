#include <ui/font.h>
#include "log.h"

#include <stdexcept>

Texture Font::generateAtlas(const std::string& filename) {
	// Initialize FreeType library
	if (msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype()) {
		// Load font file
		if (msdfgen::FontHandle* font = msdfgen::loadFont(ft, filename.c_str())) {
			
            // NOW STORED BY BASE FONT CLASS
            // storage for glyph gemoetry and their coordinates in the atlas (can be stored in FontGeometry class instead)
			//std::vector<msdf_atlas::GlyphGeometry> glyphs;
            // FontGeometry is a helper class that loads a set of glyphs from a single font.
            // It can also be used to get additional font metrics, kerning information, etc.
            //msdf_atlas::FontGeometry fontGeometry(&glyphs);
            
            // Load a set of character glyphs:
            // The second argument can be ignored unless you mix different font sizes in one atlas.
            // In the last argument, you can specify a charset other than ASCII.
            // To load specific glyph indices, use loadGlyphs instead.
            fontGeometry.loadCharset(font, 1.0, msdf_atlas::Charset::ASCII);
            // Apply MSDF edge coloring. See edge-coloring.h for other coloring strategies.
            const double maxCornerAngle = 3.0;
            for (msdf_atlas::GlyphGeometry& glyph : glyphs)
                glyph.edgeColoring(&msdfgen::edgeColoringInkTrap, maxCornerAngle, 0);
            // TightAtlasPacker class computes the layout of the atlas.
            msdf_atlas::TightAtlasPacker packer;
            // Set atlas parameters:
            // setDimensions or setDimensionsConstraint to find the best value
            packer.setDimensionsConstraint(msdf_atlas::TightAtlasPacker::DimensionsConstraint::POWER_OF_TWO_SQUARE);
            // setScale for a fixed size or setMinimumScale to use the largest that fits
            packer.setMinimumScale(24.0);
            // setPixelRange or setUnitRange
            packer.setPixelRange(2.0);
            packer.setMiterLimit(1.0);
            // Compute atlas layout - pack glyphs
            packer.pack(glyphs.data(), (int) glyphs.size());
            // Get final atlas dimensions
            int width = 0, height = 0;
            packer.getDimensions(width, height);
            // The ImmediateAtlasGenerator class facilitates the generation of the atlas bitmap.
            msdf_atlas::ImmediateAtlasGenerator<
                float, // pixel type of buffer for individual glyphs depends on generator function
                3, // number of atlas color channels
                &msdf_atlas::msdfGenerator, // function to generate bitmaps for individual glyphs
                msdf_atlas::BitmapAtlasStorage<msdf_atlas::byte, 3> // class that stores the atlas bitmap
                // For example, a custom atlas storage class that stores it in VRAM can be used.
            > generator(width, height);
            // GeneratorAttributes can be modified to change the generator's default settings.
            msdf_atlas::GeneratorAttributes attributes;
            generator.setAttributes(attributes);
            generator.setThreadCount(4); // have to look at multithreading (MT)
            // Generate atlas bitmap
            generator.generate(glyphs.data(), (int) glyphs.size());
            // The atlas bitmap can now be retrieved via atlasStorage as a BitmapConstRef.
            // The glyphs array (or fontGeometry) contains positioning data for typesetting text.
            msdfgen::BitmapConstRef<msdf_atlas::byte, 3> bitmap = (msdfgen::BitmapConstRef<msdf_atlas::byte, 3>) generator.atlasStorage();

            const char* fontName = fontGeometry.getName();
            if (fontName == nullptr)
                fontName = "<no-name-provided>";

            // somehow, we need to copy the bitmap and glyphs outside the scope.
            Log::getRendererLog()->info("Generated font atlas: {} x {} for: {} ({})", bitmap.width, bitmap.height, fontName, filename);

            // Cleanup before return
            msdfgen::destroyFont(font);
            msdfgen::deinitializeFreetype(ft);

            TextureSpecification spec;
            spec.width = bitmap.width;
            spec.height = bitmap.height;
            spec.format = GL_RGB;
            spec.scaling = TextureScaling::Linear;
            return Texture(spec, static_cast<const unsigned char*>(bitmap.pixels));
        } else {
            msdfgen::deinitializeFreetype(ft);
            
            throw std::runtime_error("MSDF Gen failed to load font: " + filename);
        }
    } else {
        throw std::runtime_error("Failed to intialize freetype when creating font file: " + filename);
    }
}

Font::Font(const std::string& filename) : glyphs(), fontGeometry(&this->glyphs), atlas(generateAtlas(filename)) {}

bool operator<(const Font& lhs, const Font& rhs) {
    return lhs.atlas < rhs.atlas;
};

bool operator==(const Font& lhs, const Font& rhs) {
    return lhs.atlas == rhs.atlas;
};

bool operator!=(const Font& lhs, const Font& rhs) {
    return !(lhs == rhs);
}