using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;

public class DataGame : MonoBehaviour {

	public Animator							anim;
	public GameObject						egg_obj;
	public GameObject						player_obj;
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
		public GameObject					stock_player;
	};
	public List<c_player>					players;
	public class							c_egg{
		public int							id;
		public int							id_player;
		public int							pos_x;
		public int							pos_y;
		public GameObject					stock_eggs;
	};
	public List<c_egg>						eggs;
	private GameObject						clone_player;
	private GameObject						clone_egg;

	public void		Init(){
		structDataMap = new List<c_datamap> ();
		teamName = new List<string> ();
		players = new List<c_player> ();
		eggs = new List<c_egg> ();
	}

	public void		MapSize(string []cmd) {
		if (cmd.Length != 3)
			throw new Exception("Donnees de la map erronees");
		width = int.Parse (cmd [1]);
		height = int.Parse (cmd [2]);
	}

	public Vector3	CoordCase(int x, int y){
		Vector3 coord = Vector3.zero;
		coord.x = x * 10;
		coord.z = -y * 10;
		coord.y = player_obj.transform.localScale.y / 2;

		return coord;
	}

	public void		TileContent(string []cmd) {
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
	
	public void		TeamName(string []cmd) {
		if (cmd.Length != 2)
			throw new Exception("Donnees du nom d'equipe erronees");
		teamName.Add (cmd [1]);
	}

	public void		PlayerNew(string []cmd) {
		c_player tmp;

		if (cmd.Length != 7)
			throw new Exception("Donnees d'un joueur erronees");
		tmp = new c_player();
		tmp.id = int.Parse (cmd [1].Substring (1, cmd [1].Length - 1));
		tmp.pos_x = int.Parse (cmd[2]);
		tmp.pos_y = int.Parse (cmd[3]);
		tmp.orientation = int.Parse (cmd[4]);
		tmp.level = int.Parse (cmd[5]);
		tmp.teamName = cmd[6];
		tmp.inventory = new int[7];
		clone_player = GameObject.Instantiate(player_obj, CoordCase(tmp.pos_x, tmp.pos_y), Quaternion.identity) as GameObject;
		tmp.stock_player = clone_player;
		players.Add(tmp);
	}

	public void		PlayerPositionOrientation(string []cmd){
		if (cmd.Length != 5)
			throw new Exception("Donnees d'orientions d'un joueur erronees");
		foreach (c_player player in players) {
			if (player.id == int.Parse (cmd [1].Substring (1, cmd [1].Length - 1))) {
				player.pos_x = int.Parse (cmd[2]);
				player.pos_y = int.Parse (cmd[3]);
				player.orientation = int.Parse (cmd[4]);
				break ;
			}
		}
	}

	public void		PlayerLevel(string []cmd){
		if (cmd.Length != 3)
			throw new Exception("Donnees du level d'un joueur erronees");
		foreach (c_player player in players) {
			if (player.id == int.Parse (cmd [1].Substring (1, cmd [1].Length - 1))) {
				player.level = int.Parse (cmd[2]);
				break ;
			}
		}
	}

	public void		PlayerInventory(string []cmd){
		if (cmd.Length != 11)
			throw new Exception("Donnees du level d'un joueur erronees");
		foreach (c_player player in players) {
			if (player.id == int.Parse (cmd [1].Substring (1, cmd [1].Length - 1))) {
				for (int obj=0; obj < player.inventory.Length; obj++) {
					player.inventory[obj] = int.Parse (cmd[obj + 2]);
				}
				break ;
			}
		}
	}
	//TODO
	public void		PlayerExpulse(string []cmd){
		if (cmd.Length != 2)
			throw new Exception("Donnees d'expulsion d'un joueur erronees");
		foreach (c_player player in players) {
			if (player.id == int.Parse (cmd [1].Substring (1, cmd [1].Length - 1))) {
				Debug.Log ("Expulsion du joueur #" + player.id);
				break ;
			}
		}
	}
	//TODO
	public void		PlayerBroadCast(string []cmd){
		if (cmd.Length != 3)
			throw new Exception("Donnees du broadcast d'un joueur erronees");
		foreach (c_player player in players) {
			if (player.id == int.Parse (cmd [1].Substring (1, cmd [1].Length - 1))) {
				Debug.Log ("Joueur #" + player.id + " dit:" + cmd[2]);
				break ;
			}
		}
	}

	//TODO
	public void		PlayerIncantBegin(string []cmd){
		if (cmd.Length >= 5)
			throw new Exception("Donnees du debut de l'incantation erronees");
		bool restart;
		int i = 0;

		do {
			restart = false;
			foreach (c_player player in players) {
				if (player.id == int.Parse (cmd [5 + i].Substring (1, cmd [5 + i].Length - 1))) {
					Debug.Log ("Joueur #" + player.id + " incante:" + cmd [2]);
					restart = true;
					i++;
					break;
				}
			}
		} while (restart);
	}

	//TODO
	public void		PlayerIncantEnd(string []cmd){
		if (cmd.Length != 4)
			throw new Exception("Donnees de la fin de l'incantation erronees");
		if (int.Parse (cmd[3]) == 0)
			Debug.Log ("L'incantation [" + cmd[1] + ", " + cmd[2] + "] est un echec");
		else
			Debug.Log ("L'incantation [" + cmd[1] + ", " + cmd[2] + "] est une reussite");
	}

	//TODO
	public void		PlayerForkEgg(string []cmd){
		Debug.Log ("ici");
		if (cmd.Length != 2)
			throw new Exception("Donnees de la ponte d'un oeuf erronees");
		foreach (c_player player in players) {
			if (player.id == int.Parse (cmd [1].Substring (1, cmd [1].Length - 1))) {
				Debug.Log ("Joueur #" + player.id + " pond un oeuf");
				break ;
			}
		}
	}

	//TODO
	public void		PlayerGetDrop(string []cmd, int getDrop){
		if (cmd.Length != 2)
			throw new Exception("Donnees d'une action sur une ressource erronees");
		foreach  (c_player player in players) {
			if (player.id == int.Parse (cmd [1].Substring (1, cmd [1].Length - 1))) {
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
	public void		PlayerDie(string []cmd) {
		if (cmd.Length != 2)
			throw new Exception("Donnees de la mort d'un joueur erronees");
		foreach (c_player player in players) {
			if (player.id == int.Parse (cmd [1].Substring (1, cmd [1].Length - 1))) {
				Debug.Log ("Joueur #" + player.id + " meurt");
				players.Remove(player);
				break ;
			}
		}
	}

	public void		EggNew(string []cmd){
		c_egg	tmp;

		if (cmd.Length != 5)
			throw new Exception("Donnees d'un oeuf erronees");
		tmp = new c_egg();
		tmp.id = int.Parse (cmd [1].Substring (1, cmd [1].Length - 1));
		tmp.id_player = int.Parse (cmd [2].Substring (1, cmd [2].Length - 1));
		tmp.pos_x = int.Parse (cmd[3]);
		tmp.pos_y = int.Parse (cmd[4]);

		eggs.Add(tmp);
	}

	public void		EggHatch(string []cmd){
		if (cmd.Length != 2)
			throw new Exception("Donnees de l'eclosion d'un oeuf erronees");
		foreach (c_egg egg in eggs) {
			if (egg.id == int.Parse (cmd [1].Substring (1, cmd [1].Length - 1))) {
				Debug.Log ("L'oeuf #" + egg.id + " eclos");
				eggs.Remove(egg);
				break ;
			}
		}
	}

	public void		EggBorn(string []cmd){
		if (cmd.Length != 2)
			throw new Exception("Donnees de connexion d'un joueur sur un oeuf erronees");
		foreach (c_egg egg in eggs) {
			if (egg.id == int.Parse (cmd [1].Substring (1, cmd [1].Length - 1))) {
				Debug.Log ("Un joueur est nee a partir de l'oeuf #" + egg.id);
				break ;
			}
		}
	}

	public void		EggDie(string []cmd){
		if (cmd.Length != 2)
			throw new Exception("Donnees de la mort d'un oeuf erronees");
		foreach (c_egg egg in eggs) {
			if (egg.id == int.Parse (cmd [1].Substring (1, cmd [1].Length - 1))) {
				Debug.Log ("L'oeuf #" + egg.id + " eclos mais pourri");
				eggs.Remove(egg);
				break ;
			}
		}
	}
	
	public void		ServerGetTime(string []cmd) {
		if (cmd.Length != 2)
			throw new Exception("Donnees de l'unite de temps erronees");
		unitTime = int.Parse (cmd [1]);
	}

	public void		ServerEndGame(string []cmd) {
		if (cmd.Length != 2)
			throw new Exception("Donnees de la fin de partie erronees");
		Debug.Log ("Victoire de l'equipe : " + cmd[1]);
	}

	public void		ServerMessage(string []cmd) {
		if (cmd.Length != 2)
			throw new Exception("Donnees de message du server erronees");
		Debug.Log ("Pizza le Hutt parle : " + cmd[1]);
	}

	public void		ServerUnknownCommand() {
		throw new Exception ("Commande inconnue");
	}

	public void		ServerBadParameter() {
		throw new Exception ("Mauvais parametres pour la commande");
	}
	

	// Use this for initialization
	void Start () {

	}
	
	// Update is called once per frame
	void Update () {

	}
}
