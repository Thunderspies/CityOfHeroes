#########################################################
##	chill touch hit
FxInfo

Flags    InheritAlpha

LifeSpan	20
##################################
Condition
	On	Time
	Time	0
	
	Event
		Type	Local
		At	back
		BHVR	Behaviors/GhostPivot.bhvr
		Part	:SoulGlo.part
		
		lifeSpan	10
	End

	Event
		Type	Local
		At	back
		BHVR	Behaviors/GhostPivot.bhvr
		
		Part	:SoulFlame2.part
		
		lifeSpan	10
		
	End

	Event
		Type	Local
		At	back
		BHVR	Behaviors/GhostPivot.bhvr
		
		Part	:SoulFlame.part
		
		lifeSpan	10
	End

End

End				