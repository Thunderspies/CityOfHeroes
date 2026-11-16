#########################################################
##	onfire
##
FxInfo  

Lifespan 50


Condition
	On 	time
	Time 	0
	Repeat 	16

	Event 
		EName 	Sparkzz
		Type 	Local
		At 	F1_L
		ChildFX V_COV\PhysicsEnabled\HotSparkExplode.fx
		Lifespan 60
		Lifespanjitter 	20
	End

		Event 
		HardwareOnly
		EName 	Sparkz
		Type 	Local
		At 	root
		BhvrOverride
			PositionOffset	0 2 0
		End
		ChildFX V_COV\PhysicsEnabled\FireBurstExplode.fx
		Lifespan 60
		Lifespanjitter 	20
	End

#	Event 
#		HardwareOnly
#		Type 	start
#		At 	Root
#		BhvrOverride
#			PositionOffset	0 1.5 0
#		End
#		ChildFX V_COV\PhysicsEnabled\HotSparkOUT.fx
#		Lifespan 60
#		Lifespanjitter 	20
#	End
End

Condition
	On 	Time
	Time 	0



	Event
		EName 	Fire
		Type	Local
		At	F1_L
		
		Part	:SmokeExplosion.part
		Part1	:FireExplosion2.part
		Lifespan	15
		
	End

	Event
		EName 	Sparks
		Type	Local
		At	F1_L
		Part1	:GernadeLauncherSparks2.part
		Sound boom_explo2 90.0 90.0 .88
		
	End

	Event
		EName 	TankBlast_flash
		Type	Local
		At	F1_L

		Part	:FireExplosionFlash.part
		Lifespan 5
		
	End

#	Event
#		EName 	RadiusFxScale
#		Type	Local
#		At	F1_L
#		bhvr    behaviors/powers/firecontrol/InfernobubbleA.bhvr
#		Part	:FireBallHitRing2a.part
#	
#	End

	Event	
		ENAME	ThunderShake1
		Type	Local
		At	Origin
		Bhvr	Behaviors/SSThunderClapShake.bhvr
		Lifespan	8
		
		
	End


	Event
		Ename	ExplosiveForce
		Type	Start
		At	Origin
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		30
			PhysForcePower		750
			PhysForcePowerJitter	100
		End

		Lifespan	1

	End

End

Condition
	On 	Time
	Time 	5

	Event
		EName 	Sparks
		Type	Destroy 
	End

End

End			