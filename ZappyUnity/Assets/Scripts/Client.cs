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
	public int								height;
	public int								width;
	public class							c_datamap {
		public int							x;
		public int							z;
		public int							type;
		public int							nbr;
		public int							tileColor;
		public GameObject					resrc;
	};
	public List<c_datamap>					structDataMap;
	public int								unitTime = 0;
	public List<string>						teamName;
	public class							c_player {
		public int							id;
		public int							pos_x;
		public int							pos_y;
		public int							orientation;
		public int							level;
		public string						teamName;
		public int							[]inventory;
	};
	public List<c_player>					players;
	public class							c_egg{
		public int							id;
		public int							id_player;
		public int							pos_x;
		public int							pos_y;
	};
	public List<c_egg>						eggs;

	private const string					GRAPHIC =	"GRAPHIC\n";
	private const string					MCT =		"mct\n";
	private const string					PPO =		"ppo #";
	private const string					PLV =		"plv #";
	private const string					PIN =		"pin #";
	private const string					SGT =		"sgt\n";
	private const string					SST =		"sst";
	private string							rtfContent = null;

	private Socket							_socket;
	private bool							_in_game = false;
	private GameObject						_loading_panel;

	// Use this for initialization

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

	void		MapSize(string []cmd) {
		if (cmd.Length != 3)
			throw new Exception("Donnees de la map erronees");
		width = int.Parse (cmd [1]);
		height = int.Parse (cmd [2]);
	}

	void		TileContent(string []cmd){
		if (cmd.Length < 10 && cmd.Length > 11)
			throw new Exception("Donnees de tuiles erronees" + cmd.Length);
		for (int data = 0; data < 7; data++) {
			c_datamap	tmp = new c_datamap();

			tmp.x = int.Parse (cmd[1]);
			tmp.z = int.Parse (cmd[2]);
			tmp.type = data;
			tmp.nbr = int.Parse (cmd[3+data]);
			if (cmd.Length == 10)
				tmp.tileColor = 0;
			else
				tmp.tileColor = int.Parse(cmd[10]);
			structDataMap.Add(tmp);
		}
	}
	
	void		TeamName(string []cmd) {
		if (cmd.Length != 2)
			throw new Exception("Donnees du nom d'equipe erronees");
		teamName.Add (cmd [1]);
	}

	void		PlayerNew(string []cmd){
		c_player tmp;

		if (cmd.Length != 7)
			throw new Exception("Donnees d'un joueur erronees");
		tmp = new c_player();
		tmp.id = int.Parse (cmd[1] + 1);
		tmp.pos_x = int.Parse (cmd[2]);
		tmp.pos_y = int.Parse (cmd[3]);
		tmp.orientation = int.Parse (cmd[4]);
		tmp.level = int.Parse (cmd[5]);
		tmp.teamName = cmd[6];
		tmp.inventory = new int[7];
		players.Add(tmp);
	}

	void		PlayerPositionOrientation(string []cmd){
		if (cmd.Length != 5)
			throw new Exception("Donnees d'orientions d'un joueur erronees");
		foreach (c_player player in players) {
			if (player.id == int.Parse (cmd[1] + 1)) {
				player.pos_x = int.Parse (cmd[2]);
				player.pos_y = int.Parse (cmd[3]);
				player.orientation = int.Parse (cmd[4]);
				break ;
			}
		}
	}

	void		PlayerLevel(string []cmd){
		if (cmd.Length != 3)
			throw new Exception("Donnees du level d'un joueur erronees");
		foreach (c_player player in players) {
			if (player.id == int.Parse (cmd[1] + 1)) {
				player.level = int.Parse (cmd[2]);
				break ;
			}
		}
	}

	void		PlayerInventory(string []cmd){
		if (cmd.Length != 11)
			throw new Exception("Donnees du level d'un joueur erronees");
		foreach (c_player player in players) {
			if (player.id == int.Parse (cmd[1] + 1)) {
				for (int obj=0; obj < player.inventory.Length; obj++) {
					player.inventory[obj] = int.Parse (cmd[obj + 2]);
				}
				break ;
			}
		}
	}
	//TODO
	void		PlayerExpulse(string []cmd){
		if (cmd.Length != 2)
			throw new Exception("Donnees d'expulsion d'un joueur erronees");
		foreach (c_player player in players) {
			if (player.id == int.Parse (cmd [1] + 1)) {
				Debug.Log ("Expulsion du joueur #" + player.id);
				break ;
			}
		}
	}
	//TODO
	void		PlayerBroadCast(string []cmd){
		if (cmd.Length != 3)
			throw new Exception("Donnees du broadcast d'un joueur erronees");
		foreach (c_player player in players) {
			if (player.id == int.Parse (cmd [1] + 1)) {
				Debug.Log ("Joueur #" + player.id + " dit:" + cmd[2]);
				break ;
			}
		}
	}

	//TODO
	void		PlayerIncantBegin(string []cmd){
		if (cmd.Length >= 5)
			throw new Exception("Donnees du debut de l'incantation erronees");
		bool restart;
		int i = 0;

		do {
			restart = false;
			foreach (c_player player in players) {
				if (player.id == int.Parse (cmd [5 + i] + 1)) {
					Debug.Log ("Joueur #" + player.id + " incante:" + cmd [2]);
					restart = true;
					i++;
					break;
				}
			}
		} while (restart);
	}

	//TODO
	void		PlayerIncantEnd(string []cmd){
		if (cmd.Length != 4)
			throw new Exception("Donnees de la fin de l'incantation erronees");
		if (int.Parse (cmd[3]) == 0)
			Debug.Log ("L'incantation [" + cmd[1] + ", " + cmd[2] + "] est un echec");
		else
			Debug.Log ("L'incantation [" + cmd[1] + ", " + cmd[2] + "] est une reussite");
	}

	//TODO
	void		PlayerForkEgg(string []cmd){
		if (cmd.Length != 2)
			throw new Exception("Donnees de la ponte d'un oeuf erronees");
		foreach (c_player player in players) {
			if (player.id == int.Parse (cmd [1] + 1)) {
				Debug.Log ("Joueur #" + player.id + " pond un oeuf");
				break ;
			}
		}
	}

	//TODO
	void		PlayerGetDrop(string []cmd, int getDrop){
		if (cmd.Length != 2)
			throw new Exception("Donnees d'une action sur une ressource erronees");
		foreach  (c_player player in players) {
			if (player.id == int.Parse (cmd [1] + 1)) {
				for (int obj = 0;obj < player.inventory.Length;obj++) {
					if (int.Parse (cmd[2]) == obj) {
						player.inventory[obj] += getDrop;
						if (player.inventory[obj] < 0)
							player.inventory[obj] = 0;
						break ;
					}
				}
				break ;
			}
		}
	}

	//TODO
	void		PlayerDie(string []cmd) {
		if (cmd.Length != 2)
			throw new Exception("Donnees de la mort d'un joueur erronees");
		foreach (c_player player in players) {
			if (player.id == int.Parse (cmd [1] + 1)) {
				Debug.Log ("Joueur #" + player.id + " meurt");
				players.Remove(player);
				break ;
			}
		}
	}

	void		EggNew(string []cmd){
		c_egg	tmp;

		if (cmd.Length != 5)
			throw new Exception("Donnees d'un oeuf erronees");
		tmp = new c_egg();
		tmp.id = int.Parse (cmd[1] + 1);
		tmp.id_player = int.Parse (cmd[2] + 1);
		tmp.pos_x = int.Parse (cmd[3]);
		tmp.pos_y = int.Parse (cmd[4]);
		eggs.Add(tmp);
	}

	void		EggHatch(string []cmd){
		if (cmd.Length != 2)
			throw new Exception("Donnees de l'eclosion d'un oeuf erronees");
		foreach (c_egg egg in eggs) {
			if (egg.id == int.Parse (cmd [1] + 1)) {
				Debug.Log ("L'oeuf #" + egg.id + " eclos");
				eggs.Remove(egg);
				break ;
			}
		}
	}

	void		EggBorn(string []cmd){
		if (cmd.Length != 2)
			throw new Exception("Donnees de connexion d'un joueur sur un oeuf erronees");
		foreach (c_egg egg in eggs) {
			if (egg.id == int.Parse (cmd [1] + 1)) {
				Debug.Log ("Un joueur est nee a partir de l'oeuf #" + egg.id);
				break ;
			}
		}
	}

	void		EggDie(string []cmd){
		if (cmd.Length != 2)
			throw new Exception("Donnees de la mort d'un oeuf erronees");
		foreach (c_egg egg in eggs) {
			if (egg.id == int.Parse (cmd [1] + 1)) {
				Debug.Log ("L'oeuf #" + egg.id + " eclos mais pourri");
				eggs.Remove(egg);
				break ;
			}
		}
	}
	
	void		ServerGetTime(string []cmd) {
		if (cmd.Length != 2)
			throw new Exception("Donnees de l'unite de temps erronees");
		unitTime = int.Parse (cmd [1]);
	}

	void		ServerEndGame(string []cmd) {
		if (cmd.Length != 2)
			throw new Exception("Donnees de la fin de partie erronees");
		Debug.Log ("Victoire de l'equipe : " + cmd[1]);
	}

	void		ServerMessage(string []cmd) {
		if (cmd.Length != 2)
			throw new Exception("Donnees de message du server erronees");
		Debug.Log ("Pizza le Hutt parle : " + cmd[1]);
	}

	void		ServerUnknownCommand() {
		throw new Exception ("Commande inconnue");
	}

	void		ServerBadParameter() {
		throw new Exception ("Mauvais parametres pour la commande");
	}
	
	IEnumerator	CheckData(float time){
		while (true) {
			if (_socket.Connected){
				if (_socket.Poll(10,SelectMode.SelectRead) && _socket.Available == 0)
					throw new Exception("La connexion au serveur est interrompue.");
			}
			if (_socket.Available > 0){
				string recv;
				while (_socket.Available > 0){
					try {
						byte[] msg = new Byte[_socket.ReceiveBufferSize];
						_socket.Receive(msg,0,_socket.ReceiveBufferSize,SocketFlags.None);
						recv=System.Text.Encoding.UTF8.GetString(msg).Trim();
						rtfContent+=recv;
					}
					catch(SocketException E) {
						throw new Exception("CheckData read"+E.Message);
					}
				}
			}
			yield return new WaitForSeconds(time);
		}
	}

	void		DataDistribution(){
		string	[]cutBlockData = rtfContent.Split ('\n');
		structDataMap = new List<c_datamap> ();
		teamName = new List<string> ();
		players = new List<c_player> ();
		eggs = new List<c_egg> ();
		for (int line = 0; line < cutBlockData.Length; line++) {
			string []cutCmd = cutBlockData[line].Split(' ');
			switch (cutCmd[0]) {
			case "msz"	: MapSize(cutCmd);break;
			case "bct"	: TileContent(cutCmd);break;
			case "tna"	: TeamName(cutCmd);break;
			case "pnw"	: PlayerNew(cutCmd);break;
			case "ppo"	: PlayerPositionOrientation(cutCmd);break;
			case "plv"	: PlayerLevel(cutCmd);break;
			case "pin"	: PlayerInventory(cutCmd);break;
			case "pex"	: PlayerExpulse(cutCmd);break;
			case "pbc"	: PlayerBroadCast(cutCmd);break;
			case "pic"	: PlayerIncantBegin(cutCmd);break;
			case "pie"	: PlayerIncantEnd(cutCmd);break;
			case "pfk"	: PlayerForkEgg(cutCmd);break;
			case "pdr"	: PlayerGetDrop(cutCmd, -1);break;
			case "pgt"	: PlayerGetDrop(cutCmd, 1);break;
			case "pdi"	: PlayerDie(cutCmd);break;
			case "enw"	: EggNew(cutCmd);break;
			case "eht"	: EggHatch(cutCmd);break;
			case "ebo"	: EggBorn(cutCmd);break;
			case "edi"	: EggDie(cutCmd);break;
			case "sgt"	: ServerGetTime(cutCmd);break;
			case "seg"	: ServerEndGame(cutCmd);break;
			case "smg"	: ServerMessage(cutCmd);break;
			case "suc"	: ServerUnknownCommand();break;
			case "sbp"	: ServerBadParameter();break;
			default		: break;
			}
		}
	}

	void		Start () {
		error = GameObject.Find("Error").GetComponent<Text>();
		_socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
		try {
			_socket.Connect(inputIP, ip);

			WhoIAm();
			StartCoroutine("CheckData", 0.01f);
			if (rtfContent != null)
			{
				Debug.Log (rtfContent);
				DataDistribution();
				DontDestroyOnLoad(gameObject);
				Application.LoadLevel("Game");
				_in_game = true;
				rtfContent = null;
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

	public void	UpdateUnitTime(int time) {
		Send (SST + time + "\n");
	}

	void		DemandInfo() {
		try {
			if (_socket.Connected) {
				Send (MCT);
				foreach(c_player player in players) {
					Send (PPO + player.id + "\n");
					Send (PLV + player.id + "\n");
					Send (PIN + player.id + "\n");
				}
				Send (SGT);
			}
		}
		catch (Exception e) {
			Debug.LogError("Error Demand Info : " + e);
		}
	}

	// Update is called once per frame
	void		Update () {
		if (_in_game) {
			DemandInfo();
		}
	}
}
