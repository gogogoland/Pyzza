using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class GenerateMap : MonoBehaviour {

	public int 					height;
	public int					width;
	public int					seed;
	public GameObject			tile;
	public Material				[]materials;

	private Noise				noise;
	private Material			[,]variant_materials;
	private GameObject			[,]tiles;
	// Use this for initialization
	void Start () {
		Test();
		Generate(height, width);
		CenterCamera(height);
	}

	void Test() {
		variant_materials = new Material[width, height];
		noise = new Noise(0);
		float r_noise;

		Random.seed = seed;
		for (int tileX = 0; tileX < width; tileX++)
		{
			for (int tileY = 0; tileY < height; tileY++)
			{
				r_noise = noise.FractalNoise2D(width, height, 3, 5, 0.1f) + 0.1f;
				float rnd = Random.value;
				if (r_noise > 0.0) {
					if (rnd < 0.25f)
						variant_materials[tileX, tileY] = materials[2];
					else if (rnd < 0.50f)
						variant_materials[tileX, tileY] = materials[1];
					else if (rnd < 1.0f)
						variant_materials[tileX, tileY] = materials[0];
				}
			}
		}
	}

	void CenterCamera(int h) {
		float x = (h * tile.transform.localScale.x * 10 / 2 ) - (tile.transform.localScale.x * 10 / 2);
		GameObject cam = GameObject.Find("Main Camera");

		cam.transform.position = new Vector3(x, 10, 0);
	}

	void Generate(int h, int w) {
		Vector3		vec = Vector3.zero;
		tiles = new GameObject[width, height];

		for (int x = 0; x < w; x++)
		{
			for (int z = 0; z < h; z++)
			{
				vec.x = x * tile.transform.localScale.x * 10;
				vec.z = z * tile.transform.localScale.z * 10;
				tiles[x, z] = GameObject.Instantiate(tile, vec, tile.transform.rotation) as GameObject;
				tiles[x, z].GetComponent<Renderer>().material = variant_materials[x, z];
			}
		}

	}

	// Update is called once per frame
	void Update ()
	{
		Test();
		for (int x = 0; x < width; x++)
		{
			for (int z = 0; z < height; z++)
				tiles[x, z].GetComponent<Renderer>().material = variant_materials[x, z];
		}
	}
}
