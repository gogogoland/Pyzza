using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
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
	public int								newTimeValue = 0;
	public float							sendRate = 10.0f;
	public bool 							inUpdate = false;
	public bool								activeScriptGenerate = false;
	
	private const string					GRAPHIC =	"GRAPHIC\n";
	private const string					MCT =		"mct\n";
	private const string					BCT = 		"bct ";
	private const string					PPO =		"ppo #";
	private const string					PLV =		"plv #";
	private const string					PIN =		"pin ";
	private const string					SGT =		"sgt\n";
	private const string					SST =		"sst ";
	private string							rtfContent = null;
	private DataGame2						_scriptData;

	private Socket							_socket;
	private GameObject						_loading_panel;
	private float							nextSend = 0.0f;
	
	// Use this for initialization

	string		Receive()
	{
		try {
			byte[] msg = new Byte[_socket.ReceiveBufferSize];
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
		if (_socket.Connected) {
			if (_socket.Poll(100,SelectMode.SelectRead) && _socket.Available == 0) {
				SceneManager.LoadScene ("Menu");
				throw new Exception("La connexion au serveur est interrompue.");
			}
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
//			Debug.Log (cutBlockData[line]);
			string []cutCmd = cutBlockData[line].Split(' ');
			switch (cutCmd[0]) {
			case "Bienvenue" : _scriptData.Init();break;
			case "msz" : _scriptData.MapSize(cutCmd);break;
			case "bct" : _scriptData.TileContent(cutCmd);break;
			case "tna" : _scriptData.TeamName(cutCmd);break;
			case "pnw" : _scriptData.PlayerNew (cutCmd);SendPIN(cutCmd);break;
			case "ppo" :if (inUpdate) { _scriptData.PlayerPositionOrientation(cutCmd);} break;
			case "plv" :if (inUpdate) { _scriptData.PlayerLevel(cutCmd);} break;
			case "pin" :if (inUpdate) { _scriptData.PlayerInventory(cutCmd);} break;
			case "pex" :if (inUpdate) { _scriptData.PlayerExpulse(cutCmd);} break;
			case "pbc" :if (inUpdate) { _scriptData.PlayerBroadCast(cutCmd);} break;
			case "pic" :if (inUpdate) { _scriptData.PlayerIncantBegin(cutCmd);} break;
			case "pie" :if (inUpdate) { _scriptData.PlayerIncantEnd(cutCmd);} break;
			case "pfk" :if (inUpdate) { _scriptData.PlayerForkEgg(cutCmd);} break;
			case "pdr" :if (inUpdate) { _scriptData.PlayerDrop(cutCmd);} break;
			case "pgt" :if (inUpdate) { _scriptData.PlayerGet(cutCmd);} break;
			case "pdi" :Debug.Log (cutBlockData[line]);if (inUpdate) { _scriptData.PlayerDie(cutCmd);} break;
			case "enw" : _scriptData.EggNew(cutCmd);break;
			case "eht" :if (inUpdate) { _scriptData.EggHatch(cutCmd);} break;
			case "ebo" :if (inUpdate) { _scriptData.EggBorn(cutCmd);} break;
			case "edi" :if (inUpdate) { _scriptData.EggDie(cutCmd);} break;
			case "sgt" : _scriptData.ServerGetTime(cutCmd);break;
			case "seg" :if (inUpdate) { _scriptData.ServerEndGame(cutCmd);} break;
			case "smg" :if (inUpdate) { _scriptData.ServerMessage(cutCmd, false);} break;
			case "suc" :if (inUpdate) { _scriptData.ServerMessage(cutCmd, true);} break;
			case "sbp" :if (inUpdate) { _scriptData.ServerMessage(cutCmd, true);} break ;
			case "pet" :if (inUpdate) { _scriptData.PlayerEat (cutCmd);} break ;
			default : break;
			}
		}
		rtfContent = null;
	}

	void		Start () {
		error = GameObject.Find("Error").GetComponent<Text>();
		_scriptData = GetComponent<DataGame2> ();
		_socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
		try {
			_socket.Connect(inputIP, ip);
			WhoIAm();
			while (rtfContent == null) {
				CheckData();
			}
			if (rtfContent != null)
			{
//				Debug.Log(rtfContent);
				DataDistribution();
				DontDestroyOnLoad(gameObject);
				for (int player = 0; player < _scriptData.players.Count; player++)
					DontDestroyOnLoad(_scriptData.players [player]);
				for (int egg = 0; egg < _scriptData.eggs.Count; egg++)
					DontDestroyOnLoad(_scriptData.eggs [egg]);
				DontDestroyOnLoad(GameObject.Find ("CanvasTalk"));
				SceneManager.LoadScene("Game");
			}
		}
		catch (Exception e)
		{
			_socket.Disconnect(true);
			if (SceneManager.GetActiveScene().name != "Game") {
				error.text = "Error : " + e;
				error.color = Color.red;
			}
			else
				SceneManager.LoadScene("Menu");
			Debug.LogError(e);
			Destroy(gameObject);
		}
	}

	public void SendBCT(int x, int y){
		Send (BCT + x + " " + y + "\n");
	}

	public void SendPIN(string []cmd) {		
		Send (PIN + cmd [1] + "\n");
	}

	void		DemandInfo() {
		try {
			if (_socket.Connected) {
				if (newTime) {
					Send (SST + newTimeValue + "\n");
					newTime = false;
				}
			}
		} catch (Exception e) {
			Debug.LogError ("Error Demand Info : " + e);
		}
	}

	// Update is called once per frame
	void		Update () {
		try {
			if (SceneManager.GetActiveScene().name == "Game") {
				if (!activeScriptGenerate) {
					GameObject.Find("GenerateMap").GetComponent<GenerateMap2>().enabled = true;
					activeScriptGenerate = true;
				}
				if (inUpdate) {
					CheckData();
					if (rtfContent != null)
						DataDistribution();
					if (Time.time > nextSend) {
						nextSend = Time.time + sendRate;
						DemandInfo();
					}
				}
			}
		}
		catch (Exception e) {
			_socket.Disconnect(true);
			Application.Quit ();
//			_socket.Disconnect(true);
//			SceneManager.LoadScene("Menu");
			Debug.LogError(e);
//			Destroy(gameObject);
		}
	}

	IEnumerator		DisconnectServer(Exception e){
		GameUI	scriptUI = GameObject.Find("Canvas").GetComponent<GameUI>();
		while(true) 
		{ 
			scriptUI.AddMsgInfo("Deconnexion du server dans " + (int)Time.time + "...");
			yield return new WaitForSeconds(1f);
		}
	}

	void		LastUpdate(){

	}
}
