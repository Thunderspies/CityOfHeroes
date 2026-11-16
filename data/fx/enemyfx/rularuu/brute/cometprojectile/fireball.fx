#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	200

Condition
	Event
		EName	InitialSwordFire
		Type	Local 
		At	Head
		
		
		Sound Fireball1 100.0 100.0 .8
		LifeSpan 45
	End
End

Condition
	On 	Time
	Time 	10
	Event
		EName	SwordFlash2
		Type	Local
		At	Hair
		BHVR	Behaviors/OffsetCrystal.bhvr
		Part1	:Flash.part
		LifeSpan 14
	End

	Event
		Type	posit
		At	SwordFlash2
		
		#BHVR	Behaviors/OffsetCrystal.bhvr
		Part	:StreaksMagnet.part
		Part	:DarkStreaksBlueMagnet.part
		Part	:DarkStreaksRedMagnet.part
		Part	:DarkStreaksMagnet.part

		Magnet	Target
		LookAt	Target

		LifeSpan 14
	End
End

Condition
	On 	Time
	Time 	16

	Event
		EName 	Prime
		Type	Start 
		At	Hair
		Bhvr	behaviors/projectile1.bhvr
		Magnet	Target
		LookAt	Target

		Part	:DarkStreaks.part
		Part	:CometRings.part
		Part	:CometRingsThin.part

		Part	:DarkBallHead.part
		Part	:DarkBallTail.part
		Part	:DarkBallStreak.part
		
	End

	Event
		EName 	Prime1
		Type	Start 
		At	Hair
		Bhvr	behaviors/projectile1.bhvr
		Magnet	Target
		LookAt	Target

		Part	:DarkStreaksBlue.part
		Part	:DarkStreaksRed.part
		#Part	:SparkTrail.part
		#Part	:SparkTrail2.part
		#Part	:BigSparkTrail.part
		#Part	:BigSparkTrail2.part
		#Part	:BlackSparkTrail.part
	End
End

Condition
	On	PrimeHit
			
	Event
		Ename	FireBallHit
		Type	local
		At	target

		
		#Part	:DarkStreaks.part
		Part	:BigDarkExplosion.part
		Part	:DarkHitspark.part
		Part	:DarkHitspark2.part
	End

	Event
		Ename	Prime
		Type	Destroy
	End

	Event
		Ename	Prime1
		Type	Destroy
	End

End

End	


			