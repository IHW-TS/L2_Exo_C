#include <stdio.h>
#include <math.h>

typedef enum {RGB, HSL} colormode;

typedef union {
    colormode mode;
    struct {
        colormode mode;
        unsigned char r, g, b;
    } rgb;
    struct {
        colormode mode;
        double h, s, l;
    } hsl;
} pixel;

pixel copy_pixel(pixel pix) {
    return pix;
}

pixel to_hsl(pixel pix) {
    if (pix.mode == HSL) {
        return copy_pixel(pix);
    }

    double r = pix.rgb.r / 255.0;
    double g = pix.rgb.g / 255.0;
    double b = pix.rgb.b / 255.0;

    double max = fmax(r, fmax(g, b));
    double min = fmin(r, fmin(g, b));
    double delta = max - min;

    double h, s, l = (max + min) / 2;

    if (delta == 0) {
        h = 0;
        s = 0;
    } else {
        s = l > 0.5 ? delta / (2 - max - min) : delta / (max + min);

        if (r == max) {
            h = (g - b) / delta + (g < b ? 6 : 0);
        } else if (g == max) {
            h = (b - r) / delta + 2;
        } else {
            h = (r - g) / delta + 4;
        }

        h *= 60;
    }

    pixel result;
    result.hsl.mode = HSL;
    result.hsl.h = h;
    result.hsl.s = s;
    result.hsl.l = l;
    return result;
}

pixel to_rgb(pixel pix) {
    if (pix.mode == RGB) {
        return copy_pixel(pix);
    }

    double h = pix.hsl.h;
    double s = pix.hsl.s;
    double l = pix.hsl.l;

    double r, g, b;

    if (s == 0) {
        r = g = b = l;
    } else {
        double q = l < 0.5 ? l * (1 + s) : l + s - l * s;
        double p = 2 * l - q;

        double hue_to_rgb(double p, double q, double t) {
            if (t < 0) t += 1;
            if (t > 1) t -= 1;
            if (t < 1 / 6.0) return p + (q - p) * 6 * t;
            if (t < 1 / 2.0) return q;
            if (t < 2 / 3.0) return p + (q - p) * (2 / 3.0 - t) * 6;
            return p;
        }

        r = hue_to_rgb(p, q, h / 360 + 1 / 3.0);
        g = hue_to_rgb(p, q, h / 360);
        b = hue_to_rgb(p, q, h / 360 - 1 / 3.0);
    }

    pixel result;
    result.rgb.mode = RGB;
    result.rgb.r = r * 255;
    result.rgb.g = g * 255;
    result.rgb.b = b * 255;
    return result;
}

void gradient(pixel start, pixel stop, pixel* tab, unsigned int len) {
    pixel start_hsl = to_hsl(start);
    pixel stop_hsl = to_hsl(stop);

    for (unsigned int i = 0; i < len; i++) {
        double t = (double)i / (len - 1);

        pixel intermediate;
        intermediate.hsl.mode = HSL;
        intermediate.hsl.h = start_hsl.hsl.h + t * (stop_hsl.hsl.h - start_hsl.hsl.h);
        intermediate.hsl.s = start_hsl.hsl.s + t * (stop_hsl.hsl.s - start_hsl.hsl.s);
        intermediate.hsl.l = start_hsl.hsl.l + t * (stop_hsl.hsl.l - start_hsl.hsl.l);

        if (start.mode == RGB) {
            tab[i] = to_rgb(intermediate);
        } else {
            tab[i] = intermediate;
        }
    }
}

int main() {
    pixel start;
    start.rgb.mode = RGB;
    start.rgb.r = 255;
    start.rgb.g = 0;
    start.rgb.b = 0;

    pixel stop;
    stop.rgb.mode = RGB;
    stop.rgb.r = 0;
    stop.rgb.g = 0;
    stop.rgb.b = 255;

    unsigned int len = 10;
    pixel gradient_colors[len];

    gradient(start, stop, gradient_colors, len);

    for (unsigned int i = 0; i < len; i++) {
        printf("Color %d: R=%d, G=%d, B=%d\n", i, gradient_colors[i].rgb.r, gradient_colors[i].rgb.g, gradient_colors[i].rgb.b);
    }

    return 0;
}

