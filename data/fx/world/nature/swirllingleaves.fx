#########################################################
##	TreeFallingLeavesLG
##
FxInfo
	
#LifeSpan 30000

Input  
	InpName	root
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prim
		Type	start 
		At	root
		geom	CenterDummy
		bhvr	Behaviors/leavesspin.bhvr
	End

	Event
		EName 	P2
		Type	Local 
		At	Prim
		altpiv	1

		Part1	WORLD/Nature/SwirllingLeavesLG.part
		Part2	WORLD/Nature/SwirllingLeavesSM.part
		#Part3	WORLD/City/Streetpaper01.part
		#Part4	WORLD/City/Streetpaper02.part

	End

End

End			