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
		At	WepL
		Sound fireblastfly 70 70 .7
		Sound fireballfly2 70 70 .7
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
		#Part	:MouthFireRight.part
		#Part	:MouthFireLeft.part
	End
	
	Event
		EName 	EyeL
		Type	local
		At	Prime
		altpiv	3
		
		Part	:PumpkinEyes.part
		Part	:PumpkinPupil.part
		#Part	:MouthFireRight.part
		#Part	:MouthFireLeft.part
	End

End

Condition
	On	PrimeHit
			
	Event
		Ename	FireBallHit
		Type	local
		At	T_Root
		Bhvr	Behaviors/OffsetHips.bhvr
		Part	:AOEHitRing.part
		Part	:BigFireExplosion.part
		Part	:FireBallHitSpark.part
	End

	Event
		Ename	All
		Type	Destroy
	End

End

End	


			