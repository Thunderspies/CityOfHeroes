#########################################################
##	cot_waterfall_med.fx
##
FxInfo

LifeSpan	180


###########################################################


Condition
	On	Time
	Time	0
	

	Event
		EName	core
		Type	Local
		At	Hips
		
		Sound legacylightblast3 80.0 80.0 .9
	End

End


Condition
	On 	Time
	Time 	7
	
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


Condition
	On 	Time
	Time 	17

	Event
		EName 	HandGlowRight2
		Type	local
		At	WepR
		Part1	:HandGlowWhite.part
		Part2	:HandGlowEmber.part
		PMagnet	WepR
	End

	Event
		EName 	HandGlowLeft2
		Type	local
		At	WepL
		Part1	:HandGlowWhite.part
		Part2	:HandGlowEmber.part
		PMagnet	WepL
	End

End

###################################################################

Condition
	On 	Time
	Time 	7
	Event
		EName 	HandGlowRight22
		Type	local
		At	WepR
		Part1	:HandGlowWhite.part
		Part2	:HandGlowEmber.part
		PMagnet	WepR
	End

	Event
		EName 	HandGlowLeft22
		Type	local
		At	WepL
		Part1	:HandGlowWhite.part
		Part2	:HandGlowEmber.part
		PMagnet	WepL
	End

End

###################################################################

Condition
	On	Time
	Time	26


	Event
		EName	Prime
		Type	start
		At	WepR
		Part3	:RapidCometBubblesWhite.part
		
		bhvr	behaviors/projectile1.bhvr
		Magnet	Target
		LookAt  Target
	
	End

	Event
		EName	cometParts
		Type	Local
		At	Prime
		Part1	:RapidCometBubbles.part
		Part2	:RapidCometBubblesBright.part
		Part3	:RapidCometBubblesLocal.part
		Part5	:RapidEnergyCoreLocal.part
	End

End


Condition
	On	Time
	Time	33


	Event
		EName	Prime1
		Type	start
		At	WepL
		Part3	:RapidCometBubblesWhite.part
		
		bhvr	behaviors/projectile1.bhvr
		Magnet	Target
		LookAt  Target
	
	End

	Event
		EName	cometParts1
		Type	Local
		At	Prime1
		Part1	:RapidCometBubbles.part
		Part2	:RapidCometBubblesBright.part
		Part3	:RapidCometBubblesLocal.part
		Part5	:RapidEnergyCoreLocal.part
	End
End

Condition
	On	Time
	Time	50


	Event
		EName	Prime2
		Type	start
		At	WepR
		Part3	:RapidCometBubblesWhite.part
		bhvr	behaviors/projectile1.bhvr
		Magnet	Target
		LookAt  Target
	
	End

	Event
		EName	cometParts2
		Type	Local
		At	Prime2
		Part1	:RapidCometBubbles.part
		Part2	:RapidCometBubblesBright.part
		Part3	:RapidCometBubblesLocal.part
		Part5	:RapidEnergyCoreLocal.part
	End

End

Condition
	On	Time
	Time	45

	Event
		EName	Prime3
		Type	start
		At	WepL
		Part3	:RapidCometBubblesWhite.part
		
		bhvr	behaviors/projectile1.bhvr
		Magnet	Target
		LookAt  Target
	
	End

	Event
		EName	cometParts3
		Type	Local
		At	Prime3
		Part1	:RapidCometBubbles.part
		Part2	:RapidCometBubblesBright.part
		Part3	:RapidCometBubblesLocal.part
		Part5	:RapidEnergyCoreLocal.part
	End

End

Condition
	On	Time
	Time	40

	Event
		EName	Prime4
		Type	start
		At	WepR
		Part3	:RapidCometBubblesWhite.part
		bhvr	behaviors/projectile1.bhvr
		Magnet	Target
		LookAt  Target
	
	End

	Event
		EName	cometParts4
		Type	Local
		At	Prime4
		Part1	:RapidCometBubbles.part
		Part2	:RapidCometBubblesBright.part
		Part3	:RapidCometBubblesLocal.part
		Part5	:RapidEnergyCoreLocal.part
	End

End

Condition
	On 	Time
	Time	40

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
	Time	60

	Event
		EName 	HandGlowRight2
		Type	Destroy
		

	End

	Event
		EName 	HandGlowLeft2
		Type	Destroy
		

	End

	Event
		EName 	HandGlowRight22
		Type	Destroy
		

	End

	Event
		EName 	HandGlowLeft22
		Type	Destroy
		

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

##		ChildFx  fire_ball.fx
##A new fx will be created with three inputs: First input:  this fx node. 
##Next input: this fx node's magnet, if any.   
##Next input, this fx node's other point, if any.  
##Note that you do put .fx at the end of the ChildFx's name.

#Condition
#	On 	Time
#	
#	Event
#		Ename	2ndBlast
#		Type	Start
#		At	origin
#		ChildFx V_COV\EnemyPowers\LegacyOfLight/WhiteRapid.fx
#		Magnet	Target
#		
#
#	End
#
#End

End			