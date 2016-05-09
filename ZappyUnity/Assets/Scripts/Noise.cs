using UnityEngine;
using System.Collections;

public class Noise
{
	/*
	** Attributes
	*/

	private int[]	hash = new int[512];
	private int		hashMask = 0xff;

	/*
	** Constructor
	*/

	public			Noise(int seed)
	{
		UnityEngine.Random.seed = seed;
		int	i;
		int	j;
		int	k;

		for (i = 0 ; i < 256 ; i++) 
			hash[i] = i;		
		while (--i != 0) {
			k = hash[i];
			j = UnityEngine.Random.Range(0, 256);
			hash[i] = hash[j];
			hash[j] = k;
		}
		for (i = 0 ; i < 256; i++) 
			hash[256 + i] = hash[i];
	}

	/*
	** Math functions
	*/

	private float	Smooth (float t) {
		return (t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f));
	}
	
	private float	Lerp(float t, float a, float b) {
		return (a) + (t)*((b)-(a));
	}

	/*
	** Value functions
	*/

	public float	Value1D (float x, int h) {
		int		hash; 
		float	grad; 

		hash = h & 15;
		grad = 1.0f + (hash & 7);
		if ((hash & 8) != 0)
			grad = -grad;
		return ( grad * x );
	}
	
	public float	Value2D (float x, float y, int h) {
		int		hash;
		float	u;
		float	v;

		hash = h & 7;
		u = hash < 4 ? x : y;
		v = hash < 4 ? y : x;
		return (((hash & 1) != 0)? -u : u) + (((hash & 2) != 0) ? -2.0f * v : 2.0f * v);
	}
	
	public float	Value3D (float x, float y, float z, int h) {
		int		hash;
		float	u;
		float	v;
		
		hash = h & 15;
		u = hash < 8 ? x : y;
		v = (hash < 4) ? y : (hash == 12 || hash == 14) ? x : z;
		return (((hash & 1) != 0)? -u : u) + (((hash & 2) != 0)? -v : v);
	}
	
	/*
	** Perlin functions
	*/

	public float	Perlin1D (float x) {
		//returns a noise value between -0.5 and 0.5
		int		ix0;
		int		ix1;
		float	fx0;
		float	fx1;
		float	g0;
		float	g1;
		float	t;

		ix0 = (int)Mathf.Floor(x);	// Integer part of x
		fx0 = x - ix0;				// Fractional part of x
		fx1 = fx0 - 1.0f;
		ix1 = (ix0 + 1) & hashMask;
		ix0 &= hashMask;			// Wrap to 0..255
		t = Smooth(fx0);
		g0 = Value1D (fx0, hash[ix0]);
		g1 = Value1D (fx1, hash[ix1]);
		return (0.188f * Lerp(t, g0, g1));
	}
	
	public float	Perlin2D (float x, float y) {
		int		ix0;
		int		iy0;
		int		ix1;
		int		iy1;
		float	fx0;
		float	fy0;
		float	fx1;
		float	fy1;
		float	tx;
		float	ty;
		int		h0;
		int		h1;
		float	g00;
		float	g01;
		float	g10;
		float	g11;
		float	g0;
		float	g1;

		ix0 = Mathf.FloorToInt(x);
		iy0 = Mathf.FloorToInt(y);
		fx0 = x - ix0;        	// Fractional part of x
		fy0 = y - iy0;        	// Fractional part of y
		fx1 = fx0 - 1.0f;
		fy1 = fy0 - 1.0f;
		ix1 = (ix0 + 1) & hashMask; // Wrap to 0..255
		iy1 = (iy0 + 1) & hashMask;
		ix0 &= hashMask;
		iy0 &= hashMask;
		tx = Smooth(fx0);
		ty = Smooth(fy0);
		h0 = hash[iy0];
		h1 = hash[iy1];
		g00 = Value2D(fx0, fy0, hash[ix0 + h0]);
		g01 = Value2D(fx0, fy1, hash[ix0 + h1]);
		g10 = Value2D(fx1, fy0, hash[ix1 + h0]);
		g11 = Value2D(fx1, fy1, hash[ix1 + h1]);
		g0 = Lerp (ty, g00, g01);
		g1 = Lerp (ty, g10, g11);
		return (Lerp (tx, g0, g1) * 0.507f);
	}
	
	public float	Perlin3D (float x, float y, float z) {
		int		ix0;
		int		iy0;
		int		iz0;
		int		ix1;
		int		iy1;
		int		iz1;
		float	fx0;
		float	fy0;
		float	fz0;
		float	fx1;
		float	fy1;
		float	fz1;
		float	tx;
		float	ty;
		float	tz;
		int		h0;
		int		h1;
		int		h00;
		int		h01;
		int		h10;
		int		h11;
		float	g000;
		float	g001;
		float	g010;
		float	g011;
		float	g100;
		float	g101;
		float	g110;
		float	g111;
		float	g00;
		float	g01;
		float	g10;
		float	g11;
		float	g0;
		float	g1;

		ix0 = Mathf.FloorToInt(x);
		iy0 = Mathf.FloorToInt(y);
		iz0 = Mathf.FloorToInt(z);
		fx0 = x - ix0;
		fy0 = y - iy0;
		fz0 = z - iz0;
		fx1 = fx0 - 1.0f;
		fy1 = fy0 - 1.0f;
		fz1 = fz0 - 1.0f;
		ix1 = ( ix0 + 1 ) & hashMask; // Wrap to 0..255
		iy1 = ( iy0 + 1 ) & hashMask;
		iz1 = ( iz0 + 1 ) & hashMask;
		ix0 &= hashMask;
		iy0 &= hashMask;
		iz0 &= hashMask;
		tx = Smooth(fx0);
		ty = Smooth(fy0);
		tz = Smooth(fz0);
		h0 = hash[iz0];
		h1 = hash[iz1];
		h00 = hash[iy0 + h0];
		h01 = hash[iy0 + h1];
		h10 = hash[iy1 + h0];
		h11 = hash[iy1 + h1];
		g000 = Value3D(fx0, fy0, fz0, hash[ix0 + h00]);
		g001 = Value3D(fx0, fy0, fz1, hash[ix0 + h01]);
		g010 = Value3D(fx0, fy1, fz0, hash[ix0 + h10]);
		g011 = Value3D(fx0, fy1, fz1, hash[ix0 + h11]);
		g100 = Value3D(fx1, fy0, fz0, hash[ix1 + h00]);
		g101 = Value3D(fx1, fy0, fz1, hash[ix1 + h01]);
		g110 = Value3D(fx1, fy1, fz0, hash[ix1 + h10]);
		g111 = Value3D(fx1, fy1, fz1, hash[ix1 + h11]);
		g00 = Lerp(tz, g000, g001);
		g01 = Lerp(tz, g010, g011);
		g10 = Lerp(tz, g100, g101);
		g11 = Lerp(tz, g110, g111);
		g0 = Lerp(ty, g00, g01);
		g1 = Lerp(ty, g10, g11);
		return (Lerp(tx, g0, g1) * 0.936f);
	}

	public float	FractalNoise1D(float x, int octNum, float frq, float amp)
	{
		float	gain = 1.0f;
		float	sum = 0.0f;
		
		for(int i = 0; i < octNum; i++)
		{
			sum += Perlin1D(x * gain / frq) * amp / gain;
			gain *= 2.0f;
		}
		return sum;
	}
	
	public float	FractalNoise2D(float x, float y, int octNum, float frq, float amp)
	{
		float gain = 1.0f;
		float sum = 0.0f;
		
		for(int i = 0; i < octNum; i++)
		{
			sum += Perlin2D(x * gain / frq, y * gain / frq) * amp / gain;
			gain *= 2.0f;
		}
		return sum;
	}
	
	public float	FractalNoise3D(float x, float y, float z, int octNum, float frq, float amp)
	{
		float gain = 1.0f;
		float sum = 0.0f;
		
		for(int i = 0; i < octNum; i++)
		{
			sum += Perlin3D(x * gain / frq, y * gain / frq, z * gain / frq) * amp / gain;
			gain *= 2.0f;
		}
		return sum;
	}
}
