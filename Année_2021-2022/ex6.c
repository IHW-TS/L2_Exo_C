#include <stdio.h>
#include <math.h>

typedef enum
{
  RGB,
  HSL
} colormode;

typedef union
{
  struct
  {
    colormode mode;
    unsigned char r, g, b;
  } rgb;
  struct
  {
    colormode mode;
    double h, s, l;
  } hsl;
} pixel;

float Min(float a, float b)
{
  return a <= b ? a : b;
}

float Max(float a, float b)
{
  return a >= b ? a : b;
}

pixel copy_pixel(pixel pix)
{
  return (pixel){.rgb = pix.rgb, .hsl = pix.hsl};
}

pixel to_hsl(pixel pix)
{

  float h = pix.hsl.h;
  float s = pix.hsl.s;
  float l = pix.hsl.l;

  float r = (pix.rgb.r / 255.0f);
  float g = (pix.rgb.g / 255.0f);
  float b = (pix.rgb.b / 255.0f);

  float cmin = Min(Min(r, g), b);
  float cmax = Max(Max(r, g), b);
  float delta = cmax - cmin;

  l = (cmax + cmin)/2;

  if (delta==0)
  {
    h = 0;
    s = 0.0f;
  }
  else
  {
    s = (h<=0.5) ? (delta/(cmax+cmin)) : (delta/(2-cmax-cmin));

    float hue;

    if (r==cmax)
    {
      hue = ((g-b)/6)/delta;
    }
    else if (g==cmax)
    {
      hue = (1.0f/3)+((b-r)/6)/delta;
    }
    else
    {
      hue = (2.0f/3)+((r-g)/6)/delta;
    }
    if (hue<0)
      hue+=1;
    if (hue>1)
      hue-=1;

    h = (int)(hue*360);
  }

  return (pixel){.rgb = pix.rgb, .hsl = pix.hsl};
}

float hue_to_rgb(float x, float y, float z)
{
  if (z<0)
  {
    z+=1;
  }
  if (z>1)
  {
    z-=1;
  }
  if ((6*z)<1)
  {
    return (x+(y-x)*6*z);
  }
  if ((2*z)<1)
  {
    return y;
  }
  if ((3*z)<2)
  {
    return (x+(y-x)*((2.0f/3)-z)*6);
  }
  return x;
}

pixel to_rgb(pixel pix)
{

  float h = pix.hsl.h;
  float s = pix.hsl.s;
  float l = pix.hsl.l;

  if (s == 0)
  {
    pix.rgb.r = pix.rgb.g = pix.rgb.b = (unsigned char)(l*255);
  }
  else
  {
    float a, b;
    float hue = (float)h / 360;
    a = (l<0.5) ? (l*(1+s)) : ((l+s)-(l*s));
    b = 2*l-b;
    pix.rgb.r = (unsigned char)(255*hue_to_rgb(a,b,hue+(1.0f/3)));
    pix.rgb.g = (unsigned char)(255*hue_to_rgb(a,b,hue));
    pix.rgb.b = (unsigned char)(255*hue_to_rgb(a,b,hue-(1.0f/3)));
  }

  return (pixel){.rgb = pix.rgb};
}

/*
void gradient (pixel start, pixel stop, pixel *tab, unsigned int len)
{
}
*/

int main(void)
{
  pixel pi;
  pi.rgb.mode = RGB;
  pi.rgb.r = 255;
  pi.rgb.g = 50;
  pi.rgb.b = 30;
  pixel t = copy_pixel(pi);
  printf("r= %d g= %d b= %d\n", t.rgb.r,t.rgb.g,t.rgb.b);
  pixel h = to_hsl(t);
  printf("h= %d s= %d l= %d\n", h.rgb.r,h.rgb.g,h.rgb.b);
  pixel c = to_rgb(h);
  printf("r= %d g= %d b= %d\n", c.rgb.r,c.rgb.g,c.rgb.b);
}
