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
	private GameObject			map;
	private GameObject			tilesParent;
	private GameObject			tilesParentCpy;

	// Use this for initialization
	void Start () {
		map = new GameObject("Map");
		tilesParent = new GameObject("Tiles");
		tilesParentCpy = new GameObject("Tiles");
		tilesParent.transform.parent = map.transform;
		Test();
		Generate(height, width);
		Test2();
		GenerateDbl();
//		Debug.Log (7 * 10 * tile.transform.position);
		Destroy(tilesParentCpy);
	}

	void	Sector5()
	{
		GameObject	map = new GameObject("Sector5");
		map.transform.position = new Vector3(width * 10, 0, 0);
		GameObject tilesCpy = GameObject.Instantiate(tilesParentCpy, map.transform.position, Quaternion.identity) as GameObject;
		tilesCpy.transform.parent = map.transform;
		for (int x = 0; x < 8; x++)
		{
			for (int z = 0; z < width; z++)
			{
				GameObject tile_current = tilesParent.transform.GetChild(z + width * x).gameObject;
				GameObject tile_clone = GameObject.Instantiate(tile_current,
				                                               tile_current.transform.localPosition + map.transform.position,
				                                               Quaternion.identity) as GameObject;
				tile_clone.transform.parent = tilesCpy.transform;
			}
		}
	}

	void	Sector4()
	{
		GameObject	map = new GameObject("Sector4");
		map.transform.position = new Vector3(width * -10, 0, 0);
		GameObject tilesCpy = GameObject.Instantiate(tilesParentCpy, map.transform.position, Quaternion.identity) as GameObject;
		tilesCpy.transform.parent = map.transform;
		for (int x = height - 8; x < height; x++)
		{
			for (int z = 0; z < width; z++)
			{
				GameObject tile_current = tilesParent.transform.GetChild(z + width * x).gameObject;
				GameObject tile_clone = GameObject.Instantiate(tile_current,
				                                               tile_current.transform.localPosition + map.transform.position,
				                                               Quaternion.identity) as GameObject;
				tile_clone.transform.parent = tilesCpy.transform;
			}
		}
	}

	void	Sector3()
	{
		GameObject	map = new GameObject("Sector3");
		map.transform.position = new Vector3(width * 10, 0, height * 10);
		GameObject tilesCpy = GameObject.Instantiate(tilesParentCpy, map.transform.position, Quaternion.identity) as GameObject;
		tilesCpy.transform.parent = map.transform;
		for (int x = 0; x < 8; x++)
		{
			for (int z = 0; z < 11; z++)
			{
				GameObject tile_current = tilesParent.transform.GetChild(z + width * x).gameObject;
				GameObject tile_clone = GameObject.Instantiate(tile_current,
				                                               tile_current.transform.localPosition + map.transform.position,
				                                               Quaternion.identity) as GameObject;
				tile_clone.transform.parent = tilesCpy.transform;
			}
		}
	}

	void	Sector2()
	{
		GameObject	map = new GameObject("Sector2");
		map.transform.position = new Vector3(0, 0, height * 10);
		GameObject tilesCpy = GameObject.Instantiate(tilesParentCpy, map.transform.position, Quaternion.identity) as GameObject;
		tilesCpy.transform.parent = map.transform;
		for (int x = 0; x < height; x++)
		{
			for (int z = 0; z < 11; z++)
			{
				GameObject tile_current = tilesParent.transform.GetChild(z + width * x).gameObject;
				GameObject tile_clone = GameObject.Instantiate(tile_current,
				                                               tile_current.transform.localPosition + map.transform.position,
				                                               Quaternion.identity) as GameObject;
				tile_clone.transform.parent = tilesCpy.transform;
			}
		}
	}

	void	Sector1()
	{
		GameObject	map = new GameObject("Sector1");
		map.transform.position = new Vector3(width * -10, 0, height * 10);
		GameObject tilesCpy = GameObject.Instantiate(tilesParentCpy, map.transform.position, Quaternion.identity) as GameObject;
		tilesCpy.transform.parent = map.transform;
		for (int x = height - 8; x < height; x++)
		{
			for (int z = 0; z < 11; z++)
			{
				GameObject tile_current = tilesParent.transform.GetChild(z + width * x).gameObject;
				GameObject tile_clone = GameObject.Instantiate(tile_current,
				                                               tile_current.transform.localPosition + map.transform.position,
				                                               Quaternion.identity) as GameObject;
				tile_clone.transform.parent = tilesCpy.transform;
			}
		}
	}

	void GenerateDbl()
	{
		Sector1();
		Sector2();
		Sector3();
		Sector4();
		Sector5();
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
				variant_ressources[tileX, tileY].transform.parent = tiles[tileX, tileY] .transform;
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
				variant_ressources[tileX, tileY].transform.parent = tiles[tileX, tileY].transform;
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

		Random.seed = seed;
		for (int tileX = 0; tileX < width; tileX++)
		{
			for (int tileY = 0; tileY < height; tileY++)
				TestAssignTile(Random.value, tileX, tileY);
		}
	}
	
	void	Test2() {
		variant_ressources = new GameObject[width, height];
		
		Random.seed = seed;
		for (int tileX = 0; tileX < width; tileX++)
		{
			for (int tileY = 0; tileY < height; tileY++)
				TestAssignResrc(Random.value, tileX, tileY);
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
				tiles[x, z].transform.parent = tilesParent.transform;
				tiles[x, z].GetComponent<Renderer>().material = variant_materials[x, z];
			}
		}

	}

	// Update is called once per frame
	void	Update ()
	{

	}
}
