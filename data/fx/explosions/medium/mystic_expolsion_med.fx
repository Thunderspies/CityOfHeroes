#########################################################
##	onfire
##
FxInfo  

Lifespan 50

Condition
	On 	time
	Time 	0
	Repeat 	15

	Event 
		EName 	Sparkz
		Type 	Local
		At 	Mystic
		ChildFX V_COV\PhysicsEnabled\HotSparkOUT.fx
		Lifespan 60
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
		At 	Mystic
		ChildFX V_COV\PhysicsEnabled\HotSparkOUT.fx
		Lifespan 120
		Lifespanjitter 	20
	End
			
	Event 
		HardwareOnly
		Type 	start
		At 	Mystic
		ChildFX V_COV\PhysicsEnabled\FireBurstUP.fx
		Lifespan 120
		Lifespanjitter 	20
	End	

	Event 
		HardwareOnly
		Type 	start
		At 	Mystic
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
		At	Mystic
		
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
		Type	Start  ##Posit
		At	Mystic
		
		Part	:SmokeExplosion.part
		
		Lifespan 15
	End

	Event
		EName 	Sparks
		Type	Start  ##Posit
		At	Mystic
		
		Part1	:FireExplosion2.part
		Part1	:GernadeLauncherSparks2.part
		Sound flash_explo 90 90 .88
		Sound explomed2 90.0 90.0 .8
		Sound boom_explo2 90.0 90.0 .8
		Sound flash_explo 90 90 .88
		
		Lifespan 8
	End

	Event
		EName 	TankBlast_flash
		Type	Start  ##Posit
		At	Mystic
	
		Part	:FireExplosionFlash.part
		Lifespan 5
		
	End

End


End			