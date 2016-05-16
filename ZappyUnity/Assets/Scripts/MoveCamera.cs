using UnityEngine;
using System.Collections;

public class MoveCamera : MonoBehaviour {

	public float		speed;
	public GameObject	cameraBorder;

	private GenerateMap	scriptmap;
	private float		tile_x;
	private float		tile_z;
	private int			width;
	private int			height;
	private GameObject	[]cloneBorderCamera;
	private Vector3		[]PosCameras;
	
	void	CenterCamera() {
		float x = (width * scriptmap.tile.transform.localScale.x * 10 / 2 ) - (scriptmap.tile.transform.localScale.x * 10 / 2);
		transform.position = new Vector3(x, transform.position.y, transform.position.z);
	}

	// Use this for initialization
	void Start () {
		cloneBorderCamera = new GameObject[6];
		scriptmap = GameObject.Find("GenerateMap").GetComponent<GenerateMap>();
		tile_x = scriptmap.tile.transform.localScale.x;
		tile_z = scriptmap.tile.transform.localScale.z;
		height = scriptmap.height;
		width = scriptmap.width;
		GenerateCameras();
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

		if (transform.position.z > tile_z - 10 / 2)
			transform.position = new Vector3(transform.position.x, transform.position.y, -height * 10);
		if (transform.position.z < -height * 10)
			transform.position = new Vector3(transform.position.x, transform.position.y, tile_z - 10 / 2);
	}

	void GenerateCameras()
	{
		Vector3		rotate = new Vector3(40, 0, 0);

		PosCameras = new Vector3[6];
		PosCameras[0] = Vector3.up * 20;
		PosCameras[1] = new Vector3(width * -10, 20, 0);
		PosCameras[2] = new Vector3(width * 10, 20, 0);
		PosCameras[3] = new Vector3(width * -10, 20, height * -10);
		PosCameras[4] = new Vector3(0, 20, height * -10);
		PosCameras[5] = new Vector3(width * 10, 20, height * -10);
		
		for (int cam = 0; cam < cloneBorderCamera.Length; cam++)
		{
			cloneBorderCamera[cam] = GameObject.Instantiate(cameraBorder, PosCameras[cam], Quaternion.Euler(rotate)) as GameObject;
			cloneBorderCamera[cam].transform.SetParent(this.transform);
			if (cam > 0)
				cloneBorderCamera[cam].GetComponent<AudioListener>().enabled = false;
			switch (cam)
			{
			case 1 : cloneBorderCamera[cam].GetComponent<Camera>().depth = -1;break;
			case 2 : cloneBorderCamera[cam].GetComponent<Camera>().depth = -1;break;
			case 3 : cloneBorderCamera[cam].GetComponent<Camera>().depth = -3;break;
			case 4 : cloneBorderCamera[cam].GetComponent<Camera>().depth = -2;break;
			case 5 : cloneBorderCamera[cam].GetComponent<Camera>().depth = -3;break;
			}
		}
		cloneBorderCamera[0].GetComponent<Camera>().depth = 0;
	}
}
