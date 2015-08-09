#include "coresdkafx.h"

REGISTER_CLASS(UVModificator, BaseModificatorUI);

//------------------------------------------------------------------------
UVModificator::UVModificator(const CObjectAbstract * Parent)
: Super(Parent)
{
	;//
}

//------------------------------------------------------------------------
UVModificator::UVModificator(const UVModificator& Source)
: Super(Source)
{
	if (this != &Source)
	{
		;//
	}
}

//------------------------------------------------------------------------
UVModificator::~UVModificator()
{
	;//
}

//------------------------------------------------------------------------
void UVModificator::Initialize()
{
	Super::Initialize();
}

//------------------------------------------------------------------------
void UVModificator::OnPropertyChanged(const char* PropertyName)
{
	const SplineAgregator * _Parent = static_cast<const SplineAgregator*>(GetParent());
	if (_Parent)
	{
		const_cast<SplineAgregator*>(_Parent)->Rebuild();
	}
}
//------------------------------------------------------------------------
bool UVModificator::ProcessUserController(const MouseMoveInputData& InputData)
{
	const SplineAgregator * _Parent = static_cast<const SplineAgregator*>(GetParent());
	
	if (_Parent)
	{
		GetAppMain()->EnterDrawCS();
		GetRenderComponent()->EnterCS();

		const_cast<SplineAgregator*>(_Parent)->Rebuild();

		GetRenderComponent()->LeaveCS();
		GetAppMain()->LeaveDrawCS();
	}
	return false; //Super::ProcessUserController(InputData);
}

//------------------------------------------------------------------------
void UVModificator::ApplyModificator(const CActor * Sender)
{
/*	const SplineAgregator * _Parent = static_cast<const SplineAgregator*>(Sender);
	if (_Parent)
	{
		GetAppMain()->EnterDrawCS();

		const_cast<SplineAgregator*>(_Parent)->UpdateCoordsUV(GetPivot(), GetSize());

		GetAppMain()->LeaveDrawCS();
	}*/
}
