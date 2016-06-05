using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class GenerateMap : MonoBehaviour {

	public int 						height;
	public int						width;
	public int						seed;
	public GameObject				tile;
	public GameObject				food_obj;
	public GameObject				ressources_obj;
	public Sprite					[]ressources_sprite;
	public Material					[]materials;

	private GameObject				[]resrcs;
	private GameObject				[,]tiles;
	private Material				[,]variant_materials;
	private GameObject				map;
	private GameObject				lineTmp;
	private GameObject				[]lines;
	private Client					_scriptClient;

	void		AssignTileColor() {
		int data = 0;

		variant_materials = new Material[height, width];

		for (int x = 0;x < width;x++) {
			for (int z = 0;z < height;z++) {
				if (z == _scriptClient.structDataMap [data].z && x == _scriptClient.structDataMap [data].x) {
					variant_materials[z, x] = materials[_scriptClient.structDataMap [data].tileColor];
					while (data < _scriptClient.structDataMap.Count - 1 && (z == _scriptClient.structDataMap [data].z && x == _scriptClient.structDataMap [data].x))
						data++;
				}
			}
		}
	}

	// Use this for initialization
	void Awake(){
		_scriptClient = GameObject.Find ("Client(Clone)").GetComponent<Client>();
		height = _scriptClient.height;
		width = _scriptClient.width;
		AssignTileColor();
	}
	
	void Start ()
	{
		InitMap();
		GenerateTile();
		GenerateResrc();
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
	
//	void	AddStructResrc(int tileX, int tileY, int type, int nbr){
//		Client.s_datamap		tmp = new Client.s_datamap();
//		Vector3		vec = Vector3.zero;
//
//		tmp.nbr = nbr + 1;
//		tmp.x = tileX;
//		tmp.z = tileY;
//		tmp.type = type;
//		if (type == 0)
//		{
//			vec = new Vector3(0, food_obj.transform.position.y, 0);
//			tmp.resrcObj = GameObject.Instantiate(food_obj, vec, Quaternion.identity) as GameObject;
//			tmp.resrcObj.transform.localScale *= 20;
//		}
//		else if (type >= 1)
//		{
//			vec = new Vector3(0, ressources_obj.transform.position.y, 0);
//			tmp.resrcObj = GameObject.Instantiate(ressources_obj, vec, Quaternion.identity) as GameObject;
//			tmp.resrcObj.transform.localScale *= 5;
//		}
//		tmp.resrcObj.transform.parent = tiles[tileY, tileX].transform;
//		tmp.resrcObj.GetComponent<SpriteRenderer>().sprite = ressources_sprite[type];
//		RepositioningResrc(tmp.resrcObj, type);
//		structResrc.Add(tmp);
//	}
	
	void	GenerateTile() {
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

	void	GenerateResrc(){
		Vector3		vec = Vector3.zero;
		resrcs = new GameObject[_scriptClient.structDataMap.Count];

		for (int data = 0; data < _scriptClient.structDataMap.Count; data++) {
			if (_scriptClient.structDataMap[data].type == 0){
				vec = new Vector3(0, food_obj.transform.position.y, 0);
				resrcs[data] = GameObject.Instantiate(food_obj, vec, Quaternion.identity) as GameObject;
				resrcs[data].transform.localScale *= 20;
			}
			else if (_scriptClient.structDataMap[data].type >= 1) {
				vec = new Vector3(0, ressources_obj.transform.position.y, 0);
				resrcs[data] = GameObject.Instantiate(ressources_obj, vec, Quaternion.identity) as GameObject;
				resrcs[data].transform.localScale *= 5;
			}
			resrcs[data].transform.parent = tiles[_scriptClient.structDataMap[data].z, _scriptClient.structDataMap[data].x].transform;
			resrcs[data].GetComponent<SpriteRenderer>().sprite = ressources_sprite[_scriptClient.structDataMap[data].type];
			RepositioningResrc(resrcs[data], _scriptClient.structDataMap[data].type);
		}
	}
	
//	void	TestAssignResrc(float rnd, int tileX, int tileY, int nbr) {
//
//		if (rnd < 0.15f)
//			AddStructResrc(tileX, tileY, 0, nbr);
//		else if (rnd < 0.50f)
//		{
//			int rd = TestRessourcesRandom();
//			if (rd != -1)
//				AddStructResrc(tileX, tileY, rd, nbr);
//		}
//	}


//	void	Test2() {		
//		Random.seed = seed;
//		for (int tileX = 0; tileX < width; tileX++)
//		{
//			for (int tileY = 0; tileY < height; tileY++)
//			{
//				int rnd = Random.Range (1, 4);
//				for (int nbr = 0; nbr < rnd; nbr++)
//					TestAssignResrc(Random.value, tileX, tileY, nbr);
//			}
//		}
//	}
	
	// Update is called once per frame
	void	Update ()
	{

	}
}