using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System.Net.Sockets;
using System;

public class Menu : MonoBehaviour {

	// Use this for initialization
	private InputField				inputIP;
	private InputField				inputPort;
	private Text					error;
	private TcpClient				client;
	private NetworkStream			serverStream;

	private int						ip = 0;

	void Start () {
		inputIP = GameObject.Find("InputFieldIP").GetComponent<InputField>();
		inputPort = GameObject.Find("InputFieldPort").GetComponent<InputField>();
		error = GameObject.Find("Error").GetComponent<Text>();
		client = new TcpClient();
	}
	
	// Update is called once per frame
	void Update () {

	}

	IEnumerator	LaunchGame() {
		error.text = "Success !";
		error.color = Color.green;
		yield return new WaitForSeconds(3);
		Application.LoadLevel("Game");
	}

	public void Connect() {
		error.text = "Loading ...";
		error.color = Color.black;
		int.TryParse(inputPort.text, out ip);
		try {
			client.Connect(inputIP.text, ip);
			StartCoroutine(LaunchGame());
		}
		catch (Exception e) 
		{
			error.text = "Error : " + e;
			error.color = Color.red;
		}
	}
}
