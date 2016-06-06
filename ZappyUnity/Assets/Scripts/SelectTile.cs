using UnityEngine;
using System.Collections;
using UnityEngine.EventSystems;

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

	void Update () {
		if (Input.GetKeyDown (KeyCode.Mouse0) && EventSystem.current.IsPointerOverGameObject() == false){
			if (cloneselect != null) {
				Destroy(cloneselect);
				scriptUI.information_case.SetActive(false);
			}
			RaycastHit	hit;
			Ray			ray = cam.ScreenPointToRay(Input.mousePosition);
			if (Physics.Raycast (ray, out hit) && hit.collider != null && hit.transform.gameObject != cloneselect) {
				cloneselect = GameObject.Instantiate(select, select.transform.position + hit.transform.position, select.transform.rotation) as GameObject;
				cloneselect.GetComponent<InfoCaseUI>().tile = hit.transform.gameObject;
			}
		}
	}
}
