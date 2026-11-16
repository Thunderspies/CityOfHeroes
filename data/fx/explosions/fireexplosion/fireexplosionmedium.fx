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

LifeSpan	120

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
		Type	start 
		At	RadiusFxScalex
		bhvr    behaviors/powers/firecontrol/Infernobubble4.bhvr
		Part1	powers/firecontrol/FireShieldCloud01B.part
		Part2	powers/firecontrol/FireShieldCloud02B.part
		Part4	Explosions\FireExplosion\FireShieldSparksWhite.part
		
		LifeSpan	30
		
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
		Sound fireexplo3 80 80 .86
		Sound fireexplo4 80 80 .86
		Sound fireexplo5 80 80 .9
		
		LifeSpan	10
			
	End


	Event
		Type	start 
		At	Root
		
		Part1	:InfernoCare.part
		Part2	:Inferno.part
		Part	:ExplosionStar.part
		Part2	:FlatInferno.part
		Part	:FlatExplosionStar.part
		Part	:ExplosionlightStreams.part
		LifeSpan	5
		

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
			PhysForceRadius		25
			PhysForcePower		300
			PhysForcePowerJitter	50
		End

		Lifespan	1

	End

End

Condition
	On 	Time
	Time 	3

	Event
		Type	start 
		At	Root
		Part1	Generic\InfernorockBits.part
		Part2	Generic\InfernorockBitz.part
		#Sound	FireExplo 100.0 30.0 .8	
		LifeSpan	5
	End

End

Condition
	On 	time
	Time 	0
	Repeat 	16

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
	Time 	3
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
	Time 	3
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
	Time 	3
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
	Time 	3
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
	Repeat	3
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

#Condition
#	On 	Time
#	Time	9
#
#	Event
#		EName 	Prime1
#		Type	Destroy
#	End
#
#End
#
#Condition
#	On 	Time
#	Time	3
#
#	Event
#		EName 	Prime2
#		Type	Destroy
#	End
#
#End
#
#
#Condition
#	On 	Time
#	Time	7
#
#	Event
#		EName 	Prime
#		Type	Destroy
#	End
#
#End	
#
#Condition
#	On 	Time
#	Time 	50
#
#	Event
#		EName 	RadiusFxScale
#		Type	Destroy 
#	End
#
#
#End
#
#Condition
#	On 	Time
#	Time 	120
#
#	Event
#		EName 	All
#		Type	Destroy 
#	End
#
#
#End
#

End			