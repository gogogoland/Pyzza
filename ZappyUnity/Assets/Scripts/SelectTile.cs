using UnityEngine;
using System.Collections;

public class SelectTile : MonoBehaviour {

	private Camera	cam;

	// Use this for initialization
	void Start () {
		cam = GetComponent<Camera>();
	}
	
	// Update is called once per frame
	void Update () {
//		if (Input.GetKeyDown(KeyCode.Mouse0))
			Ray ray = cam.ScreenPointToRay(Input.mousePosition);
		if (Input.GetKeyDown(KeyCode.Mouse0))
			Debug.DrawRay(ray.origin, ray.direction * 10, Color.red, 60.0f);
//			Ray ray = camera.ScreenPointToRay(new Vector3(200, 200, 0));
//		Debug.DrawRay(ray.origin, ray.direction * 10, Color.yellow);
	}
}
