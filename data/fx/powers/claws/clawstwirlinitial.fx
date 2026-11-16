#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	Origin
End

Input  
	Inpname	target
End

Input  
	Inpname	Mystic
End

Input  
	Inpname	root
End

lifespan	49

Condition

	On 	Time
	Time 	29

	Event
		Type Local
		At origin
		Sound Clawsfocus4 100.0 100.0 .7
	End
End

Condition
	On 	Time
	Time 	32

	Event
		EName	Streak
		Type	local
		At	Mystic
		Geom	swoosh
		bhvr	behaviors/Claws.bhvr
	End


	Event
		EName	Prime
		Type	start
		At	root
		Bhvr	behaviors/Focusprojectile1.bhvr
		Part1	:ClawRays.part
		
		Magnet	Target
		LookAt	Target
	End

End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy

	End

End

Condition
	On 	Time
	Time 	39

	Event
		EName	streak
		Type	Destroy
		
	End
	

End

End
##################################

