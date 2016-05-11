using UnityEngine;
using System.Collections;

public class RefreshBorderMap : MonoBehaviour {
	
	private GameObject			[,]variant_ressources;
	private GenerateMap			scriptMap;
	private int					height;
	private int					width;

	// Use this for initialization
	void Start () {
		scriptMap = GameObject.Find("GenerateMap").GetComponent<GenerateMap>();
		height = scriptMap.height;
		width = scriptMap.width;
	}
	
	// Update is called once per frame
	void Update () {
	
	}
}
