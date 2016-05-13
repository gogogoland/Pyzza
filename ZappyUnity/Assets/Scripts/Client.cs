using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System.Net.Sockets;
using System;

public class Client : MonoBehaviour {

	public string			inputIP;
	public int				ip;
	public Text			error;

	private TcpClient		client;
	private NetworkStream	serverStream;

	// Use this for initialization

	IEnumerator	LaunchGame() {
		error.text = "Success !";
		error.color = Color.green;
		DontDestroyOnLoad(gameObject);
		yield return new WaitForSeconds(3);
		Application.LoadLevel("Game");
	}

	void Start () {
		error = GameObject.Find("Error").GetComponent<Text>();
		client = new TcpClient();
		try {
			client.Connect(inputIP, ip);
			StartCoroutine(LaunchGame());
		}
		catch (Exception e) 
		{
			error.text = "Error : " + e;
			error.color = Color.red;
			Destroy(gameObject);
		}
	}
	
	// Update is called once per frame
	void Update () {
	
	}
}
