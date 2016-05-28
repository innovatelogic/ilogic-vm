

/*****************************************************************

  This is just a header that contains all our vertex attribute and weight
  classes

******************************************************************/

#ifndef __BONESDEF_VERTEXWEIGHTS__H
#define __BONESDEF_VERTEXWEIGHTS__H




//this contains our vertex weight info
//basically it contains a index into the bones list, the weight and bunch of optional spline parameters
class VertexInfluenceListClass
{
public:
	int Bones;						// this is an index into the bones list,which bones belongs to the weight
    float Influences;				// this is the unnormalized weight for the Bones
	float normalizedInfluences;		// this is the normalized weight for Bones

//extra data to hold spline stuff
// this contains info on the closest point on the spline to the vertex
// the data is based on the initial spline position
    int SubCurveIds;				// this which curve the point is closest to NOTE NOT USED YET
    int SubSegIds;					// this is the closest seg
    float u;						// this is the U value along the segment which is the closest point to the vertex
    Point3 Tangents;				// this is the tangent
    Point3 OPoints;					// this is the point in spline space

	VertexInfluenceListClass()
		{
		Bones = 0;          
		Influences = 0.0f;  
		normalizedInfluences = 0.0f;
//extra data to hold spline stuff
		SubCurveIds = 0;
		SubSegIds=0;
		u=0.0f;
		Tangents = Point3(1.0f,0.0f,0.0f);
		OPoints = Point3(0.0f,0.0f,0.0f);
		}
};

// this is the per vertex data class
// it contains all the attributes of the vertex and a list of weights for this vertex
class VertexListClass
{
public:
	DWORD	flags; 

// These are get/sets for our flag properties
// the properties are
// Modified		whether the vertex has been hand weighted
// Unormalized  whether the vertex is normalized
// Rigid		whether the vertex is rigid,if it is rigid only one bone will be affect the vertex
// Rigid Handle only applies to patches, when set if it is a handle it will use the weights of the knot that owns the handle
// TempSelected	used internally to hold temporary selections for cut and paste
	inline BOOL IsSelectedTemp()  
		{
		if (flags & VERTEXFLAG_TEMPSELECTED) return TRUE;
		else return FALSE;
		}
			
	inline BOOL IsModified()
		{
		if (flags & VERTEXFLAG_MODIFIED) return TRUE;
		else return FALSE;
		}
	inline BOOL IsUnNormalized()
		{
		if (flags & VERTEXFLAG_UNNORMALIZED) return TRUE;
		else return FALSE;
		}

	inline BOOL IsRigid()
		{
		if (flags & VERTEXFLAG_RIGID) return TRUE;
		else return FALSE;
		}
	
	inline BOOL IsRigidHandle()
		{
		if (flags & VERTEXFLAG_RIGIDHANDLE) return TRUE;
		else return FALSE;
		}

	inline BOOL IsHidden()
		{
		if (flags & VERTEXFLAG_HIDDEN) return TRUE;
		else return FALSE;
		}

	inline void SelectedTemp(BOOL sel)
		{
		if (sel)
			flags |= VERTEXFLAG_TEMPSELECTED;
		else flags &= ~VERTEXFLAG_TEMPSELECTED;
		}

	inline void Modified(BOOL modify)
		{
		if (modify)
			{
			if (!IsModified() )
			{
				if (IsUnNormalized())
				{
					for (int i = 0; i < tabInfluenceList.Count(); i++)
						tabInfluenceListPtr[i].Influences =  tabInfluenceListPtr[i].normalizedInfluences;
				}
				else
				{
					NormalizeWeights();
				}
				
			}
			flags |= VERTEXFLAG_MODIFIED;
			}
		else flags &= ~VERTEXFLAG_MODIFIED;
		}
	
	inline void UnNormalized(BOOL unnormalized)
		{
		if (unnormalized)
			{
			flags |= VERTEXFLAG_UNNORMALIZED;
			if (!IsModified())
				{
				Modified(TRUE);					
				}
			}
		else 
			{
			BOOL wasNormal =  !(flags  | VERTEXFLAG_UNNORMALIZED);
			flags &= ~VERTEXFLAG_UNNORMALIZED;
			if (wasNormal)
				Modified(TRUE);
				
			}
		}
	inline void Rigid(BOOL rigid)
		{
		if (rigid)
			flags |= VERTEXFLAG_RIGID;
		else flags &= ~VERTEXFLAG_RIGID;
		}

	inline void RigidHandle(BOOL rigidHandle)
		{
		if (rigidHandle)
			flags |= VERTEXFLAG_RIGIDHANDLE;
		else flags &= ~VERTEXFLAG_RIGIDHANDLE;
		}

	inline void Hide(BOOL hide)
		{
		if (hide)
			flags |= VERTEXFLAG_HIDDEN;
		else flags &= ~VERTEXFLAG_HIDDEN;
		}

    Point3 LocalPos;				//this is local position of the vertex before any skin deformation
	Point3 LocalPosPostDeform;		//this is local position of the vertex before after skin deformation

	//table of misc data    

	VertexListClass()
		{
		flags = 0; 
		closestBone = -1;
		closestBoneCache = -1;
		closetWeight = 1.0f;
		Modified (FALSE);		
		LocalPos = Point3(0.0f,0.0f,0.0f);
		tabInfluenceListPtr = NULL;
		}

	VertexListClass(VertexListClass& from)
	{
		flags = from.flags;
		LocalPos = from.LocalPos;
		LocalPosPostDeform = from.LocalPosPostDeform;
		closestBone = from.closestBone;
		closestBoneCache = from.closestBoneCache;
		closetWeight = from.closetWeight;
		tabInfluenceList = from.tabInfluenceList;
		UpdateInfluenceAddress();
	}

	VertexListClass& operator=(VertexListClass&rhs)
	{
		flags = rhs.flags;
		LocalPos = rhs.LocalPos;
		LocalPosPostDeform = rhs.LocalPosPostDeform;
		closestBone = rhs.closestBone;
		closestBoneCache = rhs.closestBoneCache;
		closetWeight = rhs.closetWeight;
		tabInfluenceList = rhs.tabInfluenceList;
		UpdateInfluenceAddress();
		return *this;
	}

	//this returns the bone that most affects this vertex
	int GetMostAffectedBone()
		{
		if ((tabInfluenceList.Count() == 0) && (closestBone != -1))
			return closestBone;

		if (tabInfluenceList.Count() == 0) return -1;

		int largestID = tabInfluenceListPtr[0].Bones;
		float largestVal = tabInfluenceListPtr[0].Influences;
		for (int i = 1; i < (tabInfluenceList.Count()); i++)
			{
			for (int j = i; j < tabInfluenceList.Count(); j++)
				{
				if (tabInfluenceListPtr[j].Influences > largestVal)
					{
					largestVal = tabInfluenceListPtr[j].Influences;
					largestID = tabInfluenceListPtr[j].Bones;
					}
				}
			}
		return largestID;

		}

	//this returns the ith index of the bone that most affects this vertex
	int GetMostAffectedBoneID()
		{
		if ((tabInfluenceList.Count() == 0) && (closestBone != -1))
			return 0;

		if (tabInfluenceList.Count() == 0) return -1;
		int largestID = 0;
		float largestVal = tabInfluenceListPtr[0].Influences;
		for (int i = 1; i < (tabInfluenceList.Count()); i++)
			{
			for (int j = i; j < tabInfluenceList.Count(); j++)
				{
				if (tabInfluenceListPtr[j].Influences > largestVal)
					{
					largestVal = tabInfluenceListPtr[j].Influences;
					largestID = j;
					}
				}
			}
		return largestID;

		}

	//this loops though the unnormalized weights 
	// and stuffs the normalized values in the normalizedInfluences variable
	void NormalizeWeights()
		{
		float sum = 0.0f;
		for (int i = 0; i < tabInfluenceList.Count(); i++)
			sum +=  tabInfluenceListPtr[i].Influences;

		for ( int i = 0; i < tabInfluenceList.Count(); i++)
			{
			if (sum == 0.0f)
				tabInfluenceListPtr[i].Influences = 0.0f;
			else tabInfluenceListPtr[i].Influences = tabInfluenceListPtr[i].Influences/sum ;
			tabInfluenceListPtr[i].normalizedInfluences = tabInfluenceListPtr[i].Influences;
			}

		}	

		int WeightCount() 
		{ 
			if ((tabInfluenceList.Count() == 0) && (closestBone != -1))
				return 1;
			return tabInfluenceList.Count(); 
		}
		void SetWeightCount(int ct) 
		{ 
			tabInfluenceList.SetCount(ct); 
			UpdateInfluenceAddress();
		}
		void ZeroWeights() 
		{ 
			tabInfluenceList.ZeroCount(); 
			UpdateInfluenceAddress();
		}

		int GetBoneIndex(int index) 
		{ 
			if ((tabInfluenceList.Count() == 0) && (closestBone != -1))
				return closestBone;
			return tabInfluenceListPtr[index].Bones; 
		}
		void SetBoneIndex(int index, int boneID) 
		{ 
			if (index < tabInfluenceList.Count())
				tabInfluenceListPtr[index].Bones = boneID; 
		}

		float GetNormalizedWeight(int index) 
		{ 
			if ((tabInfluenceList.Count() == 0) && (closestBone != -1))
				return 1.0f;
			return tabInfluenceListPtr[index].normalizedInfluences; 
		}
		float GetWeight(int index) 
		{ 
			if ((tabInfluenceList.Count() == 0) && (closestBone != -1))
				return 1.0f;
			return tabInfluenceListPtr[index].Influences; 
		}
		void SetWeight(int index,float w) 
		{ 
			if (index < tabInfluenceList.Count())
				tabInfluenceListPtr[index].Influences = w; 
			else if (closestBone != -1)
			{
				VertexInfluenceListClass vd;
				vd.Bones = closestBone;
				vd.normalizedInfluences = w;
				vd.Influences = w;
				AppendWeight(vd);
			}
		}
		void SetNormalizedWeight(int index,float w) 
		{ 
			if (index < tabInfluenceList.Count())
				tabInfluenceListPtr[index].normalizedInfluences = w; 
/*
			else if (closestBone != -1)
			{
				VertexInfluenceListClass vd;
				vd.Bones = closestBone;
				vd.normalizedInfluences = w;
				vd.Influences = w;
				AppendWeight(vd);
			}
*/

		}


		void SetWeightInfo(int index,int boneIndex, float w, float nw) 
		{ 

			if (index < tabInfluenceList.Count())
			{
				tabInfluenceListPtr[index].Bones = boneIndex;
				tabInfluenceListPtr[index].Influences = w; 
				tabInfluenceListPtr[index].normalizedInfluences = nw; 
			}
		}
		float GetCurveU(int index) 
		{ 
			if (index >= tabInfluenceList.Count())
				return 0.0f;
			return tabInfluenceListPtr[index].u; 
		}
		int GetCurveID(int index) 
		{ 
			if (index >= tabInfluenceList.Count())
				return 0;
			return tabInfluenceListPtr[index].SubCurveIds; 
		}
		int GetSegID(int index) 
		{ 
			if (index >= tabInfluenceList.Count())
				return 0;
			return tabInfluenceListPtr[index].SubSegIds; 
		}
		Point3 GetOPoint(int index) 
		{ 
			if (index >= tabInfluenceList.Count())
				return Point3(0.0f,0.0f,0.0f);
			return tabInfluenceListPtr[index].OPoints; 
		}
		Point3 GetTangent(int index) 
		{ 
			if (index >= tabInfluenceList.Count())
				return Point3(0.0f,0.0f,0.0f);
			return tabInfluenceListPtr[index].Tangents; 
		}

		void SetCurveU(int index, float u) 
		{ 
			if (index < tabInfluenceList.Count())
				tabInfluenceListPtr[index].u = u; 
		}
		void SetCurveID(int index, int id) 
		{ 
			if (index < tabInfluenceList.Count())
				tabInfluenceListPtr[index].SubCurveIds = id; 
		}
		void SetSegID(int index, int id) 
		{ 
			if (index < tabInfluenceList.Count())
				tabInfluenceListPtr[index].SubSegIds = id; 
		}
		void SetOPoint(int index, const Point3& p) 
		{ 
			if (index < tabInfluenceList.Count())
				tabInfluenceListPtr[index].OPoints = p; 
		}
		void SetTangent(int index, const Point3& p) 
		{ 
			if (index < tabInfluenceList.Count())
				tabInfluenceListPtr[index].Tangents = p; 
		}


		void SetWeightSplineInfo(int index,float u, int curve, int seg, const Point3& p, const Point3& t) 
		{ 
			if (index < tabInfluenceList.Count())
			{
				tabInfluenceListPtr[index].u = u;
				tabInfluenceListPtr[index].SubCurveIds = curve; 
				tabInfluenceListPtr[index].SubSegIds = seg; 
				tabInfluenceListPtr[index].OPoints = p;
				tabInfluenceListPtr[index].Tangents = t;
			}
		}

		void DeleteWeight(int index) 
		{ 
			if (index < tabInfluenceList.Count())
				tabInfluenceList.Delete(index,1); 
			UpdateInfluenceAddress();
		}
		void AppendWeight(VertexInfluenceListClass& w) 
		{ 
			tabInfluenceList.Append(1,&w,4);
			UpdateInfluenceAddress();
		}
		void Sort();

		VertexInfluenceListClass CopySingleWeight(int index) 
		{ 
			if (index < tabInfluenceList.Count())
				return tabInfluenceListPtr[index]; 
			else
			{
				VertexInfluenceListClass td;
				td.Influences = 1.0f;
				td.normalizedInfluences = 1.0f;
				td.Bones = closestBone;
				return td;
			}
		}
		void PasteSingleWeight(int index,VertexInfluenceListClass& td ) 
		{ 
			if (index < tabInfluenceList.Count())
				tabInfluenceListPtr[index] = td; 
			else if (tabInfluenceList.Count() == 0) 
				tabInfluenceList.Append(1,&td);
			UpdateInfluenceAddress();
		}

		Tab<VertexInfluenceListClass> CopyWeights() { return tabInfluenceList; }
		void PasteWeights(Tab<VertexInfluenceListClass>& w) 
		{ 
			tabInfluenceList = w; 
			UpdateInfluenceAddress();
		}

		//THESE should not be used and only are available for the com engine
		int* GetBoneIndexAddr(int index) { return &tabInfluenceListPtr[index].Bones; }
		float* GetNormalizedWeightAddr(int index) 
		{ 
			if ((tabInfluenceList.Count() == 0) && (closestBone != -1))
				return &closetWeight;
			return &tabInfluenceListPtr[index].normalizedInfluences; 
		}
		float* GetCurveUAddr(int index) { return &tabInfluenceListPtr[index].u; }
		int* GetCurveIDAddr(int index) { return &tabInfluenceListPtr[index].SubCurveIds; }
		int* GetSegIDAddr(int index) { return &tabInfluenceListPtr[index].SubSegIds; }
		Point3* GetOPointAddr(int index) { return &tabInfluenceListPtr[index].OPoints; }
		Point3* GetTangentAddr(int index) { return &tabInfluenceListPtr[index].Tangents; }

		int GetClosestBone() { return closestBone; }
		void SetClosestBone(int bid) { closestBone = bid; }

		int GetClosestBoneCache() { return closestBoneCache; }
		void SetClosestBoneCache(int bid) { closestBoneCache = bid; }

		private:
			inline void UpdateInfluenceAddress()
			{
				tabInfluenceListPtr = tabInfluenceList.Count() > 0 ? tabInfluenceList.Addr(0) : nullptr;
			}
			int closestBone;
			int closestBoneCache;
			float closetWeight;			
			Tab<VertexInfluenceListClass> tabInfluenceList; //this is the table of of bones and weights that affect this vertex
			VertexInfluenceListClass* tabInfluenceListPtr;

    };


//this class is used to cache our distances
//every time a bone is selected all the distances are computed from this bone
//and stored in a table of this class
class VertexDistanceClass
{
public:
	float dist;
	float u;
    int SubCurveIds;
    int SubSegIds;
    Point3 Tangents;
    Point3 OPoints;
};




//this is a legacy class to load older files
// THIS SHOULD NOT BE CHANGED
class VertexListClassOld
	{
public:
    BOOL selected;
    BOOL modified;
    Point3 LocalPos;
//table of misc data    
    Tab<VertexInfluenceListClass> d;
	int GetMostAffectedBone()
		{
		if (d.Count() == 0) return -1;
		int largestID = d[0].Bones;
		float largestVal = d[0].Influences;
		for (int i = 1; i < (d.Count()); i++)
			{
			for (int j = i; j < d.Count(); j++)
				{
				if (d[j].Influences > largestVal)
					{
					largestVal = d[j].Influences;
					largestID = d[j].Bones;
					}
				}
			}
		return largestID;

		}
	int GetMostAffectedBoneID()
		{
		if (d.Count() == 0) return -1;
		int largestID = 0;
		float largestVal = d[0].Influences;
		for (int i = 1; i < (d.Count()); i++)
			{
			for (int j = i; j < d.Count(); j++)
				{
				if (d[j].Influences > largestVal)
					{
					largestVal = d[j].Influences;
					largestID = j;
					}
				}
			}
		return largestID;

		}

    };

class VertexWeightCopyBuffer
{
public:
	Tab<INode*> bones;
	Tab<VertexListClass *> copyBuffer;

	VertexWeightCopyBuffer()
	{
	}
	~VertexWeightCopyBuffer()
	{
		FreeCopyBuffer();
	}
	void FreeCopyBuffer()
	{
		for (int i = 0; i < copyBuffer.Count(); i++)
			delete copyBuffer[i];
	}
};

#endif