using UnityEngine;
using System.Collections;

public class RefreshBorderMap : MonoBehaviour {
	
//	private GameObject			[,]variant_ressources;
//	private int					height;
//	private int					width;
	public int					lineMin;
	public int					lineMax;
	public int					columnMin;
	public int					columnMax;
	public bool					update = false;

	private GameObject			[,]tilesBorder;
	private GameObject			[,,]RessrcBorder;
	private GenerateMap			scriptMap;

	// Use this for initialization
	void	Start () {
		scriptMap = GameObject.Find("GenerateMap").GetComponent<GenerateMap>();

//		height = scriptMap.height;
//		width = scriptMap.width;
	}

	int		NumberMaxChild() {
		int		nbrChild = 0;

		for (int z = columnMin; z < columnMax; z++)
		{
			for (int x = lineMin; x < lineMax; x++)
			{
				if (nbrChild < scriptMap.tiles[z, x].transform.childCount)
					nbrChild = scriptMap.tiles[z, x].transform.childCount;
			}
		}
		return (nbrChild);
	}

	void	StockVariantRessourcesInit() {
		int i = 0;

		tilesBorder = new GameObject[columnMax - columnMin, lineMax - lineMin];
		int nbrchild = NumberMaxChild();
		RessrcBorder = new GameObject[columnMax - columnMin, lineMax - lineMin, nbrchild + 1];
		for (int z = columnMin; z < columnMax; z++)
		{
			int j = 0;
			for (int x = lineMin; x < lineMax; x++)
			{
				tilesBorder[i, j] = scriptMap.tiles[z, x];

				for (int child = 0; child < scriptMap.tiles[z, x].transform.childCount; child++)
				{
					Debug.Log(tilesBorder[i, j]);
					Debug.Log(scriptMap.tiles[z, x].transform.GetChild(child));
					RessrcBorder[z, x, child] 
					= scriptMap.tiles[z, x].transform.GetChild(child).gameObject;
					Debug.Log(child);
				}

				j++;
			}
			i++;
		}
	}
	
	// Update is called once per frame
	void	Update () {
		if (update || Input.anyKeyDown)
		{
			StockVariantRessourcesInit();
		}	
	}


}
