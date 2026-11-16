#############################################################
## Omega_Body_Huge.fx
#############################################################

FxInfo

Flags NotCompatibleWithAnimalHead

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
		Bhvr	behaviors/GenericParticleFade.bhvr
		Geom	CircleOfThorns
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	1
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 -0.05 -0.1
		End
		Part	:Plasma_Core_EyeL.part
		Part	:Smoke_Long_EyeL.part
		Part	:Eye_GlowL.part
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	2
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 -0.05 -0.1
		End
		Part	:Plasma_Core_EyeR.part
		Part	:Smoke_Long_EyeR.part
		Part	:Eye_GlowR.part
	End

	Event
		Type	Local
		At	UArmL
		Part	:Smoke_Body.part
		Part	:Smoke_Body_Highlight_Front.part
		Part	:Plasma_Core.part
		Part	:Plasma_Core_Trail.part
		Part	:Smoke_Body_Trail.part
		Part	:Smoke_Body_Trail_Small.part
		POther 	HandL
	End

	Event
		Type	Local
		At	UarmR
		Part	:Smoke_Body.part
		Part	:Smoke_Body_Highlight_Front.part
		Part	:Plasma_Core.part
		Part	:Plasma_Core_Trail.part
		Part	:Smoke_Body_Trail.part
		Part	:Smoke_Body_Trail_Small.part
		POther 	HandR
	End

	Event
		Type	Local
		At	LArmL
		Part	:Smoke_Hands.part
		Part	:Plasma_Core.part
		Part	:Plasma_Core_Trail.part

		POther	HandL
	End

	Event
		Type	Local
		At	LArmR
		Part	:Smoke_Hands.part
		Part	:Plasma_Core.part
		Part	:Plasma_Core_Trail.part

		POther	HandR
	End

	Event
		Type	Local
		At	HandL
		Part	:Plasma_Core_Big.part
	End

	Event
		Type	Local
		At	HandR
		Part	:Plasma_Core_Big.part
	End
End

#########################################################

End