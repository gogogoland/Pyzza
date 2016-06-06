using UnityEngine;
using System;
using System.Collections;
using UnityEngine.UI;
using System.Collections.Generic;

public class GameUI : MonoBehaviour {
	public GameObject					information_case;
	
	// Use this for initialization
	void		Start () {
		information_case = GameObject.Find ("Information");
		information_case.SetActive (false);
	}

	// Update is called once per frame
	void		Update () {

	}
}
