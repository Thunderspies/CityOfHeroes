#############################################################
## vclub_PoolCue01.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	PoolCue
		Type	Local
		At	Mystic
		BhvrOverride
			Scale		1.5 1.5 1.5
		End
		Geom	vclub_PoolCue01
	End
End

#############################################################

End