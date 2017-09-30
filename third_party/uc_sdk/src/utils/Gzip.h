#if !defined(GZIP_H_)
#define GZIP_H_

#include "zlib.h"

namespace uc {

class Gzip
{
public:
	//gzip compress
	static int Compress(Byte *source, uLong sourcelength, Byte *dest, uLong *destlength, int level) {
		int err = compress2(dest, destlength, source, (uLong)sourcelength, level);
		return err;

		if (err != Z_OK) {
			return -1;
		}
		return 0;
	}

	//gzip decompress
	static int Decompress(Byte *zdata, uLong nzdata, Byte *data, uLong *ndata) {
		int err = 0;
		z_stream d_stream = {0}; /* decompression stream */
		static char dummy_head[2] =
		{
			0x8 + 0x7 * 0x10,
			(((0x8 + 0x7 * 0x10) * 0x100 + 30) / 31 * 31) & 0xFF,
		};
		d_stream.zalloc = (alloc_func)0;
		d_stream.zfree = (free_func)0;
		d_stream.opaque = (voidpf)0;
		d_stream.next_in = zdata;
		d_stream.avail_in = 0;
		d_stream.next_out = data;
		if( inflateInit2(&d_stream, -MAX_WBITS) != Z_OK ) return -1;
		while (d_stream.total_out < * ndata && d_stream.total_in < nzdata) {
			d_stream.avail_in = d_stream.avail_out = 1; /* force small buffers */
			if((err = inflate(&d_stream, Z_NO_FLUSH)) == Z_STREAM_END ) break ;
			if(err != Z_OK )
			{
				if(err == Z_DATA_ERROR)
				{
					d_stream.next_in = (Bytef*) dummy_head;
					d_stream.avail_in = sizeof(dummy_head);
					if((err = inflate(&d_stream, Z_NO_FLUSH)) != Z_OK )
					{
						return -2;
					}
				}
				else return -1;
			}
		}
		if(inflateEnd(&d_stream) != Z_OK) return -1;
		*ndata = d_stream.total_out;
		return 0;
	}
};

}
#endif//GZIP_H_
