using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class GenerateMap : MonoBehaviour {

	public int 					height;
	public int					width;
	public int					seed;
	public GameObject			tile;
	public GameObject			food_obj;
	public GameObject			ressources_obj;
	public Sprite				[]ressources_sprite;
	public Material				[]materials;

	private Material			[,]variant_materials;
	private GameObject			[,]variant_ressources;

	private GameObject			[,]tiles;
	// Use this for initialization
	void Start () {
		Test();
		Generate(height, width);
	}

	int		TestRessourcesRandom() {
		float rnd = Random.value;

		if (rnd < 0.1f)
			return (6);
		else if (rnd < 0.30f)
		{
			float rnd2 = Random.value;
			if (rnd2 < 0.50f)
				return (4);
			return (5);
		}
		else if (rnd < 0.50f)
		{
			float rnd2 = Random.value;
			if (rnd2 < 0.33f)
				return (3);
			else if (rnd2 < 0.66f)
				return (2);
			return (1);
		}
		return (0);
	}

	void	TestAssignResrc(float rnd, int tileX, int tileY) {
		Vector3		vec = Vector3.zero;

		if (rnd < 0.15f)
		{
			vec.x = tileX * tile.transform.localScale.x * 10;
			vec.y = 1.4f;
			vec.z = tileY * tile.transform.localScale.z * 10;
			if (variant_ressources[tileX, tileY] == null) {
				variant_ressources[tileX, tileY] = GameObject.Instantiate(food_obj, vec, food_obj.transform.rotation) as GameObject;
				variant_ressources[tileX, tileY].GetComponent<SpriteRenderer>().sprite = ressources_sprite[0];
			}
		}
		else if (rnd < 0.50f) {
			vec.x = tileX * tile.transform.localScale.x * 10;
			vec.y = 3.0f;
			vec.z = tileY * tile.transform.localScale.z * 10;
			int rd = TestRessourcesRandom();
			if (rd != 0 && variant_ressources[tileX, tileY] == null)
			{
				variant_ressources[tileX, tileY] = GameObject.Instantiate(ressources_obj, vec, ressources_obj.transform.rotation) as GameObject;
				variant_ressources[tileX, tileY].GetComponent<SpriteRenderer>().sprite = ressources_sprite[rd];
			}
		}

	}

	void	TestAssignTile(float rnd, int tileX, int tileY) {
		if (rnd < 0.25f)
			variant_materials[tileX, tileY] = materials[2];
		else if (rnd < 0.50f)
			variant_materials[tileX, tileY] = materials[1];
		else if (rnd < 1.0f)
			variant_materials[tileX, tileY] = materials[0];
	}

	void	Test() {
		variant_materials = new Material[width, height];
		variant_ressources = new GameObject[width, height];

		Random.seed = seed;
		for (int tileX = 0; tileX < width; tileX++)
		{
			for (int tileY = 0; tileY < height; tileY++)
			{
				TestAssignResrc(Random.value, tileX, tileY);
				TestAssignTile(Random.value, tileX, tileY);
			}
		}
	}


	void	Generate(int h, int w) {
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
	void	Update ()
	{
//		Test();
		for (int x = 0; x < width; x++)
		{
			for (int z = 0; z < height; z++)
				tiles[x, z].GetComponent<Renderer>().material = variant_materials[x, z];
		}
	}
}
