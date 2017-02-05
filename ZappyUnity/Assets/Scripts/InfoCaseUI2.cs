using UnityEngine;
using UnityEngine.UI;
using System;
using System.Linq;
using System.Collections;
using System.Collections.Generic;

public class InfoCaseUI2 : MonoBehaviour {

	public GameObject		lineInfo;
	public GameObject		lineInfoPlayer;
	public GameObject		tile;

	private GameObject		_informationCase;
	private GameObject		_contentInfo;
	private GenerateMap2		_scriptMap;
	private GameUI			_scriptUI;
	private DataGame2		_scriptDataGame;
	private Text			_coordCase;

	private int				_coordX;
	private int				_coordY;

	private MoveCamera 		_scriptCamera;

	// Use this for initialization
	void	Start () {
		_scriptMap = GameObject.Find ("GenerateMap").GetComponent<GenerateMap2> ();
		_scriptUI = GameObject.Find ("Canvas").GetComponent<GameUI> ();
		_scriptDataGame = GameObject.Find ("Client(Clone)").GetComponent<DataGame2> ();
		_scriptCamera = GameObject.Find ("Cameras").GetComponent<MoveCamera> ();

		_informationCase = _scriptUI.information_case;
		_contentInfo = _informationCase.transform.GetChild (1).transform.GetChild (0).transform.GetChild (0).gameObject;
		_coordCase = _informationCase.transform.GetChild (0).transform.GetChild (1).GetComponent<Text> ();
		_informationCase.SetActive(true);

		ClearInfoCase ();
		GetInfoCase ();
	}

	void	ClearInfoCase() {
		for (int clear = 0; clear < _contentInfo.transform.childCount; clear++) {
			Destroy (_contentInfo.transform.GetChild (clear).gameObject);
		}
	}

	void	GetInfoPlayer() {
		foreach (GameObject player in _scriptDataGame.players) {
			Player script = player.GetComponent<Player>();
			if (script.GetPosX() == _coordX
				&& script.GetPosY () == _coordY){
				GameObject lineInfoClone = GameObject.Instantiate (lineInfoPlayer, lineInfo.transform.position, Quaternion.identity, _contentInfo.transform) as GameObject;
				lineInfoClone.transform.GetChild(0).GetChild(0).GetComponent<Text>().text += script.GetID();
				Toggle toggle = lineInfoClone.GetComponentInChildren<Toggle> ();
				toggle.group = lineInfoClone.GetComponentInParent<ToggleGroup>();
				int i = 0;
				for (int resrcText = 1; resrcText < 14; resrcText += 2) {
					lineInfoClone.transform.GetChild(0).GetChild(2).GetChild (resrcText).GetComponent<Text>().text = script.GetInventory(i) + "";
					i++;
				}
			}
		}
	}

	void	GetInfoResrc() {
		foreach (DataGame2.c_tile tile in _scriptDataGame.dataTiles) {
			if (_coordX == tile.posX && _coordY == tile.posY) {
				for (int resrc = 0; resrc < tile.resrcs.Length; resrc++) {
					if (tile.resrcs [resrc] > 0) {
						GameObject lineInfoClone = GameObject.Instantiate (lineInfo, lineInfo.transform.position, Quaternion.identity, _contentInfo.transform) as GameObject;
						lineInfoClone.transform.GetChild(0).GetComponent<Text>().text = _scriptDataGame.typeResrc[resrc] + "\nNombre(s): " + tile.resrcs [resrc];
						lineInfoClone.transform.GetChild(1).GetComponent<Image>().sprite = _scriptMap.ressources_sprite[resrc];
						if (resrc == 0)
							lineInfoClone.transform.GetChild(1).GetComponent<RectTransform>().rect.Set (15.0f, 0.0f, 38.0f, 38.0f) ;//= new Rect(0.0f, 0.0f, 38.0f, 38.0f);
					}
				}
				break;
			}
		}
		
	}

	void	CoordCase(){
		_coordX = (int)(tile.transform.position.x / 10);
		_coordY = (int)(-tile.transform.position.z / 10);
		_coordCase.text = "x " + _coordX + " y " + _coordY;
	}

	void	GetInfoCase() {
		CoordCase ();
		GetInfoPlayer ();
		GetInfoResrc ();
	}

	public void SetToggleOff(){
		_contentInfo.GetComponent<ToggleGroup> ().SetAllTogglesOff ();
	}

	// Update is called once per frame
	void	Update () {
		if (_contentInfo.GetComponent<ToggleGroup>().AnyTogglesOn()) {
			Toggle	toggle = _contentInfo.GetComponent<ToggleGroup>().ActiveToggles().FirstOrDefault();
			string	name = toggle.transform.parent.GetChild (0).GetComponent<Text> ().text;
			foreach (GameObject player in _scriptDataGame.players) {
				if (player.GetComponent<Player>().GetID() == int.Parse(name.Substring(7))) {
					_scriptCamera.LockPlayerCamera (player.transform);
					break;
				}
			}
		}
	}
}
