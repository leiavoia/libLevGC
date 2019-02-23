#ifndef FONTTOOLS
#define FONTTOOLS

#ifdef LGC_USE_FTGL
class FTTextureFont; // forward decl
#endif

#include <string>
#include <vector>
#include <map>

namespace LGC {








class FontImpl {

	public:

	FontImpl() {};
	FontImpl( const std::string& filename ) {};
	virtual ~FontImpl() {}

// 	virtual void Open( const std::string& filename ) = 0;
	virtual void BBox( const std::string& string, float& xmin, float& ymin, float& zmin, float& xmax, float& ymax, float& zmax ) = 0;
	virtual void Render( const std::string& string ) = 0;
	virtual void Advance( const std::string& string ) = 0;
	virtual int Error() = 0;
	virtual void SetFaceSize( int size ) = 0;
// 	virtual int FaceSize() = 0;
	};



class StubFontImpl: public FontImpl {
	public:
	StubFontImpl( const std::string& filename ) {}
	StubFontImpl() {}
	virtual ~StubFontImpl() {}
// 	void Open( const std::string& filename ) {}
	void BBox( const std::string& string, float& xmin, float& ymin, float& zmin, float& xmax, float& ymax, float& zmax ) {
		xmin = ymin = zmin = xmax = ymax = zmax = 0;
		}
	void Render( const std::string& string ) {}
	void Advance( const std::string& string ) {}
	int Error() { return 0; }
	void SetFaceSize( int size ) {}
// 	int FaceSize() { return 0; }
	};


#ifdef LGC_USE_FTGL
class FTGLFontImpl: public FontImpl {

	public:

	FTGLFontImpl( const std::string& filename );
	FTGLFontImpl();
	virtual ~FTGLFontImpl();

// 	void Open( const std::string& filename );
	void BBox( const std::string& string, float& xmin, float& ymin, float& zmin, float& xmax, float& ymax, float& zmax );
	void Render( const std::string& string );
	void Advance( const std::string& string );
	int Error();
	void SetFaceSize( int size );
// 	int FaceSize();

	private:

	FTTextureFont* font;

	};
#endif




class FontFactory {

	public:

	FontImpl* Make( const std::string& filename ) {
#ifdef LGC_USE_FTGL
		return new FTGLFontImpl(filename);
#endif
#ifndef LGC_USE_FTGL
		return new StubFontImpl(filename);
#endif

		}

	};



enum TextFormat {
	FORMAT_RIGHT,
	FORMAT_LEFT,
	FORMAT_CENTER,
	FORMAT_JUSTIFY
	};

class Font {
public:
	Font( const std::string& name, FontImpl* font ): name(name), font(font) {}
	Font(): name("NO_HANDLE"), font(NULL) {}
	~Font();
	Font( const Font& f ) {
		name = f.name;
		font = f.font;
		}
	Font& operator = ( const Font& f ) {
		name = f.name;
		font = f.font;
		return *this;
		}

	inline std::string Handle() { return name; };

	inline bool Valid() { return (font != NULL); }

	void GetStringSize( float& w, float& h, const std::string& str );

	void RenderFontToScreen( const std::string& str, float x, float y );

	void RenderFont( const std::string& str );

	void DrawTextCentered( const std::string& text, float x, float y );

	/*
	Given a block of text, this function writes the block of text
	into a display list and returns the OpenGL display list number for future use.
	Don't forget to free it when you are done using it!
	NOTE: ONLY FORMAT_LEFT is currently supported
	NOTE: a negative horizontal clip means that no clipping will be performed
	*/
	void CompressText(
		std::vector< std::string >& strings,
		std::string reference_name,
		float h_clip = -1,
		TextFormat format = FORMAT_LEFT
		);

	void UseStoredText( std::string name, float x, float y );

	// TODO: need vertical clip and paging functions



private:
	void FreeCompressedText( int x );
	std::string name;
	FontImpl* font;
	std::map< std::string, int > display_lists;
	};





} // end namespace LGC


#endif






