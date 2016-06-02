using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System.Net.Sockets;
using System;
using System.IO;
using System.Text;

public class Client : MonoBehaviour {

	public string			inputIP;
	public int				ip;
	public Text				error;
	public int				height;
	public int				width;

	private const string	GRAPHIC =	"graphe\n";
	private const string	MSZ =		"msz\n";
	private const string	BCT =		"bct\n";
	private const string	MCT =		"contenue\n";
	private const string	TNA =		"tna\n";
	private const string	PPO =		"ppo\n";
	private const string	PLV =		"plv\n";
	private const string	PIN =		"pin\n";
	private const string	SGT =		"sgt\n";
	private const string	SST =		"sst\n";

/*	public string[]command_recv = {
		"pnw",
		"pex",
		"pbc",
		"pic",
		"pie",
		"pfk",
		"pdr",
		"pgt",
		"pdi",
		"enw",
		"eht",
		"ebo",
		"edi",
		"seg",
		"smg",
		"suc",
		"sbp",
	};*/
	private TcpClient		_client;
	private NetworkStream	_serverStream;
	private StreamReader	_reader;
	private StreamWriter	_writer;
	private bool			_in_game = false;
	private GameObject		_loading_panel;

	// Use this for initialization

	void		WhoIAm(){
		byte[] buffer = Encoding.ASCII.GetBytes(GRAPHIC);
		_serverStream.Write(buffer, 0, buffer.Length);
		_serverStream.Flush();
	}

	void		SizeMap(){
		byte[] recv = new byte[2048];
		byte[] buffer = Encoding.ASCII.GetBytes(MSZ);
		string decoding;
		_serverStream.Write(buffer, 0, buffer.Length);
		_serverStream.Flush();
		int bytesread = _serverStream.Read(recv, 0, recv.Length);
		decoding = Encoding.ASCII.GetString (recv);
		string []split = decoding.Split (' ');
		width = int.Parse(split [1]);
		height = int.Parse(split [2]);
	}

	void		SizeMapContent()
	{
		byte[] recv = new byte[2048];byte[] recv2 = new byte[2048];byte[] recv3 = new byte[2048];
		byte[] buffer = Encoding.ASCII.GetBytes(MCT);
		_serverStream.Write(buffer, 0, buffer.Length);
//		_serverStream.Flush();
		byte[] bytetoread = new byte[_client.ReceiveBufferSize];
		int bytesRead = _serverStream.Read(bytetoread, 0, _client.ReceiveBufferSize);
		Debug.Log("Received : " + Encoding.ASCII.GetString(bytetoread, 0, bytesRead));
		byte[] bytetoread2 = new byte[_client.ReceiveBufferSize];
		int bytesRead2 = _serverStream.Read(bytetoread2, 0, _client.ReceiveBufferSize);
		Debug.Log("Received : " + Encoding.ASCII.GetString(bytetoread2, 0, bytesRead2));
		//		for (int i=0; i<height*width; i++) {
//		int bytesread = _serverStream.Read (recv, 0, recv.Length);
//		 bytesread = _serverStream.Read (recv2, 0, recv2.Length);
//		 bytesread = _serverStream.Read (recv3, 0, recv3.Length);
//		}
//		Debug.Log (bytesread);
//	while (bytesread > 0)
//			bytesread = _serverStream.Read(recv, 0, recv.Length);
//		Debug.Log (Encoding.ASCII.GetString(recv));
//		Debug.Log (Encoding.ASCII.GetString(recv2));
//		Debug.Log (Encoding.ASCII.GetString(recv3));
//		}
//		int bytesread = _serverStream.Read(recv, 0, recv.Length);

//		MemoryStream memStream = new MemoryStream ();
//		
//		Debug.Log (Encoding.ASCII.GetString(recv));
//		while (bytesread > 0)
//		{
//			memStream.Write(recv, 0, bytesread);
//			bytesread = _serverStream.Read(recv, 0, recv.Length);
//		}
//		_serverStream.Flush();
//		string toto = Encoding.ASCII.GetString(memStream.ToArray ());
//		Debug.Log (toto);
	}

	IEnumerator	LaunchGame() {
		error.text = "Success !";
		error.color = Color.green;
		_serverStream = _client.GetStream();
		yield return new WaitForSeconds(3);
		Application.LoadLevel("Game");
		_loading_panel = GameObject.Find ("Loading");
		_in_game = true;

//		_writer.WriteLine("contenue");
//		string recv;
//		while ((recv = _reader.ReadLine()) == null);
//		Debug.Log (recv);
//		_loading_panel.SetActive (false);
	}

	void Start () {
		error = GameObject.Find("Error").GetComponent<Text>();
		_client = new TcpClient();
		try {
			_client.Connect(inputIP, ip);
			StartCoroutine(LaunchGame());
			WhoIAm();
			width = 4;height=4;//SizeMap();
			SizeMapContent();
			DontDestroyOnLoad(gameObject);
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
		if (_in_game) {

		}
	}
}
