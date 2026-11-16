#############################################################
## CHILD_AIR_SweepingCross_Attack.fx
#############################################################

FxInfo
Lifespan 30

#############################################################

Condition
	On		Time
	Time		22

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 4.0 -10.0
			PYRRotate	90 0 90
		End
		Part	:Air_Dust_Traveling_AIR.part
		Part	:Air_Dust_Traveling_Additive_AIR.part
		Lifespan 3
	End
End

#############################################################

End