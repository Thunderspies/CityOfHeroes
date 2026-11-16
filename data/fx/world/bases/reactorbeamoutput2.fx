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
		Sound EntrancePortal1_loop 80 80 .58
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

	End

	Event
		EName 	helix2
		Type	Local
		At	Spinner
		Bhvr	behaviors\PortalRiftOffset3.bhvr
		BhvrOverride
			PositionOffset 0.0 0.0 1.5
		End
		Part	:HelixGlowUp.part
	End

	Event
		EName 	helix2
		Type	Local
		At	Spinner
		Bhvr	behaviors\PortalRiftOffset3.bhvr
		BhvrOverride
			PositionOffset 0.0 0.0 2.75
		End
		Part	:HelixGlowUp2.part
	End

	Event
		EName 	helix2
		Type	Local
		At	Spinner
		Bhvr	behaviors\PortalRiftOffset3.bhvr
		BhvrOverride
			PositionOffset 0.0 0.0 -2.75
		End
		Part	:HelixGlowUp2.part
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
	End

End