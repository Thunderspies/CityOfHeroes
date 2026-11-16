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
	InpName	BendMystic
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
	Time	16
	

	Event
		EName	core
		Type	local
		At	BendMystic
		geom	CoreWolfPack
		bhvr	behaviors/BlastScale3.bhvr
		Sound Powerpush 100.0 100.0 .9
		
	End

End


Condition
	On 	Time
	Time 	18
	
	Event
		EName 	HandGlowLeftz
		Type	local
		At	WepL
		
		Part3	:CometHandStarRed1.part
		Part4	:CometHandCloudsRed1.part
	
	End

	
	Event
		EName 	HandGlowLeft1
		Type	local
		At	WepL
		Part1	:RedPushBubblesWhite.part
		Part2	:RedPushBubbles.part
		
	
	End
	
	Event
		EName 	HandGlowLeftz2
		Type	local
		At	WepR
		
		Part3	:CometHandStarRed1.part
		Part4	:CometHandCloudsRed1.part
	
	End

	
	Event
		EName 	HandGlowLeft12
		Type	local
		At	WepR
		Part1	:RedPushBubblesWhite.part
		Part2	:RedPushBubbles.part
		
	
	End

	
End

Condition
	On 	Time
	Time 	37
	
	
	Event
		EName 	HandGlowFlashL
		Type	local
		At	WepL
		Part1	:HandGlowWhite3.part
		Part2	:HandGlowEmber3a.part
		
	
	End

	Event
		EName 	HandGlowFlashL2
		Type	local
		At	WepR
		Part1	:HandGlowWhite3.part
		Part2	:HandGlowEmber3a.part
		
	
	End
	
End
###################################################################
##PushEffect Projectile
##################################################################

Condition
	On	Time
	Time	30


	Event
		EName	Prime
		Type	start
		At	BendMystic
		geom	Push
		
		Part2	:CometBubblesRedCenter.part
		bhvr	behaviors/RedPushProjectile.bhvr
		Magnet	Target
		LookAt  Target
	
	End

	Event
		Ename	PushForce
		Type	Local
		At	Prime
		
		BhvrOverride
			pyrRotate		10 0 0
			PhysForceType		Forward
			PhysForceRadius		20
			PhysForcePower		400
			PhysForcePowerJitter	100
		End
		

	End

	
	
	Event
		EName	hookup1
		Type	local
		At	Prime
		Altpiv	1
		
		Anim	FX_dark
		Bhvr	Behaviors/PushAnimScale.bhvr
	End
	
	Event
		Type	local
		At	hookup1
		 AnimPiv Chest
		
		Part1	:PowerPushStreakRed.part
		Part3	:CometBubblesRedCenter.part
		Part3	:CometBubblesWhiteCenter2.part
		Part4	:TorrentenergyBubblesRed2.part
		Part5	:CometBubbles3Red.part
	End

	Event
		Type	local
		At	hookup1
		 AnimPiv Neck
		
		Part1	:PowerPushStreakRed.part
		Part3	:CometBubblesRedCenter.part
		Part3	:CometBubblesWhiteCenter2.part
		Part4	:TorrentenergyBubblesRed2.part
		Part5	:CometBubbles3Red.part
	End

	Event
		Type	local
		At	hookup1
		 AnimPiv Waist
		
		Part1	:PowerPushStreakRed.part
		Part3	:CometBubblesRedCenter.part
		Part3	:CometBubblesWhiteCenter2.part
		Part4	:TorrentenergyBubblesRed2.part
		Part5	:CometBubbles3Red.part
	End



	Event
		EName	hookup2
		Type	local
		At	Prime
		Altpiv	2
		
		Anim	FX_dark
		Bhvr	Behaviors/PushAnimScale.bhvr
	End
	
	Event
		Type	local
		At	hookup2
		 AnimPiv Chest
		
		Part1	:PowerPushStreakRed.part
		Part3	:CometBubblesRedCenter.part
		Part3	:CometBubblesWhiteCenter2.part
		Part4	:TorrentenergyBubblesRed2.part
		Part5	:CometBubbles3Red.part
	End

	Event
		Type	local
		At	hookup2
		 AnimPiv Neck
		
		Part1	:PowerPushStreakRed.part
		Part3	:CometBubblesRedCenter.part
		Part3	:CometBubblesWhiteCenter2.part
		Part4	:TorrentenergyBubblesRed2.part
		Part5	:CometBubbles3Red.part
	End

	Event
		Type	local
		At	hookup2
		 AnimPiv Waist
		
		Part1	:PowerPushStreakRed.part
		Part3	:CometBubblesRedCenter.part
		Part3	:CometBubblesWhiteCenter2.part
		Part4	:TorrentenergyBubblesRed2.part
		Part5	:CometBubbles3Red.part
	End

End

#################################################################################
Condition
	On	Time
	Time	20
	Event
		EName	cometPartsLocal
		Type	Local
		At	core
		
		altpiv	1
		Part5	:EnergyCoreLocalRed.part
		Part1	:CoreEnergyRed.part
		Part2	:CoreEnergyBeamRed.part
				
	End

	Event
		EName	cometBalls
		Type	Local
		At	core
		
		altpiv	1
	
		Part3	:CometBubbles2Red.part
		Part4	:CometBubbles3Red.part

		
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
		EName 	HandGlowRight1
		Type	Destroy
		

	End

	Event
		EName 	HandGlowLeft1
		Type	Destroy
		

	End

	Event
		EName 	HandGlowLeftz
		Type	Destroy
		

	End

	Event
		EName 	HandGlowRight12
		Type	Destroy
		

	End

	Event
		EName 	HandGlowLeft12
		Type	Destroy
		

	End

	Event
		EName 	HandGlowLeftz2
		Type	Destroy
		

	End
End

Condition
	On 	Time
	Time 	50
	
	Event
		EName 	cometPartsLocal
		Type	Destroy
		

	End

End

Condition
	On 	Time
	Time 	30

	Event
		EName 	cometBalls
		Type	Destroy
		

	End
	
End

Condition
	On 	Time
	Time 	42
	
	Event
		EName 	Core
		Type	Destroy
		

	End

	
End



End			