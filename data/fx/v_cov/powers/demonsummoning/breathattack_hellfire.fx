#############################################################
## ToxicFire_Aura.fx
#############################################################

FxInfo
LIfespan 65
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
	Time 	40

	Event
		Type	PositOnly
		At	MouthAnchor
		Part	:Breath_Hellfire_Body.part
		Part	:Breath_Hellfire_Body_Highlight.part
		Part	:Breath_Hellfire_Jet.part
		Part	:Breath_Hellfire_Plume.part
		LookAt	T_Chest
	End


	Event
		Type	PositOnly
		At	MouthAnchor
		Part	:Breath_Hellfire_Sparks.part
		LookAt	Target
		Lifespan 65
	End
End

#########################################################

End