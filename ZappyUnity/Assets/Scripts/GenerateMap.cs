using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class GenerateMap : MonoBehaviour {

	public struct			s_resrc{
		public int			x;
		public int			z;
		public int			type;
		public int			nbr;
		public GameObject	resrcObj;
	};


	public int 					height;
	public int					width;
	public int					seed;
	public GameObject			tile;
	public GameObject			food_obj;
	public GameObject			ressources_obj;
	public Sprite				[]ressources_sprite;
	public Material				[]materials;
	public GameObject			[,]tiles;
	public List<s_resrc>		structResrc;

	private Material			[,]variant_materials;

	private GameObject			map;
	private GameObject			lineTmp;
	private GameObject			[]lines;
	
	// Use this for initialization
	void Start ()
	{
		structResrc = new List<s_resrc>();
		InitMap();
		Test();
		Generate(height, width);
		Test2();
		Destroy(lineTmp);
		
	}
	
	void InitMap()
	{
		map = new GameObject("Map");
		lineTmp = new GameObject("Lines");
		lines = new GameObject[height];
		for (int z = 0; z < height; z++) {
			lines[z] = GameObject.Instantiate(lineTmp, map.transform.position, Quaternion.identity) as GameObject;
			lines[z].transform.parent = map.transform;
			lines[z].name = "Lines (" + z + ")";
		}
	}

	int		TestRessourcesRandom() {
		float rnd = Random.value;
		
		if (rnd < 0.1f)
			return (6);
		else if (rnd < 0.30f)
		{
			float rnd2 = Random.value;
			if (rnd2 < 0.50f)
				return (5);
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
		return (-1);
	}

	Vector3	PositionTileRandom(int tileX, int tileY, GameObject resrc){
		Vector3		vec = Vector3.zero;
		
		float rnd1 = Random.Range(-3f, 3f);
		float rnd2 = Random.Range(-3f, 3f);
		vec.x = tileX * tile.transform.localScale.x * 10 + rnd1;
		vec.y = resrc.transform.position.y;
		vec.z = -tileY * tile.transform.localScale.z * 10 + rnd2;
		return (vec);
	}

	void	RepositioningResrc(GameObject resrc, int id) {
		Vector3		vec = Vector3.zero;
		switch (id)
		{
		case 0 :
			resrc.name = "Food";
			vec = new Vector3(0.0f, resrc.transform.localPosition.y, 0.0f);
			;break;
		case 1 :
			resrc.name = "Linemate";
			vec = new Vector3(3.0f, resrc.transform.localPosition.y, 0.0f);
			;break;
		case 2 :
			resrc.name = "Deraumere";
			vec = new Vector3(3.0f, resrc.transform.localPosition.y, -3.0f);
			;break;
		case 3 :
			resrc.name = "Sibur";
			vec = new Vector3(3.0f, resrc.transform.localPosition.y, 3.0f);
			;break;
		case 4 :
			resrc.name = "Mendiane";
			vec = new Vector3(-3.0f, resrc.transform.localPosition.y, 3.0f);
			;break;
		case 5 :
			resrc.name = "Phiras";
			vec = new Vector3(-3.0f, resrc.transform.localPosition.y, 0.0f);
			;break;
		case 6 :
			resrc.name = "Thystame";
			vec = new Vector3(-3.0f, resrc.transform.localPosition.y, -3.0f);
			;break;
		}
		resrc.transform.localPosition = vec;
	}
	
	void	AddStructResrc(int tileX, int tileY, int type, int nbr){
		s_resrc		tmp = new s_resrc();
		Vector3		vec = Vector3.zero;

		tmp.nbr = nbr + 1;
		tmp.x = tileX;
		tmp.z = tileY;
		tmp.type = type;
		if (type == 0)
		{
			vec = new Vector3(0, food_obj.transform.position.y, 0);
			tmp.resrcObj = GameObject.Instantiate(food_obj, vec, Quaternion.identity) as GameObject;
			tmp.resrcObj.transform.localScale *= 20;
		}
		else if (type >= 1)
		{
			vec = new Vector3(0, ressources_obj.transform.position.y, 0);
			tmp.resrcObj = GameObject.Instantiate(ressources_obj, vec, Quaternion.identity) as GameObject;
			tmp.resrcObj.transform.localScale *= 5;
		}
		tmp.resrcObj.transform.parent = tiles[tileY, tileX].transform;
		tmp.resrcObj.GetComponent<SpriteRenderer>().sprite = ressources_sprite[type];
		RepositioningResrc(tmp.resrcObj, type);
		structResrc.Add(tmp);
	}

	
	void	TestAssignResrc(float rnd, int tileX, int tileY, int nbr) {

		if (rnd < 0.15f)
			AddStructResrc(tileX, tileY, 0, nbr);
		else if (rnd < 0.50f)
		{
			int rd = TestRessourcesRandom();
			if (rd != -1)
				AddStructResrc(tileX, tileY, rd, nbr);
		}
	}
	
	void	TestAssignTile(float rnd, int tileX, int tileY) {
		if (rnd < 0.25f)
			variant_materials[tileY, tileX] = materials[2];
		else if (rnd < 0.50f)
			variant_materials[tileY, tileX] = materials[1];
		else if (rnd < 1.0f)
			variant_materials[tileY, tileX] = materials[0];
	}
	
	void	Test() {
		variant_materials = new Material[height, width];
		
		Random.seed = seed;
		for (int tileX = 0; tileX < width; tileX++)
		{
			for (int tileY = 0; tileY < height; tileY++)
			{
				TestAssignTile(Random.value, tileX, tileY);
			}
		}
	}
	
	void	Test2() {		
		Random.seed = seed;
		for (int tileX = 0; tileX < width; tileX++)
		{
			for (int tileY = 0; tileY < height; tileY++)
			{
				int rnd = Random.Range (1, 4);
				for (int nbr = 0; nbr < rnd; nbr++)
					TestAssignResrc(Random.value, tileX, tileY, nbr);
			}
		}
	}
	
	void	Generate(int h, int w) {
		Vector3		vec = Vector3.zero;
		tiles = new GameObject[height, width];
		
		for (int z = 0; z < height; z++)
		{
			for (int x = 0; x < width; x++)
			{
				vec.x = x * tile.transform.localScale.x * 10;
				vec.z = -z * tile.transform.localScale.z * 10;
				tiles[z, x] = GameObject.Instantiate(tile, vec, tile.transform.rotation) as GameObject;
				tiles[z, x].transform.parent = lines[z].transform;
				tiles[z, x].transform.name = "Tile(" + z + ", " + x + ")";
				tiles[z, x].GetComponent<Renderer>().material = variant_materials[z, x];
			}
		}
	}
	
	// Update is called once per frame
	void	Update ()
	{

	}
}