#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	
	Inpname Origin
End

Input

	Inpname	Mystic

End

lifespan	50

Condition
	On 	Time
	Time 	10

	Event
		Type Local
		At origin
		Sound whooshbig2 60 60 .8
	End
End



Condition
	On 	Time
	Time 	16

	Event
		EName	Streak
		Type	start
		At	BendMystic
		Geom	swoosh
		bhvr	behaviors/Claws.bhvr
	End

End


Condition
	On 	Time
	Time 	18

	Event
		EName	streak
		Type	Destroy
		
	End
	

End


End
##################################
