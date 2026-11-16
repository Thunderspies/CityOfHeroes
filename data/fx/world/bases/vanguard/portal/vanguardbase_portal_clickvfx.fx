#########################################################
##	Villain Base Portal
#########################################################

FxInfo

#LifeSpan		60	

Condition
	On 	Time
	Time 	0

	Event
		EName 	gravWell
		Type	Start
		At	Root
		Magnet	gravWell
		part	:Click_PortalMist01.part
		part	:Click_PortalMist02.part
		part	:Click_PortalSpark02.part
		LifeSpan		60
	End

#	Event
#		EName 	Pulse
#		Type	Start
#		At	gravWell
#		childFX	:VillainBasePortalPulse.fx
#	End

End


Condition
	On 	Time
	Time 	0

	Event
		EName 	Core
		Type	Start 
		At	GravWell
		BhvrOverride
			PositionOffset 0 .1 0
		End
#		part	:PortalCore01.part
		part	:Click_PortalRingOuter.part
		LifeSpan		30
	End

End

Condition
	On 	Time
	Time 	20

	Event
		EName 	Arcs
		Type	Start 
		At	GravWell
		part	:Click_PortalLightningArcs.part
		part	:Click_PortalLightningArcs.part
		Lifespan	60
	End

End

Condition
	On 	Time
	Time 	15

	Event
		EName 	Rift
		Type	Start 
		At	Core
		part	:PortalGlow02.part
		Lifespan	55
	End

End

Condition
	On 	Time
	Time 	25

	Event
		EName 	Rings
		Type	Start 
		At	Core
		part	:PortalRing01.part
		part	:PortalRing02.part
		LifeSpan		50
	End

End

Condition
	On 	Time
	Time 	20

	Event
		EName 	Start
		Type	Start 
		At	Root
		part	:PortalRing04.part
		part	:PortalRing04.part
		Lifespan	20
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
		LifeSpan		80
	End

	Event
		EName 	openHelix2
		Type	Local 
		At	open
		Bhvr	:PortalRiftOffset2.bhvr
		part	:PortalGlow01.part
		LifeSpan		80
	End
End

Condition
	On 	Time
	Time 	20

	Event
		EName 	helix
		Type	posit 
		At	root
		Bhvr	:PortalRiftOffset.bhvr
		part	:Click_PortalSpark01.part
#		part	:Click_PortalGlow01.part


		part	:Click_PortalRift01.part
		part	:Click_PortalRift02.part
		part	:Click_PortalRift03.part
		part	:Click_PortalRift04.part

		LifeSpan		35
	End

End