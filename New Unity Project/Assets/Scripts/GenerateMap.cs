using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class GenerateMap : MonoBehaviour {

	public int 					height;
	public int					width;
	public GameObject			tile;
	private List<GameObject>	_tiles = new List<GameObject>();
	// Use this for initialization
	void Start () {
		Generate(height, width);
		CenterCamera(height, width);
	}

	void CenterCamera(int h, int w) {
		float x = (w * tile.transform.localScale.x / 2 * 10) - (tile.transform.localScale.x * 10 / 2);
		GameObject cam = GameObject.Find("Main Camera");

		cam.transform.position = new Vector3(x, 10, 0);
	}

	void Generate(int h, int w) {
		Vector3		vec = Vector3.zero;
		float		lenght = tile.transform.localScale.x * 10;

		for (float x = 0.0f; x < h * lenght; x += lenght)
		{
			for (float z = 0.0f; z < w * lenght; z += lenght)
			{
				vec.x = x;
				vec.z = z;
				_tiles.Add(GameObject.Instantiate(tile, vec, Quaternion.identity) as GameObject);
			}
		}
	}

	// Update is called once per frame
	void Update () {
	
	}
}
