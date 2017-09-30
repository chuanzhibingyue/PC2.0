#ifndef IMEMORY_RENDERER_H
#define IMEMORY_RENDERER_H

#include <string>

enum MemoryRendererInterpretationType
{
    mrit_Unknown,
    mrit_DecodedImage,   //RGBA, BGRA...
    mrit_FormattedImage, //png, jpg, gif...
    mrit_Private         //re-interpretate between server and client side
};
  
enum MemoryRendererColorType
{
    mrct_Unknown,
	mrct_RGB_565,
	mrct_RGBA_8888,
	mrct_BGRA_8888,
    mrct_I420
};

class MemoryRendererHeader 
{
public:
    MemoryRendererHeader(MemoryRendererInterpretationType type) : m_type(type)
    {}
    virtual ~MemoryRendererHeader(){}
    /*
      * for IPC message(or log), the returned string object can be organized via custom format.
      * the peer which received the header, can unpack the string through the same format.
    */
    virtual std::string Serialize() = 0;

    MemoryRendererInterpretationType m_type;
};

class RGBAMemoryRendererHeader : public MemoryRendererHeader
{
public:
    RGBAMemoryRendererHeader(MemoryRendererColorType colorType = mrct_RGBA_8888) 
        : MemoryRendererHeader(mrit_DecodedImage)
        , m_colorType(colorType)
        , m_frameWidth(0), m_frameHeight(0), m_stride(0)
        , m_bytesPerPixel(4)
    {}

    ~RGBAMemoryRendererHeader(){}

    std::string Serialize()
    {
        char buf[128] = { 0 };
        sprintf(buf, "rt=%d,ct=%d,w=%d,h=%d,stride=%d,bpp=%d", 
            m_type, m_colorType,
            m_frameWidth, m_frameHeight, m_stride, m_bytesPerPixel);
        return std::string(buf);
    }

    // the class derived from MemoryRendererHeader should impl operator=
    RGBAMemoryRendererHeader &operator=(const RGBAMemoryRendererHeader &rhs)
    {
        if (this == &rhs) return *this;

        m_type = rhs.m_type;
        m_colorType = rhs.m_colorType;
        m_frameWidth = rhs.m_frameWidth;
        m_frameHeight = rhs.m_frameHeight;
        m_stride = rhs.m_stride;
        m_bytesPerPixel = rhs.m_bytesPerPixel;

        return *this;
    }

    MemoryRendererColorType m_colorType;
    int m_frameWidth;
    int m_frameHeight;
    int m_stride;
    int m_bytesPerPixel;
};

class I420MemoryRendererHeader : public MemoryRendererHeader
{
public:
	I420MemoryRendererHeader(MemoryRendererColorType colorType = mrct_I420)
		: MemoryRendererHeader(mrit_DecodedImage)
		, m_colorType(colorType)
		, m_frameWidth(0), m_frameHeight(0)
	{}


    ~I420MemoryRendererHeader(){}


    std::string Serialize()
    {
        char buf[128] = { 0 };
        sprintf(buf, "rt=%d,ct=%d,w=%d,h=%d", 
            m_type, m_colorType,
            m_frameWidth, m_frameHeight);
        return std::string(buf);
    }
	// the class derived from MemoryRendererHeader should impl operator=
	I420MemoryRendererHeader &operator=(const I420MemoryRendererHeader &rhs)
	{
		if (this == &rhs) return *this;

		m_type = rhs.m_type;
		m_colorType = rhs.m_colorType;
		m_frameWidth = rhs.m_frameWidth;
		m_frameHeight = rhs.m_frameHeight;

		return *this;
	}

	MemoryRendererColorType m_colorType;
	int m_frameWidth;
	int m_frameHeight;
};

  
class IMemoryRenderer
{
public:
    IMemoryRenderer() : m_header(0)
    {}
    virtual ~IMemoryRenderer(){}

    /*
     * buf - the data should be interpreted via header(MemoryRendererHeader or subclass)
     *         alloc and free the buf is the responsibility of the caller
     */
    virtual void Render(void *buf) = 0;

    /*
     * header - the header which discribe the MemoryRenderer and it's data. 
     *          alloc and free the header is the responsibility of the caller
     */
    virtual void SetHeader(MemoryRendererHeader *header)
    {
        m_header = header;
    }
    
    MemoryRendererHeader * GetHeader(){ return m_header; }
  
protected:
    MemoryRendererHeader *m_header;
};

#endif // IMEMORY_RENDERER_H

