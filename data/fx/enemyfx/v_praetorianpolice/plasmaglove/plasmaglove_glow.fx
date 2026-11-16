#############################################################
## ChestCannon_Huge.fx
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################
## CHEST CANNON
#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	BeamTarget
		Type	Local
		At	WepR
		BhvrOverride
			PositionOffset		0 -4 0
		End
	End

	Event
		Type	Local
		At	WepR

		part	:AntiMatterReactor_Glow01.part
		part	:AntiMatterReactor_Glow02.part
		part	:AntiMatterReactor_Beam01.part
		part	:AntiMatterReactor_Beam02.part

		pOther	BeamTarget
		Sound IDF_Glow1_Loop 50 50 .1
	End
End

#############################################################

End