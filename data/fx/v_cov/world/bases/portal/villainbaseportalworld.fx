#########################################################
##	Villain Base Portal  Kickstarted world entity FX
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
		part	:KickStartPortalMist01.part
		part	:KickStartPortalMist02.part
		part	:KickStartPortalSpark02.part
	End

	Event
		EName 	Pulse
		Type	Start
		At	gravWell
		childFX	:VillainBasePortalPulse.fx
	End

	Event
		EName 	Core
		Type	Start 
		At	GravWell
		BhvrOverride
			PositionOffset 0 .1 0
		End
		part	:KickStartPortalCore01.part
		part	:PortalRingOuter.part
	End

	Event
		EName 	Rings
		Type	Start 
		At	Core
		part	:KickStartPortalRing01.part
		part	:KickStartPortalRing02.part
	End

	Event
		EName 	Center
		Type	Start 
		At	Root
		part	:KickStartPortalRift01.part
		part	:KickStartPortalRift02.part
		part	:KickStartPortalRift03.part
		part	:KickStartPortalRift04.part
		#part	:KickStartPortalRing03.part
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

End