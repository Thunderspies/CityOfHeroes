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
		part	:PortalMist01.part
		part	:PortalMist02.part
		part	:PortalSpark02.part
	End

	Event
		EName 	Pulse
		Type	Start
		At	gravWell
		childFX	:VillainBasePortalPulse.fx
	End

End


Condition
	On 	Time
	Time 	15

	Event
		EName 	Core
		Type	Start 
		At	GravWell
		BhvrOverride
			PositionOffset 0 .1 0
		End
		part	:PortalCore01.part
		part	:PortalRingOuter.part
	End

End

Condition
	On 	Time
	Time 	20

	Event
		EName 	Arcs
		Type	Start 
		At	GravWell
		part	:PortalLightningArcs.part
		part	:PortalLightningArcs.part
		Lifespan	60
	End

End

Condition
	On 	Time
	Time 	35

	Event
		EName 	Rift
		Type	Start 
		At	Core
		part	:PortalGlow02.part
		Lifespan	25
	End

End

Condition
	On 	Time
	Time 	45

	Event
		EName 	Rings
		Type	Start 
		At	Core
		part	:PortalRing01.part
		part	:PortalRing02.part
	End

End

Condition
	On 	Time
	Time 	60

	Event
		EName 	Start
		Type	Start 
		At	Root
		part	:PortalRing04.part
		part	:PortalRing04.part
		Lifespan	30
	End

	Event
		EName 	Open
		Type	Start 
		At	Root
		Bhvr	:PortalRiftOpener.bhvr
		Lifespan	30
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
		#part	:PortalRing03.part
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


Condition
	On	Death

	Event
		Ename	Spinner
		Type	Destroy
	End

	Event
		Ename	Spinner2
		Type	Destroy
	End

	Event
		Ename	Pulse
		Type	Destroy
	End

	Event
		EName 	Close
		Type	Start 
		At	Root
		Bhvr	:PortalRiftCloser.bhvr
		part	:PortalRing05.part
		Lifespan	30
	End

	Event
		EName 	Closer
		Type	Start 
		At	Root
		part	:PortalRing05.part
		part	:PortalRing05.part
		Lifespan	30
	End

	Event
		EName 	closeHelix1
		Type	Local 
		At	Close
		Bhvr	:PortalRiftOffset.bhvr
		part	:PortalGlow01.part
	End

	Event
		EName 	closeHelix2
		Type	Local 
		At	Close
		Bhvr	:PortalRiftOffset2.bhvr
		part	:PortalGlow01.part
	End

	Event
		EName 	closeout
		Type	Local
		At	Root
		Magnet	helix2
		part	:PortalMist01.part
		part	:PortalMist02.part

		Lifespan	5
	End

End

End