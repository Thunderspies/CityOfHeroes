#########################################################
##	cot_waterfall_med.fx
##
FxInfo

LifeSpan	180


Input  
	InpName	Origin
End


Input  
	InpName	Target
End

Input  
	InpName	WepR
End

Input  
	InpName	Mystic
End

###########################################################


Condition
	On	Time
	Time	0
	

	Event
		EName 	HandGlowRight1
		Type	local
		At	Mystic
		Part1	Powers/EnergyBlast/CometHandBubblesRedSolid.part
		Part2	Powers/EnergyBlast/CometHandBubblesRed.part
		Part3	Powers/EnergyBlast/CometHandBubblesBlack.part
		Sound mekblast 90.0 90.0 .76
		PMagnet	WepR

		LifeSpan	40
	End

End

###################################################################

Condition
	On 	Time
	Time 	25
	Event
		EName 	HandGlowRight22
		Type	local
		At	Mystic
		Part1	Powers/EnergyBlast/xHandGlowWhitea.part
		Part2	Powers/EnergyBlast/xHandGlowEmbera.part
		PMagnet	WepR

		LifeSpan	15
	End


End

###################################################################

####################################################################################################

Condition
	On	Time
	Time	30


	Event
		EName	Prime
		Type	start
		At	mystic
		
		Part3	Powers/EnergyBlast/RapidCometBubblesWhite.part
		Part2	Powers/EnergyBlast/RapidCometBubblesBlack.part

		bhvr	behaviors/projectile1.bhvr
		Magnet	Target
		LookAt  Target
	
	End

	Event
		EName	cometParts
		Type	Local
		At	Prime
		Part1	Powers/EnergyBlast/RapidCometBubblesRed.part
		Part	Powers/EnergyBlast/RapidCometBubblesBlack2.part
		Part	Powers/EnergyBlast/RapidCometBubblesBrightRed.part
		Part2	Powers/EnergyBlast/RapidCometBubblesBrightBlack.part
		Part3	Powers/EnergyBlast/RapidCometBubblesLocalRed.part
		Part5	Powers/EnergyBlast/RapidEnergyCoreLocalRed.part
		Part	Powers/EnergyBlast/RapidEnergyCoreLocalBlack.part
	End

End


Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
		

	End

End

End			