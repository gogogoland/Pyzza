using UnityEngine;
using System;
using System.Collections;
using UnityEngine.UI;
using System.Collections.Generic;

public class GameUI : MonoBehaviour {
	public GameObject	information_case;
	public int			value_slider;

	private GameObject	_infoTime;
	private int			_infoUnitTime;
	private DataGame	_scriptData;
	private Slider		_slider;
	private Text		_timeUI;

	// Use this for initialization
	void		Start () {
		information_case = GameObject.Find ("Information");
		_scriptData = GameObject.Find ("Client(Clone)").GetComponent<DataGame>();
		_slider = GameObject.Find ("Time").transform.GetChild (3).GetComponent<Slider> ();
		_timeUI = GameObject.Find ("timeCurrent").GetComponent<Text> ();
		information_case.SetActive (false);
	}

	// Update is called once per frame
	void		Update () {
	}

	public void	SliderTime(){
		_timeUI.text = "" + _scriptData.unitTime;
		value_slider = (int)_slider.value;
	}
}
