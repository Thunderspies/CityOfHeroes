#########################################################
##	cot_waterfall_med.fx
##
FxInfo
	
LifeSpan	200

###########################################################

Condition
	On 	Time
	Time 	30
	
	Event
		EName 	HandGlowRightz
		Type	local
		At	WepR
		
		Part3	:CometHandStarWhite1.part
		Part4	:CometHandCloudsWhite1.part
		Sound LegacyLightBlast 90 90 .88
		PMagnet	WepR
		LifeSpan	25
	End

###################################################################
	
	Event
		EName 	HandGlowRight1
		Type	local
		At	WepR
		Part1	:CometHandBubbles.part
		Part2	:CometHandBubblesWhite.part
		LifeSpan	40
		
		
		PMagnet	WepR
	End
	
	Event
		EName	cometParts
		Type	Local
		At	WepR
		Part1	:CometBubbles.part
		Part2	:CometBubblesBright.part
		Part3	:CometBubblesLocal.part
		Part4	:CometHead.part
		Part5	:EnergyCoreLocal.part
		LifeSpan	25
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

		LifeSpan	40
		
		
		PMagnet	WepR
	End

	
End
###################################################################

Condition
	On	Time
	Time	60


	Event
		EName	Prime
		Type	start
		At	WepR
		geom	CenterDummy
		Part2	:CometBubblesWhite.part
		bhvr	behaviors/projectile1.bhvr
		Magnet	Target
		LookAt  Target
	
	End
	
	Event
		EName	cometParts03
		Type	Local
		At	Prime
		
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
	
	Event
		EName	cometParts03
		Type	Destroy
		

	End
	
	Event
		EName	cometPartsLocal
		Type	Destroy
		

	End	

End
	

End			