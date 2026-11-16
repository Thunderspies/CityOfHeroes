#########################################################
##	onfire
##
FxInfo  

Input  
	InpName	hips
End

Input  
	Inpname	chest
End

Input  
	Inpname	root
End

LifeSpan	90

Condition
	On 	Time
	Time 	0


	Event
		EName   RadiusFxScalex
		Type	start
		At	root
		Bhvr	powers/firecontrol/FireShield.bhvr
		LifeSpan	50
	End

	Event
		EName 	Hook
		Type	start 
		At	root
		Geom	centerdummy

	End

	Event
		Type	start
		At	Root
		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part	
		Sound fireexplo6 80 80 .86
		Sound fireexplo7 80 80 .92
		LifeSpan	10
			
	End

	Event
		Type	start 
		At	Root
		
		Part	:BlueExplosionStar.part
#		Part1	:InfernoCare.part
		Part2	:BlueInferno.part
		Part2	:BlueFlatInferno.part
		LifeSpan	7
		

	End

	Event
		Type	start 
		At	Root
		
		Part	:ExplosionlightStreams.part
		LifeSpan	25
		

	End

	Event
		EName 	RadiusFxScale
		Type	start 
		At	Hook
		altpiv	1
		bhvr    behaviors/powers/firecontrol/Infernobubble.bhvr
		#Part1	powers/firecontrol/FireBallHitRing1.part
		#Part2	powers/firecontrol/FireBallHitRing2.part
		LifeSpan	90
		
	End

	
	Event
		Ename	ExplosiveForce
		Type	Start
		At	Root
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		30
			PhysForcePower		300
			PhysForcePowerJitter	50
		End

		Lifespan	1

	End

End

Condition
#	On 	Time
#	Time 	3

	Event
		Type	start 
		At	Root
		Part1	Generic\InfernorockBits.part
#		Part2	Generic\InfernorockBitz.part
		#Sound	FireExplo 100.0 30.0 .8	
		LifeSpan	5
	End

End

Condition
	On 	time
	Time 	0
	Repeat 	7

	Event 
		Type 	start
		At 	Root  #RadiusFxScale
		ChildFX V_COV\PhysicsEnabled\HotSparkExplode.fx
		Lifespan 60
		Lifespanjitter 	20
	End

		
	Event 
		HardwareOnly
		Type 	start
		At 	Root  #RadiusFxScale
		ChildFX V_COV\PhysicsEnabled\HotSparkExplode.fx
		Lifespan 60
		Lifespanjitter 	20
	End	

#	Event 
#		HardwareOnly
#		Type 	start
#		At 	Root  #RadiusFxScale
#		ChildFX V_COV\PhysicsEnabled\HotSparkOUT.fx
#		Lifespan 60
#		Lifespanjitter 	20
#	End
End

Condition
#	Time 	3
	Chance	.8

	Event
		Type	start
		At	Root

		Bhvr	Behaviors\StartOffset.bhvr
		BhvrOverride
			PositionOffset	0 1 0
			Drag			0.003
			physics 		1
			physRadius 		0.3
			physGravity 		.75
			physRestitution 	0.25
			physSFriction 	 	0.3
			physDFriction 	 	0.5
		End
		CEvent 	V_COV\PhysicsEnabled\FireNapalmHit.fx
		CThresh 	.001
		CDestroy 	0
		Part1	:fire.part
		Part1	:fire.part
		Lifespan	75
		LifespanJitter	15
	End

End

Condition
#	Time 	3
	Chance	.8

	Event
		Type	start
		At	Root

		Bhvr	Behaviors\StartOffset2.bhvr
		BhvrOverride
			PositionOffset	0 1 0
			Drag			0.003
			physics 		1
			physRadius 		0.3
			physGravity 		.75
			physRestitution 	0.25
			physSFriction 	 	0.3
			physDFriction 	 	0.5
		End
		CEvent 	V_COV\PhysicsEnabled\FireNapalmHit.fx
		CThresh 	.001
		CDestroy 	0
		Part1	:fire.part
		Part1	:fire.part
		Lifespan	75
		LifespanJitter	15
	End

End

Condition
#	Time 	3
	Chance	.8

	Event
		Type	start
		At	Root

		Bhvr	Behaviors\StartOffset3.bhvr
		BhvrOverride
			PositionOffset	0 1 0
			Drag			0.003
			physics 		1
			physRadius 		0.3
			physGravity 		.75
			physRestitution 	0.25
			physSFriction 	 	0.3
			physDFriction 	 	0.5
		End
		CEvent 	V_COV\PhysicsEnabled\FireNapalmHit.fx
		CThresh 	.001
		CDestroy 	0
		Part1	:fire.part
		Part1	:fire.part
		Lifespan	75
		LifespanJitter	15
	End

End

Condition
#	Time 	3
	Chance	.8

	Event
		Type	start
		At	Root

		Bhvr	Behaviors\StartOffset4.bhvr
		BhvrOverride
			PositionOffset	0 1 0
			Drag			0.003
			physics 		1
			physRadius 		0.3
			physGravity 		.75
			physRestitution 	0.25
			physSFriction 	 	0.3
			physDFriction 	 	0.5
		End
		CEvent 	V_COV\PhysicsEnabled\FireNapalmHit.fx
		CThresh 	.001
		CDestroy 	0
		Part1	:fire.part
		Part1	:fire.part
		Lifespan	75
		LifespanJitter	15
	End

End

Condition
	Time 	1
	Random	1
	Repeat	1
	RepeatJitter 1

	Event
		HardwareOnly
		Type	start
		At	Root

		Bhvr	Behaviors\StartOffset.bhvr
		BhvrOverride
			PositionOffset	0 1 0
			Drag			0.003
			physics 		1
			physRadius 		0.3
			physGravity 		.75
			physRestitution 	0.25
			physSFriction 	 	0.3
			physDFriction 	 	0.5
		End
		CEvent 	V_COV\PhysicsEnabled\FireNapalmHit.fx
		CThresh 	.001
		CDestroy 	0
		Part1	:fire.part
		Part1	:fire.part
		Lifespan	75
		LifespanJitter	15
	End

	Event
		HardwareOnly
		Type	start
		At	Root

		Bhvr	Behaviors\StartOffset2.bhvr
		BhvrOverride
			PositionOffset	0 1 0
			Drag			0.003
			physics 		1
			physRadius 		0.3
			physGravity 		.75
			physRestitution 	0.25
			physSFriction 	 	0.3
			physDFriction 	 	0.5
		End
		CEvent 	V_COV\PhysicsEnabled\FireNapalmHit.fx
		CThresh 	.001
		CDestroy 	0
		Part1	:fire.part
		Part1	:fire.part
		Lifespan	75
		LifespanJitter	15
	End

	Event
		HardwareOnly
		Type	start
		At	Root

		Bhvr	Behaviors\StartOffset3.bhvr
		BhvrOverride
			PositionOffset	0 1 0
			Drag			0.003
			physics 		1
			physRadius 		0.3
			physGravity 		.75
			physRestitution 	0.25
			physSFriction 	 	0.3
			physDFriction 	 	0.5
		End
		CEvent 	V_COV\PhysicsEnabled\FireNapalmHit.fx
		CThresh 	.001
		CDestroy 	0
		Part1	:fire.part
		Part1	:fire.part
		Lifespan	75
		LifespanJitter	15
	End

	Event
		HardwareOnly
		Type	start
		At	Root

		Bhvr	Behaviors\StartOffset4.bhvr
		BhvrOverride
			PositionOffset	0 1 0
			Drag			0.003
			physics 		1
			physRadius 		0.3
			physGravity 		.75
			physRestitution 	0.25
			physSFriction 	 	0.3
			physDFriction 	 	0.5
		End
		CEvent 	V_COV\PhysicsEnabled\FireNapalmHit.fx
		CThresh 	.001
		CDestroy 	0
		Part1	:fire.part
		Part1	:fire.part
		Lifespan	75
		LifespanJitter	15
	End

End

End			