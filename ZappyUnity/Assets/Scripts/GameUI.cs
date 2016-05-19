using UnityEngine;
using System;
using System.Collections;
using UnityEngine.UI;
using System.Collections.Generic;

public class GameUI : MonoBehaviour {
	public GameObject					tile;
	public GameObject					lineInfo;

	private GameObject					information_case;
	private GameObject					contentInfo;
	private bool						once = false;
	private Scrollbar					scrollBar;
	private GenerateMap					scriptMap;
	private struct s_info
	{
		public int							[]nbr;
		public string						[]ressources;
		public Sprite						[]sprite;	
	};
	private s_info						info;
	
	// Use this for initialization
	void		Start () {
		information_case = GameObject.Find ("Information");
		contentInfo = information_case.transform.GetChild (1).transform.GetChild (0).transform.GetChild (0).gameObject;
		information_case.SetActive (false);
		scrollBar = information_case.transform.GetChild (1).transform.GetChild (2).GetComponent<Scrollbar>();

		scriptMap = GameObject.Find ("GenerateMap").GetComponent<GenerateMap> ();
		InitStructInfo ();
	}

	void InitStructInfo(){
		info = new s_info ();
		info.nbr = new int[7];
		info.ressources = new string[7];
		info.ressources [0] = "Food";
		info.ressources [1] = "Linemate";
		info.ressources [2] = "Deraumere";
		info.ressources [3] = "Sibur";
		info.ressources [4] = "Mendiane";
		info.ressources [5] = "Phiras";
		info.ressources [6] = "Thystame";
		info.sprite = new Sprite [scriptMap.ressources_sprite.Length];
		Array.Copy(scriptMap.ressources_sprite, info.sprite, scriptMap.ressources_sprite.Length);
	}

	void ClearTab() {
		for (int i = 0; i < info.nbr.Length; i++)
			info.nbr[i] = 0;
	}
	
	void	StockAllNameResrc(GameObject tile) {
		if (tile.transform.childCount > 0) {
			for (int resrcInTile = 0; resrcInTile < tile.transform.childCount; resrcInTile++) {
				for (int name = 0; name < info.ressources.Length; name++) {
					if (info.ressources[name] == tile.transform.GetChild(resrcInTile).name)
						info.nbr[name]++;
				}
			}
		}
	}

	void		GetInfoCase(GameObject tile) {
		StockAllNameResrc (tile);

		for (int resrc = 0; resrc < info.ressources.Length; resrc++) {
			if (info.nbr[resrc] > 0)
			{
				GameObject lineInfoClone = GameObject.Instantiate (lineInfo, lineInfo.transform.position, Quaternion.identity) as GameObject;
				lineInfoClone.transform.SetParent (contentInfo.transform);
				lineInfoClone.transform.GetChild(0).GetComponent<Text>().text = info.ressources[resrc] + "\nNombre(s): " + info.nbr[resrc];
				lineInfoClone.transform.GetChild(1).GetComponent<Image>().sprite = info.sprite[resrc];
				if (resrc == 0)
					lineInfoClone.transform.GetChild(1).GetComponent<RectTransform>().rect.Set (15.0f, 0.0f, 38.0f, 38.0f) ;//= new Rect(0.0f, 0.0f, 38.0f, 38.0f);
			}
		}

		for (int child = 0; child < tile.transform.childCount; child++) {

		}
	}

	void	ClearInfoCase() {
		ClearTab ();
		for (int clear = 0; clear < contentInfo.transform.childCount; clear++) {
			Destroy(contentInfo.transform.GetChild(clear).gameObject);
		}
	}
	
	public void	CaseInfo(bool display, GameObject tile)
	{
		scrollBar.value = 1.0f;
		if (display == true && tile != null) {
			if (once == false) {
				ClearInfoCase ();
				GetInfoCase (tile);
			}
			information_case.SetActive (true);
			once = true;
		} else {
			information_case.SetActive (false);
			once = false;
		}
	}

	// Update is called once per frame
	void		Update () {

	}
}
