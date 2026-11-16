#############################################################
## Summon_Attack.fx
#############################################################

FxInfo
LifeSpan	180

## HANDS #####################################################


Condition
	On	Time
	Time	20

	Event
		Type	Local
		At	Root
		Sound Seers_SummonReinforcements_Attack 100 100 1
	End
End

Condition
	On	Time
	Time	0

	Event
		EName	ReticleAnchor
		Type	StartPositOnly
		At	Root
		BhvrOverride
			PositionOffset	0 0.2 0
		End
	End


End

Condition
	On	Time
	Time	20

	Event
		Type	PositOnly
		At	Face
		Part	:Seers_Judgement_Receive_Torso_Rings_Falling.part
		Part	:Seers_Judgement_Receive_Torso_Rings_Rising.part
#		Lifespan 300
	End
End

Condition
	On	Time
	Time	60

	Event
		Type	PositOnly
		At	ReticleAnchor
#		Part	:Seers_Judgement_Receive_Target_Reticle.part
		Lifespan 30
	End
End

##############################################################################

End