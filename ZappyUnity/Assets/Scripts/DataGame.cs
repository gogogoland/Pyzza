using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using System;
using System.Collections;
using System.Collections.Generic;

public class DataGame : MonoBehaviour {

	public Animator							anim;
	public Sprite							[]bubbleTex;
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
	public class c_team
	{
		public string						name;
		public Color						color;
	};
	public List<c_team>						teamName;
	public List<GameObject>					players;
	public List<GameObject>					eggs;
	public bool								colorChanged = false;

	private GameObject						egg_obj;
	private GameObject						player_obj;
	private GameObject						bubble_obj;
	private GameObject						clone_player;
	private GameObject						clone_egg;
	private GameObject						invok_obj;

	private string							[]typeResrc;

	private bool							update = false;
	private Vector2							vectupdate = new Vector2(-1, -1);

	private GameObject						victory;
	private bool							startSceneGame = true;
	private GameUI							scriptUI;


	void	Awake()
	{
		player_obj = Resources.Load ("Prefabs/Player") as GameObject;
		egg_obj = Resources.Load ("Prefabs/Egg") as GameObject;
		bubble_obj = Resources.Load ("Prefabs/BubbleTalk") as GameObject;
		invok_obj = Resources.Load ("Prefabs/Invocation") as GameObject;
	}

	public void		Init(){
		structDataMap = new List<c_datamap> ();
		teamName = new List<c_team> ();
		players = new List<GameObject> ();
		eggs = new List<GameObject> ();
		typeResrc = new string[7];
		typeResrc[0] = "Food";
		typeResrc[1] = "Linemate";
		typeResrc[2] = "Deraumere";
		typeResrc[3] = "Sibur";
		typeResrc[4] = "Mendiane";
		typeResrc[5] = "Phiras";
		typeResrc[6] = "Thystame";
	}

	public void		MapSize(string []cmd) {
		if (cmd.Length != 3)
			throw new Exception("Donnees de la map erronees");
		width = int.Parse (cmd [1]);
		height = int.Parse (cmd [2]);
	}

	Vector3			CoordCase(int x, int y, bool egg){
		Vector3 coord = Vector3.zero;
		coord.x = x * 10;
		coord.z = -y * 10;
		if (egg)
			coord.y = 1.5f;
		else
			coord.y = 2.5f;
		return coord;
	}

	void			InvockBubbleTalk(Transform player, string talk, int typeBubble){
		GameObject cloneBubble = GameObject.Instantiate(bubble_obj, player.transform.position, Quaternion.identity, GameObject.Find ("CanvasTalk").transform) as GameObject;

		cloneBubble.transform.GetChild(0).GetComponent<Text>().text = talk;
		cloneBubble.GetComponent<BubbleTalk>().posPlayer = player;
		cloneBubble.GetComponent<Image> ().sprite = bubbleTex[typeBubble];
	}

	void			InvockPentacle(int x, int z) {
		GameObject clone = GameObject.Instantiate(invok_obj, Vector3.zero, Quaternion.identity, GameObject.Find ("Tile(" + z + ", " + x + ")").transform) as GameObject;
		clone.transform.localPosition = Vector3.up * 0.1f;
	}

	void			DestroyPentacle(int x, int z) {
		Transform invoc = GameObject.Find ("Tile(" + z + ", " + x + ")").transform.FindChild("Invocation(Clone)");
		if (!invoc)
			return ;
		if (invoc.gameObject)
			Destroy (invoc.gameObject);
	}

	public void		UpdateTile(int x, int y) {
		update = true;
		vectupdate.x = x;
		vectupdate.y = y;
	}

	private bool	_GetOrDropResrc(string []cmd){
		foreach (c_datamap tile in structDataMap) {
			if (tile.x == int.Parse (cmd[1]) && tile.z == int.Parse (cmd[2])) {
				for (int data = 0; data < 7; data++) {
					if (tile.type == data && tile.nbr != int.Parse (cmd[3+data])) {
						GameObject.Find ("GenerateMap").GetComponent<GenerateMap>().UpdateResrc(tile, int.Parse (cmd[3+data]));
						return (true);
					}
				}
			}
		}
		return (false);
	}

	private void	_TileBadUpload(string []cmd){
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
			if (update == true && vectupdate.x != -1 && vectupdate.y != -1) {
				GameObject.Find ("GenerateMap").GetComponent<GenerateMap>().UpdateTile(tmp);
			}
			structDataMap.Add(tmp);
		}
	}

	public void		TileContent(string []cmd) {
		if (cmd.Length < 10 && cmd.Length > 11)
			throw new Exception("Donnees de tuiles erronees" + cmd.Length);
		if (_GetOrDropResrc (cmd))
			return ;
		_TileBadUpload (cmd);
	}
	
	public void		TeamName(string []cmd) {
		if (cmd.Length != 2)
			throw new Exception("Donnees du nom d'equipe erronees");
		c_team	team = new c_team ();
		team.color = new Color( UnityEngine.Random.value, UnityEngine.Random.value, UnityEngine.Random.value, 0.78f );
		team.name = cmd [1];
		teamName.Add (team);
	}

	public void		PlayerNew(string []cmd) {
		if (cmd.Length != 7)
			throw new Exception("Donnees d'un joueur erronees");
		clone_player = GameObject.Instantiate(player_obj, CoordCase(int.Parse (cmd[2]), int.Parse (cmd[3]), false), Quaternion.identity) as GameObject;
		Player script = clone_player.GetComponent<Player> ();
		script.PlayerNew(int.Parse (cmd [1].Substring (1, cmd [1].Length - 1)),
		                 int.Parse (cmd[2]),
		                 int.Parse (cmd[3]),
		                 int.Parse (cmd[4]),
		                 int.Parse (cmd[5]),
		                 cmd[6]);
		players.Add(clone_player);
	}

	public void		PlayerPositionOrientation(string []cmd){
		if (cmd.Length != 5)
			throw new Exception("Donnees d'orientions d'un joueur erronees");
		foreach (GameObject player in players) {
			Player script = player.GetComponent<Player>();
			if (script.GetID() == int.Parse (cmd [1].Substring (1, cmd [1].Length - 1))) {
				script.SetPosOrient(int.Parse (cmd[2]),
									int.Parse (cmd[3]),
									int.Parse (cmd[4]));
				break ;
			}
		}
	}

	public void		PlayerLevel(string []cmd){
		if (cmd.Length != 3)
			throw new Exception("Donnees du level d'un joueur erronees");
		foreach (GameObject player in players) {
			Player script = player.GetComponent<Player>();
			if (script.GetID() == int.Parse (cmd [1].Substring (1, cmd [1].Length - 1))) {
				script.SetLevel(int.Parse (cmd[2]));
				break ;
			}
		}
	}

	public void		PlayerInventory(string []cmd){
		if (cmd.Length != 11)
			throw new Exception("Donnees de l'inventaire d'un joueur erronees");
		foreach (GameObject player in players) {
			Player script = player.GetComponent<Player>();
			if (script.GetID() == int.Parse (cmd [1].Substring (1, cmd [1].Length - 1))) {
				for (int obj=0; obj < script.GetInventory().Length; obj++) {
					script.SetResource(obj, int.Parse (cmd[obj + 4]));
				}
				break ;
			}
		}
	}

	public void		PlayerExpulse(string []cmd){
		if (cmd.Length != 2)
			throw new Exception("Donnees d'expulsion d'un joueur erronees");
		foreach (GameObject player in players) {
			Player script = player.GetComponent<Player>();
			if (script.GetID() == int.Parse (cmd [1].Substring (1, cmd [1].Length - 1))) {
				InvockBubbleTalk(player.transform, "FUS ROH DA !", 1);
//				scriptUI.AddMsgInfo("Expulsion du joueur #" + script.GetID());
				break ;
			}
		}
	}

	public void		PlayerBroadCast(string []cmd){
		if (cmd.Length <= 1)
			throw new Exception("Donnees du broadcast d'un joueur erronees");
		string talk = "...";

		if (cmd.Length != 2) {
			int 	word;
			talk = "";
			for (word = 2; word < cmd.Length - 1; word++)
				talk += cmd[word] + " ";
			talk += cmd[word];
		}

		foreach (GameObject player in players) {
			Player script = player.GetComponent<Player>();
			if (script.GetID() == int.Parse (cmd [1].Substring (1, cmd [1].Length - 1))) {
				if (cmd.Length >= 3) {
					InvockBubbleTalk(player.transform, talk, 0);
					scriptUI.AddMsgInfo("Joueur #" + script.GetID() + " dit : " + talk);
				}
				break ;
			}
		}
	}

	public void		PlayerIncantBegin(string []cmd){
		if (cmd.Length < 5)
			throw new Exception("Donnees du debut de l'incantation erronees");
		InvockPentacle (int.Parse (cmd [1]), int.Parse (cmd [2]));
		for (int id = 4; id < cmd.Length; id++) {
			foreach (GameObject player in players) {
				Player script = player.GetComponent<Player>();

				if (script.GetPosX() == int.Parse (cmd[1])
				    && script.GetPosY() == int.Parse (cmd[2])
				    && script.GetID() == int.Parse (cmd [id].Substring (1, cmd [id].Length - 1))){
					scriptUI.AddMsgInfo("Joueur #" + script.GetID() + " incante au level: " + cmd [3] + " en [" + cmd[1] + ", " + cmd[2] + "]");
				}
			}
		}
	}

	public void		PlayerIncantEnd(string []cmd){
		if (cmd.Length != 4)
			throw new Exception("Donnees de la fin de l'incantation erronees");
		DestroyPentacle(int.Parse (cmd [1]), int.Parse (cmd [2]));
		if (int.Parse (cmd[3]) == 0)
			scriptUI.AddMsgInfo("L'incantation [" + cmd[1] + ", " + cmd[2] + "] est un echec");
		else
			scriptUI.AddMsgInfo("L'incantation [" + cmd[1] + ", " + cmd[2] + "] est une reussite");
	}
	
	public void		PlayerForkEgg(string []cmd){
		if (cmd.Length != 2)
			throw new Exception("Donnees de la ponte d'un oeuf erronees");
		foreach (GameObject player in players) {
			Player script = player.GetComponent<Player>();
			if (script.GetID() == int.Parse (cmd [1].Substring (1, cmd [1].Length - 1))) {
				InvockBubbleTalk(player.transform, "Je pond un oeuf", 2);
//				scriptUI.AddMsgInfo("Joueur #" + script.GetID() + " pond un oeuf");
				break ;
			}
		}
	}
	
	public void		PlayerGet(string []cmd){
		if (cmd.Length != 3)
			throw new Exception("Donnees d'une action sur une ressource erronees");
		foreach (GameObject player in players) {
			Player script = player.GetComponent<Player>();

			if (script.GetID() == int.Parse (cmd [1].Substring (1, cmd [1].Length - 1))) {
				script.SetObjConcern(int.Parse (cmd[2]));
//				scriptUI.AddMsgInfo("Joueur #" + script.GetID() + " prend " + int.Parse (cmd[2]));
				break ;
			}
		}
	}

	public void		PlayerDrop(string []cmd) {
		if (cmd.Length != 3)
			throw new Exception("Donnees d'un drop d'une ressource erronees");
		foreach (GameObject player in players) {
			Player script = player.GetComponent<Player>();
			if (script.GetID() == int.Parse (cmd [1].Substring (1, cmd [1].Length - 1))) {
				script.SetObjConcern(int.Parse (cmd[2]));
//				scriptUI.AddMsgInfo("Joueur #" + script.GetID() + " pose " + int.Parse (cmd[2]));
			}
		}
	}

	void			DropResrc(GameObject modelToClone, int idResrc, int resize, Transform tile, GenerateMap scriptGM)
	{
		Vector3		vec = Vector3.zero;
		
		GameObject	resrc;
		vec = new Vector3(0, modelToClone.transform.position.y, 0);
		resrc = GameObject.Instantiate(modelToClone, vec, Quaternion.identity, tile) as GameObject;
		resrc.transform.localScale *= resize;
		resrc.GetComponent<SpriteRenderer>().sprite = scriptGM.ressources_sprite[idResrc];
		scriptGM.RepositioningResrc(resrc, idResrc);
		scriptGM.resrcs.Add(resrc);
	}

	public void		PlayerDie(string []cmd) {
		if (cmd.Length != 2)
			throw new Exception("Donnees de la mort d'un joueur erronees");
		foreach (GameObject player in players) {
			Player script = player.GetComponent<Player>();
			if (script.GetID () == int.Parse (cmd [1].Substring (1, cmd [1].Length - 1))) {
				scriptUI.AddMsgInfo("Joueur #" + script.GetID () + " meurt");
				script.Die ();
				players.Remove(player);
				break ;
			}
		}
	}

	public void		EggNew(string []cmd){
		if (cmd.Length != 5)
			throw new Exception("Donnees d'un oeuf erronees");
		clone_egg = GameObject.Instantiate(egg_obj, CoordCase(int.Parse (cmd[3]), int.Parse (cmd[4]), true), Quaternion.identity) as GameObject;

		Egg script = clone_egg.GetComponent<Egg> ();
		if (script == null)
			Debug.LogError ("!!!");
		script.EggNew(int.Parse (cmd [1].Substring (1, cmd [1].Length - 1)),
			           int.Parse (cmd [2].Substring (1, cmd [2].Length - 1)),
			           int.Parse (cmd [3]),
		    	       int.Parse (cmd [4]));
		eggs.Add(clone_egg);
	}

	public void		EggHatch(string []cmd){
		if (cmd.Length != 2)
			throw new Exception("Donnees de l'eclosion d'un oeuf erronees");
		foreach (GameObject egg in eggs) {
			Egg script = egg.GetComponent<Egg> ();
			if (!script)

			if (script.GetID() == int.Parse (cmd [1].Substring (1, cmd [1].Length - 1))) {
//				scriptUI.AddMsgInfo("L'oeuf #" + script.GetID() + " eclos");
				script.Hatch();
				break ;
			}
		}
	}

	public void		EggBorn(string []cmd){
		if (cmd.Length != 2)
			throw new Exception("Donnees de connexion d'un joueur sur un oeuf erronees");
		foreach (GameObject egg in eggs) {
			Egg script = egg.GetComponent<Egg> ();
			if (script.GetID() == int.Parse (cmd [1].Substring (1, cmd [1].Length - 1))) {
//				scriptUI.AddMsgInfo("Un joueur est nee a partir de l'oeuf #" + script.GetID());
				script.Die(true);
				eggs.Remove(egg);
				break ;
			}
		}
	}

	public void		EggDie(string []cmd){
		if (cmd.Length != 2)
			throw new Exception("Donnees de la mort d'un oeuf erronees");
		foreach (GameObject egg in eggs) {
			Egg script = egg.GetComponent<Egg> ();
			if (script.GetID() == int.Parse (cmd [1].Substring (1, cmd [1].Length - 1))) {
//				scriptUI.AddMsgInfo("L'oeuf #" + script.GetID() + " eclos mais pourri");
				script.Die(false);
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
		victory.SetActive (true);
		Text text = victory.transform.GetChild (1).GetComponent<Text> ();
		text.text = cmd [1];
		foreach (c_team team in teamName) {
			if (team.name == cmd [1]) {
				victory.GetComponent<Image> ().color = new Color (1.0f - team.color.r, 1.0f - team.color.g, 1.0f - team.color.b);
				text.color = team.color;
				break ;
			}
		}
		scriptUI.AddMsgInfo("Victoire de l'equipe : " + cmd[1]);
	}

	public void		ServerMessage(string []cmd, bool error) {
		if (error) {
			scriptUI.AddMsgInfo("Pizza le Hutt s'enerve : Je ne comprends pas ton langage !");
			return ;
		}
		string talk = "...";

		if (cmd.Length != 1) {
			int 	word;
			talk = "";
			for (word = 1; word < cmd.Length - 1; word++)
				talk += cmd[word] + " ";
			talk += cmd[word];
		}
		scriptUI.AddMsgInfo("Pizza le Hutt dit : " + talk);
	}

	public void		PlayerEat (string[]cmd) {
		if (cmd.Length != 2)
			throw new Exception("Donnees du dejeuner du joueur erronees");
		foreach (GameObject player in players) {
			Player script = player.GetComponent<Player>();
			if (script.GetID () == int.Parse (cmd [1].Substring (1, cmd [1].Length - 1))) {
				InvockBubbleTalk(player.transform, "* miam *", 2);
//				scriptUI.AddMsgInfo("Joueur #" + script.GetID () + " mange");
				break ;
			}
		}
	}

	void			InputColorTeamChanged() {
		if (Input.GetKey(KeyCode.C)) {
			foreach(c_team team in teamName)
				team.color = new Color (UnityEngine.Random.value, UnityEngine.Random.value, UnityEngine.Random.value, 0.78f);
			colorChanged = true;
		}
	}

	// Use this for initialization
	void StartSceneGame () {
		if (SceneManager.GetActiveScene().name == "Game" && startSceneGame == true) {
			victory = GameObject.Find ("Canvas/Victory");
			scriptUI = GameObject.Find ("Canvas").GetComponent<GameUI>();
			victory.SetActive (false);
			startSceneGame = false;
		}
	}

	// Update is called once per frame
	void Update () {
		StartSceneGame ();
		InputColorTeamChanged ();
	}
}
