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
		
		Sound PowerBolt11 100.0 100.0 .85
	End

	Event
		Ename	PushDirection
		Type	Posit
		At	core
		
		Lookat	Target

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
##2ndFade
###################################################################
Condition
	On 	Time
	Time 	43
	Event
		EName 	HandGlowRight
		Type	local
		At	WepR
		#Part1	:HandGlowWhite2.part
		#Part2	:HandGlowEmber2.part
		PMagnet	WepR
	End

	Event
		EName 	HandGlowLeft
		Type	local
		At	WepL
		#Part1	:HandGlowWhite2.part
		#Part2	:HandGlowEmber2.part
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
		#Part1	:RapidCometHead.part
		#Part2	:RapidBlastStreak.part
		Part3	:RapidCometBubblesWhite.part
		
		bhvr	behaviors/projectile1.bhvr
		Magnet	Target
		LookAt  Target
	
	
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		100
			PhysForcePowerJitter	50
		End
	End

	Event
		Ename	PushForce
		Type	Local
		At	PushDirection
		
		BhvrOverride
			PositionOffset		0 0 5
			pyrRotate		15 0 0
			PhysForceType		Forward
			PhysForceRadius		10
			PhysForcePower		200
			PhysForcePowerJitter	50
		End
		

		LifeSpan	1

	End

	Event
		EName	cometParts
		Type	Local
		At	Prime
		Part1	:RapidCometBubbles.part
		Part2	:RapidCometBubblesBright.part
		Part3	:RapidCometBubblesLocal.part
		#Part4	:RapidCometHead.part
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
		#Part1	:RapidCometHead.part
		#Part2	:RapidBlastStreak.part
		Part3	:RapidCometBubblesWhite.part
		
		bhvr	behaviors/projectile1.bhvr
		Magnet	Target
		LookAt  Target
	
	
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		100
			PhysForcePowerJitter	50
		End
	End

	Event
		Ename	PushForce
		Type	Local
		At	PushDirection
		
		BhvrOverride
			PositionOffset		0 0 5
			pyrRotate		15 0 0
			PhysForceType		Forward
			PhysForceRadius		10
			PhysForcePower		200
			PhysForcePowerJitter	50
		End
		

		LifeSpan	1

	End

	Event
		EName	cometParts1
		Type	Local
		At	Prime1
		Part1	:RapidCometBubbles.part
		Part2	:RapidCometBubblesBright.part
		Part3	:RapidCometBubblesLocal.part
		#Part4	:RapidCometHead.part
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
		#Part1	:RapidCometHead.part
		#Part2	:RapidBlastStreak.part
		Part3	:RapidCometBubblesWhite.part
		bhvr	behaviors/projectile1.bhvr
		Magnet	Target
		LookAt  Target
	
	
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		100
			PhysForcePowerJitter	50
		End
	End

	Event
		Ename	PushForce
		Type	Local
		At	PushDirection
		
		BhvrOverride
			PositionOffset		0 0 5
			pyrRotate		15 0 0
			PhysForceType		Forward
			PhysForceRadius		10
			PhysForcePower		200
			PhysForcePowerJitter	50
		End
		

		LifeSpan	1

	End

	Event
		EName	cometParts2
		Type	Local
		At	Prime2
		Part1	:RapidCometBubbles.part
		Part2	:RapidCometBubblesBright.part
		Part3	:RapidCometBubblesLocal.part
		#Part4	:RapidCometHead.part
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
		#Part1	:RapidCometHead.part
		#Part2	:RapidBlastStreak.part
		Part3	:RapidCometBubblesWhite.part
		
		bhvr	behaviors/projectile1.bhvr
		Magnet	Target
		LookAt  Target
	
	
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		100
			PhysForcePowerJitter	50
		End
	End

	Event
		Ename	PushForce
		Type	Local
		At	PushDirection
		
		BhvrOverride
			PositionOffset		0 0 5
			pyrRotate		15 0 0
			PhysForceType		Forward
			PhysForceRadius		10
			PhysForcePower		200
			PhysForcePowerJitter	50
		End
		

		LifeSpan	1

	End

	Event
		EName	cometParts3
		Type	Local
		At	Prime3
		Part1	:RapidCometBubbles.part
		Part2	:RapidCometBubblesBright.part
		Part3	:RapidCometBubblesLocal.part
		#Part4	:RapidCometHead.part
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
		#Part1	:RapidCometHead.part
		#Part2	:RapidBlastStreak.part
		Part3	:RapidCometBubblesWhite.part
		bhvr	behaviors/projectile1.bhvr
		Magnet	Target
		LookAt  Target
	
	
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		100
			PhysForcePowerJitter	50
		End
	End

	Event
		Ename	PushForce
		Type	Local
		At	PushDirection
		
		BhvrOverride
			PositionOffset		0 0 5
			pyrRotate		15 0 0
			PhysForceType		Forward
			PhysForceRadius		10
			PhysForcePower		200
			PhysForcePowerJitter	50
		End
		

		LifeSpan	1

	End

	Event
		EName	cometParts4
		Type	Local
		At	Prime4
		Part1	:RapidCometBubbles.part
		Part2	:RapidCometBubblesBright.part
		Part3	:RapidCometBubblesLocal.part
		#Part4	:RapidCometHead.part
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
	
#	Event
#		Ename	2ndBlast
#		Type	Start
#		At	origin
#		ChildFx POWERS/EnergyBlast/Rapid2.fx
#		Magnet	Target
		

#	End

#End

End			