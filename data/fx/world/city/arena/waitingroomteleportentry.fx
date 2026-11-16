#########################################################
##	SStreetSteeam01
##
FxInfo

LifeSpan	60

Condition
	On 	Time
	Time 	8
	
	Event
		Type	Local 
		At	Root
		Part	:TeleportSmallDotsOut.part
		Part	:TeleporterLineThinRandomOut.part
		Part	:TeleporterLongLineDotsThinOut.part
		Part	:TeleporterLineStreakThinOut.part
		
		Part	:CoreRandomDotsFlatOppositeTealOut.part
		Part	:CoreRandomDotsFlatTealOut.part
		
		LifeSpan	25

	End

	
	Event
		EName 	Base
		Type	Local 
		At	root

		
		Part	:TeleporterTechLightRay2Out.part
		Part	:TeleporterTechLightRay2invertOut.part
		Part	:TeleporterTechLightRay2aOut.part

		LifeSpan	6

	End
	
	Event
		EName 	Base
		Type	Local 
		At	root

		Part	:TeleporterLightRayOut.part

		LifeSpan	5

	End

	Event
		EName 	Base
		Type	Local 
		At	root

		Part	:BaseSplatDown.part
		
		LifeSpan	55

	End

End


Condition
	On 	Time
	Time 	7
	
	Event
		Type	Local 
		At	Root
		
		BHVR	:RootOffset.bhvr
		Part	:TechCircleThinLineCleanOut.part
		Part	:TechCircleThinLineRuggedOut.part
		Part	:TechCircleThinLineCleanUpOut.part
		Part	:TechCircleThinLineRuggedUpOut.part

		
		LifeSpan	40

	End

End
#######################################################################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Base
		Type	Local 
		At	root
		Geom	OverHeadDummy
	
	End

	Event
		EName 	CoreUP
		Type	Local 
		At	Base
		altpiv	1

		Part	:CoreLineDotsDown.part
		
		Part	:CoreRandomDotsDown.part
		Part	:CoreRandomDotsDownTeal.part
		Part	:CoreRandomDotsDownOpposite.part
		Part	:CoreRandomDotsDownTealOpposite.part

		Part	:CoreLineDown.part
		Part	:CoreLineDown2.part
				
		LifeSpan	41
		
	End

	Event
		Type	Local 
		At	Base
		altpiv	1
		Part	:CoreGlowDown.part
		Part	:CoremistDown.part
		
		LifeSpan	28
		
	End

End

End			