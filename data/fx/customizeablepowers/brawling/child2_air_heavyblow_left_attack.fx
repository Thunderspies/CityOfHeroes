#############################################################
## CHILD_AIR_SweepingCross_Attack.fx
#############################################################

FxInfo
Lifespan 30

#############################################################
Condition
	On	Time
	Time	9

	Event
		EName	SwooshAnchor
		Type	Local
		At	LArmL
		BhvrOverride
			PositionOffset	8.0 1.5 -5.0
			PYRRotate	0.0 -60.0 0.0 #0.0 90.0 0.0
		End
		Lifespan 10
	End

	Event
		Type	Local
		At	SwooshAnchor
		Part	:Air_Dust_Traveling_Tiny_AIR.part
		Part	:Air_Dust_Traveling_Additive_Tiny_AIR.part
		Lifespan 10
	End
End

#############################################################

End