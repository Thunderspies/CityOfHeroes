#########################################################
##	template

FxInfo


Condition
	On	Time
	Time	10

	Event	
		Type	Local
		At	Root
		Sound miasma_loop 80 80 .22
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	arrow
		Type	start
		At	Root
		Part	:PoisonFlat.part
		Part	:Poison.part
		Sound GasArrowHit 90 90 .8

	End
	
End


End

		