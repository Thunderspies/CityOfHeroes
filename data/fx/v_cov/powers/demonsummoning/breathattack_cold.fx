#############################################################
## ToxicFire_Aura.fx
#############################################################

FxInfo
LIfespan 180

#########################################################

Condition
	On	Time
	Time	0

	Event
		EName	MouthAnchor
		Type	Local
		At	Head
		Sound Demon_BreathAttack_Cold 100 100 .9
		BhvrOverride
			PositionOffset	0.0 -0.4 0.0
		End
	End
End

#########################################################

Condition
	On 	Time
	Time 	35

	Event
		Type	PositOnly
		At	MouthAnchor
		Part	:Breath_Cold_Body.part
		Part	:Breath_Cold_Plume.part
		Part	:Breath_Cold_Jet.part
		LookAt	T_Chest

		Lifespan 45
	End



	Event
		Type	PositOnly
		At	MouthAnchor
		Part	:Breath_Cold_Snowflakes.part
		LookAt	T_Chest
		Lifespan 55
	End
End

#########################################################

End