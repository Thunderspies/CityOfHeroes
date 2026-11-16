#########################################################
##	
##

FxInfo


##################################



Condition
	On 	Time
	Time 	0

	Event
		EName 	Center
		Type	Local
		At	Chest
		BhvrOverride
			PositionOffset	0 .25 0
			pyrRotateJitter	10 180 0
		End
	End

	Event
		EName 	Offset
		Type	Local
		At	Center
		BhvrOverride
			PositionOffset	0 0 .5
		End
	End

	Event
		EName 	Knife
		Type	Local  #--> This is where new options would be extremely helpful!
		At	Offset
		bhvr	:NatKnifeHit.bhvr
		geom	FX_Natural_Kunai
	End

End

End							