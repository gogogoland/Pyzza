using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System.Collections.Generic;

public class Player : MonoBehaviour {

	public GameObject					lvlUp_obj;
	public GameObject					drop_obj;
	public Sprite						[]lvlPizzaSprite;

	private Dictionary<int, Color>		colorParticleDrop;
	private GameObject					playername_obj;
	private GameObject					invok_obj;

	[SerializeField]private int			_id;
	[SerializeField]private int			_posX;
	[SerializeField]private int			_posY;
	[SerializeField]private int			[]_orientation;
	[SerializeField]private int			_level;
	[SerializeField]private string		_teamName;
	[SerializeField]private int			[]_inventory;

	private Animator					anim;
	private int							DEFAULT = 4;//= 0;
	private int							NORTH = 0;//= 1;
	private int							WEST = 1;//= 2;
	private int							SOUTH = 2;//= 3;
	private int							EST = 3;//= 4;
	private int							Xmax;
	private int							Ymax;

	private GameObject					nameUI;

	private DataGame2					_scriptDataGame;
	private AudioSource					[]audiosrcs;

	private SpriteRenderer 				flagMiniMap;
	private SpriteRenderer 				flag;
	private SpriteRenderer				lvlPizza;


	// Use this for initialization
	void Awake () {
		_scriptDataGame = GameObject.Find ("Client(Clone)").GetComponent<DataGame2> ();
		anim = GetComponent<Animator> ();
		_orientation = new int[2];
		_inventory = new int[7];
		colorParticleDrop = new Dictionary<int, Color> ();
		colorParticleDrop.Add (0, Color.white);
		colorParticleDrop.Add (1, Color.red + Color.yellow);
		colorParticleDrop.Add (2, Color.grey);
		colorParticleDrop.Add (3, Color.cyan);
		colorParticleDrop.Add (4, Color.black);
		colorParticleDrop.Add (5, Color.green);
		colorParticleDrop.Add (6, Color.red);
		lvlPizza = transform.FindChild ("Flag").transform.GetChild (0).GetComponent<SpriteRenderer> ();
		playername_obj = Resources.Load ("Prefabs/PlayerName") as GameObject;
		invok_obj = Resources.Load ("Prefabs/Invocation") as GameObject;
		audiosrcs = GetComponentsInChildren<AudioSource> ();
		flagMiniMap = transform.FindChild ("FlagMiniMap").GetComponent<SpriteRenderer> ();
		flag = transform.FindChild ("Flag").GetComponent<SpriteRenderer> ();
	}

	public void		PlayerNew(int id, int posX, int posY, int orientation, int level, string teamName) {
		_id = id;
		_posX = posX;
		_posY = posY;
		_orientation[0] = orientation;
		_orientation[1] = DEFAULT;
		_level = level;
		_teamName = teamName;
		transform.position = new Vector3 (transform.position.x, 2.5f, transform.position.z);
	}
	
	public int		GetID(){
		return (_id);
	}

	public int		GetPosX(){
		return (_posX);
	}
	
	public int		GetPosY(){
		return (_posY);
	}

	public int		[]GetOrientation(){
		return (_orientation);
	}

	public int		GetLevel(){
		return (_level);
	}

	public string	GetTeamName(){
		return (_teamName);
	}

	public int		[]GetInventory(){
		return (_inventory);
	}

	public int		GetInventory(int id) {
		return (_inventory [id]);
	}

	public void		SetResource(int positionInv, int value){
		if (_inventory[positionInv] != value)
			_inventory [positionInv] = value;
	}
	
	public void		SetObjConcern(int id){
		Animate (2);
		GameObject particleDrop = GameObject.Instantiate(drop_obj, transform);
		particleDrop.GetComponent<ParticleSystemRenderer> ().material.color = colorParticleDrop [id];
		particleDrop.transform.localPosition = Vector3.zero;
		particleDrop.transform.localScale = Vector3.one;

		Destroy(particleDrop, particleDrop.GetComponent<ParticleSystem>().main.startLifetimeMultiplier);
	}

	public void		SetPosOrient(int newPosX, int newPosY, int orientation){
		if (_orientation[0] != orientation) {
			_orientation[1] = _orientation[0];
			_orientation[0] = orientation;
		}
		if (_posX == newPosX && _posY == newPosY) {
			Animate (0);
		}
		if (_posX != newPosX) {
			PlaySound ("Walk");
			Animate (1);
			if (_posX == 0 && newPosX == Xmax)
				StartCoroutine("_MoveLeftRight", -1);
			else if (_posX == Xmax && newPosX == 0)
				StartCoroutine("_MoveLeftRight", Xmax + 1);
			else
				StartCoroutine("_MoveLeftRight", newPosX);
			_posX = newPosX;
		}
		if (_posY != newPosY) {
			PlaySound ("Walk");
			Animate (1);
			if (_posY == 0 && newPosY == Ymax)
				StartCoroutine("_MoveUpDown", -1);
			else if (_posY == Ymax && newPosY == 0)
				StartCoroutine("_MoveUpDown", Xmax + 1);
			else
				StartCoroutine("_MoveUpDown", newPosY);
			_posY = newPosY;
		}
	}

	string	CardinalDirection(){
		if (_orientation[0] == NORTH) {
			if (_orientation[1] == EST)
				return ("NE");
			else
				return ("NW");
		}
		else if (_orientation[0] == EST) {
			if (_orientation[1] == NORTH)
				return ("NE");
			else
				return ("SE");
		}
		else if (_orientation[0] == SOUTH) {
			if (_orientation[1] == EST)
				return ("SE");
			else
				return ("SW");
		}
		else if (_orientation[0] == WEST) {
			if (_orientation[1] == NORTH)
				return ("NW");
			else
				return ("SW");
		}
		return ("SW");
	}
	
	private IEnumerator	_MoveLeftRight(int posX){
		Vector3 target = new Vector3 (posX * 10, transform.position.y, transform.position.z);
		float t = 0.0f;
		while (t < 1) {
			t += Time.deltaTime / (7/_scriptDataGame.unitTime);
			transform.position = Vector3.MoveTowards (transform.position, target, t / 10);
			_posX = posX;
			yield return null;
		}
		Animate (0);
		if (posX == -1)
			_posX = Xmax;
		else if (posX == Xmax + 1)
			_posX = 0;
		transform.position = new Vector3 (_posX * 10.0f, transform.position.y, transform.position.z);

	}

	private IEnumerator	_MoveUpDown(int posY){
		Vector3 target = new Vector3 (transform.position.x, transform.position.y, posY * -10.0f);
		float t = 0.0f;
		while (t < 1) {
			t += Time.deltaTime / (7/_scriptDataGame.unitTime);
			transform.position = Vector3.MoveTowards (transform.position, target, t / 10);
			_posY = posY;
			yield return null;
		}
		Animate (0);
		if (posY == -1)
			_posY = Ymax;
		else if (posY == Ymax + 1)
			_posY = 0;
		transform.position = new Vector3 (transform.position.x, transform.position.y, _posY * -10.0f);
	}
	
	public void		SetLevel(int level) {
		if (_level != level) {
			_level = level;
			GameObject particleLvl = GameObject.Instantiate (lvlUp_obj, transform);
			PlaySound ("LevelUp");
			particleLvl.transform.localPosition = Vector3.zero;
			Destroy (particleLvl, particleLvl.GetComponent<ParticleSystem> ().main.startLifetimeMultiplier);
			lvlPizza.sprite = lvlPizzaSprite [_level - 1];
			if (_level == 9)
				lvlPizza.color = Color.red;
		} else {
			PlaySound ("FailedLvlUp");
		}
		DestroyPentacle ();
	}

	public void		InvockPentacle() {
		GameObject pentacle = GameObject.Instantiate (invok_obj, transform);
		pentacle.transform.localPosition = new Vector3(0, -transform.position.y / 10, 0);
	}

	void			DestroyPentacle() {
		Transform		pentacle = transform.FindChild ("Invocation(Clone)");

		if (pentacle)
			Destroy (pentacle.gameObject);
	}

	public void		Animate(int etat){
		anim.SetInteger("Etat", etat);
		anim.SetTrigger(CardinalDirection());
	}

	Color			MyColorTeam(){
		Color color = Color.white;
		foreach(DataGame2.c_team team in _scriptDataGame.teamName)
		{
			if (team.name == _teamName)
				return (team.color);
		}
		return (Color.white);
	}

	void			Start(){
		Animate (0);

		Color color = MyColorTeam ();
		flagMiniMap.color = color;
		color.a = 1.0f;
		flag.color = color;
		Xmax = _scriptDataGame.width - 1;
		Ymax = _scriptDataGame.height - 1;
		nameUI = GameObject.Instantiate (playername_obj, transform.position, Quaternion.identity, GameObject.Find ("CanvasTalk").transform) as GameObject;
		nameUI.transform.GetComponent<Text> ().text += _id;
	}

	void			Update(){
		if (_scriptDataGame.colorChanged) {
			Color color = MyColorTeam ();
			flagMiniMap.color = color;
			color.a = 1.0f;
			flag.color = color;
		}
	}

	void			LateUpdate(){
		nameUI.transform.position = new Vector3(Camera.main.WorldToScreenPoint (transform.position).x, Camera.main.WorldToScreenPoint (transform.position).y - 60.0f, 0);
	}

	public void		Die() {
		Animate (3);
	}

	public void		DestroyMe()
	{
		Destroy (nameUI);
		Destroy(gameObject);
	}		

	public void PlaySound(string name){
		foreach(AudioSource audio in audiosrcs){
			if (audio.clip.name == name) {
				audio.Play ();
				break;
			}
		}
	}
}
