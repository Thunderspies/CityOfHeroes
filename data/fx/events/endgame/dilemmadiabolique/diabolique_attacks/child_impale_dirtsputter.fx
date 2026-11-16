#
FxInfo

	Flags DontSuppress
	Lifespan 300

###########################################################

Condition
	On Time
	Time 0

	Event
		Type	Start
		At	Origin
		BhvrOverride
			PositionOffset	0 19.0 0.0
		End
		Part	:Spawn_Dust.part
		Part	:Spawn_Dust_Thick.part

		Lifespan 1
	End
End

Condition
	On Time
	Time 	86

	Event
		Type	Start
		At	Origin
		BhvrOverride
			PositionOffset	0 -2.0 0.0
		End
		Part	:Impale_Shards_Black.part
		Lifespan 2
	End
End

###########################################################

End