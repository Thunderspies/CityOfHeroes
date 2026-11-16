#########################################################
##	greenblobby
########################################################
FxInfo

Input  
	Inpname	Root
End

Input  
	Inpname	Chest
End


########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	liquid
		Type	local
		At	Root
		Part1	:greenblobby.part
		
	End

End	

Condition
	On 	Time
	Time 	0

	Event
		EName 	caustic
		Type	local
		At	Root
		Part1	:greenblobby2.part
				
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	bubbles
		Type	local
		At	Root
		Part1	:greenblobby3.part
				
	End

End

Condition
	On 	Time
	Time 	0

	Event
		EName 	sparkles
		Type	local
		At	Root
		Part1	:greenblobby4.part
				
	End

End	

Condition
	On 	Time
	Time 	0

	Event
		EName 	bubbles2
		Type	local
		At	Root
		Part1	:greenblobby5.part
				
	End

End	


End
##################################

