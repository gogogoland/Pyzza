using UnityEngine;
using System.Collections;

public class SelectTile : MonoBehaviour {

	public GameObject	select;
	private Camera		cam;
	private GameObject	cloneselect;

	// Use this for initialization
	void Start () {
		cam = GetComponent<Camera>();
	}
	
	// Update is called once per frame
	void Update () {
		if (Input.GetKeyDown(KeyCode.Mouse0))
			SelectCase();
	}

	void SelectCase()
	{
		RaycastHit hit;
		Ray ray = cam.ScreenPointToRay(Input.mousePosition);
		if (Physics.Raycast(ray, out hit)) {
			if (hit.collider != null) {
				if (cloneselect == null)
					cloneselect = GameObject.Instantiate(select, select.transform.position + hit.transform.position, select.transform.rotation) as GameObject;
				else
					Destroy(cloneselect);
			}
		}
	}
}
