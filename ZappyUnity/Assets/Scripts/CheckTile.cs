using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CheckTile : MonoBehaviour {
	
	private int					coordX;
	private int					coordY;
	private Client				scriptClient;
	private DataGame2			scriptData;
	private GenerateMap2		scriptMap;
	// Use this for initialization
	void Start () {
		scriptClient = GameObject.Find ("Client(Clone)").GetComponent<Client>();
		scriptData = GameObject.Find("Client(Clone)").GetComponent<DataGame2>();
		scriptMap = GameObject.Find("GenerateMap").GetComponent<GenerateMap2>();
		coordX = (int)(transform.position.x / 10);
		coordY = (int)(-transform.position.z / 10);
	}

	DataGame2.c_tile	DataTile(){
		foreach (DataGame2.c_tile tileData in scriptData.dataTiles) {
			if (tileData.posX == coordX && tileData.posY == coordY)
				return (tileData);
		}
		return (null);
	}

	// Update is called once per frame
	void Update () {
		if (GetComponent<Renderer> ().material.mainTexture == null && scriptClient.inUpdate) {
			Debug.LogWarning ("NoMaterial");
			scriptClient.SendBCT (coordX, coordY);
			DataGame2.c_tile tileData = DataTile ();
			if (tileData != null)
				GetComponent<Renderer> ().material = scriptMap.UpdateResrcBadUpload (tileData);
			else
				Debug.LogWarning ("TileData = null");
		} else
			this.enabled = false;
	}
}