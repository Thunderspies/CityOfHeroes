#########################################################
##	TemperatureControl
########################################################

FxInfo

Input  
	Inpname	root
End

Input  
	Inpname	head
End
##########################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	chest
		Sound mindcontrol2_loop 80 80 .5
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 330
	End
End



Condition
	On 	Time
	Time 	0

	Event
		EName	core
		Type	Local
		At	head
		Geom	LocalPivot01
		Part1	:HeadDizzies.part
		Part2	:HeadDizzyRings.part
		part3	:MindGlowHit2.part
		Sound mentalhit 80 80 .8
	End


End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	head
		#Sound	mindcontrol2_loop 80 20 .55
	End


End

End