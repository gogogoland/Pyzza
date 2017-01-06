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
	private string					[]resrcName;

	void		AssignTilesColor() {
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
		resrcName = new string[7];
		resrcName[0] = "Food";
		resrcName[1] = "Linemate";
		resrcName[2] = "Deraumere";
		resrcName[3] = "Sibur";
		resrcName[4] = "Mendiane";
		resrcName[5] = "Phiras";
		resrcName[6] = "Thystame";
	}
	
	void Start ()
	{
		AssignTilesColor();
		InitMap();
		GenerateTiles();
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
			vec = new Vector3(0.0f, resrc.transform.localPosition.y, 0.0f);
			;break;
		case 1 :
			vec = new Vector3(3.0f, resrc.transform.localPosition.y, 0.0f);
			;break;
		case 2 :
			vec = new Vector3(3.0f, resrc.transform.localPosition.y, -3.0f);
			;break;
		case 3 :
			vec = new Vector3(3.0f, resrc.transform.localPosition.y, 3.0f);
			;break;
		case 4 :
			vec = new Vector3(-3.0f, resrc.transform.localPosition.y, 3.0f);
			;break;
		case 5 :
			vec = new Vector3(-3.0f, resrc.transform.localPosition.y, 0.0f);
			;break;
		case 6 :
			vec = new Vector3(-3.0f, resrc.transform.localPosition.y, -3.0f);
			;break;
		}
		resrc.name = resrcName [id];
		resrc.transform.localPosition = vec;
	}

	void	GenerateTiles() {
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

	void	NbrResrcPerTile(GameObject modelToClone, DataGame.c_datamap upTile, int resize){
		Vector3		vec = Vector3.zero;
		
		for (int nbr = 0; nbr < upTile.nbr; nbr++) {
			GameObject	tmp;
			vec = new Vector3(0, modelToClone.transform.position.y, 0);
			tmp = GameObject.Instantiate(modelToClone, vec, Quaternion.identity) as GameObject;
			tmp.transform.localScale *= resize;
			tmp.transform.SetParent(tiles[upTile.z, upTile.x].transform);
			tmp.GetComponent<SpriteRenderer>().sprite = ressources_sprite[upTile.type];
			RepositioningResrc(tmp, upTile.type);
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

	void	GenerateResrc(DataGame.c_datamap upTile) {
		if (upTile.type == FOOD)
			NbrResrcPerTile(food_obj, upTile, 20);
		else if (upTile.type >= RESRC)
			NbrResrcPerTile(ressources_obj, upTile, 5);
	}

	public Vector2 CheckAllTiles() {
		Vector2 ret = new Vector2 (-1.0f, -1.0f);
		for (int z = 0; z < height; z++) {
			for (int x = 0; x < width; x++) {
				if (tiles[z, x].GetComponent<Renderer>().material.mainTexture == null) {
					ret.x = x;
					ret.y = z;
					break ;
				}
			}
		}
		return (ret);
	}

	public void	UpdateTile(DataGame.c_datamap upTile) {
		variant_materials[upTile.z, upTile.x] = materials[upTile.tileColor];
		tiles [upTile.z, upTile.x].GetComponent<Renderer> ().material = variant_materials [upTile.z, upTile.x];
		GenerateResrc (upTile);
	}

	public void	UpdateResrc(DataGame.c_datamap newTile, int newNbr) {
		GameObject tile = GameObject.Find ("Tile(" + newTile.z + ", " + newTile.x + ")");

		if (newNbr > newTile.nbr) {

		}
//			Debug.Log ("CreateResrc");
//		else if (newNbr < newTile.nbr)
//			Debug.Log ("SupprResrc");

		foreach (Transform child in tile.GetComponentsInChildren<Transform>()) {
			if (child.name == resrcName[newTile.type]) {

			}
		}
//		Debug.Log (newNbr);
//		Debug.Log (newTile.nbr);

	}

	// Update is called once per frame
	void	Update ()
	{

	}
}