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

	private Transform					lvlPizza;
	private GameObject					nameUI;

	// Use this for initialization
	void Awake () {
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
		lvlPizza = transform.GetChild (1).transform.GetChild (0);
		playername_obj = Resources.Load ("Prefabs/PlayerName") as GameObject;

	}

	public void		PlayerNew(int id, int posX, int posY, int orientation, int level, string teamName) {
		_id = id;
		_posX = posX;
		_posY = posY;
		_orientation[0] = orientation;
		_orientation[1] = DEFAULT;
		_level = level;
		_teamName = teamName;
		transform.localScale = new Vector3((float)_level, (float)_level, (float)_level);
		transform.position = new Vector3 (transform.position.x, (9 + _level) * transform.position.y / 10, transform.position.z);
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
		GameObject particleDrop = GameObject.Instantiate(drop_obj);
		particleDrop.GetComponent<ParticleSystemRenderer> ().material.color = colorParticleDrop [id];
		particleDrop.transform.SetParent(transform);
		particleDrop.transform.localPosition = Vector3.zero;
		particleDrop.transform.localScale = Vector3.one;

		Destroy(particleDrop, particleDrop.GetComponent<ParticleSystem>().startLifetime);
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
			t += Time.deltaTime;
			transform.position = Vector3.Lerp (transform.position, target, t);
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
			t += Time.deltaTime;
			transform.position = Vector3.Lerp (transform.position, target, t);
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
			transform.localScale += Vector3.one;
			transform.position = new Vector3(transform.position.x, transform.position.y + 0.275f, transform.position.z);
			GameObject particleLvl = GameObject.Instantiate(lvlUp_obj);
			particleLvl.transform.SetParent(transform);
			particleLvl.transform.localPosition = Vector3.zero;
			particleLvl.transform.localScale = Vector3.one;
			particleLvl.GetComponent<ParticleSystem>().startLifetime = transform.localScale.x / 10;
			Destroy(particleLvl, particleLvl.GetComponent<ParticleSystem>().startLifetime);
			lvlPizza.GetComponent<SpriteRenderer>().sprite = lvlPizzaSprite[_level - 1];
			if (_level == 9)
				lvlPizza.GetComponent<SpriteRenderer>().color = Color.red;
		}
	}


	public void		Animate(int etat){
		anim.SetInteger("Etat", etat);
		anim.SetTrigger(CardinalDirection());
	}

	void			Start(){
		Animate (0);

		Color color = GameObject.Find ("Client(Clone)").GetComponent<DataGame> ().teamName [_teamName];
		transform.GetChild (0).GetComponent<SpriteRenderer> ().color = color;
		color.a = 1.0f;
		transform.GetChild (1).GetComponent<SpriteRenderer> ().color = color;
		Xmax = GameObject.Find ("Client(Clone)").GetComponent<DataGame> ().width - 1;
		Ymax = GameObject.Find ("Client(Clone)").GetComponent<DataGame> ().height - 1;
		nameUI = GameObject.Instantiate (playername_obj, transform.position, Quaternion.identity) as GameObject;
		nameUI.transform.SetParent(GameObject.Find ("CanvasTalk").transform);
		nameUI.transform.GetComponent<Text> ().text += _id;
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
}
