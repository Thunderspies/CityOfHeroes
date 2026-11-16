#############################################################
## Costume Aura - Flies (Body)
#############################################################

FxInfo

Flags NotCompatibleWithAnimalHead

########################################################

Condition
	On	Time
	Time	0

	Event
		EName 	EyeAnchor
		Type	Local
		At	Eyes
		Geom	CircleOfThorns
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	1
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.0 -.01
		End
		Part	:Eye_Glow.part
		Part	:Nucleus_Glow_Eyes.part
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	2
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.0 -.01
		End
		Part	:Eye_Glow.part
		Part	:Nucleus_Glow_Eyes.part
	End
End

#############################################################

End