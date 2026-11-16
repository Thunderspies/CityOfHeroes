#########################################################
##	Villain Base Portal
#########################################################

FxInfo


Condition
	On 	Time
	Time 	0

	Event
		EName 	gravWell
		Type	Start
		At	Root
		Magnet	gravWell
		Sound MA_DataColumn_Loop 103 103 .55
	End

	Event
		EName 	Pulse
		Type	Start
		At	gravWell
		childFX	:ReactorBeamChild.fx
	End

	Event
		EName 	Core
		Type	Start
		At	GravWell
		BhvrOverride
			PositionOffset 0 .1 0
		End
		part	:PortalCore01.part
		part	:PortalRing01.part
		part	:PortalRingDecal.part

	End
	Event

		EName 	Core
		Type	Start
		At	GravWell
		BhvrOverride
			PositionOffset 0 115 0
		End
		part	:PortalCore01.part
		part	:PortalRing01.part
		part	:PortalRingDecal.part

	End

	Event
		EName 	Spinner
		Type	Start
		At	Root
		Bhvr	V_COV\World\Bases\Portal\PortalRiftSpinner.bhvr
		BhvrOverride
			Spin	0.0 0.17 0.0
		End
	#	Part	:DataBeamsUpBig.part

	End

	Event
		EName 	lightBeamsBottom
		Type	Local
		At	Spinner
		Bhvr	behaviors\PortalRiftOffset3.bhvr
		BhvrOverride
			PositionOffset 0.0 -4.0 9.8
		End
		Part	:LightBeams2.part

	End

	Event
		EName 	lightBeamsTop
		Type	Local
		At	Spinner
		Bhvr	behaviors\PortalRiftOffset3.bhvr
		BhvrOverride
			PositionOffset 0.0 120.0 10.5
		End
		Part	:LightBeams5.part

	End

End

