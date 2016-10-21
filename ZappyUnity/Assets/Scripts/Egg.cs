using UnityEngine;
using System.Collections;

public class Egg : MonoBehaviour {

	private int							_id;
	private int							_idPlayer;
	private int							_posX;
	private int							_posY;

	private Animator					anim;

	// Use this for initialization
	void Awake () {
		anim = GetComponent<Animator> ();
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

	IEnumerator		DieTime(float time)
	{
		yield return new WaitForSeconds (time);
		Destroy(gameObject);
	}

	public void		Hatch()
	{
		anim.SetInteger ("Etat", 1);
	}

	public void		Die(bool playerAlive)
	{
		if (playerAlive)
			anim.SetInteger("Etat", 3);
		else
			anim.SetInteger("Etat", 2);
		StartCoroutine("DieTime", 5.0f);
	}
	
	void Update(){
	}
}
