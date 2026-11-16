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

LifeSpan	200

###########################################################

Condition
	On	Time
	Time	17

	Event	
		Type	Local
		At	Origin
		Sound Rumble1 100 100 .76
	End

End



Condition
	On 	Time
	Time 	28
	
	Event
		EName 	HandGlowRightz
		Type	local
		At	mystic
		
		Part3	:CometHandStarRed1.part
		Part4	:CometHandCloudsRed1.part
		Sound vandalblast 90.0 90.0 .76
		
		Part1	:CometHandBubblesRedSolid.part
		Part	:CometHandBubblesRed.part
		Part2	:CometHandBubblesBlack.part
		
		
		PMagnet	WepR
	End

		
End

###################################################################

Condition
	On 	Time
	Time 	57
	
	Event
		EName 	HandGlowFlashR
		Type	local
		At	mystic
		Part1	:HandGlowWhite3.part
		Part2	:HandGlowEmber3a.part
		
		
		PMagnet	WepR
	End

		
End
###################################################################

Condition

	On	Time
	Time	57.5

	Event
		EName	core
		Type	local
		At	Mystic
		geom	CoreWolfPack
		bhvr	behaviors/BlastScale3a.bhvr
		
		
	End

End

Condition
	On	Time
	Time	60

	

	Event
		EName	Prime
		Type	start
		At	Mystic
		geom	CenterDummy
		Part2	:CometBubblesWhitex.part
		Part3	:CometBubblesBlack.part
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
	Time 	65
	
	Event
		EName 	BeamBody
		Type	Destroy
		

	End

	
End

Condition
	On 	Time
	Time 	57
	
	Event
		EName 	HandGlowRight1
		Type	Destroy
		

	End

	
End

Condition
	On 	Time
	Time 	60
	
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

	
End


End			