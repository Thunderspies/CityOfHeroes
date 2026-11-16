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


#Condition
#	On 	Cycle
#	Time 	15
#
#	Event
#		EName 	splatStart
#		Type	start
#		Geom	CenterDummy
#
#		Lifespan	5
#
#	End
#
#	Event
#		EName 	Splat
#		Type	start
#		At	SplatStart
#		Altpiv	1
#
#		BHVR	:PortalRingSplat.bhvr
#		Splat	swirlenergy.tga # energyswirl.tga   #
#
#		Lifespan	45
#	End
#
#End

Condition
	On 	Cycle
	Time 	155
	Chance	0.9

	Event
		EName 	Close
		Type	Start 
		Bhvr	:PortalRiftCloser.bhvr
		part	:PortalRing05.part
		Lifespan	30
	End

	Event
		EName 	Close
		Type	Start 
		part	:PortalRing05.part
		Lifespan	30
	End

	Event
		EName 	closeHelix1
		Type	Local 
		At	Close
		Bhvr	:PortalRiftOffset.bhvr
		part	:PortalGlow01.part
		Lifespan	30
	End

	Event
		EName 	closeHelix2
		Type	Local 
		At	Close
		Bhvr	:PortalRiftOffset2.bhvr
		part	:PortalGlow01.part
		Lifespan	30
	End

End

Condition
	On 	Cycle
	Time 	190	
	Chance	0.8

	Event
		EName 	gravWell
		Type	Local
		Magnet	helix2
		part	:PortalMist02.part
		part	:PortalMist01.part
		part	:PortalMist02.part

		Lifespan	5
	End

End


Condition
	On 	Cycle
	Time 	45
	Chance	0.5
	
	Event
		EName 	Arcs
		Type	Start
		part	:PortalLightningArcs.part
		Lifespan	20
	End
End

Condition
	On 	Cycle
	Time 	175
	Chance	0.9

	Event
		EName 	Start
		Type	Start 
		part	:PortalRing04.part
		Lifespan	30
	End

	Event
		EName 	Open
		Type	Start 
		Bhvr	:PortalRiftOpener.bhvr
		Lifespan	30
	End

	Event
		EName 	openHelix1
		Type	Local 
		At	Open
		Bhvr	:PortalRiftOffset.bhvr
		part	:PortalGlow01.part
		Lifespan	30
	End

	Event
		EName 	openHelix2
		Type	Local 
		At	open
		Bhvr	:PortalRiftOffset2.bhvr
		part	:PortalGlow01.part
		Lifespan	30
	End

End

Condition
	On 	Cycle
	Time 	135
	Chance	0.6

	Event
		EName 	flashhh
		Type	Start 
		part	:PortalRift01.part
		part	:PortalRift02.part
		part	:PortalRift03.part
		part	:PortalRift04.part

		Lifespan	5
	End

End


End