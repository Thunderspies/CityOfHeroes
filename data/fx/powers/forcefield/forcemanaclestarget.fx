#########################################################
##	forceBubble
########################################################
FxInfo

Input  
	Inpname	HIPS
End


Condition
	On 	Time
	Time 	5

	Event
		EName 	Prime
		Type	Local 
		At	HIPS
		Bhvr 	behaviors/powers/forcefield/ForceBubble.bhvr
		
		Geom	FX_ForceBubble
		Part1	powers/forcefield/bubblesphere.part
		POther	Next
		PMagnet	Next
		
	End

	Event
		EName 	Next
		Type	Local
		At	Prime
		 AltPiv 1
	#	Part1	coldcontrol/icesword2.part

		POther	Prime
		PMagnet	Prime
	End

End

End
