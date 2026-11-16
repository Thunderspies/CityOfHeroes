#########################################################
##	Mag Flux Bolt
#########################################3

FxInfo

LifeSpan	200

###########################################################

Condition
	On	Time
	Time	9
	

	Event
		EName	Base
		Type	local
		At	Root
		
		BhvrOverride
			PositionOffset	0 4 3
		End
		
	End

	Event
		EName	core
		Type	local
		At	Base
		
		geom	FX_MetalBall
		BhvrOverride
			Scale		1 1 1
			ScaleRate	1.25 1.25 1.25
			 EndScale	5 5 5
			FadeInLength	20
		End
		
		Lifespan	22
		
	End

End


Condition
	On 	Time
	Time 	8
	
	Event
		EName 	HandGlowRightz
		Type	local
		At	WepR
		Part	:MagnetismRing01.part
		Part	:MagnetismRing02.part
		Part1	:MagnetPoleFlux01.part
		Part1	:MagnetPoleFluxLarge.part
		Part2	:MagnetPoleGlow01.part
		Part3	:MagnetPoleStar01.part
		
		PMagnet	WepL

		Lifespan	30
	End

	Event
		EName 	HandGlowLeftz
		Type	local
		At	WepL
		Part	:MagnetismRing01.part
		Part	:MagnetismRing02.part
		Part1	:MagnetPoleFlux01.part
		Part1	:MagnetPoleFluxLarge.part
		Part2	:MagnetPoleGlow01.part
		Part3	:MagnetPoleStar01.part
		PMagnet	WepR
		
		Lifespan	30
	End

	
End

Condition
	On 	Time
	Time 	37
	
	Event
		EName 	HandGlowFlashR
		Type	local
		At	WepR
		Part1	POWERS\EnergyBlast\HandGlowWhite3.part
		Part2	POWERS\EnergyBlast\HandGlowEmber3.part
		
		
		PMagnet	WepR
	End

	Event
		EName 	HandGlowFlashL
		Type	local
		At	WepL
		Part1	POWERS\EnergyBlast\HandGlowWhite3.part
		Part2	POWERS\EnergyBlast\HandGlowEmber3.part
		
		PMagnet	WepL
	End

	
End

Condition
	On	Time
	Time	30


	Event
		EName	Prime
		Type	start
		At	Core
		geom	FX_MetalBall
		#Part2	:CometBubblesWhite.part
		bhvr	behaviors/projectile1.bhvr
		Magnet	Target
		LookAt  Target

		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		150
			PhysForcePowerJitter	50
		End
	
	End

#	Event
#		EName	cometParts
#		Type	Local
#		At	Prime
#		altpiv	1
#		Part1	:CometBubbles.part
#		Part2	:CometBubblesBright.part
#		Part3	:CometBubblesLocal.part
#		Part4	:CometHead.part
#		Part5	:EnergyCoreLocal.part
#	End
#
#End
#
#
#Condition
#	On 	PrimeHit
#	
#	Event
#		EName 	Prime
#		Type	Destroy
#		
#
#	End
#
#	Event
#		EName 	cometParts
#		Type	Destroy
#		
#
#	End
#
#End
#
#

End			