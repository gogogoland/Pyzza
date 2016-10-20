﻿using UnityEngine;
using System.Collections;

public class Player : MonoBehaviour {

	private	int							_idObjTmp = -1;

	[SerializeField]private int			_id;
	[SerializeField]private int			_posX;
	[SerializeField]private int			_posY;
	[SerializeField]private int			[]_orientation;
	[SerializeField]private int			_level;
	[SerializeField]private string		_teamName;
	[SerializeField]private int			[]_inventory;

	private float						_moveSpeed;

	private Animator					anim;

	//TODO if server change
	private int							DEFAULT = 4;//= 0;
	private int							NORTH = 1;
	private int							WEST = 0;//= 4;
	private int							SOUTH = 3;
	private int							EST = 2;

	private int							AnimSW = 0;
	private int							AnimNW = 1;
	private int							AnimNE = 2;
	private int							AnimSE = 3;
	
	// Use this for initialization
	void Awake () {
		anim = GetComponent<Animator> ();
		_orientation = new int[2];
		_inventory = new int[7];
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
	/*
	public void		GetOrDrop(int idInv, bool drop) {
		for (int obj = 0; obj < _inventory.Length; obj++) {
			if (idInv == obj){
				if (drop) {
					_inventory [idInv] -= 1;
					if (_inventory [idInv] < 0)
						_inventory [idInv] = 0;
				}
				else
					_inventory [idInv] += 1;
				break ;
			}
		}
	}
	*/
	public void		SetObjConcern(int id){
		id = _idObjTmp;
//		anim.SetInteger ("Wait", 0);
		AnimatePlay("Attack");
	}
	/*
	public void		UpdateInventory(){
		if (_idObjTmp == -1)
			return ;
		for (int obj = 0; obj < _inventory.Length; obj++) {
			if (_idObjTmp == obj)
		}
	}*/

	public void		SetPosOrient(int posX, int posY, int orientation){
		if (_orientation[0] != orientation) {
			_orientation[1] = _orientation[0];
			_orientation[0] = orientation;
		}
		if (_posX == posX && _posY == posY) {
			AnimatePlay("Wait");
		}
		if (_posX != posX) {
			_posX = posX;
			AnimatePlay("Move");
			StartCoroutine("_MoveLeftRight", posX);
		}
		if (_posY != posY) {
			_posY = posY;
			AnimatePlay("Move");
			StartCoroutine("_MoveUpDown", posY);
		}
	}

	void	AnimateStop(string action){
	}

	void	AnimatePlay(string action){
		anim.SetInteger (action, CardinalDirection());
	}

	int		CardinalDirection(){
		if (_orientation[0] == NORTH) {
			if (_orientation[1] == EST)
				return (AnimNE);
			else
				return (AnimNW);
		}
		else if (_orientation[0] == EST) {
			if (_orientation[1] == NORTH)
				return (AnimNE);
			else
				return (AnimSE);
		}
		else if (_orientation[0] == SOUTH) {
			if (_orientation[1] == EST)
				return (AnimSE);
			else
				return (AnimSW);
		}
		else if (_orientation[0] == WEST) {
			if (_orientation[1] == NORTH)
				return (AnimNW);
			else
				return (AnimSW);
		}
		return (AnimSW);
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
	}
	
	public void		SetLevel(int level) {
		if (_level != level) {
			_level = level;
			transform.localScale += Vector3.one;
			transform.position = new Vector3(transform.position.x, transform.position.y + 0.275f, transform.position.z);
		}
	}
	
	public void		SetMoveSpeed(int unitTime) {
		_moveSpeed = unitTime * 7.0f;
	}

	public void		Idle(){
//		anim.SetInteger ("Move", 0);
//		anim.SetInteger ("Attack", 0);
		AnimatePlay ("Wait");
	}

	void			Start(){
		AnimatePlay ("Wait");
	}

	public void			Death(){
		AnimatePlay ("Die");
		StartCoroutine("DieTime", 5.0f);
	}

	IEnumerator		DieTime(float time)
	{
		yield return new WaitForSeconds (time);
		Destroy(gameObject);
	}
}
