using UnityEngine;
using System.Collections;
using UnityEngine.UI;
using System.Collections.Generic;

public class GameUI : MonoBehaviour {
	public GameObject					tile;
	public GameObject					lineInfo;

	private GameObject					information_case;
	private GameObject					contentInfo;
	private bool						once = false;
	private int							resrcDbl = 1;
	private Dictionary<string, int>		nameResrc;
	private int							[]nbr;
	private string						[]ressources;
	// Use this for initialization
	void		Start () {
		information_case = GameObject.Find ("Information");
		contentInfo = information_case.transform.GetChild (1).transform.GetChild (0).transform.GetChild (0).gameObject;
		information_case.SetActive (false);
		nbr = new int[7];
		ressources = new string[7];
		ClearTab ();
		InitRessrcBase ();
	}

	void		CumulSameResrc(int child) {
		for (int cmp = child; cmp < tile.transform.childCount; cmp++) {
			Debug.Log ("coucou");
			if (cmp + 1 != tile.transform.childCount && string.Compare(tile.transform.GetChild(cmp).name, tile.transform.GetChild(cmp + 1).name) == 0)
				resrcDbl++;
		}
	}

	public void ClearTab() {
		for (int i = 0; i < nbr.Length; i++)
			nbr[i] = 0;
	}

	void InitRessrcBase(){
		ressources [0] = "Food";
		ressources [1] = "Linemate";
		ressources [2] = "Deraumere";
		ressources [3] = "Sibur";
		ressources [4] = "Mendiane";
		ressources [5] = "Phiras";
		ressources [6] = "Thystame";
	}

	public void	ClearInfoCase() {
		resrcDbl = 1;
		for (int clear = 0; clear < contentInfo.transform.childCount; clear++) {
			Destroy(contentInfo.transform.GetChild(clear).gameObject);
		}
	}

	public void ClearSameResrc(int child) {
		for (int clear = child; clear > -1; clear--) {
			if (clear - 1 > -1 && string.Compare(tile.transform.GetChild(clear).name, tile.transform.GetChild(clear - 1).name) == 0)
				Destroy(contentInfo.transform.GetChild(clear).gameObject);
		}
	}

	void	StockAllNameResrc() {
		ClearTab ();
		if (tile.transform.childCount > 0) {
			for (int resrcInTile = 0; resrcInTile < tile.transform.childCount; resrcInTile++) {
				for (int name = 0; name < ressources.Length; name++) {
					if (ressources[name] == tile.transform.GetChild(resrcInTile).name)
						nbr[name]++;
				}
			}
		}
	}

	int		GetNbrResrc(string name){
		for (int get = 0; get < ressources.Length; get++) {
			if (string.Compare(name, ressources[get]) == 0)
				return (nbr[get]);
		}
		return (0);
	}

	void		GetInfoCase() {
		StockAllNameResrc ();
		for (int child = 0; child < tile.transform.childCount; child++) {
			GameObject lineInfoClone = GameObject.Instantiate (lineInfo, lineInfo.transform.position, Quaternion.identity) as GameObject;
			lineInfoClone.transform.SetParent (contentInfo.transform);
			CumulSameResrc(child);
			lineInfoClone.transform.GetChild(0).GetComponent<Text>().text = tile.transform.GetChild(child).name + "\nNombre(s): " + GetNbrResrc(tile.transform.GetChild(child).name);
			lineInfoClone.transform.GetChild(1).GetComponent<Image>().sprite = tile.transform.GetChild(child).GetComponent<SpriteRenderer>().sprite;
			ClearSameResrc(child);
		}

	}
	
	// Update is called once per frame
	void		Update () {
		if (GameObject.Find ("Selection(Clone)") != null) {
			if (once == false) {
				ClearInfoCase();
				GetInfoCase();
			}
			information_case.SetActive (true);
			once = true;
		} else if (information_case.activeInHierarchy) {
			information_case.SetActive (false);
			once = false;
		}
	}
}
