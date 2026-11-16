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
		At	Eyes
		BhvrOverride
			PositionOffset		0 0 4
		End
		Sound ClockworkLoyalists_Attack_Chest_Loop 150 150 0.25
	End

	Event
		Type	Local
		At	Eyes
		part	Costumes\PraetorianClockwork\AntiMatterReactor_Glow01.part
		part	Costumes\PraetorianClockwork\AntiMatterReactor_Glow02.part
		part	Costumes\PraetorianClockwork\AntiMatterReactor_Beam01.part
		pOther	BeamTarget
	End
End

#############################################################

End