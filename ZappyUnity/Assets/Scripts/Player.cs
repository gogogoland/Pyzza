﻿using UnityEngine;
using System.Collections;

public class Player : MonoBehaviour {

	private int							_id;
	private int							_posX;
	private int							_posY;
	private int							_orientation;
	private int							_level;
	private string						_teamName;
	private int							[]_inventory;

	private float						_moveSpeed;
	private int							_maxMapX;
	private int							_maxMapY;
	
	// Use this for initialization
	void Start () {
	}

	public void		PlayerNew(int id, int posX, int posY, int orientation, int level, string teamName) {
		_id = id;
		_posX = posX;
		_posY = posY;
		_orientation = orientation;
		_level = level;
		_teamName = teamName;
		_inventory = new int[7];
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

	public int		GetOrientation(){
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

	public void		SetPosOrient(int posX, int posY, int orientation){
		if (_posX != posX) {
			StartCoroutine("_MoveLeftRight", posX);

		}
		if (_posY != posY) {
			StartCoroutine("_MoveUpDown", posY);
		}
		if (_orientation != orientation)
			_orientation = orientation;
	}
	
	private IEnumerator	_MoveLeftRight(int posX){
		Vector3 target = new Vector3 (posX * 10, transform.position.y, transform.position.z);
		float t = 0.0f;
		while (t < 1) {
			t += Time.deltaTime / _moveSpeed;
			transform.position = Vector3.Lerp (transform.position, target, t);
			_posX = posX;
			yield return null;
		}
	}

	private IEnumerator	_MoveUpDown(int posY){
		Vector3 target = new Vector3 (transform.position.y, posY * 10, transform.position.z);
		float t = 0.0f;
		while (t < 1) {
			t += Time.deltaTime / _moveSpeed;
			transform.position = Vector3.Lerp (transform.position, target, t);
			_posY = posY;
			yield return null;
		}
	}
	
	public void		SetLevel(int level) {
		if (_level != level)
			_level = level;
	}
	
	public void		SetMoveSpeed(int unitTime) {
		_moveSpeed = unitTime * 7.0f;
	}

}
