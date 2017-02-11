using UnityEngine;
using System.Collections;

public class Egg : MonoBehaviour {

	[SerializeField]private int							_id;
	[SerializeField]private int							_idPlayer;
	[SerializeField]private int							_posX;
	[SerializeField]private int							_posY;

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

	public void	DestroyMe()
	{
		Destroy(gameObject);
	}

	public void	Hatch()
	{
		anim.SetInteger ("Etat", 1);
		Debug.LogWarning ("L'oeuf #" + GetID() + " est bien pourri");
	}

	public void	Die(bool playerAlive)
	{
		if (playerAlive)
			anim.SetInteger("Etat", 3);
		else
			anim.SetInteger("Etat", 2);
		Debug.LogWarning ("L'oeuf #" + GetID() + " est bien mort");
	}
	
	void Update(){
	}
}
