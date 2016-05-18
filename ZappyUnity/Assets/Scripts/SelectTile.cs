using UnityEngine;
using System.Collections;

public class SelectTile : MonoBehaviour {

	public GameObject	select;

	private Camera		cam;
	private GameObject	cloneselect;
	private GameUI		scriptUI;

	// Use this for initialization
	void Start () {
		cam = GetComponent<Camera>();
		scriptUI = GameObject.Find ("Canvas").GetComponent<GameUI> ();
	}
	
	// Update is called once per frame
	void Update () {
		if (Input.GetKeyDown (KeyCode.Mouse0)){
			if (cloneselect != null) {
				Destroy(GameObject.Find ("Selection(Clone)"));
				scriptUI.ClearInfoCase();
			}
			GameObject hit = SelectCase ();
			if (hit != null)
				scriptUI.tile = hit;
		}

	}

	GameObject SelectCase()
	{
		RaycastHit hit;
		Ray ray = cam.ScreenPointToRay(Input.mousePosition);
		if (Physics.Raycast(ray, out hit) && hit.collider != null) {
			if (cloneselect == null)
				cloneselect = GameObject.Instantiate(select, select.transform.position + hit.transform.position, select.transform.rotation) as GameObject;
			else
			{
				Destroy(GameObject.Find ("Selection(Clone)"));
				if (hit.transform.gameObject != cloneselect)
					cloneselect = GameObject.Instantiate(select, select.transform.position + hit.transform.position, select.transform.rotation) as GameObject;
			}
			return (hit.transform.gameObject);
		}
		return (null);
	}
}
