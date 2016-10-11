using UnityEngine;
using System.Collections;

public class Player : MonoBehaviour {

	public int							id;
	public int							id_mom = 0;
	public int							pos_x;
	public int							pos_y;
	public int							orientation;
	public int							level;
	public string						teamName;
	public int							[]inventory;

	// Use this for initialization
	void Start () {
		inventory = new int[7];
	}
	
	// Update is called once per frame
	void Update () {
	
	}
}
