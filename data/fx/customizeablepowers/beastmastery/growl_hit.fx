###########################################################

FxInfo
Lifespan 60

###########################################################

Condition
	On	Time
	Time	7

	Event
		At Root
		Type Local
	#	Bhvr	:Dust_Expanding.bhvr
	#	Part 	:Howl_Wind_Ring.part

		Lifespan 20
	End

	Event
		At	Head
		Type	Local
		Part	:Growl_Ring_Hit.part
		Part 	:Growl_Flare_Hit.part
		Part 	:Growl_Streaks_Hit.part

		LifeSpan 5
	End
End


###########################################################

End