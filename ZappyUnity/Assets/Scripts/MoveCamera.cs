using UnityEngine;
using System.Collections;

public class MoveCamera : MonoBehaviour {

	public float		speed;
	private GenerateMap	scriptmap;
	private float		tile_x;
	private float		tile_z;
	private int			width;
	private int			height;
	
	void	CenterCamera() {
		float x = (width * scriptmap.tile.transform.localScale.x * 10 / 2 ) - (scriptmap.tile.transform.localScale.x * 10 / 2);
		transform.position = new Vector3(x, transform.position.y, transform.position.z);
	}

	// Use this for initialization
	void Start () {
		scriptmap = GameObject.Find("GenerateMap").GetComponent<GenerateMap>();
		tile_x = scriptmap.tile.transform.localScale.x;
		tile_z = scriptmap.tile.transform.localScale.z;
		height = scriptmap.height;
		width = scriptmap.width;
		CenterCamera();
	}
	
	// Update is called once per frame
	void Update () {
		if (Input.GetKey(KeyCode.A) || Input.GetKey(KeyCode.LeftArrow))
			transform.Translate(Vector3.left * Time.deltaTime * speed, Space.World);
		if (Input.GetKey(KeyCode.D) || Input.GetKey(KeyCode.RightArrow))
			transform.Translate(Vector3.right * Time.deltaTime * speed, Space.World);
		if (Input.GetKey(KeyCode.W) || Input.GetKey(KeyCode.UpArrow))
			transform.Translate(Vector3.forward * Time.deltaTime * speed, Space.World);
		if (Input.GetKey(KeyCode.S) || Input.GetKey(KeyCode.DownArrow))
			transform.Translate(Vector3.back * Time.deltaTime * speed, Space.World);
		if (transform.position.x < -tile_x)
			transform.position = new Vector3(width * 10 - tile_x, transform.position.y, transform.position.z);
		if (width * 10 - tile_x < transform.position.x)
			transform.position = new Vector3(-tile_x, transform.position.y, transform.position.z);

		if (transform.position.z > 0 + tile_z - 10)
			transform.position = new Vector3(transform.position.x, transform.position.y, -height * 10);
		if (transform.position.z < -height * 10)
			transform.position = new Vector3(transform.position.x, transform.position.y, + tile_z - 10);
		Debug.DrawRay(transform.position, transform.forward * 100, Color.red);
	}
}
