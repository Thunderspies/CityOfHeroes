#########################################################
##	onfire
##
FxInfo  

Lifespan 150

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
		Lifespan 8
	End

	Event
		EName 	Sparks
		Type	Posit
		At	Chest
		Part1	:GernadeLauncherSparks2.part
		#Part2	:GernadeLauncherSpark.part
		Sound grexplo2 90.0 90.0 1.0
		Sound exploburst 90.0 90.0 .66
		Sound explohit2 90.0 90.0 .75
		Lifespan 50
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
		
	Repeat	4
	
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
		ChildFX	V_COV\PhysicsEnabled\ShrapnelSparkOUT.fx
		
	End

#	
#	Event
#		HardwareOnly
#		EName	Sparkout
#		Type 	Start
#		At	Chest
#		BhvrOverride
#			PositionOffset	0 0 0
#		End
#		ChildFX	V_COV\PhysicsEnabled\FireBurstScatter.fx
#		
#	End

End

Condition
	On	Time
	Time	0
		
	Repeat	6
	
	Event
		EName	Sparkout
		Type 	Start
		At	Chest
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	V_COV\PhysicsEnabled\FireBurstScatter.fx
		CThresh	0.001
		
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

End

#######################################################################################################

Condition
	Time 	3
	Chance	.8

	Event
		Type	start
		At	chest

		Bhvr	Behaviors\StartOffset.bhvr
		Part1	:fire.part
		Part1	:fire.part
		Lifespan	130
	End

End

Condition
	Time 	3
	Chance	.8

	Event
		Type	start
		At	chest

		Bhvr	Behaviors\StartOffset2.bhvr
		Part1	:fire.part
		Part1	:fire.part
		Lifespan	130
	End

End

Condition
	Time 	3
	Chance	.8

	Event
		Type	start
		At	chest

		Bhvr	Behaviors\StartOffset3.bhvr
		Part1	:fire.part
		Part1	:fire.part
		Lifespan	130
	End

End

Condition
	Time 	3
	Chance	.8

	Event
		Type	start
		At	chest

		Bhvr	Behaviors\StartOffset4.bhvr
		Part1	:fire.part
		Part1	:fire.part
		Lifespan	130
	End

End

##########################################################################################################

End			