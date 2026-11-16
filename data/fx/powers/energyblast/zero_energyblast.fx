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
	InpName	chest
End

LifeSpan	200

###########################################################

Condition
	On	Time
	Time	0

	

	Event
		EName	Prime
		Type	start
		At	chest
		geom	CenterDummy
		Part2	:CometBubblesWhitex.part
		Part3	:CometBubblesBlack.part
		bhvr	behaviors/projectile1.bhvr
		Magnet	Target
		LookAt  Target
	
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		12
			PhysForcePower		200
			PhysForcePowerJitter	50
		End
	End

	Event
		Ename	PushForce
		Type	Local
		At	Prime
		
		BhvrOverride
			PositionOffset		0 0 3
			pyrRotate		15 0 0
			PhysForceType		Forward
			PhysForceRadius		12
			PhysForcePower		200
			PhysForcePowerJitter	50
		End
		
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


End			