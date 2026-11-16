#########################################################
##	EBlast
########################################################
FxInfo

Input  
	Inpname	WepR
End

################################


Condition
	On 	Time
	Time 	0

	Event
		EName 	Thingy
		Type	local
		At	WepR
		Part1	:StunGun01.part
		Part2	powers/ElectricityControl/ElectricityInnerGlow.part
		Part3	:StunGun03.part

	End

End


Condition
	On 	Time
	Time 	30

	Event

	Type	Destroy
	EName 	Thingy

	End

End


End
