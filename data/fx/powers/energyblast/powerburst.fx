#########################################################
##	cot_waterfall_med.fx
##
FxInfo
	
Input  
	InpName	Origin
End


Input  
	InpName	Target
End


Input  
	InpName	mystic
End

Input  
	InpName	WepR
End

Input  
	InpName	WepL
End

LifeSpan	200

###########################################################


Condition
	On	Time
	Time	13
	

	Event
		EName	core
		Type	local
		At	Mystic
		geom	MegaCore
		bhvr	behaviors/BlastScale3.bhvr
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
		
		Part3	:CometHandStarWhite1.part
		Part4	:CometHandCloudsWhite1.part
		
		PMagnet	WepR
	End

	Event
		EName 	HandGlowLeftz
		Type	local
		At	WepL
		
		Part3	:CometHandStarWhite1.part
		Part4	:CometHandCloudsWhite1.part
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
		Part1	:CometHandBubbles.part
		Part2	:CometHandBubblesWhite.part
		
		
		PMagnet	WepR
	End

	Event
		EName 	HandGlowLeft1
		Type	local
		At	WepL
		Part1	:CometHandBubbles.part
		Part2	:CometHandBubblesWhite.part
		
		PMagnet	WepL
	End

	
End

###################################################################

Condition
	On 	Time
	Time 	37
	
	Event
		EName 	HandGlowFlashR
		Type	local
		At	WepR
		Part1	:HandGlowWhite3.part
		Part2	:HandGlowEmber3.part
		
		
		PMagnet	WepR
	End

	Event
		EName 	HandGlowFlashL
		Type	local
		At	WepL
		Part1	:HandGlowWhite3.part
		Part2	:HandGlowEmber3.part
		
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
		At	Mystic
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
		Part1	:CometBubbles.part
		Part2	:CometBubblesBright.part
		Part3	:CometBubblesLocal.part
		Part4	:CometHead.part
		Part5	:EnergyCoreLocal.part
	End

End

Condition
	On	Time
	Time	20
	Event
		EName	cometPartsLocal
		Type	Local
		At	core
		
		altpiv	1
		Part5	:EnergyCoreLocal.part
		Part1	:CoreEnergy.part
		Part2	:CoreEnergyBeam.part
		Part3	:CometBubbles2.part
		Part4	:CometBubbles3.part

		#Magnet	Target
		#LookAt	Target

	End

	
End


Condition
	On	Time
	Time	35


	Event
		EName	Prime1
		Type	start
		At	Mystic
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
		Part1	:CometBubbles.part
		Part2	:CometBubblesBright.part
		Part3	:CometBubblesLocal.part
		Part4	:CometHead.part
		Part5	:EnergyCoreLocal.part
	End

End

Condition
	On	Time
	Time	40


	Event
		EName	Prime2
		Type	start
		At	Mystic
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
		Part1	:CometBubbles.part
		Part2	:CometBubblesBright.part
		Part3	:CometBubblesLocal.part
		Part4	:CometHead.part
		Part5	:EnergyCoreLocal.part
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

Condition
	On Time
	Time 150

	Event
		EName	All
		Type	Destroy
	End
End


End			