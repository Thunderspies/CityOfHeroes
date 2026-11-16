#########################################################
##	onfire
##
FxInfo  

Lifespan 70

Input  
	InpName	Root
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Fire
		Type	Posit
		At	Root
		Part1	:FireExplosion.part
		#Part2	powers/firecontrol/FireWeaponHitSpark.part
		LifeSpan	8
	End

	Event
		EName 	Sparks
		Type	Posit
		At	Root
		Part1	:GernadeLauncherSparks.part
		#Part2	:GernadeLauncherSpark.part
		Sound grexplo2 120.0 120.0 1.0
		LifeSpan	5
		
	End

	
	Event
		Ename	ExplosiveForce
		Type	Start
		At	Root
		
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
		
	Repeat	16
	
	Event
		EName	Sparkout
		Type 	Start
		At	Root
		BhvrOverride
			PositionOffset	0 1 0
		End
		ChildFX	V_COV\PhysicsEnabled\ShrapnelSparkExplode.fx
		
	End
	
	Event
		HardwareOnly
		EName	Sparkout
		Type 	Start
		At	Root
		BhvrOverride
			PositionOffset	0 1 0
		End
		ChildFX	V_COV\PhysicsEnabled\ShrapnelSparkExplode.fx
		
	End

	
	Event
		HardwareOnly
		EName	Sparkout
		Type 	Start
		At	Root
		BhvrOverride
			PositionOffset	0 1 0
		End
		ChildFX	V_COV\PhysicsEnabled\ShrapnelSparkOUT.fx
		
	End
End

End			