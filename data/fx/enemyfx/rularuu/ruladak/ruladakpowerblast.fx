#########################################################
##	cot_waterfall_med.fx
##
FxInfo
	
LifeSpan	200

###########################################################


Condition
	On	Time
	Time	19
	

	Event
		EName	core
		Type	local
		At	BendMystic
		geom	CoreWolfPack
		bhvr	behaviors/RularuuBlastScale.bhvr
		Sound PowerBolt4 100.0 100.0 .7
		
	End

End


Condition
	On 	Time
	Time 	18
	
	Event
		EName 	HandGlowRightz
		Type	local
		At	WepR
		
		Part3	:CometHandStarRed1.part
		Part4	:CometHandCloudsRed1.part
		
		PMagnet	WepR
	End

	Event
		EName 	HandGlowLeftz
		Type	local
		At	WepL
		
		Part3	:CometHandStarRed1.part
		Part4	:CometHandCloudsRed1.part

		PMagnet	WepL
	End

	
End
###################################################################

Condition
	On 	Time
	Time 	18
	
	Event
		EName 	HandGlowRight1
		Type	local
		At	WepR
		Part1	:CometHandBubblesRed.part
		Part2	:CometHandBubblesWhite.part
		
		
		PMagnet	WepR
	End

	Event
		EName 	HandGlowLeft1
		Type	local
		At	WepL
		Part1	:CometHandBubblesRed.part
		Part2	:CometHandBubblesWhite.part
		
		PMagnet	WepL
	End

	
End
###################################################################

Condition
	On	Time
	Time	30


	Event
		EName	Prime
		Type	start
		At	BendMystic
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
		Part5	:EnergyCoreLocalRed.part
		Part	:CometBubblesBlack.part
	End

End

Condition
	On	Time
	Time	35


	Event
		EName	Prime1
		Type	start
		At	BendMystic
		geom	CenterDummy
		Part2	:CometBubblesWhite.part
		bhvr	behaviors/projectile1.bhvr
		Magnet	Target
		LookAt  Target
	
	End

	Event
		EName	cometParts02
		Type	Local
		At	Prime1
		altpiv	1
		Part1	:CometBubblesRed.part
		Part2	:CometBubblesBrightRed.part
		Part3	:CometBubblesLocalRed.part
		Part4	:CometHeadRed.part
		Part5	:EnergyCoreLocalRed.part
		Part	:CometBubblesBlack.part
	End

End

Condition
	On	Time
	Time	40


	Event
		EName	Prime2
		Type	start
		At	BendMystic
		geom	CenterDummy
		Part2	:CometBubblesWhite.part
		bhvr	behaviors/projectile1.bhvr
		Magnet	Target
		LookAt  Target
	
	End

	Event
		EName	cometParts03
		Type	Local
		At	Prime2
		altpiv	1
		Part1	:CometBubblesRed.part
		Part2	:CometBubblesBrightRed.part
		Part3	:CometBubblesLocalRed.part
		Part4	:CometHeadRed.part
		Part5	:EnergyCoreLocalRed.part
		Part	:CometBubblesBlack.part
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
	On 	Prime1Hit

	Event
		EName 	Prime1
		Type	Destroy
		

	End

	Event
		EName 	cometParts02
		Type	Destroy
		

	End

End

Condition
	On 	Prime2Hit

	Event
		EName 	Prime2
		Type	Destroy
		

	End

	Event
		EName 	cometParts03
		Type	Destroy
		

	End

End


Condition
	On 	Time
	Time 	45
	
	Event
		EName 	BeamBody
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
	Time 	40
	
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