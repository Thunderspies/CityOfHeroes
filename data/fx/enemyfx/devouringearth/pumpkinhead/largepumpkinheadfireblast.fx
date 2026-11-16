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
		Sound fireblastfly 160 160 .88
		Sound fireballfly2 160 160 .88
		Sound Pumpkin1 120 120 .95
		Sound Pumpkin2 120 120 .92
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
		Bhvr	:Largeprojectile1.bhvr
		Magnet	T_Chest
		LookAt	T_Chest
		Part	:LargeFireBallHead.part
		Part	:LargeFireBallTail.part
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
		Sound explohit2 80 80 .66
	End

	Event
		Ename	Prime
		Type	Destroy
	End

End

End	


			