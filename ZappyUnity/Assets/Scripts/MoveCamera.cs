using UnityEngine;
using System.Collections;

public class MoveCamera : MonoBehaviour {

	public float		speed;
	public float		speedRot;
	public GameObject	cameraBorder;
	public GameObject	[]cloneBorderCamera;
	public GameObject	select;
	public GameObject	camLocation;

	private GenerateMap	scriptmap;
	private float		tile_x;
	private float		tile_z;
	private int			width;
	private int			height;
	private Vector3		[]PosCameras;
	private GameObject	orientationCam;
	private Camera		camMinimap;

	void	CenterCamera() {
		float x = (width * scriptmap.tile.transform.localScale.x * 10 / 2 ) - (scriptmap.tile.transform.localScale.x * 10 / 2);
		transform.position = new Vector3(x, transform.position.y, transform.position.z);
	}

	void	MiniMap() {
		GameObject minimap = GameObject.Find ("CamMiniMap");
		float x = width / 2.0f * 10.0f - 5.0f;
		float z = -height / 2.0f * 10.0f + 5.0f;
		float size;
		if (height >= width)
			size = height / 2 * 10 + 5.0f;
		else
			size = width * 4 - 10;
		minimap.transform.position = new Vector3 (x, 100.0f, z);
		camMinimap = minimap.GetComponent<Camera>();
		camMinimap.orthographicSize = size;
		camMinimap.enabled = false;
	}

	// Use this for initialization
	void Start () {
		cloneBorderCamera = new GameObject[9];
		scriptmap = GameObject.Find("GenerateMap").GetComponent<GenerateMap>();
		tile_x = scriptmap.tile.transform.localScale.x;
		tile_z = scriptmap.tile.transform.localScale.z;
		height = scriptmap.height;
		width = scriptmap.width;
		GenerateCameras();
		CenterCamera();
		MiniMap ();
	}
	
	// Update is called once per frame
	void Update () {
		if (Input.GetKeyDown(KeyCode.Tab))
			camMinimap.enabled = !camMinimap.enabled;
		if (Input.GetKey(KeyCode.A) || Input.GetKey(KeyCode.LeftArrow))
			transform.Translate(-orientationCam.transform.right * Time.deltaTime * speed, Space.World);
		if (Input.GetKey(KeyCode.D) || Input.GetKey(KeyCode.RightArrow))
			transform.Translate(orientationCam.transform.right * Time.deltaTime * speed, Space.World);
		if (Input.GetKey(KeyCode.W) || Input.GetKey(KeyCode.UpArrow))
			transform.Translate(orientationCam.transform.forward * Time.deltaTime * speed, Space.World);
		if (Input.GetKey(KeyCode.S) || Input.GetKey(KeyCode.DownArrow))
			transform.Translate(-orientationCam.transform.forward * Time.deltaTime * speed, Space.World);
		if (Input.GetKey(KeyCode.PageUp))
		{
			for (int cam = 0; cam < cloneBorderCamera.Length; cam++)
				cloneBorderCamera[cam].transform.Rotate(Vector3.up * Time.deltaTime * speedRot, Space.World);
			orientationCam.transform.Rotate(Vector3.up * Time.deltaTime * speedRot, Space.World);
		}
		if (Input.GetKey(KeyCode.PageDown))
		{
			for (int cam = 0; cam < cloneBorderCamera.Length; cam++)
				cloneBorderCamera[cam].transform.Rotate(Vector3.up * Time.deltaTime * -speedRot, Space.World);
			orientationCam.transform.Rotate(Vector3.up * Time.deltaTime * -speedRot, Space.World);
		}
		if (Input.GetKey (KeyCode.LeftShift) || Input.GetKey (KeyCode.RightShift))
			speed = 100;
		else
			speed = 50;
		if (transform.position.x < -tile_x)
			transform.position = new Vector3(width * 10 - tile_x, transform.position.y, transform.position.z);
		if (width * 10 - tile_x < transform.position.x)
			transform.position = new Vector3(-tile_x, transform.position.y, transform.position.z);
		if (transform.position.z > tile_z - 1)//0
			transform.position = new Vector3(transform.position.x, transform.position.y, -height * 10);
		if (transform.position.z < -height * 10)//150
			transform.position = new Vector3(transform.position.x, transform.position.y, 0);
	}
	
	void	GenerateCameras()
	{
		Vector3		rotate = new Vector3(40, 0, 0);


		PosCameras = new Vector3[9];
		PosCameras[0] = Vector3.up * 20;

		PosCameras[1] = new Vector3(width * -10, 20, height * -10);
		PosCameras[2] = new Vector3(0, 20, height * -10);
		PosCameras[3] = new Vector3(width * 10, 20, height * -10);

		PosCameras[4] = new Vector3(width * -10, 20, 0);
		PosCameras[5] = new Vector3(width * 10, 20, 0);

		PosCameras[6] = new Vector3(width * -10, 20, height * 10);
		PosCameras[7] = new Vector3(0, 20, height * 10);
		PosCameras[8] = new Vector3(width * 10, 20, height * 10);

		for (int cam = 0; cam < cloneBorderCamera.Length; cam++)
		{
			cloneBorderCamera[cam] = GameObject.Instantiate(cameraBorder, PosCameras[cam], Quaternion.Euler(rotate), transform) as GameObject;
			cloneBorderCamera[cam].GetComponent<SelectTile>().select = select;
			if (cam > 0)
				cloneBorderCamera[cam].GetComponent<AudioListener>().enabled = false;
			switch (cam)
			{
			case 1 : cloneBorderCamera[cam].GetComponent<Camera>().depth = -3;break;
			case 2 : cloneBorderCamera[cam].GetComponent<Camera>().depth = -2;break;
			case 3 : cloneBorderCamera[cam].GetComponent<Camera>().depth = -3;break;

			case 4 : cloneBorderCamera[cam].GetComponent<Camera>().depth = -1;break;
			case 0 : cloneBorderCamera[cam].GetComponent<Camera>().depth = 0; break;
			case 5 : cloneBorderCamera[cam].GetComponent<Camera>().depth = -1;break;

			case 6 : cloneBorderCamera[cam].GetComponent<Camera>().depth = -3;break;
			case 7 : cloneBorderCamera[cam].GetComponent<Camera>().depth = -2;break;
			case 8 : cloneBorderCamera[cam].GetComponent<Camera>().depth = -3;break;
			}
		}
		orientationCam = new GameObject("Orientation");
		orientationCam.transform.SetParent(this.transform);

		GameObject cloneLocation = GameObject.Instantiate (camLocation, Vector3.zero, Quaternion.identity, cloneBorderCamera[0].transform) as GameObject;
		cloneLocation.transform.localPosition = Vector3.zero;
	}
}
