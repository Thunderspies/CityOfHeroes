###########################################################

FxInfo
Flags DontSuppress

###########################################################

Condition
	On	Time
	Time	0

		Event
			EName	FortifyPack_DireWolf_Cycle
			At 	Hips
			Type 	Local
			Bhvr	behaviors/GenericParticleFade.bhvr
			CHILDFX	:PackBuff_Wodan_GRANDCHILD.fx
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
			CHILDFX	:PackBuff_Wodan_GRANDCHILD.fx
			Lifespan 50
		End
End

###########################################################

End