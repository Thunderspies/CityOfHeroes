#########################################################
##	TemperatureControl
########################################################

FxInfo

#LifeSpan	100

Input
	Inpname	origin
End

##########################

Condition
	On 	Time
	Time 	0




	Event
		Type	Local
		At	origin
		Sound targeting_loop 100 100 .2
		bhvr	behaviors/soundFade1.bhvr
		Flags	PowerLoopingSound
		Lifespan 240
	End

End




Condition
	On 	Time
	Time 	0




	Event
		EName	core2
		Type	Local
		At	origin


		part1	:SmallExpandingRing.part
		


	End

End


End