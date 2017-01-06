using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class Player : MonoBehaviour {

	public GameObject					lvlUp_obj;
	public GameObject					drop_obj;

	private Dictionary<int, Color>		colorParticleDrop;

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

	// Use this for initialization
	void Awake () {
		anim = GetComponent<Animator> ();
		_orientation = new int[2];
		_inventory = new int[7];
		colorParticleDrop = new Dictionary<int, Color> ();
		colorParticleDrop.Add (0, new Color (0.93f, 0.87f, 0.84f));
		colorParticleDrop.Add (1, new Color (1.0f, 1.0f, 0.52f));
		colorParticleDrop.Add (2, new Color (0.83f, 0.88f, 0.76f));
		colorParticleDrop.Add (3, new Color (0.43f, 0.73f, 1.0f));
		colorParticleDrop.Add (4, new Color (0.65f, 0.65f, 0.65f));
		colorParticleDrop.Add (5, new Color (0.28f, 0.74f, 0.13f));
		colorParticleDrop.Add (6, Color.red);
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

	public void		SetPosOrient(int posX, int posY, int orientation){
		if (_orientation[0] != orientation) {
			_orientation[1] = _orientation[0];
			_orientation[0] = orientation;
		}
		if (_posX == posX && _posY == posY) {
			Animate (0);
		}
		if (_posX != posX) {
			_posX = posX;
			Animate (1);
			StartCoroutine("_MoveLeftRight", posX);
		}
		if (_posY != posY) {
			_posY = posY;
			Animate (1);
			StartCoroutine("_MoveUpDown", posY);
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
	}

	private IEnumerator	_MoveUpDown(int posY){
		Vector3 target = new Vector3 (transform.position.x, transform.position.y, posY * -10);
		float t = 0.0f;
		while (t < 1) {
			t += Time.deltaTime;
			transform.position = Vector3.Lerp (transform.position, target, t);
			_posY = posY;
			yield return null;
		}
		Animate (0);
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
		transform.GetChild (1).GetComponent<SpriteRenderer> ().color = color;
	}

	public void		Die() {
		Animate (3);
	}
	
	public void		DestroyMe()
	{
		Destroy(gameObject);
	}
}
