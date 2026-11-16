#############################################################
## Emerge_Activation.fx
#############################################################

FxInfo
Lifespan 120

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		Sound 	Apparitions_Emerge_01 100 100 .8
	End

	Event
		Type	PositOnly
		At	UArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.0 0.0
		End
		Part	:Emerge_Smoke.part
		POther	UArmL
	End

	Event
		Type	PositOnly
		At	Head
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.0 0.0
		End
		Part	:Emerge_Smoke.part
	End
End

#########################################################

End