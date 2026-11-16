#############################################################
## GenericRiktiEnergyField.fx
#############################################################

FxInfo

Lifespan 10

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	RootRotateOffset
		Type	Local
		At	Root
		BhvrOverride
			PyrRotateJitter 0 180 0
		End
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	TopOffset
		Type	Local
		At	RootRotateOffset
		BhvrOverride
			PositionOffset	0 14.5 0
			PyrRotateJitter 0 20 0
			SpinJitter	0 0.02 0
		End
	End

	Event
		EName	CylinderOffsetTop
		Type	Local
		At	TopOffset
		BhvrOverride
			PositionOffset	7 0 0
		End
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName	BottomOffset
		Type	Local
		At	RootRotateOffset
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PyrRotateJitter 0 20 0
			SpinJitter	0 0.02 0
		End
	End

	Event
		EName	CylinderOffsetBottom
		Type	Local
		At	BottomOffset
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	7 0 0
		End
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	CylinderOffsetTop
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	:SmallBeam01.part
		Part1	:SmallBeam02.part
		Part1	:SmallBeamGlow1.part
		pOther	CylinderOffsetBottom
	End
End

#############################################################

End