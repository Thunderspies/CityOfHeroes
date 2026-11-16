###########################################################

FxInfo
Flags DontSuppress

###########################################################

Condition
	On	time
	Time	0


		Event
			EName	FortifyPack_DireWolf_Cycle
			At 	Hips
			Type 	Local
			Bhvr	behaviors/GenericParticleFade.bhvr
			CHILDFX	:PackBuff_ShadowHunterDog_GRANDCHILD.fx
			Lifespan 50
		End
End

Condition
	On	Cycle
	Time	150


		Event
			EName	FortifyPack_DireWolf_Cycle
			At 	Hips
			Type 	Local
			Bhvr	behaviors/GenericParticleFade.bhvr
			CHILDFX	:PackBuff_ShadowHunterDog_GRANDCHILD.fx
			Lifespan 50
		End
End

###########################################################

End