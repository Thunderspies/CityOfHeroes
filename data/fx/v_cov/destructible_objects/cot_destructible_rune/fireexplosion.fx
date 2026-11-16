#########################################################
##	onfire
##
FxInfo  

LifeSpan	120

Condition
	On 	Time
	Time 	0


	Event
		Ename	hookup
		Type	local
		At	root

		Bhvroverride
			PositionOffset	0 5 0
			FadeOutLength	220
		End

	End
End

LifeSpan	120

Condition
	On 	Time
	Time 	0


#	Event
#		EName   RadiusFxScalex
#		Type	start
#		At	hookup
#		Bhvr	powers/firecontrol/FireShield.bhvr
#		LifeSpan	50
#	End
#
#	Event
#		Type	start 
#		At	RadiusFxScalex
#		bhvr    behaviors/powers/firecontrol/Infernobubble4.bhvr
#		Part1	powers/firecontrol/FireShieldCloud01B.part
#		Part2	powers/firecontrol/FireShieldCloud02B.part
#		Part4	Explosions\FireExplosion\FireShieldSparksWhite.part
#		
#		LifeSpan	30
#		
#	End

	Event
		EName 	Hook
		Type	start 
		At	hookup
		Geom	centerdummy

	End

	Event
		Type	start
		At	hookup
		Part1	powers/firecontrol/FireBallTargetSteam.part
		Part2	powers/firecontrol/FireBallTargetFlame2.part
		Part3	powers/firecontrol/FireBallSpark.part	
		Sound fireexplo3 80 80 .86
		Sound fireexplo4 80 80 .86
		Sound fireexplo5 80 80 .9
		
		LifeSpan	10
			
	End


	Event
		Type	start 
		At	hookup
		
		Part1	powers/firecontrol/InfernoCare.part
		Part2	powers/firecontrol/Inferno.part
		Part	Explosions\FireExplosion\ExplosionStar.part
		Part2	Explosions\FireExplosion\FlatInferno.part
		Part	Explosions\FireExplosion\FlatExplosionStar.part
		Part	Explosions\FireExplosion\ExplosionlightStreams.part
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
		At	hookup
		
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
		At	hookup
		Part1	Generic\InfernorockBits.part
		Part2	Generic\InfernorockBitz.part
		#Sound	FireExplo 100.0 30.0 .8	
		LifeSpan	5
	End

End

Condition
	On 	time
	Time 	0
	Repeat 	24

	Event 
		Type 	start
		At 	hookup  #RadiusFxScale
		ChildFX V_COV\PhysicsEnabled\HotSparkExplode.fx
		Lifespan 60
		Lifespanjitter 	20
	End
	
	Event 
		HardwareOnly
		Type 	start
		At 	hookup  #RadiusFxScale
		ChildFX V_COV\PhysicsEnabled\HotSparkExplode.fx
		Lifespan 60
		Lifespanjitter 	20
	End	

	Event 
		HardwareOnly
		Type 	start
		At 	hookup  #RadiusFxScale
		ChildFX V_COV\PhysicsEnabled\HotSparkOUT.fx
		Lifespan 60
		Lifespanjitter 	20
	End
End

Condition
	Time 	3
	Chance	.8

	Event
		Type	start
		At	hookup

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
		Part1	powers/firecontrol/fire.part
		Part1	powers/firecontrol/fire.part
		Lifespan	75
		LifespanJitter	15
	End

End

Condition
	Time 	3
	Chance	.8

	Event
		Type	start
		At	hookup

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
		Part1	powers/firecontrol/fire.part
		Part1	powers/firecontrol/fire.part
		Lifespan	75
		LifespanJitter	15
	End

End

Condition
	Time 	3
	Chance	.8

	Event
		Type	start
		At	hookup

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
		Part1	powers/firecontrol/fire.part
		Part1	powers/firecontrol/fire.part
		Lifespan	75
		LifespanJitter	15
	End

End

Condition
	Time 	3
	Chance	.8

	Event
		Type	start
		At	hookup

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
		Part1	powers/firecontrol/fire.part
		Part1	powers/firecontrol/fire.part
		Lifespan	75
		LifespanJitter	15
	End

End



Condition
	Time 	1
	Random	1
	Repeat	8
	RepeatJitter 1

	Event
		HardwareOnly
		Type	start
		At	hookup

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
		Part1	powers/firecontrol/fire.part
		Part1	powers/firecontrol/fire.part
		Lifespan	75
		LifespanJitter	15
	End

	Event
		HardwareOnly
		Type	start
		At	hookup

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
		Part1	powers/firecontrol/fire.part
		Part1	powers/firecontrol/fire.part
		Lifespan	75
		LifespanJitter	15
	End

	Event
		HardwareOnly
		Type	start
		At	hookup

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
		Part1	powers/firecontrol/fire.part
		Part1	powers/firecontrol/fire.part
		Lifespan	75
		LifespanJitter	15
	End

	Event
		HardwareOnly
		Type	start
		At	hookup

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
		Part1	powers/firecontrol/fire.part
		Part1	powers/firecontrol/fire.part
		Lifespan	75
		LifespanJitter	15
	End

End

End