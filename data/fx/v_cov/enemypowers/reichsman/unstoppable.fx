#########################################################
##  Unstoppable FX (Villain)
#########################################################

FxInfo

#########################################################

Condition
	On	Time
	Time	0

	Event
		EName	3
		Type	Local
		At	Chest
		bhvr	Behaviors/GenericParticleFade.bhvr
		part	POWERS/Invulnerability/Evil_Unstoppable_Sparkles.part
		Part	:LightRays.part
		Part	:LightRaysSub.part
		Sound regen3 60 60 .9
	End

	Event
		Type	Local
		At	Chest
		Sound regen7 60 60 .8
	End
End

Condition
	On	Time
	Time	38

	Event
		Ename	Ring
		Type	Local
		At	Chest
		Part	:Ring.part
	End
End


#########################################################

End