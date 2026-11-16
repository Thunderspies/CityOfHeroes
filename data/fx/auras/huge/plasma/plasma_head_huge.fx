#############################################################
## Android_Plasma_WeldingRay_Continuing.fx
#############################################################

FxInfo

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
			PositionOffset	0.0 -0.05 -0.05
		End
		Part	:Plasma_Core_EyeL.part
		Part	:Plasma_Long_EyeL.part
		Part	:Eye_GlowL.part
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	2
		BhvrOverride
			PositionOffset	0.0 -0.05 -0.05
		End
		Part	:Plasma_Core_EyeR.part
		Part	:Plasma_Long_EyeR.part
		Part	:Eye_GlowR.part
	End

	Event
		Type	PositOnly
		At	Head
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Plasma_Hand_Glow.part
		Part	:Plasma_Core.part
		Part	:Plasma_Long.part
		Part	:Plasma_Highlight_Hands.part
		POther	Hair
	End
End

#############################################################

End