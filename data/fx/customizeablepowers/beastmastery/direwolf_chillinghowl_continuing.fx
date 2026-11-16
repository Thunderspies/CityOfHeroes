#############################################################
## PistolHit2.fx
#############################################################

FxInfo

#############################################################

Input
	Inpname Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Prime
		Type 	Local
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			StartJitter	0.5 1.0 0.5
		End
		Part	:Cryonic_Continuing_Mist_Large.part

	End
End

#############################################################

End