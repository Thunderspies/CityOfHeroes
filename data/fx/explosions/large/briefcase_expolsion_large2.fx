#########################################################
##	onfire
##
FxInfo  

Lifespan 150


Condition
	On 	time
	Time 	0
	Repeat 	15

	Event 
		EName 	Sparkz
		Type 	Local
		At 	root
		BhvrOverride
			PositionOffset	0 1.5 0
		End
		ChildFX V_COV\PhysicsEnabled\FireBurstOUT.fx
		Lifespan 60
		Lifespanjitter 	20
	End
End

Condition
	On 	time
	Time 	0
	Repeat 	10
	
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
			
	Event 
		HardwareOnly
		Type 	start
		At 	Root
		BhvrOverride
			PositionOffset	0 1.5 0
		End
		ChildFX V_COV\PhysicsEnabled\HotSparkExplode.fx
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
		EName 	Sparks
		Type	Local
		At	root
		Part1	:GernadeLauncherSparks2.part
		
		LifeSpan	5
	End

	Event
		EName 	Fire
		Type	Local
		At	F1_L
		
		Part1	:FireExplosion2.part
		
		LifeSpan	8
	End

	Event
		EName 	Fire
		Type	Local
		At	F1_L
		
		Part	:SmokeExplosion.part
		Sound briefcaseexplo1 70.0 70.0 .95
		LifeSpan	18
	End

	Event
		EName 	TankBlast_flash
		Type	Local
		At	F1_L

		Part	:FireExplosionFlash.part
		Part	:Explosion_flame_B.part
		Lifespan 5
		
	End

	Event	
		ENAME	ThunderShake1
		Type	Local
		At	Origin
		Bhvr	behaviors\CameraShake_briefcase.bhvr
		
		LifeSpan	8
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Origin
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		30
			PhysForcePower		500
			PhysForcePowerJitter	50
		End

		Lifespan	1

	End


End

Condition
	On 	Time
	Time 	5

	Event
		EName 	Prime
		Type	start 
		At	root
		Part1	:Mediumfire_flame_A.part
		Part2	:Mediumfire_flame_B.part
		
		Part3	:Mediumfire_flame_Flat.part
		#Part4	WORLD/Fire/spark1.part
		Part5	:mediumfire_Smoke.part
		Sound torch2_loop 35 35 .33
	End
End

End			