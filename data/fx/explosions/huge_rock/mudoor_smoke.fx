#########################################################
##	onfire
##
FxInfo  

###################################################################
LifeSpan	9999







Condition

	On	Time
	Time	0

	Event
		Type	Local
		At	root
		Sound steamblast1 170 170 .55
	End
End


Condition

	On	Time
	Time	0

	Event
		Type	Local
		At	root
#		Sound	smoke2_loop 170 60 .33
	End
End







Condition
	Time 	0
	Repeat	3

	Event
		EName 	SmokeSparx
		Type	local 
		At	root

		BhvrOverride

#		PositionOffset		0 17 0
			StartJitter	15 5 0
		End

		Part	:InfernoExplosion.part
		
#		LifeSpan	3

	End

End

End			
