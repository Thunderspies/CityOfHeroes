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
		Sound molotovhit1 80 80 .86
		Sound fireexplo 80 80 .72
		Sound firezap 80 80 .76
		Sound molotovhit1 80 80 .86
		LifeSpan	10

	End

	Event
		Type	start
		At	Root
		Part1	:Reichsman_TargetSteam.part
		Part2	:Reichsman_Flame2.part
		Part1	:Reichsman_FlatTargetSteam.part
		Part2	:Reichsman_FlatFlame2.part
		Part3	:Reichsman_Spark.part
		LifeSpan	70

	End

	Event
		Type	start
		At	Root

		Part	Powers/ElectricityControl/DarkStarCore.part
		Part	V_COV/EnemyPowers/Reichsman/SuperStrengthHitEmber.part
		Part2	:DarkInferno.part
		Part2	:DarkFlatInferno.part
		LifeSpan	7


	End

	Event
		Type	start
		At	Root

		Part	:Reichsman_ExplosionLightStreams.part
		LifeSpan	7


	End


	Event
		Ename	ExplosiveForce
		Type	Start
		At	Root

		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		20
			PhysForcePower		500
			PhysForcePowerJitter	50
		End

		Lifespan	1

	End

End

Condition

	Event
		Type	start
		At	Root
		Part1	:InfernoRockBits.part
		LifeSpan	5
	End

End


Condition
	On 	Time
	Time 	0
	Repeat 	4

	Event
	#	HardwareOnly
		Type 	start
		At 	Root  #RadiusFxScale
		ChildFX V_COV/PhysicsEnabled/Reichsman_ElectricSpark.fx
		Lifespan 60
		Lifespanjitter 	20
	End

End

Condition
	Time 	1
	Random	1
	Repeat	2
	RepeatJitter 1

	Event
		HardwareOnly
		Type	start
		At	Root

		Bhvr	Behaviors/StartOffset.bhvr
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
		CEvent 	V_COV/PhysicsEnabled/Reichsman_FireNapalmHit.fx
		CThresh 	.001
		CDestroy 	0
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
		CEvent 	V_COV\PhysicsEnabled\Reichsman_FireNapalmHit.fx
		CThresh 	.001
		CDestroy 	0
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
		CEvent 	V_COV\PhysicsEnabled\Reichsman_FireNapalmHit.fx
		CThresh 	.001
		CDestroy 	0
		Part1	:fireAdd.part
		Part1	:fireSub.part
		Lifespan	75
		LifespanJitter	15
	End

	Event
	#	HardwareOnly
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
		CEvent 	V_COV\PhysicsEnabled\Reichsman_FireNapalmHit.fx
		CThresh 	.001
		CDestroy 	0
		Part1	:fireAdd.part
		Part1	:fireSub.part
		Lifespan	75
		LifespanJitter	15
	End

End

End