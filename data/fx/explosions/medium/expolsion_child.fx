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
		At 	origin
		ChildFX V_COV\PhysicsEnabled\HotSparkOUT.fx
		Lifespan 120
		Lifespanjitter 	20
	End
End
Condition
	On 	time
	Time 	0
	Repeat 	5
		
	Event 
		HardwareOnly
		EName 	Sparkz
		Type 	Local
		At 	origin
		ChildFX V_COV\PhysicsEnabled\HotSparkOUT.fx
		Lifespan 120
		Lifespanjitter 	20
	End
			
	Event 
		HardwareOnly
		Type 	start
		At 	origin
		ChildFX V_COV\PhysicsEnabled\FireBurstUP.fx
		Lifespan 120
		Lifespanjitter 	20
	End	

	Event 
		HardwareOnly
		Type 	start
		At 	origin
		ChildFX V_COV\PhysicsEnabled\HotSparkUP.fx
		Lifespan 120
		Lifespanjitter 	20
	End
End

Condition
	On 	Time
	Time 	0

	
	Event
		Ename	ExplosiveForce
		Type	Start
		At	origin
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		20
			PhysForcePower		500
			PhysForcePowerJitter	50
		End

		Lifespan	1

	End

	Event
		EName 	Fire
		Type	Posit
		At	origin
		
		Part	:SmokeExplosion.part
		
		Lifespan 15
	End

	Event
		EName 	Sparks
		Type	Posit
		At	origin
		
		Part1	:FireExplosion2.part
		Part1	:GernadeLauncherSparks2.part
		Sound grexplo2 120.0 120.0 1.0
		
		Lifespan 8
	End

	Event
		EName 	TankBlast_flash
		Type	Posit
		At	origin
	
		Part	:FireExplosionFlash.part
		Lifespan 5
		
	End

End


End			