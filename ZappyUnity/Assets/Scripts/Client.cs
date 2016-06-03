using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System.Net.Sockets;
using System;
using System.Text;
using System.Collections.Generic;

public class Client : MonoBehaviour {

	public string			inputIP;
	public int				ip;
	public Text				error;
	public int				height;
	public int				width;
	public struct			s_datamap {
		public int			x;
		public int			z;
		public int			type;
		public int			nbr;
	};
	public List<s_datamap>	structDataMap;

	private int				[,]_recvDataMap;
	private const string	GRAPHIC =	"GRAPHIC\n";
	private const string	MSZ =		"msz\n";
	private const string	BCT =		"bct\n";
	private const string	MCT =		"mct\n";
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
	private Socket			_socket;
	private bool			_in_game = false;
	private GameObject		_loading_panel;

	// Use this for initialization

	void		WhoIAm(){
		byte[] buffer = Encoding.UTF8.GetBytes(GRAPHIC);
		if (_socket.Send(buffer,buffer.Length, SocketFlags.None) == 0)
			Debug.LogError("WhoIAm(): Aucune donnée n'a été envoyée");
	}

	string		Receive()
	{
		try {
			byte[] msg = new Byte[_socket.Available];
			_socket.Receive(msg,0,_socket.Available,SocketFlags.None);
			return (System.Text.Encoding.ASCII.GetString(msg));
		}
		catch (SocketException E) {
			Debug.LogError("CheckData read"+E.Message);
		}
		return (null);
	}

	void		SizeMap() {
		string decoding = Receive();
		string []split = decoding.Split (' ');
		width = int.Parse(split[1]);
		height = int.Parse(split[2]);
	}

	void		SizeMapContent() {
		string decoding = Receive();
		string [] lines = decoding.Split ('\n');
		_recvDataMap = new int[lines.Length, 10];
		for (int line = 0; line < lines.Length; line++) {
			string []splitData = lines[line].Split (' ');
			for (int data = 1; data < splitData.Length;data++) {
				_recvDataMap[line, data-1] = int.Parse (splitData[data]);
			}
			Debug.Log(lines[line]);
		}
	}

	IEnumerator	LaunchGame() {
		error.text = "Success !";
		error.color = Color.green;
		yield return new WaitForSeconds(3);
		Application.LoadLevel("Game");
		_loading_panel = GameObject.Find ("Loading");
		_in_game = true;
	}

	void Start () {
		error = GameObject.Find("Error").GetComponent<Text>();
		_socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
		try {
//			_socket.Connect(inputIP, ip);
			_socket.Connect("127.0.0.1", 4242);
			StartCoroutine(LaunchGame());
			WhoIAm();
			SizeMap();
			SizeMapContent();
			DontDestroyOnLoad(gameObject);
		}
		catch (Exception e) 
		{
			error.text = "Error : " + e;
			error.color = Color.red;
			Debug.LogError(e);
			Destroy(gameObject);
		}
		_loading_panel.SetActive (false);
	}
	
	// Update is called once per frame
	void Update () {
		if (_in_game) {

		}
	}
}
