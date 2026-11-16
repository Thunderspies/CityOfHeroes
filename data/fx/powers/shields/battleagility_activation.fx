#############################################################
## BattleAgility_Activation.fx
#############################################################

FxInfo

Lifespan 150

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		EName	ChestOffset
		Type	Local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		0 0.25 0
		End
		Part	:Shields_Activation_CoreGlow1.part
		Part	:Shields_Activation_CoreStar1.part
		pOther	Offset1
		Lifespan 35
	End
End

#########################################################

#Condition
#	On	Time
#	Time	20
#
#	Event
#		Ename	BodyAura_Initial
#		Type	Local
#		At	Origin
#		ChildFX	:Shields_Continuing_Pulse.fx
#		Lifespan 60
#	End
#End


Condition
	On 	Time
	Time 	5

	Event
		EName	sound
		Type	Local
		At	hips
		Sound battle_agility 60 60 .85

	End
End

#############################################################

End