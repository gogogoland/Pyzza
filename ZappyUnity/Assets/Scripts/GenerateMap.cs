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

	public List<GameObject>			resrcs;
	private GameObject				[,]tiles;
	private Material				[,]variant_materials;
	private GameObject				map;
	private GameObject				lineTmp;
	private GameObject				[]lines;
	private DataGame				_scriptData;

	private int						FOOD = 0;
	private int						RESRC = 1;

	void		AssignTileColor() {
		int data = 0;

		variant_materials = new Material[height, width];

		for (int x = 0;x < width;x++) {
			for (int z = 0;z < height;z++) {
				if (z == _scriptData.structDataMap [data].z && x == _scriptData.structDataMap [data].x) {
					variant_materials[z, x] = materials[_scriptData.structDataMap [data].tileColor];
					while (data < _scriptData.structDataMap.Count - 1 && (z == _scriptData.structDataMap [data].z && x == _scriptData.structDataMap [data].x))
						data++;
				}
			}
		}
	}

	// Use this for initialization
	void Awake(){
		_scriptData = GameObject.Find ("Client(Clone)").GetComponent<DataGame>();
		height = _scriptData.height;
		width = _scriptData.width;
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

	public void	RepositioningResrc(GameObject resrc, int id) {
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

	void	NbrResrcPerTile(GameObject modelToClone, int index, int resize){
		Vector3		vec = Vector3.zero;

		for (int nbr = 0; nbr < _scriptData.structDataMap[index].nbr; nbr++) {
			GameObject	tmp;
			vec = new Vector3(0, modelToClone.transform.position.y, 0);
			tmp = GameObject.Instantiate(modelToClone, vec, Quaternion.identity) as GameObject;
			tmp.transform.localScale *= resize;
			tmp.transform.SetParent(tiles[_scriptData.structDataMap[index].z,_scriptData.structDataMap[index].x].transform);
			tmp.GetComponent<SpriteRenderer>().sprite = ressources_sprite[_scriptData.structDataMap[index].type];
			RepositioningResrc(tmp, _scriptData.structDataMap[index].type);
			resrcs.Add(tmp);
		}
	}

	void	GenerateResrc(){
		resrcs = new List<GameObject> ();
		for (int data = 0; data < _scriptData.structDataMap.Count; data++) {
			if (_scriptData.structDataMap[data].type == FOOD)
				NbrResrcPerTile(food_obj, data, 20);
			else if (_scriptData.structDataMap[data].type >= RESRC)
				NbrResrcPerTile(ressources_obj, data, 5);
		}
	}

	// Update is called once per frame
	void	Update ()
	{

	}
}