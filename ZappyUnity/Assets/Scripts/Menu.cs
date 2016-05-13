using UnityEngine;
using UnityEngine.UI;
using System.Collections;

public class Menu : MonoBehaviour {

	// Use this for initialization
	public GameObject				client;

	private InputField				inputIP;
	private InputField				inputPort;
	private GameObject				clientclone;

	void Start () {
		inputIP = GameObject.Find("InputFieldIP").GetComponent<InputField>();
		inputPort = GameObject.Find("InputFieldPort").GetComponent<InputField>();
	}
	
	// Update is called once per frame
	void Update () {
	}

	public void Connect() {
		clientclone = GameObject.Instantiate(client, Vector3.zero, Quaternion.identity) as GameObject;
		clientclone.GetComponent<Client>().inputIP = inputIP.text;
		int.TryParse(inputPort.text, out clientclone.GetComponent<Client>().ip);

	}
}
