#########################################################
##	template

FxInfo

Input  
	Inpname	Hips
End


Input  
	Inpname	Head
End



Input  
	Inpname	chest
End



Input  
	Inpname	waist
End

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	Monkey
		Type	Local
		##Bhvr	behaviors/Rune_spin.bhvr
		At	Hips
		Anim	FX_dark
	End

	Event
		EName	Punch1
		Type	Local
		At	Monkey
		 AnimPiv Chest
		Part1	powers/dougtest1.part
		#Part2	powers/powerpunch/powerpunch2.part
		#Part3	powers/powerpunch/powerpunch3.part
	End

	Event
		EName	Punch2
		Type	Local
		At	Monkey
		 AnimPiv Hair
		Part1	powers/dougtest1.part
		#Part2	powers/powerpunch/powerpunch2.part
		#Part3	powers/powerpunch/powerpunch3.part
	End

	Event
		EName	Punch3
		Type	Local
		At	Monkey
		 AnimPiv LlegL
		Part1	powers/dougtest1.part
		#Part2	powers/powerpunch/powerpunch2.part
		#Part3	powers/powerpunch/powerpunch3.part
	End
	
	Event
		EName	Punch4
		Type	Local
		At	Monkey
		 AnimPiv LlegR
		Part1	powers/dougtest1.part
		#Part2	powers/powerpunch/powerpunch2.part
		#Part3	powers/powerpunch/powerpunch3.part
	End

	Event
		EName	Punch5
		Type	Local
		At	Monkey
		 AnimPiv Head
		Part1	powers/dougtest1.part
		#Part2	powers/powerpunch/powerpunch2.part
		#Part3	powers/powerpunch/powerpunch3.part
	End

	Event
		EName	Punch5
		Type	Local
		At	Monkey
		 AnimPiv Hips
		Part1	powers/dougtest1.part
		#Part2	powers/powerpunch/powerpunch2.part
		#Part3	powers/powerpunch/powerpunch3.part
	End

End


Condition
	On 	Time
	Time 	127
	
	Event
		Type	Destroy
		EName	All
	End
End	

End		