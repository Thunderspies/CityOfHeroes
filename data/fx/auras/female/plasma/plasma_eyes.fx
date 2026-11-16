#############################################################
## Android_Plasma_WeldingRay_Continuing.fx
#############################################################

FxInfo

Flags NotCompatibleWithAnimalHead

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On 	Time
	Time 	0

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
		BhvrOverride
			PositionOffset	0.0 0.01 -0.05
		End
		Part	:Eye_Glow.part
		Part	:Plasma_Core_EyeL.part
		Part	:Plasma_Long_EyeL.part
		Part	:Eye_GlowL.part
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	2
		BhvrOverride
			PositionOffset	0.0 0.01 -0.05
		End
		Part	:Eye_Glow.part
		Part	:Plasma_Core_EyeR.part
		Part	:Plasma_Long_EyeR.part
		Part	:Eye_GlowR.part
	End

#############################################################

End