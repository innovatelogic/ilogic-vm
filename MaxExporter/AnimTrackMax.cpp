#include "expstdafx.h"
/*
//----------------------------------------------------------------------------------
int AnimTrack::Build(IGameObject * IObject, class SkeletonObjectMax * Skeleton)
{
	if (Skeleton->Bones.size() > 0)
	{
		TimeValue t0 = ExporterMAX::GetExporter()->GetIGame()->GetSceneStartTime();
		TimeValue t1 = ExporterMAX::GetExporter()->GetIGame()->GetSceneEndTime();
		TimeValue DeltaTime = ExporterMAX::GetExporter()->GetIGame()->GetSceneTicks();

		int SamplingRate = 1;

		int numkeys = (t1 - t0) / (DeltaTime * SamplingRate) + 1;

		AnimFrames.reserve(numkeys);

		AffineParts PRS;

		// iterate throw all keys
		for (long Index = t0; Index <= t1; Index += DeltaTime * SamplingRate)
		{
			AnimFrame frame(Skeleton->GetNumBones());

			Skeleton->RootBone->BuildFrame(&frame, Index);

			AnimFrames.push_back(frame);
		}
	}

	//pIGameNode->GetObjectTM(ExporterMAX::GetExporter()->GetStaticFrame());
	return 1;
}*/