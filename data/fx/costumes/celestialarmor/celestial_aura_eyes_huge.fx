#############################################################
## Alpha_Eyes_Male.fx
#############################################################

FxInfo
Flags NotCompatibleWithAnimalHead DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName 	EyeAnchor
		Type	Local
		At	Eyes
		Bhvr	behaviors/GenericParticleFade.bhvr
		Geom	CircleOfThorns
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	1
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	-0.015 -0.03 -0.1
		End
		Part	:Star_Eyes.part
		Part	:Eye_Glow.part
		Part	:Glow_Eyes.part
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	2
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.015 -0.03 -0.1
		End
		Part	:Star_Eyes.part
		Part	:Eye_Glow.part
		Part	:Glow_Eyes.part
	End
End

#############################################################

End