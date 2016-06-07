using UnityEngine;
using System;
using System.Collections;
using UnityEngine.UI;
using System.Collections.Generic;

public class GameUI : MonoBehaviour {
	public GameObject	information_case;
	private GameObject	_infoTime;
	private int			_infoUnitTime;
	private Client		_scriptClient;
	private bool		_once = true;
	private Slider		_slider;
	private Text		_timeUI;

	// Use this for initialization
	void		Start () {
		information_case = GameObject.Find ("Information");
		_scriptClient = GameObject.Find ("Client(Clone)").GetComponent<Client>();
		_slider = GameObject.Find ("Time").transform.GetChild (4).GetComponent<Slider> ();
		_timeUI = GameObject.Find ("timeCurrent").GetComponent<Text> ();
		information_case.SetActive (false);
	}

	// Update is called once per frame
	void		Update () {
		_timeUI.text = "" + _scriptClient.unitTime;
		if 
	}
}
