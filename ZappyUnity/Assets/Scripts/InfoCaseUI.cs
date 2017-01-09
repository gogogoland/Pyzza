using UnityEngine;
using UnityEngine.UI;
using System;
using System.Collections;
using System.Collections.Generic;

public class InfoCaseUI : MonoBehaviour {

	public GameObject		lineInfo;
	public GameObject		lineInfoPlayer;
	public GameObject		tile;
	
	private GameObject		_informationCase;
	private GameObject		_contentInfo;
	private GenerateMap		_scriptMap;
	private GameUI			_scriptUI;
	private DataGame		_scriptDataGame;
	private Text			_coordCase;
	private class			c_info
	{
		public int			[]nbr;
		public string		[]ressources;
		public Sprite		[]sprite;	
	};
	private c_info			_info;

	private int				_coordX;
	private int				_coordY;

	// Use this for initialization
	void	Start () {
		_scriptMap = GameObject.Find ("GenerateMap").GetComponent<GenerateMap> ();
		_scriptUI = GameObject.Find ("Canvas").GetComponent<GameUI> ();
		_scriptDataGame = GameObject.Find ("Client(Clone)").GetComponent<DataGame> ();

		_informationCase = _scriptUI.information_case;
		_contentInfo = _informationCase.transform.GetChild (1).transform.GetChild (0).transform.GetChild (0).gameObject;
		_coordCase = _informationCase.transform.GetChild (0).transform.GetChild (1).GetComponent<Text> ();
		_informationCase.SetActive(true);
		InitStructInfo ();
		ClearInfoCase ();
		GetInfoCase ();
	}

	void	ClearInfoCase() {
		for (int i = 0; i < _info.nbr.Length; i++)
			_info.nbr [i] = 0;
		for (int clear = 0; clear < _contentInfo.transform.childCount; clear++) {
			Destroy (_contentInfo.transform.GetChild (clear).gameObject);
		}
	}
		
	void	InitStructInfo(){
		_info = new c_info ();
		_info.nbr = new int[7];
		_info.ressources = new string[7];
		_info.ressources [0] = "Food";
		_info.ressources [1] = "Linemate";
		_info.ressources [2] = "Deraumere";
		_info.ressources [3] = "Sibur";
		_info.ressources [4] = "Mendiane";
		_info.ressources [5] = "Phiras";
		_info.ressources [6] = "Thystame";
		_info.sprite = new Sprite [_scriptMap.ressources_sprite.Length];
		Array.Copy(_scriptMap.ressources_sprite, _info.sprite, _scriptMap.ressources_sprite.Length);
	}

	void	CoordCase(){
		_coordX = (int)(tile.transform.position.x / 10);
		_coordY = (int)(-tile.transform.position.z / 10);
		_coordCase.text = "x " + _coordX + " y " + _coordY;
	}

	void	StockAllNameResrc() {
		if (tile.transform.childCount > 0) {
			for (int resrcInTile = 0; resrcInTile < tile.transform.childCount; resrcInTile++) {
				for (int name = 0; name < _info.ressources.Length; name++) {
					if (_info.ressources[name] == tile.transform.GetChild(resrcInTile).name)
						_info.nbr[name]++;
				}
			}
		}
	}

	void	GetInfoResrc(){
		for (int resrc = 0; resrc < _info.ressources.Length; resrc++) {
			if (_info.nbr[resrc] > 0)
			{
				GameObject lineInfoClone = GameObject.Instantiate (lineInfo, lineInfo.transform.position, Quaternion.identity) as GameObject;
				lineInfoClone.transform.SetParent (_contentInfo.transform);
				lineInfoClone.transform.GetChild(0).GetComponent<Text>().text = _info.ressources[resrc] + "\nNombre(s): " + _info.nbr[resrc];
				lineInfoClone.transform.GetChild(1).GetComponent<Image>().sprite = _info.sprite[resrc];
				if (resrc == 0)
					lineInfoClone.transform.GetChild(1).GetComponent<RectTransform>().rect.Set (15.0f, 0.0f, 38.0f, 38.0f) ;//= new Rect(0.0f, 0.0f, 38.0f, 38.0f);
			}
		}
	}

	void	GetInfoPlayer() {
		foreach (GameObject player in _scriptDataGame.players) {
			Player script = player.GetComponent<Player>();
			if (script.GetPosX() == _coordX
			    && script.GetPosY () == _coordY){
				GameObject lineInfoClone = GameObject.Instantiate (lineInfoPlayer, lineInfo.transform.position, Quaternion.identity) as GameObject;
				lineInfoClone.transform.SetParent(_contentInfo.transform);
				lineInfoClone.transform.GetChild(0).GetComponent<Text>().text += script.GetID();
				int i = 0;
				for (int resrcText = 1; resrcText < 14; resrcText += 2) {
					lineInfoClone.transform.GetChild(2).GetChild (resrcText).GetComponent<Text>().text = script.GetInventory(i) + "";
					i++;
				}
			}
		}
	}

	void	GetInfoCase() {
		CoordCase ();
		StockAllNameResrc ();
		GetInfoResrc ();
		GetInfoPlayer ();
	}
	
	// Update is called once per frame
	void	Update () {
	
	}
}
