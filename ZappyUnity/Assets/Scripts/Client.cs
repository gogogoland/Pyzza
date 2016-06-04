﻿using UnityEngine;
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
	public int				unitTime;
	public List<string>		teamName;
	public struct			s_player {
		public int			id;
		public int			pos_x;
		public int			pos_y;
		public int			orientation;
		public int			level;
		public string		teamName;
	};
	public List<s_player>	players;
	public struct			s_egg{
		public int			id;
		public int			id_player;
		public int			pos_x;
		public int			pos_y;
	};
	public List<s_egg>		eggs;

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
	private const int		READWRITE =	3;
	private const int		READ =		2;
	private const int		WRITE =		1;
	private const int		ERROR =		-1;
	private string			rtfContent = null;

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
	
	int		Select(){
		if (_socket.Poll (-1, SelectMode.SelectWrite)) {
			if (_socket.Poll (-1, SelectMode.SelectRead)) {
				Debug.Log ("toto");
				return (READWRITE);
			} else
				return (WRITE);
		} else if (_socket.Poll (-1, SelectMode.SelectRead))
			return (READ);
		else if (_socket.Poll (-1, SelectMode.SelectError))
			return (ERROR);
		return (0);
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
	
	void		WhoIAm(){
		if (_socket.Connected) {
			byte[] buffer = Encoding.UTF8.GetBytes (GRAPHIC);
			if (_socket.Send (buffer, buffer.Length, SocketFlags.None) == 0)
				throw new Exception("WhoIAm(): Aucune donnée n'a été envoyée");
			error.text = "Success !";
			error.color = Color.green;
		}
	}

//	IEnumerator	LaunchGame() {
//		error.text = "Success !";
//		error.color = Color.green;
//		yield return new WaitForSeconds(3);
//		Application.LoadLevel("Game");
//		_loading_panel = GameObject.Find ("Loading");
//		_in_game = true;
//	}

	void		MapSize(string []cmd) {
		if (cmd.Length != 3)
			throw new Exception("Donnees de la map erronees");
		width = int.Parse (cmd [1]);
		height = int.Parse (cmd [2]);
	}

	void		TileContent(string []cmd){
		if (cmd.Length != 10)
			throw new Exception("Donnees de tuiles erronees");
		for (int data = 0; data < 7; data++) {
			s_datamap	tmp = new s_datamap();

			tmp.x = int.Parse (cmd[1]);
			tmp.z = int.Parse (cmd[2]);
			tmp.type = data;
			tmp.nbr = int.Parse (cmd[3+data]);
			structDataMap.Add(tmp);
		}
	}

	void		ServerGetTime(string []cmd) {
		if (cmd.Length != 2)
			throw new Exception("Donnees de l'unite de temps erronees");
		unitTime = int.Parse (cmd [1]);
	}

	void		TeamName(string []cmd) {
		if (cmd.Length != 2)
			throw new Exception("Donnees du nom d'equipe erronees");
		teamName.Add (cmd [1]);
	}

	void		PlayerNetwork(string []cmd){
		if (cmd.Length != 7)
			throw new Exception("Donnees d'un joueur erronees");
		s_player	tmp = new s_player();

		tmp.id = int.Parse (cmd[1] + 1);
		tmp.pos_x = int.Parse (cmd[2]);
		tmp.pos_y = int.Parse (cmd[3]);
		tmp.orientation = int.Parse (cmd[4]);
		tmp.level = int.Parse (cmd[5]);
		tmp.teamName = cmd[6];
		players.Add(tmp);
	}

	void		EggNetwork(string []cmd){
		if (cmd.Length != 5)
			throw new Exception("Donnees d'un oeuf erronees");
		s_egg	tmp = new s_egg();
		
		tmp.id = int.Parse (cmd[1] + 1);
		tmp.id_player = int.Parse (cmd[2] + 1);
		tmp.pos_x = int.Parse (cmd[3]);
		tmp.pos_y = int.Parse (cmd[4]);
		eggs.Add(tmp);
	}
	
	IEnumerator CheckData(){
		while (true) {
			if (_socket.Connected){
				if (_socket.Poll(10,SelectMode.SelectRead) && _socket.Available == 0)
					throw new Exception("La connexion au serveur est interrompue.");
			}
			if (_socket.Available > 0){
				string recv;
				while (_socket.Available > 0){
					try
					{
						byte[] msg=new Byte[_socket.Available];
						//Réception des données
						_socket.Receive(msg,0,_socket.Available,SocketFlags.None);
						recv=System.Text.Encoding.UTF8.GetString(msg).Trim();
						//On concatène les données reues(max 4ko) dans
						//une variable de la classe
						rtfContent+=recv;
					}
					catch(SocketException E)
					{
						throw new Exception("CheckData read"+E.Message);
					}
				}
			}
			yield return new WaitForSeconds(0.01f);
		}
	}

	void DataDistribution(){
		string [] cutBlockData = rtfContent.Split ('\n');
		structDataMap = new List<s_datamap> ();
		teamName = new List<string> ();
		players = new List<s_player> ();
		eggs = new List<s_egg> ();
		for (int line = 0; line < cutBlockData.Length; line++) {
			string []cutCmd = cutBlockData[line].Split(' ');
			switch (cutCmd[0]) {
			case "msz"	: MapSize(cutCmd);break;
			case "bct"	: TileContent(cutCmd);break;
			case "sgt"	: ServerGetTime(cutCmd);break;
			case "tna"	: TeamName(cutCmd);break;
			case "pnw"	: PlayerNetwork(cutCmd);break;
			case "enw"	: EggNetwork(cutCmd);break;
			default: throw new Exception("Donnees non recevable :" + cutCmd[0]);break;
			}
		}
	}

	void Start () {
		error = GameObject.Find("Error").GetComponent<Text>();
		_socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
		try {
//			_socket.Connect(inputIP, ip);
			_socket.Connect("127.0.0.1", 4242);
//			StartCoroutine(LaunchGame());
			WhoIAm();
			StartCoroutine(CheckData());
			if (rtfContent != null)
				DataDistribution();
//			WhoIAm();
//			SizeMap();
//			SizeMapContent();
			DontDestroyOnLoad(gameObject);
		}
		catch (Exception e) 
		{
			error.text = "Error : " + e;
			error.color = Color.red;
			Debug.LogError(e);
			Destroy(gameObject);
		}
//		_loading_panel.SetActive (false);
	}
	
	// Update is called once per frame
	void Update () {
		if (_in_game) {

		}
	}
}