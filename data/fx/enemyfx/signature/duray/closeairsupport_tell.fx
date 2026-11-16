#############################################################
## CloseAirSupport_Attack.fx
#############################################################

FxInfo
#Lifespan 60

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		Sound 	CloseAirSupport_Tell_Loop 100 100 .5
		bhvr	behaviors/Sound/SoundIn05.bhvr
	End


	Event
		At	ROot
		Type	Local
		BhvrOverride
			PositionOffset	0.0 0.0 0.0
		End
		Part	:Targeting_Arrows.part
		Magnet	TargetAnchor
		LookAt	TargetAnchor
	End
End

#############################################################

End