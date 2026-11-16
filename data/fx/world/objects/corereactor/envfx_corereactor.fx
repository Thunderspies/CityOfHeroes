#############################################################
## EnvFX_CoreReactor.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

#########################################################

#	Event
#		EName	Dummy
#		Type	Start
#		At	Root
#		Geom	_P_Tech_Reactor_01
#	End
#
#	Event
#		EName	Dummy2
#		Type	Start
#		At	Root
#		BhvrOverride
#			PositionOffset	0 48 0
#			PyrRotate	180 0 0
#		End
#		Geom	_P_Tech_Reactor_01


	Event
		EName	CoreAnchor
		Type	Start
		At	Root
		Geom	FX_P_Tech_Reactor
	End

#########################################################

	Event
		EName	CoreAnchor_Offset
		Type	Start
		At	CoreAnchor
		BhvrOverride
			PositionOffset	0 24 0
		End
		part	:Root_Core.part
		part	:Root_Core_Glow.part
		part	:Root_Core_Globules.part
		part	:Root_Core_Globules_Tiny.part
		part	:Root_Core_LightRays.part
	End

#########################################################

	Event
		Type	Start
		At	CoreAnchor
		AltPiv	1
		part	:Offset_Steam.part
		part	:Offset_Steam_Lingering.part
	End

	Event
		Type	Start
		At	CoreAnchor
		AltPiv	2
		part	:Offset_Steam.part
		part	:Offset_Steam_Lingering.part
	End

	Event
		Type	Start
		At	CoreAnchor
		AltPiv	3
		part	:Offset_Steam.part
		part	:Offset_Steam_Lingering.part
	End

	Event
		Type	Start
		At	CoreAnchor
		AltPiv	4
		part	:Offset_Steam.part
		part	:Offset_Steam_Lingering.part
	End

#########################################################

	Event
		Type	Start
		At	CoreAnchor
		AltPiv	1
		BhvrOverride
			PositionOffset	0 45 0
		End
		part	:Offset_Steam_Stream.part
		part	:Offset_Steam_Stream_Lingering.part
	End

	Event
		Type	Start
		At	CoreAnchor
		AltPiv	2
		BhvrOverride
			PositionOffset	0 45 0
		End
		part	:Offset_Steam_Stream.part
		part	:Offset_Steam_Stream_Lingering.part
	End

	Event
		Type	Start
		At	CoreAnchor
		AltPiv	3
		BhvrOverride
			PositionOffset	0 45 0
		End
		part	:Offset_Steam_Stream.part
		part	:Offset_Steam_Stream_Lingering.part
	End

	Event
		Type	Start
		At	CoreAnchor
		AltPiv	4
		BhvrOverride
			PositionOffset	0 45 0
		End
		part	:Offset_Steam_Stream.part
		part	:Offset_Steam_Stream_Lingering.part
	End

#########################################################

	Event
		EName	Anchor5
		Type	Start
		At	CoreAnchor
		AltPiv	5
	End

	Event
		EName	Anchor6
		Type	Start
		At	CoreAnchor
		AltPiv	6
	End

	Event
		EName	Anchor7
		Type	Start
		At	CoreAnchor
		AltPiv	7
	End

	Event
		EName	Anchor8
		Type	Start
		At	CoreAnchor
		AltPiv	8
	End

	Event
		EName	Anchor9
		Type	Start
		At	CoreAnchor
		AltPiv	9
	End

	Event
		EName	Anchor10
		Type	Start
		At	CoreAnchor
		AltPiv	10
	End

	Event
		EName	Anchor11
		Type	Start
		At	CoreAnchor
		AltPiv	11
	End

	Event
		EName	Anchor12
		Type	Start
		At	CoreAnchor
		AltPiv	12
	End
End

Condition
	On	Cycle
	Time	10
	Chance	0.65

	Event
		Type	Start
		At	CoreAnchor
		BhvrOverride
			PositionOffset	0 24 0
		End
		Part	:Root_Lightning.part
		Part	:Root_Lightning_Tendril.part
		Lifespan 10
	End
End

Condition
	On	Cycle
	Time	12
	Chance 	0.8

	Event
		Type	Start
		At	Anchor5
		Part	:Beam_Electricity_Tendril.part
		Part	:Beam_Glow.part
		POther	Anchor9
		Lifespan 10
	End
	Event
		Type	Start
		At	Anchor9
		Part	:Beam_Electricity_Tendril_Inverted.part
		POther	Anchor5
		Lifespan 10
	End
End

Condition
	On	Cycle
	Time	12
	Chance 	0.8

	Event
		Type	Start
		At	Anchor6
		Part	:Beam_Electricity_Tendril.part
		Part	:Beam_Glow.part
		POther	Anchor10
		Lifespan 10
	End
	Event
		Type	Start
		At	Anchor10
		Part	:Beam_Electricity_Tendril_Inverted.part
		POther	Anchor6
		Lifespan 10
	End
End

Condition
	On	Cycle
	Time	12
	Chance 	0.8

	Event
		Type	Start
		At	Anchor7
		Part	:Beam_Electricity_Tendril.part
		Part	:Beam_Glow.part
		POther	Anchor11
		Lifespan 10
	End

	Event
		Type	Start
		At	Anchor11
		Part	:Beam_Electricity_Tendril_Inverted.part
		POther	Anchor7
		Lifespan 10
	End
End

Condition
	On	Cycle
	Time	12
	Chance 	0.8

	Event
		Type	Start
		At	Anchor8
		Part	:Beam_Electricity_Tendril.part
		Part	:Beam_Glow.part
		POther	Anchor12
		Lifespan 10
	End

	Event
		Type	Start
		At	Anchor12
		Part	:Beam_Electricity_Tendril_Inverted.part
		POther	Anchor8
		Lifespan 10
	End
End
