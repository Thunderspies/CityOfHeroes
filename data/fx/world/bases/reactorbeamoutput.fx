#########################################################
##	Villain Base Portal
#########################################################

FxInfo

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		#Sound	EntrancePortal1_loop 80 70 .10
	End
End



Condition
	On	Time
	Time 	0

	Event
		EName 	Spinner
		Type	Start
		At	Root
		Bhvr	V_COV\World\Bases\Portal\PortalRiftSpinner.bhvr
		# Sound MA_ContactPad_Loop 40 40 .50
	End

	Event
		EName 	helix2
		Type	Local
		At	Spinner
		Bhvr	behaviors\PortalRiftOffset3.bhvr
		BhvrOverride
			PositionOffset 0.0 0.0 2.25
		End
		Part	:DataBeamsUp.part
	End

	Event
		EName 	LightBeams
		Type	Start
		At	Root
		Part	:LightBeams3.part

	End

#	Event
#		EName 	helix2
#		Type	Local
#		At	Spinner
#		Bhvr	behaviors\PortalRiftOffset3.bhvr
#		BhvrOverride
#			PositionOffset 0.0 0.0 -1.75
#			Spin 0.0 -0.2 0.0
#		End
#		Part	:DataBeamsUp.part
#	End

	Event
		EName 	helix2
		Type	Local
		At	Spinner
		Bhvr	behaviors\PortalRiftOffset3.bhvr
		BhvrOverride
			PositionOffset 0.0 0.0 .8
		End
		Part	:DataBeamsUp.part
	End



	Event
		EName 	gravWell
		Type	Start
		At	Root
		Magnet	gravWell
	End

	Event
		EName 	Core
		Type	Start
		At	GravWell
		BhvrOverride
			PositionOffset 0 .18 0
		End
		part	:PortalCore02.part

	End

	Event
		EName 	Rings
		Type	Start
		At	Core
		part	:PortalRing02.part
		part	:PortalRingDecal02.part
	End

End