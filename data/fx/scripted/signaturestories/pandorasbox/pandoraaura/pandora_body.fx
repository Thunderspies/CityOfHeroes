#
FxInfo
Lifespan 99999

###########################################################

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Local
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :Matter_FlareUp02.part
			LifeSpan 30
		End

		Event
			At Hips
			Type Local
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :Blobs_FlareUp.part
			Part :Matter_FlareUp01.part
			Part :Matter_FlareUp02.part
			LifeSpan 30
		End

		Event
			At Root
			Type Local
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :Growl_Ring.part
			Part :Growl_Wind_Shockwave.part
			LifeSpan 30
		End
	End

	Condition
		On Time
		Time 0

		Event
			At Head
			Type Local
			EName HeadParts
			PMagnet Chest
			POther Chest
			Part :Matter_Small_Move.part
			Part :Matter_Small.part
		End

		Event
			At Chest
			Type Local
			EName ChestParts
			PMagnet Hips
			POther Hips
			Part :Matter_Small_Move.part
			Part :Matter_Big.part
			Part :Matter_Clump.part
		End

		Event
			At Hips
			Type Local
			EName HipParts
			PMagnet Chest
			POther Chest
			Part :Matter_Clump.part
		End

		Event
			At LArmL
			Type Local
			EName LeftArm
			PMagnet UArmL
			POther UArmL
			Part :Matter_Small.part
		End

		Event
			At LArmR
			Type Local
			EName RightArm
			PMagnet UArmR
			POther UArmR
			Part :Matter_Small.part
		End

		Event
			At ULegL
			Type Local
			EName LeftLegUp
			PMagnet LLegL
			POther LLegL
			Part :Matter_Small_Move.part
			Part :Matter_Small.part
		End

		Event
			At ULegR
			Type Local
			EName RightLegUp
			PMagnet LLegR
			POther LLegR
			Part :Matter_Small_Move.part
			Part :Matter_Small.part
		End

		Event
			At LLegL
			Type Local
			EName LeftLeg
			PMagnet ULegL
			POther ULegL
			Part :Matter_Small_Move.part
			Part :Matter_Small.part
		End

		Event
			At LLegR
			Type Local
			EName RightLeg
			PMagnet ULegR
			POther ULegR
			Part :Matter_Small_Move.part
			Part :Matter_Small.part
		End

#####################################

		Event
			At Chest
			Type PositOnly
			EName Core
			Part :Blobs_Move.part
		End

		Event
			At Hips
			Type PositOnly
			Part :Blobs_Move.part
		End

		Event
			At Hips
			Type Local
			BhvrOverride
				PositionOffset	0 -1 0
			End
			Part :Blobs_Move.part
		End

		Event
			At Head
			Type Local
			BhvrOverride
				PositionOffset 0 0.1 0
			End
			Part :Blob_Streaks.part
			Part :Blobs.part
			Part :Blobs_Move.part
		End

		Event
			At UArmL
			Type PositOnly
			Part :Blobs.part
			Part :Blob_Streaks.part
		End

		Event
			At UArmR
			Type PositOnly
			Part :Blobs.part
			Part :Blob_Streaks.part
		End

		Event
			At LArmL
			Type PositOnly
			LookAt UarmL
			POther UarmL
			Part :BlobGlow_SM.part
			Part :Blobs_SM.part
			Part :Blobs_SM2.part
			Part :Blobs.part
			Part :Blob_Streaks.part
		End

		Event
			At LArmR
			Type PositOnly
			LookAt UarmR
			POther UarmR
			Part :BlobGlow_SM.part
			Part :Blobs_SM.part
			Part :Blobs_SM2.part
			Part :Blobs.part
			Part :Blob_Streaks.part
		End

		Event
			At LArmL
			Type PositOnly
			LookAt HandL
			POther HandL
			Part :Blobs_SM2.part
		End

		Event
			At LArmR
			Type PositOnly
			LookAt HandR
			POther HandR
			Part :Blobs_SM2.part
		End

		Event
			At HandL
			Type PositOnly
			LookAt LarmL
			POther LarmL
			Part :BlobGlow_SM.part
			Part :Blobs_SM.part
		End

		Event
			At HandR
			Type PositOnly
			LookAt LarmR
			POther LarmR
			Part :BlobGlow_SM.part
			Part :Blobs_SM.part
		End

		Event
			At WepL
			Type PositOnly
			Part :BlobGlow_SM.part
			Part :Blobs_SM.part
		End

		Event
			At WepR
			Type PositOnly
			Part :BlobGlow_SM.part
			Part :Blobs_SM.part
		End

		Event
			At ULegL
			Type PositOnly
			EName LegHighL
			Part :Blobs.part
		End

		Event
			At ULegR
			Type PositOnly
			EName LegHighR
			Part :Blobs.part
		End

		Event
			At LLegL
			Type PositOnly
			EName LegLowL
			LookAt LegHighL
			POther LegHighL
			Part :Blobs_Med.part
		End

		Event
			At LLegL
			Type PositOnly
			EName LegLowL2
			LookAt ULegL
			POther ULegL
			Part :Blobs_Med2.part
		End

		Event
			At LLegR
			Type PositOnly
			EName LegLowR
			LookAt LegHighR
			POther LegHighR
			Part :Blobs_Med.part
		End

		Event
			At LLegR
			Type PositOnly
			EName LegLowL2
			LookAt ULegR
			POther ULegR
			Part :Blobs_Med2.part
		End

		Event
			At FootL
			Type PositOnly
			EName LegFootL
			LookAt LegLowL
			POther LegLowL
			Part :BlobGlow_SM.part
			Part :Blobs_SM.part
			Part :Blobs_SM2.part
		End

		Event
			At FootR
			Type PositOnly
			EName LegFootR
			LookAt LegLowR
			POther LegLowR
			Part :BlobGlow_SM.part
			Part :Blobs_SM.part
			Part :Blobs_SM2.part
		End

	End

End
