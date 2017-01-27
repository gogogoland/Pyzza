using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CheckTile : MonoBehaviour {
	
	private int					coordX;
	private int					coordY;
	private bool				check = false;
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
	
	// Update is called once per frame
	void Update () {
		if (!check) {
			if (GetComponent<Renderer> ().material.mainTexture == null) {
				scriptClient.SendBCT (coordX, coordY);Debug.LogWarning ("plop2");
				foreach (DataGame2.c_tile tileData in scriptData.dataTiles) {
					if (tileData.posX == coordX && tileData.posY == coordY) {
						scriptMap.UpdateResrcBadUpload (tileData, transform);
						Debug.LogWarning ("plop");
						break;
					}
				}
			}
			check = true;
		}
	}
}