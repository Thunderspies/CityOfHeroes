#############################################################
## Rikti Heavy Assault Suit - Thruster FX
#############################################################

FxInfo

Flags InheritAlpha

## SOUND FX ###########################################################

Condition
	On	Time
	Time	0
	
	Event	
		Ename   targetchill
		Type	Local
		At	Chest
		Sound flight2_loop 40.0 40.0 .09
	End
End

## EYE FX ###########################################################

Condition
	On	Time
	Time	0

	Event	
		Ename   Eye
		Type	Local
		At	Head
		BhvrOverride
			PositionOffset	0 0.75 0.75
		End
		part2	:Eye_Glow.part
	End

#	Event	
#		Ename   EyeBeamOffset
#		Type	Local
#		At	Head
#		BhvrOverride
#			PositionOffset	0 0 16
#		End
#		part2	:Eye_Beam.part
#		part2	:Eye_BeamGlow.part
#		pOther	Eye
#	End
End

## THRUSTERS FX ###########################################################

Condition
	On	Time
	Time	0
	
	Event	
		Ename   PodThruster
		Type	Local
		At	T1_L
#		BhvrOverride
#			PositionOffset	0 -0.5 -0.25
#		End
		part2	:ThrusterExhaust_Glow.part
	End

	Event	
		Ename   PodThruster
		Type	Local
		At	T1_R
#		BhvrOverride
#			PositionOffset	0 -0.5 -0.25
#		End
		part2	:ThrusterExhaust_Glow.part
	
	End

	Event	
		Ename   PodThruster
		Type	Local
		At	F1_L
		BhvrOverride
			PositionOffset	0.25 0.25 0.75
		End
		part1	:ThrusterExhaust_Alpha.part
		part2	:ThrusterExhaust_Bright.part
		part2	:ThrusterExhaust_Glow.part
	End

	Event	
		Ename   PodThruster
		Type	Local
		At	F1_R
		BhvrOverride
			PositionOffset	-0.25 0.25 0.75
		End
		part1	:ThrusterExhaust_Alpha.part
		part2	:ThrusterExhaust_Bright.part
		part2	:ThrusterExhaust_Glow.part
	
	End

End

Condition
	On	Time
	Time	0
	
	Event	
		Ename   Weapon
		Type	Local
		At	WepR
#		BhvrOverride
#			PositionOffset	0 0.5 0
#		End
		part2	:ThrusterExhaust_Glow.part
	End

	Event	
		Ename   Weapon
		Type	Local
		At	WepL
#		BhvrOverride
#			PositionOffset	0 0.5 0
#		End
		part2	:ThrusterExhaust_Glow.part
	
	End
End

## PHYSICS FORCES ###########################################################

Condition
	On	Time
	Time	0

	Event
		Ename	LevitationForce
		Type	Local
		At	Root
		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		12
			PhysForcePower		75
			PhysForcePowerJitter	10
			PhysForceSpeedScaleMax	60
		End
	End

	Event
		Ename	ConstantLevitationForce
		Type	Posit
		At	Root
		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		7
			PhysForcePower		25
			PhysForcePowerJitter	10
		End
	End
	
	Event
		Ename	WakeForce
		Type	Local
		At	Root
		
		BhvrOverride
			PhysForceType		In
			PhysForceRadius		10
			PhysForcePower		40
			PhysForcePowerJitter	10
			PhysForceSpeedScaleMax	75
		End
	End
End

#############################################################

End				