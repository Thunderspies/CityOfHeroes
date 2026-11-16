#########################################################
##	onfire
##
FxInfo  

Lifespan 50

Input  
	InpName	Chest
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Fire
		Type	Posit
		At	Chest
		
		Part	:SmokeExplosion.part
		Part1	:FireExplosion2.part
		
	End

	Event
		EName 	Sparks
		Type	Posit
		At	Chest
		Part1	:GernadeLauncherSparks2.part
		#Part2	:GernadeLauncherSpark.part
		Sound grexplo2 120.0 120.0 1.0
		
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Chest
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		20
			PhysForcePower		300
			PhysForcePowerJitter	25
		End

		Lifespan	1

	End
End


Condition
	On	Time
	Time	0
		
	Repeat	15
	
	Event
		EName	Sparkup
		Type 	Start
		At	Chest
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	V_COV\PhysicsEnabled\ShrapnelSparkOUT.fx
		
	End

	Event
		HardwareOnly
		EName	Sparkup
		Type 	Start
		At	Chest
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	V_COV\PhysicsEnabled\ShrapnelSparkOUT.fx
		
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

Condition
	On 	Time
	Time 	8

	Event
		EName 	Fire
		Type	Destroy 
	End


End


End			