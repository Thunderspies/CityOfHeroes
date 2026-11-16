#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	200


Condition
	On	Time	
	Time	38

	Event
		Type	Local
		At	WepR
		Sound fireblastfly 100 100 .88
		Sound fireballfly2 100 100 .88
	End
End


Condition
	On 	Time
	Time 	42

	Event
		EName 	Prime
		Type	Start 
		At	WepR
		Geom	PumpkinHead
		Bhvr	:projectile1.bhvr
		Magnet	T_Chest
		LookAt	T_Chest
		Part	:FireBallHead.part
		Part	:FireBallTail.part
		#Part	:FireBallStreak.part
		Part	:SparkTrail.part
	End

	Event
		EName 	EyeL
		Type	local
		At	Prime
		altpiv	2
		
		Part	:PumpkinEyes.part
		Part	:PumpkinPupil.part
	End
	
	Event
		EName 	EyeL
		Type	local
		At	Prime
		altpiv	3
		
		Part	:PumpkinEyes.part
		Part	:PumpkinPupil.part
		
	End

End

Condition
	On	PrimeHit
			
	Event
		Ename	FireBallHit
		Type	local
		At	T_Chest
		#Part	:AOEHitRing.part
		Part	:BigFireExplosion.part
		Part	:FireBallHitSpark.part
	End

	Event
		Ename	All
		Type	Destroy
	End

End

End	


			