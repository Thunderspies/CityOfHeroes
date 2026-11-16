#############################################################
## Ice Control - Ice Slick (Child)
#############################################################

FxInfo

#############################################################

#Condition
#	On	Cycle
#	Time	15
#	Chance	.8
#
#	Event
#		EName	splat
#		Type	start
#		At	Root
#		Splat	Splat_ElectricRing.tga	
#		Bhvr	:IceSlickRing.bhvr
#		LifeSpan	8
#	End
#End

#############################################################

Condition
	On	Time
	Time	1

	Event
		EName	splat
		Type	start
		At	Root
#		Splat	Splat_ElectricRing.tga	
#		Bhvr	:IceSlickRing.bhvr
		LifeSpan	8
	End
End

#############################################################

End			