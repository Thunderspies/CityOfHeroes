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

LifeSpan	200
###########################################################


Condition
	On	Time
	Time	0
	

	Event
		EName	core
		Type	start
		At	Origin
		geom	Rapid
		bhvr	behaviors/Scale1.bhvr	
		#Sound   PowerBolt1 100.0 30.0 .85
	End

End


Condition
	On	Time
	Time	0


	Event
		EName	Prime
		Type	start
		At	core
		Altpiv	1
	
		Part3	:CometBubblesWhite.part
		
		bhvr	behaviors/projectile1.bhvr
		Magnet	Target
		LookAt  Target
	
	End

	Event
		EName	cometParts
		Type	Local
		At	Prime
		Part1	:CometBubbles.part
		Part2	:CometBubblesBright.part
		Part3	:CometBubblesLocal.part
		
		Part5	:EnergyCoreLocal.part
	End

End


Condition
	On	Time
	Time	43


	Event
		EName	Prime1
		Type	start
		At	core
		Altpiv	2
		
		Part3	:CometBubblesWhite.part
		
		bhvr	behaviors/projectile1.bhvr
		Magnet	Target
		LookAt  Target
	
	End

	Event
		EName	cometParts1
		Type	Local
		At	Prime1
		Part1	:CometBubbles.part
		Part2	:CometBubblesBright.part
		Part3	:CometBubblesLocal.part
		
		Part5	:EnergyCoreLocal.part
	End
End

Condition
	On	Time
	Time	55


	Event
		EName	Prime2
		Type	start
		At	core
		Altpiv	3
	
		Part3	:CometBubblesWhite.part
		bhvr	behaviors/projectile1.bhvr
		Magnet	Target
		LookAt  Target
	
	End

	Event
		EName	cometParts2
		Type	Local
		At	Prime2
		Part1	:CometBubbles.part
		Part2	:CometBubblesBright.part
		Part3	:CometBubblesLocal.part
		
		Part5	:EnergyCoreLocal.part
	End

End

Condition
	On	Time
	Time	55

	Event
		EName	Prime3
		Type	start
		At	core
		Altpiv	4
			
		Part3	:CometBubblesWhite.part
		
		bhvr	behaviors/projectile1.bhvr
		Magnet	Target
		LookAt  Target
	
	End

	Event
		EName	cometParts3
		Type	Local
		At	Prime3
		Part1	:CometBubbles.part
		Part2	:CometBubblesBright.part
		Part3	:CometBubblesLocal.part
		
		Part5	:EnergyCoreLocal.part
	End

End

Condition
	On	Time
	Time	57

	Event
		EName	Prime4
		Type	start
		At	core
		Altpiv	5
		
		Part3	:CometBubblesWhite.part
		bhvr	behaviors/projectile1.bhvr
		Magnet	Target
		LookAt  Target
	
	End

	Event
		EName	cometParts4
		Type	Local
		At	Prime4
		Part1	:CometBubbles.part
		Part2	:CometBubblesBright.part
		Part3	:CometBubblesLocal.part
		
		Part5	:EnergyCoreLocal.part
	End

End


Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
		

	End

End

Condition
	On 	Prime1Hit

	Event
		EName 	Prime1
		Type	Destroy
		

	End

End

Condition
	On 	Prime2Hit

	Event
		EName 	Prime2
		Type	Destroy
		

	End

End

Condition
	On 	Prime3Hit

	Event
		EName 	Prime3
		Type	Destroy
		

	End


End

Condition
	On 	Prime4Hit

	Event
		EName 	Prime4
		Type	Destroy
		

	End

End


End			