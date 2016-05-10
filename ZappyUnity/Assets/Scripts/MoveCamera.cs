using UnityEngine;
using System.Collections;

public class MoveCamera : MonoBehaviour {

	public float		speed;
	private GenerateMap	scriptmap;

	
	void	CenterCamera(int w) {
		float x = (w * scriptmap.tile.transform.localScale.x * 10 / 2 ) - (scriptmap.tile.transform.localScale.x * 10 / 2);
		transform.position = new Vector3(x, 15, 0);
	}

	// Use this for initialization
	void Start () {
		scriptmap = GameObject.Find("GenerateMap").GetComponent<GenerateMap>();
		CenterCamera(scriptmap.width);
	}
	
	// Update is called once per frame
	void Update () {
		if (Input.GetKey(KeyCode.A) || Input.GetKey(KeyCode.LeftArrow))
			transform.Translate(Vector3.left * Time.deltaTime * speed, Space.World);
		if (Input.GetKey(KeyCode.D) || Input.GetKey(KeyCode.RightArrow))
			transform.Translate(Vector3.right * Time.deltaTime * speed, Space.World);
		if (Input.GetKey(KeyCode.W) || Input.GetKey(KeyCode.UpArrow))
			transform.Translate(Vector3.forward * Time.deltaTime * speed, Space.World);
		if (Input.GetKey(KeyCode.S) || Input.GetKey(KeyCode.DownArrow))
			transform.Translate(Vector3.back * Time.deltaTime * speed, Space.World);
	}
}
