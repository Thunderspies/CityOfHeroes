#########################################################
##	cot_waterfall_med.fx
##
FxInfo
	

LifeSpan	200

###########################################################


Condition
	On	Time
	Time	13
	

	Event
		EName	core
		Type	local
		At	BendMystic
		geom	CoreWolfPack
		bhvr	behaviors/RularuuBlastScaleBig.bhvr
		Sound pbexplosive 100.0 100.0 .75
		
	End

End


Condition
	On 	Time
	Time 	10
	
	Event
		EName 	HandGlowRightz
		Type	local
		At	WepR
		BHVR	Behaviors/GenericParticleFade.bhvr
		
		Part3	:CometHandStarRed1.part
		Part4	:CometHandCloudsRed1.part
		
		PMagnet	WepR
	End

	Event
		EName 	HandGlowLeftz
		Type	local
		At	WepL
		BHVR	Behaviors/GenericParticleFade.bhvr
		
		Part3	:CometHandStarRed1.part
		Part4	:CometHandCloudsRed1.part
		PMagnet	WepL
	End

	
	Event
		EName 	HandGlowRight1
		Type	local
		At	WepR
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part1	:CometHandBubblesWhite.part
		Part2	:CometHandBubblesRed.part
		
		
		PMagnet	WepR
	End

	Event
		EName 	HandGlowLeft1
		Type	local
		At	WepL
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part1	:CometHandBubblesWhite.part
		Part2	:CometHandBubblesRed.part
		
		PMagnet	WepL
	End

	
End

###################################################################

Condition
	On 	Time
	Time 	20
	
	Event
		EName 	HandGlowFlashR
		Type	local
		At	WepR
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part1	:HandGlowWhite3.part
		Part2	:HandGlowEmber3.part
		
		
		PMagnet	WepR
	End

	Event
		EName 	HandGlowFlashL
		Type	local
		At	WepL
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part1	:HandGlowWhite3.part
		Part2	:HandGlowEmber3.part
		
		PMagnet	WepL
	End

	
End
###################################################################

Condition
	On	Time
	Time	25


	Event
		EName	Prime
		Type	start
		At	core
		geom	CenterDummy
		Part2	:CometBubblesWhite.part
		bhvr	behaviors/projectile1.bhvr
		Magnet	Target
		LookAt  Target
	
	End

	Event
		EName	cometParts
		Type	Local
		At	Prime
		altpiv	1
		Part1	:CometBubblesRed.part
		Part2	:CometBubblesBrightRed.part
		Part3	:CometBubblesLocalRed.part
		Part4	:CometHeadRed.part
		Part5	:CometBubblesBlack.part
	End

End

Condition
	On	Time
	Time	20
	Event
		EName	cometPartsLocal
		Type	Local
		At	core
		
		Part	:EnergyCoreLocalRed.part
		Part	:CoreEnergy.part
		Part	:CoreEnergyBeam.part
		Part	:CometBubbles2.part
		Part	:CometBubbles3.part

		#Magnet	Target
		#LookAt	Target

	End

	
End



Condition
	On 	PrimeHit
	
	Event
		EName 	Prime
		Type	Destroy
		

	End

	Event
		EName 	cometParts
		Type	Destroy
		

	End

End


Condition
	On 	Time
	Time 	37
	
	Event
		EName 	HandGlowRight1
		Type	Destroy
	End

	Event
		EName 	HandGlowLeft1
		Type	Destroy
	End

End


Condition
	On 	Time
	Time 	39
	
	Event
		EName 	cometPartsLocal
		Type	Destroy
		

	End

	Event
		EName 	Core
		Type	Destroy
		

	End

	Event
		EName 	HandGlowRightz
		Type	Destroy
		

	End

	Event
		EName 	HandGlowLeftz
		Type	Destroy
		

	End

End


End			