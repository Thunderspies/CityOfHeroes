#############################################################
## CapturedReichsman.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		Bhvr	behaviors\GenericParticleFadeQuick.bhvr
		BhvrOverride
			Scale		1 1 1
			Alpha		128
			Spin		0 -0.001 0
		End
		Geom	FX_ContainmentField_Blue
	End
End

Condition
	On	Time
	Time	0

	Event
		EName	GodRayOffsetAbove
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 14.5 0
		End
		Part	:Decal1.part
	End

	Event
		EName	GodRayOffsetBelow
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 0.0 0
		End
		Part	:Decal1.part
	End

	Event
		Type	PositOnly
		At	GodRayOffsetBelow
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:Cage_GodRay1.part
		part	:Cage_GodRay2.part
		pOther	GodRayOffsetAbove
	End
End

#############################################################

End