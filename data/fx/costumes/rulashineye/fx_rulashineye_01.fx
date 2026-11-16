#############################################################
## FX_SeedHamidonTentacle_01.fx
#############################################################

FxInfo

Flags InheritAlpha DontSuppress DontInheritTexFromCostume

Condition
	On 	Time
	Time 	0

	Event
		EName	RulaShinEye
		Type	Local
		At	Chest
		Anim	Rularuu_Eye_Costume_01
		BhvrOverride
			Scale			0.15 0.15 0.15
			PositionOffset		-0.02 -0.5 0.3
#			PYRrotate 		15 -45 220
# 			PYRrotateJitter 	0 0 0
		End
	End
End

#############################################################

End