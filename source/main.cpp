#include <string.h>
#include <math.h>

#include <3ds.h>

#define WIDTH  400
#define HEIGHT 240

class Complex
{
public:
	Complex()
	{
		this->r = 0.0f;
		this->i = 0.0f;
	}

	Complex(float r, float i)
	{
		this->r = r;
		this->i = i;
	}

	float abs()
	{
		return sqrtf(this->r * this->r + this->i * this->i);
	}

	Complex operator+(const Complex& right)
	{
		return Complex(this->r + right.r, this->i + right.i);
	}

	Complex operator-(const Complex& right)
	{
		return Complex(this->r - right.r, this->i - right.i);
	}

	Complex operator*(const Complex& right)
	{
		// (a + i b)(c + i d) = a c + i a d + i b c - b d = (a c - b d) + i (a d + b c)
		return Complex(this->r * right.r - this->i * right.i, this->r * right.i + this->i * right.r);
	}

	float r;
	float i;
};

Complex f(Complex z)
{
	Complex c(0.221, 0.713);
	return z * z - c;
}

int main()
{
	gfxInitDefault();
	//gfxSet3D(true); // uncomment if using stereoscopic 3D

	// Main loop
	while (aptMainLoop())
	{
		gspWaitForVBlank();
		hidScanInput();

		// Your code goes here

		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break; // break in order to return to hbmenu

		u8* fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
		memset(fb, 0, 240*400*3);

		for (int x = 0; x < WIDTH; x++)
		{
			for (int y = 0; y < HEIGHT; y++)
			{
				float xr = float(x - float(WIDTH) / 2.0f) / (float(WIDTH) / 2.0f);
				float yr = float(y - float(HEIGHT) / 2.0f) / (float(HEIGHT) / 2.0f);

				Complex z(xr, yr);
				float it = 0.0f;

				while (z.abs() < 2.0f)
				{
					z = f(z);
					it += 0.05f;
				}

				float r = z.r / z.abs();
				float g = z.i / z.abs();
				float b = it;

				if (b > 1.0f)
				{
					b = 1.0f;
				}

				fb[3 * (y + x * HEIGHT)] = int(r * 255.0f);
				fb[3 * (y + x * HEIGHT) + 1] = int(g * 255.0f);
				fb[3 * (y + x * HEIGHT) + 2] = int(b * 255.0f);
			}
		}

		// Example rendering code that displays a white pixel
		// Please note that the 3DS screens are sideways (thus 240x400 and 240x320)
		/*u8* fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
		memset(fb, 0, 240*400*3);
		fb[3*(10+10*240)] = 0xFF;
		fb[3*(10+10*240)+1] = 0xFF;
		fb[3*(10+10*240)+2] = 0xFF;*/

		// Flush and swap framebuffers
		gfxFlushBuffers();
		gfxSwapBuffers();
	}

	gfxExit();
	return 0;
}
