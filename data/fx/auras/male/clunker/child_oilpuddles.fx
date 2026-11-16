#############################################################
## Child_OilPuddles.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Ename	PuddleAnchor
		Type 	Local
		At	Origin
		Until	Air
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 0 -0.35
		End
	End
End


Condition
	On	Time
	Time	0

	Event
		Type 	PositOnly
		At	PuddleAnchor
		Part	:Puddles_Idle.part
	End
End

#############################################################
End