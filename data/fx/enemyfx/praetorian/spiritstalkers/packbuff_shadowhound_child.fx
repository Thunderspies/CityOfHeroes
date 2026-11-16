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
			CHILDFX	:PackBuff_ShadowHound_GRANDCHILD.fx
			Lifespan 50
		End
End

###########################################################

End