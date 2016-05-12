using UnityEngine;
using UnityEngine.UI;
using System.Collections;

public class Menu : MonoBehaviour {

	// Use this for initialization
	private InputField		inputIP;
	private InputField		inputPort;
	private Text			error;

	private int				ip = 0;
	void Start () {
		inputIP = GameObject.Find("InputFieldIP").GetComponent<InputField>();
		inputPort = GameObject.Find("InputFieldPort").GetComponent<InputField>();
		error = GameObject.Find("Error").GetComponent<Text>();
	}
	
	// Update is called once per frame
	void Update () {
		if (inputPort.text != "")
			int.TryParse(inputPort.text, out ip);
//			
	}

	public void Connect() {
		NetworkConnectionError e;

		int.TryParse(inputPort.text, out ip);
		if (ip != 0 && inputIP.text != "")
		{
			e = Network.Connect(inputIP.text, ip);
			if (string.Compare(e.ToString(), "NoError") != 0)
				error.text = "Error : " + e;
		}
		else
			error.text = "Error : Missing Ip or Port";
		//		if (string.Compare((e = Network.Connect(inputIP.text, int.Parse (inputPort.text)))), "NoError")
//			error.text = "Error : " + e;

	}
}
