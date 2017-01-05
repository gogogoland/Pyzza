using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;
using System.Collections;

public class Menu : MonoBehaviour {

	// Use this for initialization
	public GameObject				client;

	private InputField				inputIP;
	private InputField				inputPort;
	private GameObject				clientclone;
	private	EventSystem				system;
	private bool					sensTab = false;

	void Start () {
		inputIP = GameObject.Find("InputFieldIP").GetComponent<InputField>();
		inputPort = GameObject.Find("InputFieldPort").GetComponent<InputField>();
		system = EventSystem.current;
	}
	
	// Update is called once per frame
	void Update () {
		if (Input.GetKeyDown (KeyCode.Tab) && system.currentSelectedGameObject) {
			Selectable next;
			if (!sensTab)
				next = system.currentSelectedGameObject.GetComponent<Selectable> ().FindSelectableOnDown ();
			else
				next = system.currentSelectedGameObject.GetComponent<Selectable> ().FindSelectableOnUp ();

			if (next != null) {
				
				InputField inputfield = next.GetComponent<InputField> ();
				if (inputfield != null)
					inputfield.OnPointerClick (new PointerEventData (system));
				
				system.SetSelectedGameObject (next.gameObject, new BaseEventData (system));
			}
			else {
				sensTab = !sensTab;
			}			
		}
	}

	public void Connect() {
		clientclone = GameObject.Instantiate(client, Vector3.zero, Quaternion.identity) as GameObject;
		clientclone.GetComponent<Client>().inputIP = inputIP.text;
		int.TryParse(inputPort.text, out clientclone.GetComponent<Client>().ip);
	}
}
