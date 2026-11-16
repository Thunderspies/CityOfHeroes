###########################################################

FxInfo
Lifespan 60

###########################################################


#Condition
#	On Time
#	Time 0
#
##	Event
##		At Root
##		Type Local
##		Part 	:Howl_Wind_Vertical.part
##		LifeSpan 15
##	End
#End

Condition
	On	Time
	Time	7

	Event
		At Root
		Type Local
		BhvrOverride
			PositionOffset	0.0 1.0 0.0
		End
		Part	:Howl_Ring.part
		Part 	:Howl_Wind_Shockwave.part
		Part 	:Howl_Wind_Shockwave_Core.part

		LifeSpan 15
	End
End


###########################################################

End