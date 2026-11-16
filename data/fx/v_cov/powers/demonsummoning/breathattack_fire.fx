#############################################################
## ToxicFire_Aura.fx
#############################################################

FxInfo
LIfespan 70

#########################################################

Condition
	On	Time
	Time	0

	Event
		EName	MouthAnchor
		Type	Local
		At	Head
		Sound Demon_BreathAttack_Fire 100 100 .9
		BhvrOverride
			PositionOffset	0.0 -0.4 0.0
		End
	End
End

#########################################################

Condition
	On 	Time
	Time 	30

	Event
		Type	PositOnly
		At	MouthAnchor
		Part	:Breath_Fire_Body.part
		Part	:Breath_Fire_Jet.part
		Part	:Breath_Smoke_Plume.part
		LookAt	T_Chest
		Lifespan 40
	End


End

Condition
	On	Time
	Time	35

	Event
		Type	PositOnly
		At	MouthAnchor
		Part	:Breath_Fire_Sparks.part
		LookAt	Target
		Lifespan 50
	End
End

#########################################################

End