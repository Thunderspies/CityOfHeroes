#############################################################
## Omega_Body_Huge.fx
#############################################################

FxInfo

#########################################################
#################### BODY GLOW 1 ########################
#########################################################



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
		BhvrOverride
			PositionOffset	0.0 -0.04 -0.0
		End
		Part	:Plasma_Core_EyeL.part
		Part	:Smoke_Long_EyeL.part
		Part	:Eye_GlowL.part
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	2
		BhvrOverride
			PositionOffset	0.0 -0.04 -0.0
		End
		Part	:Plasma_Core_EyeR.part
		Part	:Smoke_Long_EyeR.part
		Part	:Eye_GlowR.part
	End

	Event
		Type	Local
		At	Head
		Part	:Plasma_Core.part
		Part	:Plasma_Core_Trail.part
		Part	:Smoke_Body.part
		Part	:Smoke_Body_Highlight.part
		Part	:Smoke_Body_Trail.part
		Part	:Smoke_Body_Trail_Small.part
		POther  Hair
	End
End

#########################################################

End