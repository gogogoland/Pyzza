using UnityEngine;
using UnityEngine.UI;
using System.Collections;

public class Menu : MonoBehaviour {

	// Use this for initialization
	private InputField				inputIP;
	private InputField				inputPort;
	private Text					error;

	private int						ip = 0;

	void Start () {
		inputIP = GameObject.Find("InputFieldIP").GetComponent<InputField>();
		inputPort = GameObject.Find("InputFieldPort").GetComponent<InputField>();
		error = GameObject.Find("Error").GetComponent<Text>();
	}
	
	// Update is called once per frame
	void Update () {

	}

	public void Connect() {
		error.text = "Loading ...";
		error.color = Color.black;

		int.TryParse(inputPort.text, out ip);
		if (ip != 0 && inputIP.text != "")
			Network.Connect(inputIP.text, ip);
		else
		{
			error.color = Color.red;
			error.text = "Error : Missing Ip or Port";
		}
	}

	void OnFailedToConnect(NetworkConnectionError err) {
		error.color = Color.red;
		if (string.Compare(err.ToString(), "NoError") != 0)
			error.text = "Error : " + err;
	}

	void OnConnectedToServer() {
		error.color = Color.green;
		error.text = "Success !";
		Application.LoadLevel("Game");
	}
}
