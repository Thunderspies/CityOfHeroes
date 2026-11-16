#########################################################
##	jmp1_teleporter
##
FxInfo

Input
	InpName	Origin
End


Condition
	On 	Time
	Time 	0
##	Chance	.8

	Event
		EName 	Clouds
		Type	start
		At	head
		BhvrOverride
			PyrRotate 	-90 180 0
		End
		Part	:jmp1teleBubbles.part
		Part	:jmp1tele1.part
		Part	:Rift_Hurricane_Swirl.part
		Part	:Rift_BlackHole.part
	End
End


