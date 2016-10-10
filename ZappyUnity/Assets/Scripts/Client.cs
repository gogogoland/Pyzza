using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System.Net.Sockets;
using System;
using System.Text;
using System.Collections.Generic;

public class Client : MonoBehaviour {

	public string							inputIP;
	public int								ip;
	public Text								error;
	public bool								newTime = false;
	
	private const string					GRAPHIC =	"GRAPHIC\n";
	private const string					MCT =		"mct\n";
	private const string					PPO =		"ppo #";
	private const string					PLV =		"plv #";
	private const string					PIN =		"pin #";
	private const string					SGT =		"sgt\n";
	private const string					SST =		"sst ";
	private string							rtfContent = null;
	private DataGame						_scriptData;
	private GameUI							_scriptUI;

	private Socket							_socket;
	private bool							_in_game = false;
	private GameObject						_loading_panel;

	// Use this for initialization

	string		Receive()
	{
		try {
			byte[] msg = new Byte[_socket.ReceiveBufferSize + 1];
			_socket.Receive(msg,0,_socket.Available,SocketFlags.None);
			return (System.Text.Encoding.ASCII.GetString(msg));
		}
		catch (SocketException E) {
			Debug.LogError("CheckData read"+E.Message);
		}
		return (null);
	}

	void		Send(string send){
		byte[] buffer = Encoding.UTF8.GetBytes (send);
		if (_socket.Send (buffer, buffer.Length, SocketFlags.None) == 0)
			throw new Exception("CheckData write : " + send);
	}
	
	void		WhoIAm(){
		if (_socket.Connected) {
			Send(GRAPHIC);
			error.text = "Success !";
			error.color = Color.green;
		}
	}

	void	CheckData(){
		if (_socket.Connected){
			if (_socket.Poll(10,SelectMode.SelectRead) && _socket.Available == 0)
				throw new Exception("La connexion au serveur est interrompue.");
		}
		if (_socket.Available > 0){
			string recv = null;

			while (_socket.Available > 0){
				try {
					recv=Receive();
					rtfContent+=recv;
				}
				catch(SocketException E) {
					throw new Exception("CheckData read"+E.Message);
				}
			}
		}
	}

	void		DataDistribution(){
		string	[]cutBlockData = rtfContent.Split ('\n');

		for (int line = 0; line < cutBlockData.Length; line++) {
			Debug.Log (cutBlockData[line]);
			string []cutCmd = cutBlockData[line].Split(' ');
			switch (cutCmd[0]) {
			case "Bienvenue" : _scriptData.Init();break;
			case "msz" : _scriptData.MapSize(cutCmd);break;
			case "bct" : _scriptData.TileContent(cutCmd);break;
			case "tna" : _scriptData.TeamName(cutCmd);break;
			case "pnw" : _scriptData.PlayerNew(cutCmd);break;
			case "ppo" : _scriptData.PlayerPositionOrientation(cutCmd);break;
			case "plv" : _scriptData.PlayerLevel(cutCmd);break;
			case "pin" : _scriptData.PlayerInventory(cutCmd);break;
			case "pex" : _scriptData.PlayerExpulse(cutCmd);break;
			case "pbc" : _scriptData.PlayerBroadCast(cutCmd);break;
			case "pic" : _scriptData.PlayerIncantBegin(cutCmd);break;
			case "pie" : _scriptData.PlayerIncantEnd(cutCmd);break;
			case "pfk" : _scriptData.PlayerForkEgg(cutCmd);break;
			case "pdr" : _scriptData.PlayerGetDrop(cutCmd, -1);break;
			case "pgt" : _scriptData.PlayerGetDrop(cutCmd, 1);break;
			case "pdi" : _scriptData.PlayerDie(cutCmd);break;
			case "enw" : _scriptData.EggNew(cutCmd);break;
			case "eht" : _scriptData.EggHatch(cutCmd);break;
			case "ebo" : _scriptData.EggBorn(cutCmd);break;
			case "edi" : _scriptData.EggDie(cutCmd);break;
			case "sgt" : _scriptData.ServerGetTime(cutCmd);break;
			case "seg" : _scriptData.ServerEndGame(cutCmd);break;
			case "smg" : _scriptData.ServerMessage(cutCmd);break;
			case "suc" : _scriptData.ServerUnknownCommand();break;
			case "sbp" : _scriptData.ServerBadParameter();break;
			default : break;
			}
		}
		if (Input.GetKey(KeyCode.L))
			Debug.Log (rtfContent);
		rtfContent = null;
	}

	void		Start () {
		error = GameObject.Find("Error").GetComponent<Text>();
		_scriptData = GetComponent<DataGame> ();
		_socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
		try {
			_socket.Connect(inputIP, ip);

			WhoIAm();
			CheckData();
			if (rtfContent != null)
			{
				DataDistribution();
				DontDestroyOnLoad(gameObject);
				Application.LoadLevel("Game");
				_scriptUI = GameObject.Find("Canvas").GetComponent<GameUI> ();
				DemandInfo();
				_in_game = true;
			}
		}
		catch (Exception e)
		{
			_socket.Disconnect(true);
			if (!_in_game) {
				error.text = "Error : " + e;
				error.color = Color.red;
			}
			else
				Application.LoadLevel("Menu");
			Debug.LogError(e);
			Destroy(gameObject);
		}
	}

	void		DemandInfo() {
		try {
			if (_socket.Connected) {
				Send (MCT);
				Debug.Log (_scriptData.players.Count);
				for (int player=0; player < _scriptData.players.Count; player++) {
					Send (PPO + _scriptData.players [player].id + "\n");
					Send (PLV + _scriptData.players [player].id + "\n");
					Send (PIN + _scriptData.players [player].id + "\n");
				}
				Send (SGT);
				if (newTime) {
					Send (SST + GameObject.Find("Canvas").GetComponent<GameUI> ().value_slider + "\n");
					newTime = false;
				}
			}
		} catch (Exception e) {
			Debug.LogError ("Error Demand Info : " + e);
		}
	}

	IEnumerator		DemandInfoCourout() {
		while (true) {
			DemandInfo ();
			yield return new WaitForSeconds (1);
		}
	}

	// Update is called once per frame
	void		Update () {
		if (_in_game) {
			StartCoroutine(DemandInfoCourout());
			CheckData();
			if (rtfContent != null)
				DataDistribution();
		}

	}
}
