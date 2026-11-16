#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	Origin
End

Input  
	Inpname	Mystic
End

Input  
	Inpname	wepR
End

Input  
	Inpname	Chest
End

Input  
	Inpname	Target
End

Input  
	Inpname	root
End

Lifespan	200

########################################################################


Condition
	On 	Time
	Time 	40

	Event
		EName	Prime
		Type	Start
		At	WepR
		Geom	GEO_WepR_Ringblade_04
		Bhvr	:Ring.bhvr
		Magnet	Target
		LookAt	Target
	End

	Event
		EName	RightRingFX
		Type	Local
		At	Prime
		altpiv	1
		Part	:Ring_Toss_Streaks.part
		Part	:Ring_Glow_Main.part
		Part	:Ring_Glow_Main3.part
		Part	:Ring_Glow_01.part	
	End

End



Condition
	On 	Time
	Time 	14.5

	Event
		EName	Streak
		Type	local
		At	Mystic
		Geom	swoosh
		bhvr	behaviors/Claws.bhvr
		Sound clawtwirl5 100.0 100.0 .7
	End

End



Condition
	On 	Time
	Time 	17.5
	
	Event
		EName	streak
		Type	Destroy
		
	End
	
End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy

	End

End

End
##################################

