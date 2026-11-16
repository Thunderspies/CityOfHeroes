#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

LifeSpan	200

Input  
	InpName	Origin
End

Input  
	InpName	Target
End    

Input  
	Inpname	Head
End


#########################################################

Condition
	On 	Time
	Time 	25

	Event
		EName	Prime
		Type	start
		At	LarmL

		Bhvr	:GreenGlassScale.bhvr
		Geom	PoisonGlassBall
		Magnet	Target
		LookAt	Target
		
		Part	:LiquidTrailBlackQuick.part
		Part	:LiquidTrailBlackQuick.part
		Part	:LiquidTrailBlackQuick.part
		Part	:LiquidTrail.part
		Part	:LiquidTrailBlack.part
		Part	:LiquidTrail.part
		Part	:LiquidTrailBlack.part
		Part	:LiquidTrailBlack.part

		Part	:LiquidTrailQuick.part
		Part	:LiquidTrailQuick.part
		Part	:LiquidTrailQuick.part
		Part	:LiquidTrailQuick.part
		Part	:LiquidTrailQuick.part
		Part	:LiquidTrailQuickSmallThin.part
		Part	:LiquidTrailQuickSmallThin.part

	#		Part	:QuillTrailStreak.part

	#		Part	:QuillTrail.part
	#		Part	:QuillTrailLight.part

	#		Part2	weaponFx/QuillTrailSpecks.part
	#		Part3	weaponFx/QuillBubblesTrail.part
		Part5	:SmallBallzTrail.part
		Part5	:SmallBallzTrailBlack.part
		Part5	:SmallFallingDrips.part
		Part5	:SmallFallingDripsBlack.part
		Sound Palmpoisonblast 80 80 .8

	End

	Event
		EName	Prime2
		Type	local
		At	Prime					
		
		Part	:PoisonHeadLocalBlack.part
		Part	:PoisonHeadLocalBlack.part
		Part	:PoisonHeadLocalBlack.part
		Part	:PoisonHeadLocal.part
		Part	:PoisonHeadLocal.part
		Part	:PoisonHeadLocal.part
		Part	:PoisonHeadLocal.part
		
	End

	
End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
		

	End

	Event
		EName 	Prime2
		Type	Destroy
		

	End
End

End				