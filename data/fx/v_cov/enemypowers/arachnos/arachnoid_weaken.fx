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
	On	Time
	Time	1
	
	Event
		EName	Emitter
		Type	Local
		At	Head
		
		BhvrOverride
			Behavior
			PositionOffset	0 -.085 .55
		End

#		Geom	Testing_Target
	End
End


Condition
	On 	Time
	Time 	25

	Event
		EName	Prime
		Type	start
		At	Emitter

		Bhvr	V_COV\Powers\Poison\GreenGlassScale.bhvr
		BhvrOverride
			Behavior
			PositionOffset	0 -.3 0
		End

		Geom	PoisonGlassBall
		Magnet	Target
		LookAt	Target
		
		Part	V_COV\Powers\Poison\LiquidTrailBlackQuick.part
		Part	V_COV\Powers\Poison\LiquidTrailBlackQuick.part
		Part	V_COV\Powers\Poison\LiquidTrailBlackQuick.part
		Part	V_COV\Powers\Poison\LiquidTrail.part
		Part	V_COV\Powers\Poison\LiquidTrailBlack.part
		Part	V_COV\Powers\Poison\LiquidTrail.part
		Part	V_COV\Powers\Poison\LiquidTrailBlack.part
		Part	V_COV\Powers\Poison\LiquidTrailBlack.part

		Part	V_COV\Powers\Poison\LiquidTrailQuick.part
		Part	V_COV\Powers\Poison\LiquidTrailQuick.part
		Part	V_COV\Powers\Poison\LiquidTrailQuick.part
		Part	V_COV\Powers\Poison\LiquidTrailQuick.part
		Part	V_COV\Powers\Poison\LiquidTrailQuick.part
		Part	V_COV\Powers\Poison\LiquidTrailQuickSmallThin.part
		Part	V_COV\Powers\Poison\LiquidTrailQuickSmallThin.part

	#		Part	V_COV\Powers\Poison\QuillTrailStreak.part

	#		Part	V_COV\Powers\Poison\QuillTrail.part
	#		Part	V_COV\Powers\Poison\QuillTrailLight.part

	#		Part2	weaponFx/QuillTrailSpecks.part
	#		Part3	weaponFx/QuillBubblesTrail.part
		Part5	V_COV\Powers\Poison\SmallBallzTrail.part
		Part5	V_COV\Powers\Poison\SmallBallzTrailBlack.part
		Part5	V_COV\Powers\Poison\SmallFallingDrips.part
		Part5	V_COV\Powers\Poison\SmallFallingDripsBlack.part

	End

	Event
		EName	Prime2
		Type	local
		At	Prime					
		
		Part	V_COV\Powers\Poison\PoisonHeadLocalBlack.part
		Part	V_COV\Powers\Poison\PoisonHeadLocalBlack.part
		Part	V_COV\Powers\Poison\PoisonHeadLocalBlack.part
		Part	V_COV\Powers\Poison\PoisonHeadLocal.part
		Part	V_COV\Powers\Poison\PoisonHeadLocal.part
		Part	V_COV\Powers\Poison\PoisonHeadLocal.part
		Part	V_COV\Powers\Poison\PoisonHeadLocal.part
		
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