#########################################################
##	car on fire
#########################################################

FxInfo  

#Lifespan 180


########  Main Fire Particles   #########################

Condition
	On 	Time
	Time 	0


	Event
		EName 	base
		Type	Posit
		At	Chest
		BhvrOverride
			PositionOffset	0 0 0
		End
		
	End



	Event
		EName 	Sound
		Type	Posit
		At	base	
#		Sound	molotovhit1 80 30 .86
#		#Sound	fireexplo 80 30 .72
#		#Sound	firezap 80 30 .76
#		#Sound	molotovhit1 80 30 .86
#		Sound	grexplo2 120.0 30.0 1.0

		LifeSpan	10
			
	End

	Event
		EName 	Fire
		Type	local
		At	base
		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part
		ChildFX	small_fire.fx

		#LifeSpan	20
	
	End

	Event
		EName 	Smoke
		Type	local
		At	base
		
		Part	:SmokeExplosion.part
		
		#Lifespan 15
	End
End

########   Generic Explosion Physics Particles   ###################
 
Condition
	On 	Cycle
	Time 	3
	Chance	.1

	RepeatJitter 	1

	Event 
		EName 	Sparkz
		Type 	Start
		At 	base
		ChildFX V_COV\PhysicsEnabled\HotSparkUP.fx
		Lifespan 60
	End
End


Condition
	On 	Cycle
	Time 	4
	Chance	.15

	RepeatJitter	1

	Event 
		EName 	napalmDrip
		Type 	Start
		At 	base
		Bhvr	V_COV\PhysicsEnabled\FireBurstScatter.bhvr
		BhvrOverride
			InitialVelocity		0.0 0.0 0.0
			InitialVelocityJitter	0.3 0.2 0.3	 
			Drag			.04
		End
		Part1	V_COV\PhysicsEnabled\FireSplash.part
		Part2	V_COV\PhysicsEnabled\FireSplash.part
		Part3	V_COV\PhysicsEnabled\FireSplashEmbers.part
		
		CEvent 	V_COV\PhysicsEnabled\FireNapalmHit.fx
		CThresh 	.001
		CDestroy 	0
		
		Lifespan	90
		LifespanJitter	30
	End
End


End			