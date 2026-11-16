#############################################################################
##Head dizzy
########################################################
FxInfo

Input  
	Inpname	head
End

Condition
	On 	Time
	Time 	0

	Event
		EName	core
		Type	Local
		At	head
		#Part1	:FireFlies.part
		Part2	:FireFliesStill3.part
		#Sound	flies_loop 40 30 .22
	End


End


End
