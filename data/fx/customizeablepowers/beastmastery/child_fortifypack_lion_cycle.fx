###########################################################

FxInfo
Flags DontSuppress

##########################################################

Condition
	On	Cycle
	Time	150


		Event
			EName	FortifyPack_Lion_Cycle
			At 	Root
			Type 	Local
			Bhvr	behaviors/GenericParticleFade.bhvr
			CHILDFX	:CHILD_FortifyPack_Lion.fx
			Lifespan 50
		End
End

###########################################################

End