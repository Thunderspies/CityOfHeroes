###########################################################

FxInfo
Lifespan 60

###########################################################

Condition
	On	Time
	Time	7

#	Event
#		At Root
#		Type Local
#		Bhvr	:Dust_Expanding.bhvr
#		Part 	:Howl_Wind_Ring.part
#		Lifespan 20
#	End

	Event
		At	Root
		Type	Local
		BhvrOverride
			PositionOffset	0.0 2.0 0.0
		End
		Part	:Growl_Ring.part
		Part 	:Growl_Wind_Shockwave.part
		LifeSpan 20
	End
End


###########################################################

End