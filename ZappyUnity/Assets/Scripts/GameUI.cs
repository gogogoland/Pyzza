﻿using UnityEngine;
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
	private DataGame2	_scriptData;
	private Client		_scriptClient;
	private Slider		_slider;
	private Text		_timeUI;
	private Transform	_contentMsgInfo;

	private int 		TOO_MUCH_LETTER = 75;

	// Use this for initialization
	void					Start () {
		information_case = GameObject.Find ("Information");
		GameObject client = GameObject.Find ("Client(Clone)");
		_contentMsgInfo = GameObject.Find ("MsgContent").transform;
		_scriptData = client.GetComponent<DataGame2>();
		_scriptClient = client.GetComponent<Client> ();
		_slider = GameObject.Find ("SliderTime").GetComponent<Slider> ();
		_timeUI = GameObject.Find ("timeCurrent").GetComponent<Text> ();
		information_case.SetActive (false);
		value_slider = _scriptData.unitTime;
		_slider.value = value_slider;
		_timeUI.text = "" + value_slider;
	}

	// Update is called once per frame
	void					Update () {
		_timeUI.text = "" + (int)_slider.value;
	}

	public void				SST(){
		value_slider = (int)_slider.value;
		_scriptClient.newTime = true;
		_scriptClient.newTimeValue = (int)_slider.value;
	}

	public void 			AddMsgInfo(string msg) {
		Transform	firstmsg = _contentMsgInfo.transform.GetChild (0);

//		_contentMsgInfo.transform.position = Vector3.zero;
		if (msg.Length >= TOO_MUCH_LETTER) {
			msg = msg.Remove (TOO_MUCH_LETTER);
			msg += " (...)";
		}
		firstmsg.GetComponent<Text> ().text = msg;
		firstmsg.SetAsLastSibling ();
//		GameObject cloneMsgInfo = GameObject.Instantiate (_msgInfo, Vector3.zero, Quaternion.identity, _contentMsgInfo) as GameObject;
//		cloneMsgInfo.GetComponent<Text> ().text = msg;
	}
}
