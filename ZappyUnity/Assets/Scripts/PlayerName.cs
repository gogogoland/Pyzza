using UnityEngine;
using System.Collections;

public class PlayerName : MonoBehaviour {

	public Transform		posPlayer;

	// Use this for initialization
	void Start () {
	}
	
	// Update is called once per frame
	void LastUpdate () {
		if (posPlayer)
			transform.position = new Vector3(Camera.main.WorldToScreenPoint (posPlayer.position).x, Camera.main.WorldToScreenPoint (posPlayer.position).y, 0);
		else
			Destroy(gameObject);
	}
}
