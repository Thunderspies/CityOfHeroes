#########################################################
##	template

FxInfo

LifeSpan 65

Condition
		
	Event
		EName 	Fire
		Type	Posit
		At	Chest
		Part1	:FireExplosion.part
		Lifespan 8
	End

	Event
		EName 	Sparks
		Type	Posit
		At	Chest
		Part1	:GernadeLauncherSparks.part
		Part	:FireExplosionFlash.part
		Sound grexplo2 120.0 120.0 1.0
		Lifespan 5
		
	End

	Event
		EName	Prime
		Type 	Local
		At	Chest
		Part1	:Shotgunflash.part
		Part2	:ShotgunflashStar.part
		Lifespan	15
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Chest
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		15
			PhysForcePower		200
			PhysForcePowerJitter	20
		End

		Lifespan	1

	End

End

Condition
	On	Time
	Time	0
		
	Repeat	11
	
	Event
		EName	Sparkout
		Type 	Start
		At	Chest
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	V_COV\PhysicsEnabled\ShrapnelSparkExplode.fx
	End
	
	Event
		EName	Sparkout
		Type 	Start
		At	Chest
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	V_COV\PhysicsEnabled\ShrapnelSparkOUT.fx
	End

		
	Event
		HardwareOnly
		EName	Sparkout
		Type 	Start
		At	Chest
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	V_COV\PhysicsEnabled\ShrapnelSparkExplode.fx
	End
	
	Event
		HardwareOnly
		EName	Sparkout
		Type 	Start
		At	Chest
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	V_COV\PhysicsEnabled\ShrapnelSparkOUT.fx
	End

End

End	

	