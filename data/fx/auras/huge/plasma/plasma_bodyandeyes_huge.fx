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
		At	LArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Plasma_Core.part
		POther	UArmL
	End

	Event
		Type	PositOnly
		At	LArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Plasma_Core.part
		POther	UArmR
	End

	Event
		Type	PositOnly
		At	HandL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Plasma_Core.part
		POther	LArmL
	End

	Event
		Type	PositOnly
		At	HandR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Plasma_Core.part
		POther	LArmR
	End

	Event
		Type	PositOnly
		At	UArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Plasma_Core.part
		Part	:Plasma_Long.part
		Part	:Plasma_Highlight.part
		POther	Neck
	End

	Event
		Type	PositOnly
		At	UArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Plasma_Core.part
		Part	:Plasma_Long.part
		Part	:Plasma_Highlight.part
		POther	Neck
	End

	Event
		Type	Local
		At	Hips
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 -1.5 0.0
		End
		Part	:Plasma_Core_Trail.part
	End

	Event
		Type	PositOnly
		At	LLegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Plasma_Core.part
		Part	:Plasma_Long.part
		POther	ULegL
	End

	Event
		Type	PositOnly
		At	LLegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Plasma_Core.part
		Part	:Plasma_Long.part
		POther	ULegR
	End

	Event
		Type	PositOnly
		At	FootL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Plasma_Core_Big.part
		Part	:Plasma_Long.part
		POther	LLegL
	End

	Event
		Type	PositOnly
		At	FootR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Plasma_Core_Big.part
		Part	:Plasma_Long.part
		POther	LLegR
	End

#############################################################

	Event
		Type	PositOnly
		At	Neck
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Plasma_Core.part
		Part	:Plasma_Long.part
		POther	Hair
	End

	Event
		Type	PositOnly
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Plasma_Core_Trail.part
	End
End

#############################################################

End