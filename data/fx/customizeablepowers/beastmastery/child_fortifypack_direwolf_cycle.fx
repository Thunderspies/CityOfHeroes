###########################################################

FxInfo
Flags DontSuppress

###########################################################

Condition
	On	Cycle
	Time	150


		Event
			EName	FortifyPack_DireWolf_Cycle
			At 	Hips
			Type 	Local
			Bhvr	behaviors/GenericParticleFade.bhvr
			CHILDFX	:CHILD_FortifyPack_DireWolf.fx
			Lifespan 50
		End
End

###########################################################

End