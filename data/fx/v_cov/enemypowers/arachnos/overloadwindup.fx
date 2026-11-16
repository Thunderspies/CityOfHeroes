#########################################################
##	
##

FxInfo
LifeSpan  100

Input  
	InpName	Target
End

Input  
	InpName	Mystic
End

Input  
	InpName	WepR
End

##################################





Condition
	On 	Time
	Time 	0


	Event
		Type	local
		At	C_HandR
		bhvr	:Bot_Dis_Arc_R.bhvr
		part1	:HotDiskGlows.part
		part1	:HotDiskGlows1.part
	
		LifeSpan	32
	End

	Event
		Type	local
		At	C_HandL
		bhvr	:Bot_Dis_Arc_L.bhvr
		part1	:HotDiskGlows.part
		part1	:HotDiskGlows1.part
	
		LifeSpan	32
	End
end

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	C_FootR
		bhvr	:Bot_Dis_Arc_b.bhvr
		part1	:HotDiskGlows.part
		part1	:HotDiskGlows1.part
	
		LifeSpan	32
	End

End

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	C_FootL
		bhvr	:Bot_Dis_Arc_b.bhvr
		part1	:HotDiskGlows.part
		part1	:HotDiskGlows1.part
	
		LifeSpan	32
	End

End