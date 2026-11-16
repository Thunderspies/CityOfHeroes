#########################################################
##	FireAura
FxInfo
Lifespan	200

########################################################################################################

Condition
	On 	Time 
	Time 	0
	
	Event
		EName	Eye
		Type	Local
		At	Chest
		BHVR	Behaviors/SentinelOffsetEye.bhvr
		Sound ignite2 130 130 .66
		Lifespan	200
	End

	Event
		Type	Local 
		At	chest
			
		Part1	:BlastStreakGlowHead.part
		Part2	:RadiationHandElectronsLocal2.part
		Part3	:InfectBubbleRingssHandsStreak.part
		Part5	:InfectBubbleDotsHandsStreak.part
		#Sound	firesword_loop 120 100 .42
		LifeSpan	90	#15
	End

End

Condition
	On 	Time
	Time 	40


	Event
		Type	Local 
		At	chest
			
		
		Part	:Flash.part
		
		LifeSpan	25	#15
	End

	Event
		Ename	Prime
		Type	Start 
		At	chest
		Bhvr	behaviors/SentinelProjectile.bhvr
		Magnet	Target
		LookAt	Target
		Part1	:Streak.part
		Part2	:Streak.part
		Part3	:Streak.part
		Part4	:Streak.part
		Part5	:Streak.part
		Sound neutrino1 140 140 .89
		Lifespan	200
	End

	Event
		Ename	Prime1
		Type	Start 
		At	chest
		Bhvr	behaviors/SentinelProjectile.bhvr
		Magnet	Target
		LookAt	Target

		Part	:Streak.part		
		Part	:BlastStreakGlow.part
		Part	:DotsStreak.part	
		Lifespan	200
	End

	
End


Condition
	On 	PrimeHit
	
	Event
		Ename 	Prime
		Type	Destroy
	End

End

Condition
	On 	Prime1Hit
	
	Event
		Ename 	Prime1
		Type	Destroy
	End

End

End				