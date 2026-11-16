#############################################################
## VillainBasePortal.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	gravWell
		Type	Start
		At	Root
		part	:PortalMist01.part
		part	:PortalMist02.part
		part	:PortalSpark02.part
		Magnet	gravWell
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Core
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset 0 0.025 0
		End
		part	:PortalCore01.part
	End

	Event
		EName 	Core
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset 0 0.05 0
		End
		part	:PortalRingOuter.part
		part	:PortalRing01.part
		part	:PortalRing02.part
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Pulse
		Type	Start
		At	gravWell
		childFX	:VillainBasePortalPulse.fx
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Open
		Type	Start
		At	Root
		Bhvr	:PortalRiftOpener.bhvr
		Lifespan 30
	End

	Event
		EName 	openHelix1
		Type	Local
		At	Open
		Bhvr	:PortalRiftOffset.bhvr
		part	:PortalGlow01.part
	End

	Event
		EName 	openHelix2
		Type	Local
		At	open
		Bhvr	:PortalRiftOffset2.bhvr
		part	:PortalGlow01.part
	End

	Event
		EName 	Center
		Type	Start
		At	Root
		part	:PortalRift01.part
		part	:PortalRift02.part
		part	:PortalRift03.part
		part	:PortalRift04.part
	End

	Event
		EName 	Spinner
		Type	Start
		At	Root
		Bhvr	:PortalRiftSpinner.bhvr
	End

	Event
		EName 	helix
		Type	Local
		At	Spinner
		Bhvr	:PortalRiftOffset.bhvr
		part	:PortalSpark01.part
		part	:PortalGlow01.part
	End

	Event
		EName 	helix2
		Type	Local
		At	Spinner
		Bhvr	:PortalRiftOffset2.bhvr
		part	:PortalSpark01.part
	End

	Event
		EName 	Spinner2
		Type	Start
		At	Root
		Bhvr	:PortalRiftSpinner2.bhvr
	End

	Event
		EName 	helix3
		Type	Local
		At	Spinner2
		Bhvr	:PortalRiftOffset.bhvr
		part	:PortalSpark01.part
		part	:PortalGlow01.part
	End

	Event
		EName 	helix4
		Type	Local
		At	Spinner2
		Bhvr	:PortalRiftOffset2.bhvr
		part	:PortalSpark01.part
	End
End

#############################################################

End