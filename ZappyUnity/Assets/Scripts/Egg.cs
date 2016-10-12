using UnityEngine;
using System.Collections;

public class Egg : MonoBehaviour {

	private int							_id;
	private int							_idPlayer;
	private int							_posX;
	private int							_posY;

	// Use this for initialization
	void Start () {
	
	}

	public int	GetID(){
		return (_id);
	}

	public int GetIDPlayer(){
		return (_idPlayer);
	}

	public int GetPosX(){
		return (_posX);
	}

	public int GetPosY(){
		return (_posY);
	}

	public void EggNew(int id, int idPlayer, int posX, int posY){
		_id = id;
		_idPlayer = idPlayer;
		_posX = posX;
		_posY = posY;
	}
}
