using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;
using System;
using System.Collections;
using System.Collections.Generic;

public class GameUI : MonoBehaviour {
	public GameObject	information_case;
	public int			value_slider = 0;

	private GameObject	_infoTime;
	private int			_infoUnitTime;
	private DataGame	_scriptData;
	private Client		_scriptClient;
	private Slider		_slider;
	private Text		_timeUI;

	// Use this for initialization
	void		Start () {
		information_case = GameObject.Find ("Information");
		GameObject client = GameObject.Find ("Client(Clone)");
		_scriptData = client.GetComponent<DataGame>();
		_scriptClient = client.GetComponent<Client> ();
		_slider = GameObject.Find ("SliderTime").GetComponent<Slider> ();
		_timeUI = GameObject.Find ("timeCurrent").GetComponent<Text> ();
		information_case.SetActive (false);
		value_slider = _scriptData.unitTime;
		_slider.value = value_slider;
		_timeUI.text = "" + value_slider;
	}

	// Update is called once per frame
	void		Update () {
		_timeUI.text = "" + (int)_slider.value;
	}
	/*
	void		LateUpdate() {
		GameObject []bubbles = GameObject.FindGameObjectsWithTag ("Talk");
		foreach (GameObject bubble in bubbles) {

		}
	}*/

	public void	SST(){
		value_slider = (int)_slider.value;
		_scriptClient.newTime = true;
		_scriptClient.newTimeValue = (int)_slider.value;
	}


}
