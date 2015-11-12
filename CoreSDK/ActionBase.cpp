#include "coresdkafx.h"

REGISTER_CLASS_ACTION_EX(ActionBase, CObjectAbstract)
	new PropertyVector2f("Position", (BYTE*)&((ActionBase*)NULL)->Position - (BYTE*)NULL, "ActionBase", "Value", READ_WRITE, CTRL_EDIT, SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
END_REGISTER_CLASS_ACTION(ActionBase, CObjectAbstract);

#define CAPTION_HEIGHT 35
const int ConnectorWidth = 15;
const int ConnectorHeight = 8;

//----------------------------------------------------------------------------------------------
template <class T>
bool GetObjectIndex(const T * Object, const T * Root, int & OutIndex)
{
	bool bResult = false;

	assert(Object && Root);

	if (Object == Root){
		bResult = true;
	}
	else
	{
		// add to all parent's child
		for (size_t Index = 0; Index < Root->ChildNodes.size(); ++Index)
		{
			if (GetObjectIndex(Object, Root->ChildNodes[Index], ++OutIndex))
			{
				bResult = true;
				break;
			}
		}
	}
	return bResult;
}

//----------------------------------------------------------------------------------------------
template <class T>
T* GetObjectByIndexEx(T * Root, int IndexFind, int IndexCurrent = 0)
{
	if (IndexFind == IndexCurrent){
		return Root;
	}
	else
	{
		// add to all parent's child
		for (size_t Index = 0; Index < Root->ChildNodes.size(); ++Index)
		{
			if (T* Obj = GetObjectByIndexEx(Root->ChildNodes[Index], IndexFind, ++IndexCurrent))
			{
				return Obj;
			}
		}
	}
	return 0;
}

//----------------------------------------------------------------------------------------------
template <class T>
T* GetObjectByIndex(T * Root, int Index)
{
	return GetObjectByIndexEx(Root, Index, 0);
}

//----------------------------------------------------------------------------------------------
SocketAction::SocketAction(ActionBase * owner, ELinkDirection dir)
: Owner(owner)
, LinkSocket(NULL)
, Dir(dir)
{

}

//----------------------------------------------------------------------------------------------
SocketAction::~SocketAction()
{

}

//----------------------------------------------------------------------------------------------
void SocketAction::Render(HDC hdc, const Matrix &ViewMatrix) const
{
	Vector2f Pos = Owner->GetPosition2f();
	Vector2f Size = Owner->GetSize();

	Vector Points[2];
	Points[0].Set(Pos.x + ((Dir == ELink_In) ? -ConnectorWidth : Size.x), Pos.y + ((CAPTION_HEIGHT - ConnectorHeight) * 0.5f), 0.f);
	Points[1].Set(Pos.x + ((Dir == ELink_In) ? 0.f : (Size.x + ConnectorWidth)), Pos.y + (CAPTION_HEIGHT * 0.5f) + ConnectorHeight, 0.f);

	Vector TPoints[2];
	TPoints[0] = transform_coord(Points[0], ViewMatrix);
	TPoints[1] = transform_coord(Points[1], ViewMatrix);

	RECT rc;
	rc.left = (int)TPoints[0].x;
	rc.top = (int)TPoints[0].y;
	rc.right = (int)TPoints[1].x;
	rc.bottom = (int)TPoints[1].y;

	HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
	::FillRect(hdc, &rc, blackBrush);

	// draw links
	if (LinkSocket)
	{
		Vector2f LinkPos = LinkSocket->Owner->GetPosition2f();
		Vector2f LinkSize = LinkSocket->Owner->GetSize();

		Vector2f StartLink(Pos.x + (Size.x + ConnectorWidth), Pos.y + (CAPTION_HEIGHT * 0.5f) + ConnectorHeight * 0.5f);
		Vector2f EndLink(LinkPos.x - ConnectorWidth, LinkPos.y + (CAPTION_HEIGHT * 0.5f) + ConnectorHeight * 0.5f);

		// draw arrow
		Vector APoints[3];
		APoints[0].Set(EndLink.x, EndLink.y, 0.f);
		APoints[1].Set(EndLink.x - 15, EndLink.y - 7, 0.f);
		APoints[2].Set(EndLink.x - 15, EndLink.y + 7, 0.f);
		
		Vector TAPoints[4];
		TAPoints[0] = transform_coord(APoints[0], ViewMatrix);
		TAPoints[1] = transform_coord(APoints[1], ViewMatrix);
		TAPoints[2] = transform_coord(APoints[2], ViewMatrix);

		POINT pntArray[5] = {
			{(int)TAPoints[0].x, (int)TAPoints[0].y},
			{(int)TAPoints[1].x, (int)TAPoints[1].y},
			{(int)TAPoints[2].x, (int)TAPoints[2].y},
		};

		::Polygon(hdc, pntArray, 3);

		EndLink.x -= 15;

		Vector BPoints[4];
		BPoints[0].Set(StartLink.x, StartLink.y, 0.f);
		BPoints[1].Set(StartLink.x + fabs(EndLink.x - StartLink.x) * 0.5f, StartLink.y, 0.f);
		BPoints[2].Set(EndLink.x - fabs(EndLink.x - StartLink.x) * 0.5f, EndLink.y, 0.f);
		BPoints[3].Set(EndLink.x, EndLink.y, 0.f);

		Vector TBPoints[4];
		TBPoints[0] = transform_coord(BPoints[0], ViewMatrix);
		TBPoints[1] = transform_coord(BPoints[1], ViewMatrix);
		TBPoints[2] = transform_coord(BPoints[2], ViewMatrix);
		TBPoints[3] = transform_coord(BPoints[3], ViewMatrix);

		//Vector Start(BPoints[0].x, BPoints[0].y, 0.f);

		const size_t Detail = 10;
		MoveToEx(hdc, (int)TBPoints[0].x, (int)TBPoints[0].y, 0);

		for (size_t Index = 1; Index <= Detail; ++Index)
		{
			Vector Result;
			Cubic_Bezier_Spline(TBPoints[3], TBPoints[2], TBPoints[1], TBPoints[0], (float)Index / Detail, Result);
			LineTo(hdc, (int)Result.x, (int)Result.y);

// 			Vector N = cross(N, Start - Result, Vector(0.f, 0.f, 1.f));
// 			N.normalize();
// 			N*=5;

			//LineTo(hdc, (int)(Result.x + N.x), (int)(Result.y + N.y));
			
			//Start = Result;
		}
	}
}

//----------------------------------------------------------------------------------------------
bool SocketAction::ProcessMouseClick(const Vector2f& MousePos, bool link /*= false*/) const
{
	bool bResult = false;

	Vector2f Pos = Owner->GetPosition2f();
	Vector2f Size = Owner->GetSize();

	Vector Points[2];
	Points[0].Set(Pos.x + ((Dir == ELink_In) ? -ConnectorWidth : Size.x), Pos.y + ((CAPTION_HEIGHT - ConnectorHeight) * 0.5f), 0.f);
	Points[1].Set(Pos.x + ((Dir == ELink_In) ? 0.f : (Size.x + ConnectorWidth)), Pos.y + (CAPTION_HEIGHT * 0.5f) + ConnectorHeight, 0.f);

	if (link)
	{
		// draw links
		if (LinkSocket)
		{
			Vector2f LinkPos = LinkSocket->Owner->GetPosition2f();
			Vector2f LinkSize = LinkSocket->Owner->GetSize();

			Vector2f StartLink(Pos.x + (Size.x + ConnectorWidth), Pos.y + (CAPTION_HEIGHT * 0.5f) + ConnectorHeight * 0.5f);
			Vector2f EndLink(LinkPos.x - ConnectorWidth, LinkPos.y + (CAPTION_HEIGHT * 0.5f) + ConnectorHeight * 0.5f);

			EndLink.x -= 15;

			Vector BPoints[4];
			BPoints[0].Set(StartLink.x, StartLink.y, 0.f);
			BPoints[1].Set(StartLink.x + fabs(EndLink.x - StartLink.x) * 0.5f, StartLink.y, 0.f);
			BPoints[2].Set(EndLink.x - fabs(EndLink.x - StartLink.x) * 0.5f, EndLink.y, 0.f);
			BPoints[3].Set(EndLink.x, EndLink.y, 0.f);

			Vector Start(BPoints[0].x, BPoints[0].y, 0.f);

			const size_t Detail = 10;
			for (size_t Index = 1; Index <= Detail; ++Index)
			{
				Vector Result;
				Cubic_Bezier_Spline(BPoints[3], BPoints[2], BPoints[1], BPoints[0], (float)Index / Detail, Result);
				
				Vector N = cross(N, Start - Result, Vector(0.f, 0.f, 1.f));
				N.normalize();
				N*=5;

				Vector A = Start + (N * 0.5f);
				Vector B = Start + (N * -0.5f);
				Vector C = Result + (N * 0.5f);
				Vector D = Result + (N * -0.5f);

				if (IntersectRayWithTriangle(MousePos, Vector2f(A.x, A.y), Vector2f(B.x, B.y), Vector2f(C.x, C.y)) ||
					IntersectRayWithTriangle(MousePos, Vector2f(B.x, B.y), Vector2f(C.x, C.y), Vector2f(D.x, D.y)))
				{
					bResult = true;
					break;
				}
			}
		}
	}
	else
	{
		bResult = (MousePos.x >= Points[0].x && MousePos.x <= Points[1].x) &&
					(MousePos.y >= Points[0].y && MousePos.y <=  Points[1].y);
	}
	return bResult;
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
ActionBase::ActionBase(const CObjectAbstract * Parent)
: Super(Parent)
, ParentAction(const_cast<ActionBase*>(static_cast<const ActionBase*>(Parent)))
, OwnerActor(NULL)
, bPlayOnce(true)
, Count(0)
, Position(0.f, 0.f)
, Size(200.f, 100.f)
{
}

//----------------------------------------------------------------------------------------------
ActionBase::ActionBase(const ActionBase & Source)
: Super(Source)
, OwnerActor(NULL)
{
	if (this != &Source)
	{
		//ParentActor = Source.ParentActor;
		bPlayOnce = Source.bPlayOnce;
		Count = 0;
		Position = Source.Position;
		Size = Source.Size;
	}
}

//----------------------------------------------------------------------------------------------
ActionBase::~ActionBase()
{
	;//
}

//----------------------------------------------------------------------------------------------
void ActionBase::SuperDeserializer(TiXmlElement *xml_current_tree)
{
	XML_DEF_ATTRIBUTES_NODE(Link);
	if (!Link.empty())
	{
		ValueParser LinkStore(Link);
		CStringUtility<int> ParseString(LinkStore.GetValueString("Link"), ";");

		TempLinkIndexes = ParseString.m_vector;
	}

	int PlainElementIndex = 0;
	NObjectFactory::TClassFactory * classFactory = GetClassFactoryA();
	
	XML_FOR_EACH_TREE(xml_current_tree)
	{
		NObjectFactory::TClassFactory::TMapGenerator::const_iterator Iter = classFactory->m_MapGenerator.begin();

		for (;Iter != classFactory->m_MapGenerator.end(); ++Iter)
		{
			if (xml_current_tree->Value() == Iter->first)
			{
				XML_DEF_ATTRIBUTES_NODE(Transient);
				XML_DEF_ATTRIBUTES_NODE(Value);

				ValueParser ValueStore(Value);
				if (Transient == "0" || Transient == "")
				{
					ActionBase * NewObject = static_cast<ActionBase*>(GENERATE_OBJECT_SDK(Iter->first.c_str(), ValueStore.GetValueString("Name").c_str(), this));
					if (NewObject)
					{
						NewObject->SetPlainDeserializationPos(PlainElementIndex++);
						AddChildNode(NewObject);
						NewObject->SuperDeserializer(XML_CURRENT_NODE);
					}
				}
				else
				{
					for (size_t Index = 0; Index < ChildNodes.size(); ++Index)
					{
						if (ChildNodes[Index]->IsTransient() &&
							ChildNodes[Index]->GetName() == ValueStore.GetValueString("Name"))
						{
							ChildNodes[Index]->SetPlainDeserializationPos(PlainElementIndex++);
							ChildNodes[Index]->SuperDeserializer(XML_CURRENT_NODE);
							break;
						}
					}
				}
			}
		}
	}
	Super::SuperDeserializer(xml_current_tree);
}

//----------------------------------------------------------------------------------------------
void ActionBase::SuperSerializer(std::ofstream &Stream, bool bSaveAsExternal /*= false*/)
{
	std::string OutValue;
	GetValueString(OutValue);

	std::string Link = GetLinkageString();
	if (!Link.empty()){
		OutValue += " " + Link;
	}
	
	Stream << "<" << GetType() << OutValue << ">";

	if (!GetExternal() || bSaveAsExternal)
	{
		TVecActionChildIterator IterChild = ChildNodes.begin();
		while (IterChild != ChildNodes.end())
		{
			(*IterChild)->SuperSerializer(Stream);
			++IterChild;
		}
	}
	Stream << "</" << GetType() << ">";
}

//----------------------------------------------------------------------------------------------
void ActionBase::SuperSerializer(std::stringstream &stream, bool bSaveAsExternal /*= false*/)
{

}

//----------------------------------------------------------------------------------------------
std::string ActionBase::GetLinkageString()
{
	std::string StrValue("");

	for (TVecSocketsConstIterator Iter = VecSockets.begin(); Iter != VecSockets.end(); ++Iter)
	{
		if ((*Iter)->Dir == ELink_Out)
		{
			int Index = 0;
			if ((*Iter)->LinkSocket && GetObjectIndex((*Iter)->LinkSocket->Owner, GetRootNode(), Index))
			{
				//StrValue += boost::lexical_cast<std::string>(Index) + ";";
			}
			else
			{
				//StrValue += boost::lexical_cast<std::string>(-1) + ";";
			}
		}
	}

	if (!StrValue.empty())
	{
		StrValue = "Link=\"Link:" + StrValue +  "\"";
	}

	return StrValue;
}

//----------------------------------------------------------------------------------------------
/** Warning: this function releases all depend childs & free self memory */
bool ActionBase::ProcessRelease(bool ForceOperation /*= false*/)
{	
	bool bReleaseObject = false;

	if (ForceOperation || IsPendingToDelete()) // need to be deleted
	{
		TVecActionChildIterator IterActor = ChildNodes.begin();
		while (IterActor != ChildNodes.end())
		{
			(*IterActor)->ProcessRelease(true);   // force release childs
			(*IterActor) = NULL;				  // zero pointer
			IterActor++;
		}

		ChildNodes.erase(std::remove(ChildNodes.begin(), ChildNodes.end(), static_cast<ActionBase*>(NULL)), ChildNodes.end());

		return Super::ProcessRelease(ForceOperation); // free memory
	}
	else
	{
		TVecActionChildIterator IterActor = ChildNodes.begin();
		while (IterActor != ChildNodes.end())
		{
			if ((*IterActor)->ProcessRelease(ForceOperation)) // found pending to delete child object
			{
				(*IterActor) = NULL;
				bReleaseObject = true;
			}
			++IterActor;
		}

		if (bReleaseObject == true)
		{
			ChildNodes.erase(std::remove(ChildNodes.begin(), ChildNodes.end(), static_cast<ActionBase*>(NULL)), ChildNodes.end());
		}
	}

	return false;
}

//----------------------------------------------------------------------------------------------
void ActionBase::Initialize()
{
	Super::Initialize();

	std::for_each(ChildNodes.begin(),ChildNodes.end(), std::mem_fun(&ActionBase::Initialize));
}

//----------------------------------------------------------------------------------------------
void ActionBase::PostLink()
{
	size_t Index = 0;
	for (size_t IndexSocket = 0; IndexSocket < VecSockets.size() && !TempLinkIndexes.empty(); ++IndexSocket)
	{
		if (VecSockets[IndexSocket]->Dir == ELink_Out)
		{
			assert(Index < TempLinkIndexes.size());

			if (ActionBase* Object = GetObjectByIndex(GetRootNode(), TempLinkIndexes[Index]))
			{
				// find in socket
				for (size_t LinkSocket = 0; LinkSocket < Object->VecSockets.size(); ++LinkSocket)
				{
					if (Object->VecSockets[LinkSocket]->Dir == ELink_In)
					{
						VecSockets[Index]->LinkSocket = Object->VecSockets[LinkSocket];
						break;
					}
				}
			}
			++Index;
		}
	}

	std::for_each(ChildNodes.begin(), ChildNodes.end(), std::mem_fun(&ActionBase::PostLink));

	Super::PostLink();
}

//----------------------------------------------------------------------------------------------
bool ActionBase::AddChildNode(const ActionBase * Object, int Index /*= INDEX_NONE*/)
{
	bool bResult = false;

	if (Object != NULL && std::find(ChildNodes.begin(), ChildNodes.end(), Object) == ChildNodes.end())
	{
		if (Index == INDEX_NONE)
		{
			ChildNodes.push_back(const_cast<ActionBase*>(Object));
		}
		else
		{
			TVecActionChildIterator IterPlaceTo = ChildNodes.begin();
			std::advance(IterPlaceTo, Index);

			if (IterPlaceTo != ChildNodes.end()){
				ChildNodes.insert(IterPlaceTo, const_cast<ActionBase*>(Object));
			}
			else{
				ChildNodes.push_back(const_cast<ActionBase*>(Object));
			}
		}

		//CALL_EVENT_DELEGATE_SENDER(Object, Event_AddObjectChild);
		bResult = true;
	}
	return bResult;
}

//----------------------------------------------------------------------------------------------
ActionBase* ActionBase::GetRootNode()
{
	ActionBase * IterActor = this;
	while (IterActor->GetParent())
	{
		IterActor = IterActor->GetParent();
	}
	return IterActor;
}

//----------------------------------------------------------------------------------------------
void ActionBase::Execute(class CActor * EventActor)
{
	DoExecute(EventActor);

	for (TVecSocketsConstIterator Iter = VecSockets.begin(); Iter != VecSockets.end(); ++Iter)
	{
		if ((*Iter)->Dir == ELink_Out && (*Iter)->LinkSocket)
		{
			assert((*Iter)->LinkSocket->Owner);
			(*Iter)->LinkSocket->Owner->Execute(EventActor);
		}
	}
}

//----------------------------------------------------------------------------------------------
void ActionBase::Render(HDC hdc, const Matrix &ViewMatrix)
{
	TVecActionChildIterator IterActor = ChildNodes.begin();
	while (IterActor != ChildNodes.end())
	{
		(*IterActor)->Render(hdc, ViewMatrix);
		IterActor++;
	}

	//RenderCaption(hdc);
	//RenderFuncZone(hdc);

	HPEN hpen, hPenOld;
	hpen = CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
	hPenOld = (HPEN)SelectObject(hdc, hpen);

	Vector CaptionPoints[3];
	CaptionPoints[0].Set(Position.x, Position.y, 0.f);
	CaptionPoints[1].Set(Position.x + Size.x, Position.y, 0.f);
	CaptionPoints[2].Set(Position.x + Size.x, Position.y + CAPTION_HEIGHT, 0.f);

	Vector TCaptionPoints[3];
	TCaptionPoints[0] = transform_coord(CaptionPoints[0], ViewMatrix);
	TCaptionPoints[1] = transform_coord(CaptionPoints[1], ViewMatrix);
	TCaptionPoints[2] = transform_coord(CaptionPoints[2], ViewMatrix);

	RECT rc;
	rc.left = (int)TCaptionPoints[0].x;
	rc.top = (int)TCaptionPoints[0].y;
	rc.right = (int)TCaptionPoints[2].x;
	rc.bottom = (int)TCaptionPoints[2].y;

	HBRUSH blueBrush = CreateSolidBrush(RGB(196, 219, 249));
	::FillRect(hdc, &rc, blueBrush);

 	Vector BorderPoints[4];
 	BorderPoints[0].Set(Position.x, Position.y, 0.f);
 	BorderPoints[1].Set(Position.x + Size.x, Position.y, 0.f);
 	BorderPoints[2].Set(Position.x + Size.x, Position.y + CAPTION_HEIGHT, 0.f);
 	BorderPoints[3].Set(Position.x, Position.y + CAPTION_HEIGHT, 0.f);

	Vector TBorderPoints[5];
	TBorderPoints[0] = transform_coord(BorderPoints[0], ViewMatrix);
	TBorderPoints[1] = transform_coord(BorderPoints[1], ViewMatrix);
	TBorderPoints[2] = transform_coord(BorderPoints[2], ViewMatrix);
	TBorderPoints[3] = transform_coord(BorderPoints[3], ViewMatrix);
	TBorderPoints[4] = transform_coord(BorderPoints[4], ViewMatrix);

	POINT pntArray[5] = {
		{(int)TBorderPoints[0].x, (int)TBorderPoints[0].y},
		{(int)TBorderPoints[1].x, (int)TBorderPoints[1].y},
		{(int)TBorderPoints[2].x, (int)TBorderPoints[2].y},
		{(int)TBorderPoints[3].x, (int)TBorderPoints[3].y},
		{(int)TBorderPoints[0].x, (int)TBorderPoints[0].y},
	};

	::Polyline(hdc, pntArray, 5);

	float scale = ViewMatrix[0];

	HFONT hCustomFont = CreateFont(
		(int)(16 * scale),
		(int)(7 * scale),
		0,0,
		(int)(500 * scale),
		0,0,0,
		ANSI_CHARSET,OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH,
		L"Verdana");

	SelectObject(hdc, hCustomFont);
	::SetBkMode(hdc, TRANSPARENT);
	::SetTextColor(hdc, RGB(0, 0, 0));

	Vector TextPos(Position.x + 10, Position.y + 5, 0.f);
	Vector TTextPos = transform_coord(TextPos, ViewMatrix);

	std::wstring wstr;
	wstr = ConvertStringToWideString(GetName());
	::TextOut(hdc, (int)TTextPos.x, (int)TTextPos.y , wstr.c_str(), (int)wcslen(wstr.c_str()));

	DeleteObject(hCustomFont);

	::SelectObject(hdc, hPenOld);
	::DeleteObject(hpen);

	//std::for_each(ChildNodes.begin(), ChildNodes.end(), std::bind2nd(std::mem_fun(&ActionBase::Render), hdc, ViewMatrix));
}

//----------------------------------------------------------------------------------------------
const ActionBase* ActionBase::ProcessMouseClick(const Vector2f& MousePos) const
{
	if (!IsTransient() && (MousePos.x >= Position.x && MousePos.x <= Position.x + Size.x) &&
		(MousePos.y >= Position.y && MousePos.y <= Position.y + CAPTION_HEIGHT))
	{
		return this;
	}

	TVecActionChildConstIterator Iter = ChildNodes.begin();
	while (Iter != ChildNodes.end())
	{
		if (const ActionBase* Action = (*Iter)->ProcessMouseClick(MousePos)){
			return Action;
		}
		++Iter;
	}
	return 0;
}

//----------------------------------------------------------------------------------------------
SocketAction* ActionBase::ProcessMouseClickSocket(const Vector2f& MousePos, bool link /*= false*/) const
{
	for (TVecSocketsConstIterator Iter = VecSockets.begin(); Iter != VecSockets.end(); ++Iter)
	{
		if ((*Iter)->ProcessMouseClick(MousePos, link)){
			return (*Iter);
		}
	}

	TVecActionChildConstIterator Iter = ChildNodes.begin();
	while (Iter != ChildNodes.end())
	{
		if (SocketAction* Socket = (*Iter)->ProcessMouseClickSocket(MousePos, link)){
			return Socket;
		}
		++Iter;
	}
	return 0;
}

//----------------------------------------------------------------------------------------------
void ActionBase::SetParent(const ActionBase * Parent)
{
	ParentAction = const_cast<ActionBase*>(Parent);
}

//----------------------------------------------------------------------------------------------
void ActionBase::RenderCaption(HDC hdc)
{
// 	HPEN hpen, hPenOld;
// 
// 	POINT pntArray[5] = {
// 		{(int)Position.x, (int)Position.y},
// 		{(int)(Position.x + Size.x), (int)Position.y},
// 		{(int)(Position.x + Size.x), (int)(Position.y + CAPTION_HEIGHT)},
// 		{(int)Position.x, (int)(Position.y + CAPTION_HEIGHT)},
// 		pntArray[0], // holy shirt!!
// 	};
// 
// 	::Polyline(hdc, pntArray, 5);
// 
// 	::SetBkMode(hdc, TRANSPARENT);
// 	::SetTextColor(hdc, RGB(0, 0, 0));
// 
// 	std::wstring wstr;
// 	wstr = ConvertStringToWideString(GetName());
// 	::TextOut(hdc, (int)Position.x + 10, (int)Position.y + 5, wstr.c_str(), (int)wcslen(wstr.c_str()));
// 
// 	::SelectObject(hdc, hPenOld);
// 	::DeleteObject(hpen);
}

//----------------------------------------------------------------------------------------------
void ActionBase::RenderFuncZone(HDC hdc)
{
	HPEN hpen, hPenOld;
	hpen = CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
	hPenOld = (HPEN)SelectObject(hdc, hpen);

	POINT pntArray[5] = {
		{(int)(Position.x), (int)Position.y + CAPTION_HEIGHT},
		{(int)(Position.x + Size.x), (int)Position.y + CAPTION_HEIGHT},
		{(int)(Position.x + Size.x), (int)(Position.y + Size.y + CAPTION_HEIGHT)},
		{(int)Position.x, (int)(Position.y + Size.y + CAPTION_HEIGHT)},
		pntArray[0], // holy shirt!!
	};

	Polyline(hdc, pntArray, 5);

	SelectObject(hdc, hPenOld);
	DeleteObject(hpen);
}

//----------------------------------------------------------------------------------------------
bool ActionBase::RegisterSocket(SocketAction *Socket)
{
	TVecSocketsIterator IterFind = std::find(VecSockets.begin(), VecSockets.end(), Socket);
	if (IterFind == VecSockets.end())
	{
		VecSockets.push_back(Socket);
		return true;
	}
	return false;
}

//----------------------------------------------------------------------------------------------
bool ActionBase::UnRegisterSocket(SocketAction *Socket)
{
	TVecSocketsIterator IterFind = std::find(VecSockets.begin(), VecSockets.end(), Socket);
	if (IterFind != VecSockets.end())
	{
		VecSockets.erase(IterFind);
		return true;
	}
	return false;
}