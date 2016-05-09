using UnityEngine;
using System.Collections;

public class TerrainGenerator : MonoBehaviour {

	//Terrain data settings
	public int					mapHeightMapSize = 513; //Higher number will create more detailed height maps
	public int					mapAlphaMapSize = 1024; //This is the control map that controls how the splat textures will be blended
	public int					mapDetailMapSize = 512; //Resolutions of detail (Grass) layers
	public int					mapTerrainSize = 2048;
	public int					mapTerrainHeight = 512;
	//Terrain settings
	public int					terrainTilesX = 2; //Number of terrain tiles on the x axis
	public int					terrainTilesZ = 2; //Number of terrain tiles on the z axis
	public float				terrainPixelMapError = 6.0f; //A lower pixel error will draw terrain at a higher Level of detail but will be slower
	public float				terrainBaseMapDist = 1000.0f; //The distance at which the low res base map will be drawn. Decrease to increase performance
	//Prototypes
	public Texture2D			prototypeSplat0;
	public Texture2D			prototypeSplat1;
	public float				prototypeSplatTileSize0 = 10.0f;
	public float				prototypeSplatTileSize1 = 2.0f;
	public Texture2D			prototypeDetail0;
	public Texture2D			prototypeDetail1;
	public Texture2D			prototypeDetail2;
	public GameObject			prototypeTree0;
	public GameObject			prototypeTree1;
	public GameObject			prototypeTree2;
	//Detail settings
	public DetailRenderMode		detailMode;
	public int					detailObjectDistance = 400; //The distance at which details will no longer be drawn
	public float				detailObjectDensity = 4.0f; //Creates more dense details within patch
	public int					detailResolutionPerPatch = 32; //The size of detail patch. A higher number may reduce draw calls as details will be batch in larger patches
	public Color				detailGrassHealthyColor = Color.white;
	public Color				detailGrassDryColor = Color.white;
	public float				detailWavingGrassStrength = 0.4f;
	public float				detailWavingGrassAmount = 0.2f;
	public float				detailWavingGrassSpeed = 0.4f;
	public Color				detailWavingGrassTint = Color.white;
	//Frequence Noise settings. A higher frq will create larger scale details.
	public float				noiseGroundFrq = 800.0f;
	public float				noiseMountainFrq = 1200.0f;
	public float				noiseTreeFrq = 400.0f;
	public float				noiseDetailFrq = 100.0f;
	//Tree settings
	public int					treeSpacing = 32; //spacing between trees
	public float				treeDistance = 2000.0f; //The distance at which trees will no longer be drawn
	public float				treeBillboardDistance = 400.0f; //The distance at which trees meshes will turn into tree billboards
	public float				treeCrossFadeLength = 20.0f; //As trees turn to billboards there transform is rotated to match the meshes, a higher number will make this transition smoother
	public int					treeMaximumFullLODCount = 400; //The maximum number of trees that will be drawn in a certain area. 
	
	//Private
	private Terrain[,]			_terrain;
	private Vector2 			_centerTerrain;
	// Noise Objects
	private Noise				_groundNoise;
	private Noise				_mountainNoise;
	private Noise				_treeNoise;
	private Noise				_detailNoise;
	// Prototype Attributes
	private SplatPrototype[]	_splatPrototypes;
	private TreePrototype[]		_treeProtoTypes;
	private DetailPrototype[]	_detailProtoTypes;

	private void _IsPowerOfTwo()
	{
		if(!Mathf.IsPowerOfTwo(mapHeightMapSize - 1))
		{
			Debug.Log("TerrainGenerator::_IsPowerOfTwo - height map size must be pow2 + 1 number");
			mapHeightMapSize = Mathf.ClosestPowerOfTwo(mapHeightMapSize) + 1;
		}
		if(!Mathf.IsPowerOfTwo(mapAlphaMapSize))
		{
			Debug.Log("TerrainGenerator::_IsPowerOfTwo - Alpha map size must be pow2 number");
			mapAlphaMapSize = Mathf.ClosestPowerOfTwo(mapAlphaMapSize);
		}
		if(!Mathf.IsPowerOfTwo(mapDetailMapSize))
		{
			Debug.Log("TerrainGenerator::_IsPowerOfTwo - Detail map size must be pow2 number");
			mapDetailMapSize = Mathf.ClosestPowerOfTwo(mapDetailMapSize);
		}
		if(detailResolutionPerPatch < 8)
		{
			Debug.Log("TerrainGenerator::_IsPowerOfTwo - Detail resolution per patch must be >= 8, changing to 8");
			detailResolutionPerPatch = 8;
		}
	}

	private void _CreateProtoTypes()
	{
		//Ive hard coded 2 splat prototypes, 3 tree prototypes and 3 detail prototypes.
		//This is a little inflexible way to do it but it made the code simpler and can easly be modified 

		//SplatPrototype
		_splatPrototypes = new SplatPrototype[2];
		
		_splatPrototypes[0] = new SplatPrototype();
		_splatPrototypes[0].texture = prototypeSplat0;
		_splatPrototypes[0].tileSize = new Vector2(prototypeSplatTileSize0, prototypeSplatTileSize0);
		
		_splatPrototypes[1] = new SplatPrototype();
		_splatPrototypes[1].texture = prototypeSplat1;
		_splatPrototypes[1].tileSize = new Vector2(prototypeSplatTileSize1, prototypeSplatTileSize1);

		//TreePrototype
		_treeProtoTypes = new TreePrototype[3];
		
		_treeProtoTypes[0] = new TreePrototype();
		_treeProtoTypes[0].prefab = prototypeTree0;
		
		_treeProtoTypes[1] = new TreePrototype();
		_treeProtoTypes[1].prefab = prototypeTree1;
		
		_treeProtoTypes[2] = new TreePrototype();
		_treeProtoTypes[2].prefab = prototypeTree2;

		//DetailPrototype
		_detailProtoTypes = new DetailPrototype[3];

		_detailProtoTypes[0] = new DetailPrototype();
		_detailProtoTypes[0].prototypeTexture = prototypeDetail0;
		_detailProtoTypes[0].renderMode = detailMode;
		_detailProtoTypes[0].healthyColor = detailGrassHealthyColor;
		_detailProtoTypes[0].dryColor = detailGrassDryColor;
		
		_detailProtoTypes[1] = new DetailPrototype();
		_detailProtoTypes[1].prototypeTexture = prototypeDetail1;
		_detailProtoTypes[1].renderMode = detailMode;
		_detailProtoTypes[1].healthyColor = detailGrassHealthyColor;
		_detailProtoTypes[1].dryColor = detailGrassDryColor;
		
		_detailProtoTypes[2] = new DetailPrototype();
		_detailProtoTypes[2].prototypeTexture = prototypeDetail2;
		_detailProtoTypes[2].renderMode = detailMode;
		_detailProtoTypes[2].healthyColor = detailGrassHealthyColor;
		_detailProtoTypes[2].dryColor = detailGrassDryColor;
	}

	private void	_FillDetailMap(Terrain terrainData, int tileX, int tileZ)
	{
		//each layer is drawn separately so if you have a lot of layers your draw calls will increase 
		int[,]	detailMap0 = new int[mapDetailMapSize, mapDetailMapSize];
		int[,]	detailMap1 = new int[mapDetailMapSize, mapDetailMapSize];
		int[,]	detailMap2 = new int[mapDetailMapSize, mapDetailMapSize];
		float	ratio = (float)mapTerrainSize / (float)mapDetailMapSize;
		Random.seed = 0;
		float	unit;
		float	normX;
		float	normZ;
		float	angle;
		float	frac;
		float	worldPosX;
		float	worldPosZ;
		float	noise;
		float	rnd;
		
		for (int x = 0; x < mapDetailMapSize; x++) 
		{
			for (int z = 0; z < mapDetailMapSize; z++) 
			{
				detailMap0[z,x] = 0;
				detailMap1[z,x] = 0;
				detailMap2[z,x] = 0;
				unit = 1.0f / (mapDetailMapSize - 1);
				normX = x * unit;
				normZ = z * unit;
				// Get the steepness value at the normalized coordinate.
				angle = terrainData.terrainData.GetSteepness(normX, normZ);
				// Steepness is given as an angle, 0..90 degrees. Divide
				// by 90 to get an alpha blending value in the range 0..1.
				frac = angle / 90.0f;
				if (frac < 0.5f)
				{
					worldPosX = (x + tileX * (mapDetailMapSize - 1)) * ratio;
					worldPosZ = (z + tileZ * (mapDetailMapSize - 1)) * ratio;
					noise = _detailNoise.FractalNoise2D(worldPosX, worldPosZ, 3, noiseDetailFrq, 1.0f);
					if (noise > 0.0f) 
					{
						rnd = Random.value;
						//Randomly select what layer to use
						if (rnd < 0.33f)
							detailMap0[z,x] = 1;
						else if (rnd < 0.66f)
							detailMap1[z,x] = 1;
						else
							detailMap2[z,x] = 1;
					}
				}
			}
		}
		terrainData.terrainData.wavingGrassStrength = detailWavingGrassStrength;
		terrainData.terrainData.wavingGrassAmount = detailWavingGrassAmount;
		terrainData.terrainData.wavingGrassSpeed = detailWavingGrassSpeed;
		terrainData.terrainData.wavingGrassTint = detailWavingGrassTint;
		terrainData.detailObjectDensity = detailObjectDensity;
		terrainData.detailObjectDistance = detailObjectDistance;
		terrainData.terrainData.SetDetailResolution(mapDetailMapSize, detailResolutionPerPatch);
		terrainData.terrainData.SetDetailLayer(0, 0, 0, detailMap0);
		terrainData.terrainData.SetDetailLayer(0, 0, 1, detailMap1);
		terrainData.terrainData.SetDetailLayer(0, 0, 2, detailMap2);
	}

	private void	_FillTreeInstances(Terrain terrainData, int tileX, int tileZ)
	{
		Random.seed = 0;
		float			unit;
		float			offsetX;
		float			offsetZ;
		float			normX;
		float			normZ;
		float			angle;
		float			frac;
		float			worldPosX;
		float			worldPosZ;
		float			noise;
		float			ht;
		TreeInstance	temp;
		
		for (int x = 0; x < mapTerrainSize; x += treeSpacing) 
		{
			for (int z = 0; z < mapTerrainSize; z += treeSpacing) 
			{
				unit = 1.0f / (mapTerrainSize - 1);
				offsetX = Random.value * unit * treeSpacing;
				offsetZ = Random.value * unit * treeSpacing;
				normX = x * unit + offsetX;
				normZ = z * unit + offsetZ;
				// Get the steepness value at the normalized coordinate.
				angle = terrainData.terrainData.GetSteepness(normX, normZ);
				// Steepness is given as an angle, 0..90 degrees. Divide
				// by 90 to get an alpha blending value in the range 0..1.
				frac = angle / 90.0f;
				
				if (frac < 0.5f) //make sure tree are not on steep slopes
				{
					worldPosX = x + tileX * (mapTerrainSize - 1);
					worldPosZ = z + tileZ * (mapTerrainSize - 1);
					
					noise = _treeNoise.FractalNoise2D(worldPosX, worldPosZ, 3, noiseTreeFrq, 1.0f);
					ht = terrainData.terrainData.GetInterpolatedHeight(normX, normZ);
					
					if (noise > 0.0f && ht < mapTerrainHeight * 0.4f)
					{
						temp = new TreeInstance();
						temp.position = new Vector3(normX,ht,normZ);
						temp.prototypeIndex = Random.Range(0, 3);
						temp.widthScale = 1;
						temp.heightScale = 1;
						temp.color = Color.white;
						temp.lightmapColor = Color.white;
						terrainData.AddTreeInstance(temp);
					}
				}
			}
		}
		terrainData.treeDistance = treeDistance;
		terrainData.treeBillboardDistance = treeBillboardDistance;
		terrainData.treeCrossFadeLength = treeCrossFadeLength;
		terrainData.treeMaximumFullLODCount = treeMaximumFullLODCount;
	}
	
	private void	_FillAlphaMap(TerrainData terrainData)
	{
		float[,,] map  = new float[mapAlphaMapSize, mapAlphaMapSize, 2];
		Random.seed = 0;
		float normX;
		float normZ;
		float angle;
		float frac;
		
		for(int x = 0; x < mapAlphaMapSize; x++) 
		{
			for (int z = 0; z < mapAlphaMapSize; z++) 
			{
				// Get the normalized terrain coordinate that
				// corresponds to the the point.
				normX = x * 1.0f / (mapAlphaMapSize - 1);
				normZ = z * 1.0f / (mapAlphaMapSize - 1);
				// Get the steepness value at the normalized coordinate.
				angle = terrainData.GetSteepness(normX, normZ);
				// Steepness is given as an angle, 0..90 degrees. Divide
				// by 90 to get an alpha blending value in the range 0..1.
				frac = angle / 90.0f;
				map[z, x, 0] = frac;
				map[z, x, 1] = 1.0f - frac;
			}
		}
		terrainData.alphamapResolution = mapAlphaMapSize;
		terrainData.SetAlphamaps(0, 0, map);
	}
	
	private void	_FillHeights(float [,]heightmap, int tileX, int tileZ)
	{
		float ratio = (float)mapTerrainSize / (float)mapHeightMapSize;
		float worldPosX;
		float worldPosZ;
		float mountains;
		float plain;

		for(int x = 0; x < mapHeightMapSize; x++)
		{
			for(int z = 0; z < mapHeightMapSize; z++)
			{
				worldPosX = (x + tileX * (mapHeightMapSize - 1)) * ratio;
				worldPosZ = (z + tileZ * (mapHeightMapSize - 1)) * ratio;
				mountains = Mathf.Max(0.0f, _mountainNoise.FractalNoise2D(worldPosX, worldPosZ, 6, noiseMountainFrq, 0.8f));
				plain = _groundNoise.FractalNoise2D(worldPosX, worldPosZ, 4, noiseGroundFrq, 0.1f) + 0.1f;
				heightmap[z,x] = plain + mountains;
			}
		}
	}

	private void	_ConfigTerrainData()
	{
		float[,] heightMap = new float[mapHeightMapSize, mapHeightMapSize];

		for(int x = 0; x < terrainTilesX; x++)
		{
			for(int z = 0; z < terrainTilesZ; z++)
			{
				_FillHeights(heightMap, x, z);
				
				TerrainData terrainData = new TerrainData();
				
				terrainData.heightmapResolution = mapHeightMapSize;
				terrainData.SetHeights(0, 0, heightMap);
				terrainData.size = new Vector3(mapTerrainSize, mapTerrainHeight, mapTerrainSize);
				terrainData.splatPrototypes = _splatPrototypes;
				terrainData.treePrototypes = _treeProtoTypes;
				terrainData.detailPrototypes = _detailProtoTypes;
				
				_FillAlphaMap(terrainData);
				
				_terrain[x,z] = Terrain.CreateTerrainGameObject(terrainData).GetComponent<Terrain>();
				_terrain[x,z].transform.position = new Vector3(mapTerrainSize * x + _centerTerrain.x, 0, mapTerrainSize * z + _centerTerrain.y);
				_terrain[x,z].heightmapPixelError = terrainPixelMapError;
				_terrain[x,z].basemapDistance = terrainBaseMapDist;
				
				//disable this for better frame rate
				_terrain[x,z].castShadows = false;
				
				_FillTreeInstances(_terrain[x,z], x, z);
				_FillDetailMap(_terrain[x,z], x, z);
			}
		}
	}

	private void	_RemoveNeighboursTerrain()
	{
		Terrain right;
		Terrain left;
		Terrain bottom;
		Terrain top;

		for(int x = 0; x < terrainTilesX; x++)
		{
			for(int z = 0; z < terrainTilesZ; z++)
			{
				right = null;
				left = null;
				bottom = null;
				top = null;				
				if(x > 0)
					left = _terrain[(x - 1), z];
				if(x < terrainTilesX - 1)
					right = _terrain[(x + 1), z];
				if(z > 0)
					bottom = _terrain[x, (z - 1)];
				if(z < terrainTilesZ - 1)
					top = _terrain[x, (z + 1)];
				_terrain[x,z].SetNeighbors(left, top, right, bottom);
			}
		}
	}

	// Use this for initialization
	void Start () {
		_groundNoise = new Noise(0);
		_mountainNoise = new Noise(1);
		_treeNoise = new Noise(2);
		_detailNoise = new Noise(3);
		_IsPowerOfTwo();
		_terrain = new Terrain[terrainTilesX, terrainTilesZ];
		//this will center terrain at origin
		_centerTerrain = new Vector2(-mapTerrainSize * terrainTilesX * 0.5f, -mapTerrainSize * terrainTilesZ * 0.5f);
		_CreateProtoTypes();
		_ConfigTerrainData ();
		_RemoveNeighboursTerrain ();
	}
	
	// Update is called once per frame
	void Update () {
	
	}
}
