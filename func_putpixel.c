#include <stdio.h>
#include <math.h>
#include <sys/types.h>
#include <linux/fb.h>
#include <sys/ioctl.h>

static u_int screen_width = 1920;
static u_int screen_height = 1080;
static FILE* fp;

static void hsv_to_rgb(double* colR, double* colG, double* colB,
		       double H, double S, double V)
{
	const double R60 = M_PI / 3;
	H = fmod(H, 2 * M_PI);
	double h = fmod(H, R60) / R60;
	double s = V - (S * V);
	double _iv = h * V;
	double iv = _iv + ((V - _iv) * (1 - S));
	double _dv = V - (h * V);
	double dv = _dv + ((V - _dv) * (1 - S));
	int i = (int)(H / R60);

	double r, g, b;

	switch (i) {
	case 0:
		r = V;
		g = iv;
		b = s;
		break;

	case 1:
		r = dv;
		g = V;
		b = s;
		break;

	case 2:
		r = s;
		g = V;
		b = iv;
		break;

	case 3:
		r = s;
		g = dv;
		b = V;
		break;

	case 4:
		r = iv;
		g = s;
		b = V;
		break;

	default:
		r = V;
		g = s;
		b = dv;
		break;
	}

	(*colR) = (255 + 0.5) * r;
	(*colG) = (255 + 0.5) * g;
	(*colB) = (255 + 0.5) * b;
}

static u_long rgb_to_color(double R, double G, double B)
{
	u_long r = (u_long)R;
	u_long g = (u_long)G;
	u_long b = (u_long)B;
	return (r << 0) | (g << 8) | (b << 16);
}

static u_long hsv_to_color(double H, double S, double V)
{
	double R, G, B;
	hsv_to_rgb(&R, &G, &B, H, S, V);
	return rgb_to_color(R, G, B);
}

void __func_putpixel(double x, double y,
	 	     double H, double S, double B)
{
	u_long color = hsv_to_color(H, S, B);
	int i = ((screen_width * 4) * y) + (x * 4);
	fseek(fp, i, SEEK_SET);
	fwrite((void*)&color, 4, 1, fp);
}

void __init_func_putpixel(void)
{
	fp = fopen("/dev/fb0", "wb");

#ifdef __DEBUG__
	struct fb_fix_screeninfo fixinfo;
	ioctl(fp, FBIOGET_FSCREENINFO, &fixinfo);
	printf("identification string eg [%s]\n"
	       "Start of frame buffer mem [%u]\n"
	       "Length of frame buffer mem [%u]\n"
	       "see FB_TYPE_ [%u]\n"
	       "Interleave for interleaved Planes [%u]\n"
	       "see FB_VISUAL_ [%u]\n"
	       "zero if no hardware panning [%u]\n"
	       "zero if no hardware panning [%u]\n"
	       "zero if no hardware ywrap [%u]\n"
	       "length of a line in bytes [%u]\n"
	       "Start of Memory Mapped I/O [%u]\n"
	       "Length of Memory Mapped I/O [%u]\n"
	       "Indicate to driver which [%u]\n",
	       fixinfo.id,
	       fixinfo.smem_start,
	       fixinfo.smem_len,
	       fixinfo.type,
	       fixinfo.type_aux,
	       fixinfo.visual,
	       fixinfo.xpanstep,
	       fixinfo.ypanstep,
	       fixinfo.ywrapstep,
	       fixinfo.line_length,
	       fixinfo.mmio_start,
	       fixinfo.mmio_len,
	       fixinfo.accel);
	fixinfo.type = FB_VISUAL_DIRECTCOLOR;

	struct fb_var_screeninfo varinfo;
	ioctl(fp, FBIOGET_VSCREENINFO, &varinfo);
	printf("visible resolution x[%u] y[%u]\n"
	       "virtual resolution x[%u] y[%u]\n"
	       "offset from virtual to visible x[%u] y[%u]\n"
	       "guess what [%d]\n"
	       "!= 0 Graylevels instead of colors [%u]\n"
	       "bitfield in fb mem if true color, r ofs[%u] len[%u] msb_right[%u]\n"
	       "bitfield in fb mem if true color, g ofs[%u] len[%u] msb_right[%u]\n"
	       "bitfield in fb mem if true color, b ofs[%u] len[%u] msb_right[%u]\n"
	       "bitfield in fb mem if true color, t ofs[%u] len[%u] msb_right[%u]\n"
	       "!= 0 Non standard pixel format [%u]\n"
	       "see FB_ACTIVATE_ [%u]\n"
	       "height of picture in mm [%u]\n"
	       "width of picture in mm [%u]\n"
	       "(OBSOLETE) see fb_info.flags [%u]\n",
	       varinfo.xres, varinfo.yres,
	       varinfo.xres_virtual, varinfo.yres_virtual,
	       varinfo.xoffset, varinfo.yoffset,
	       varinfo.bits_per_pixel,
	       varinfo.grayscale,
	       varinfo.red.offset, varinfo.red.length, varinfo.red.msb_right,
	       varinfo.green.offset, varinfo.green.length, varinfo.green.msb_right,
	       varinfo.blue.offset, varinfo.blue.length, varinfo.blue.msb_right,
	       varinfo.transp.offset, varinfo.transp.length, varinfo.transp.msb_right,
	       varinfo.nonstd,
	       varinfo.activate,
	       varinfo.height,
	       varinfo.width,
	       varinfo.accel_flags);

	struct fb_cmap cmap;
	ioctl(fp, FBIOGETCMAP, &cmap);
	printf("\nstart[%u] len[%u]\n", cmap.start, cmap.len);
#endif
}
