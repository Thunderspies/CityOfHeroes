#########################################################
##	forceBubble
########################################################
FxInfo

Input  
	Inpname	Origin
End

Input  
	Inpname	Target
End

Input  
	Inpname	wepR
End

Input  
	Inpname	T_Root
End


########################################################

Condition
	On 	Time
	Time 	10

	Event
		EName 	node
		Type	local
		At	wepR
		Geom	Staff_02

	End

End

Condition
	On 	Time
	Time 	21

	Event
		EName 	Prime
		Type	start
		At	node
		altpiv	1
		Part	:Trampoline.part
		Part	:Trampoline1.part
		Part	:Trampoline2.part
		Sound elecball2 80 80 .8
		

	End
End





End