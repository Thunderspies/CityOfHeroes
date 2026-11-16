#########################################################
##	SStreetSteeam01
##
FxInfo

LifeSpan	60

Condition
	On 	Time
	Time 	0

	Event
		EName 	Base
		Type	Local 
		At	root

		Part	:TeleportSmallDots.part
		Part	:TeleporterLineThinRandom.part
		Part	:TeleporterLongLineDotsThin.part
		Part	:TeleporterLineStreakThin.part
		
		Part	:CoreRandomDotsFlatOppositeTeal.part
		Part	:CoreRandomDotsFlatTeal.part
		
		LifeSpan	25

	End
	
	Event
		Type	Local 
		At	root
		
		Part	:TechCircleThinLineClean.part
		Part	:TechCircleThinLineRugged.part
		Part	:TechCircleThinLineCleanUp.part
		Part	:TechCircleThinLineRuggedUp.part
		
		LifeSpan	40

	End
	
	Event
		EName 	Base
		Type	Local 
		At	root

		
		Part	:TeleporterTechLightRay2.part
		Part	:TeleporterTechLightRay2invert.part
		Part	:TeleporterTechLightRay2a.part

		LifeSpan	6

	End
	
	Event
		EName 	Base
		Type	Local 
		At	root

		Part	:TeleporterLightRay.part

		LifeSpan	5

	End

	Event
		EName 	Base
		Type	Local 
		At	root

		Part	:BaseSplat.part
		
		LifeSpan	55

	End

End

Condition
	On 	Time
	Time 	7

	Event
		EName 	CoreleftUP
		Type	Local 
		At	root

		Bhvr	:left.bhvr
		Part	:OutsideLineUpOpposite.part
		LifeSpan	40


	End
	
	Event
		EName 	CoreRightUP
		Type	Local 
		At	root

		Bhvr	:Right.bhvr
		Part	:OutsideLineUp.part
		LifeSpan	40
		
	End
	
	Event
		EName 	CoreUP
		Type	Local 
		At	root

		Part	:CoreLineDotsUp.part
		
		Part	:CoreRandomDotsUp.part
		Part	:CoreRandomDotsUpTeal.part
		Part	:CoreRandomDotsUpOpposite.part
		Part	:CoreRandomDotsUpTealOpposite.part

		Part	:CoreLineUp.part
		Part	:CoreLineUp2.part
				
		LifeSpan	41
		
	End

	Event
		Type	Local 
		At	root
		Part	:CoreGlow.part
		Part	:Coremist.part
		
		LifeSpan	15
		
	End

End

End			