using UnityEngine;
using System.Collections;

public class BubbleTalk : MonoBehaviour {

	public Transform		posPlayer;

	// Use this for initialization
	void Start () {
		StartCoroutine ("BubbleDeath");
	}
	
	// Update is called once per frame
	void LateUpdate () {
		if (posPlayer)
			transform.position = new Vector3(Camera.main.WorldToScreenPoint (posPlayer.position).x, Camera.main.WorldToScreenPoint (posPlayer.position).y + 80.0f, 0);
	}

	IEnumerator		BubbleDeath() {
		yield return new WaitForSeconds (3);
		Destroy (this.gameObject);
	}
}
