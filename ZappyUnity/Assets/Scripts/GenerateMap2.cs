using System.Threading;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GenerateMap2 : MonoBehaviour {

	public int 						height;
	public int						width;
	public GameObject				tile_obj;
	public GameObject				food_obj;
	public GameObject				ressources_obj;
	public Sprite					[]ressources_sprite;
	public Material					[]materials;

	private DataGame2				_scriptData;
	private GameObject				map;
	private GameObject				lineTmp;
	private GameObject				[]lines;
	private GameObject				[,]tiles;
	private Material				[,]variant_materials;
	private List<GameObject>		resrcs;
	private string					[]resrcName;
	private int						FOOD = 0;

	void	Awake() {
		_scriptData = GameObject.Find ("Client(Clone)").GetComponent<DataGame2>();
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

	void		AssignTilesColor() {
		variant_materials = new Material[height, width];

		for (int tile = 0; tile < _scriptData.dataTiles.Count; tile++)
			variant_materials[_scriptData.dataTiles[tile].posY, _scriptData.dataTiles[tile].posX] = materials[_scriptData.dataTiles [tile].tileColor];
	}

	void InitMap()
	{
		map = new GameObject("Map");
		lineTmp = new GameObject("Lines");
		lines = new GameObject[height];
		for (int z = 0; z < height; z++) {
			lines[z] = GameObject.Instantiate(lineTmp, map.transform.position, Quaternion.identity, map.transform) as GameObject;
			lines[z].name = "Lines (" + z + ")";
		}
	}

	void	GenerateAllTiles() {
		Vector3		vec = Vector3.zero;
		tiles = new GameObject[height, width];

		for (int z = 0; z < height; z++)
		{
			for (int x = 0; x < width; x++)
			{
				vec.x = x * tile_obj.transform.localScale.x * 10;
				vec.z = -z * tile_obj.transform.localScale.z * 10;
				tiles[z, x] = GameObject.Instantiate(tile_obj, vec, tile_obj.transform.rotation, lines[z].transform) as GameObject;
				tiles[z, x].transform.name = "Tile(" + z + ", " + x + ")";
				tiles[z, x].GetComponent<Renderer>().material = variant_materials[z, x];
			}
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

	void InstantiateResrc(GameObject modelToClone, int typeResrc, DataGame2.c_tile tile, int resize) {
		GameObject	cloneResrc;

		cloneResrc = GameObject.Instantiate (modelToClone, modelToClone.transform.position, Quaternion.identity, tiles [tile.posY, tile.posX].transform) as GameObject;
		cloneResrc.transform.localScale *= resize;
		cloneResrc.GetComponent<SpriteRenderer>().sprite = ressources_sprite[typeResrc];
		RepositioningResrc(cloneResrc, typeResrc);
		resrcs.Add(cloneResrc);
	}

	void	GenerateResrc(DataGame2.c_tile tile){
		for (int type = 0; type < tile.resrcs.Length; type++) {
			if (tile.resrcs [type] > 0) {
				if (type == FOOD)
					InstantiateResrc (food_obj, type, tile, 20);
				else
					InstantiateResrc (ressources_obj, type, tile, 5);
			}
		}
	}

	void	InitResrc(){
		resrcs = new List<GameObject> ();
		for (int tile = 0; tile < _scriptData.dataTiles.Count; tile++) {
			GenerateResrc(_scriptData.dataTiles[tile]);
		}
	}

	// Use this for initialization
	void Start () {
		AssignTilesColor ();
		InitMap();
		GenerateAllTiles();
		InitResrc();
		Destroy(lineTmp);
		_scriptData.GetComponent<Client> ()._inUpdate = true;
	}
	
	// Update is called once per frame
	void Update () {
		
	}

	void	_DeleteResrc(int type, GameObject tile) {
		if (tile) {
			Transform delete = tile.transform.FindChild (resrcName [type]);
			if (delete)
				Destroy (delete.gameObject);
		}
	}

	public void	UpdateResrcGetDrop(DataGame2.c_tile newTile, int newNbr, int typeResrc) {
		GameObject tile = GameObject.Find ("Tile(" + newTile.posY + ", " + newTile.posX + ")");

		if (newNbr > newTile.resrcs[typeResrc] && newTile.resrcs[typeResrc] == 0) {
			Debug.LogWarning ("CreateResrc");
			newTile.resrcs[typeResrc] = newNbr;
		}
		else if (newNbr < newTile.resrcs[typeResrc] && newNbr == 0) {
			newTile.resrcs[typeResrc] = newNbr;
			if (newNbr == 0)
				_DeleteResrc(typeResrc, tile);
		}
	}

	public Material UpdateResrcBadUpload(DataGame2.c_tile tileData) {
		Debug.LogWarning ("UpdateResrc");
		GenerateResrc (tileData);
		return (materials[tileData.tileColor]);
	}
}
